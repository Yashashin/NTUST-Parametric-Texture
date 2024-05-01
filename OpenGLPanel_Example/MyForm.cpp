#include "MyForm.h"

#include "Paintbrush.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThread]

void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	OpenGLPanel_Example::MyForm form;
	Application::Run(% form);
}

void OpenGLPanel_Example::MyForm::OnMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Delta > 0)
	{
		main_camera.wheelEvent(-5);
	}
	else
	{
		main_camera.wheelEvent(5);
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e)
{
	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized!\n";
		return;
	}

	//Init 
	paramGL = new ParamGL();
	model = new MeshObject();
	Texture* tex = new Texture();
	model->textures.push_back(tex);
	pickingTexture = new PickingTexture();
	currentTextureID = 0;
	renderMode = RenderMode::WIREFRAME;
	main_camera.Reset();
	// Load shader from files
	ShaderInfo wireframeShaders[] =
	{
		{ GL_VERTEX_SHADER,  "../Shader/wireframe.vs" },
		{ GL_GEOMETRY_SHADER, "../Shader/wireframe.gs"},
		{ GL_FRAGMENT_SHADER, "../Shader/wireframe.fs" },
		{ GL_NONE, NULL }
	};
	paramGL->wireframeShader = LoadShaders(wireframeShaders);
	ShaderInfo pickingShaders[] = {
		{GL_VERTEX_SHADER, "../Shader/picking.vs"},
		{GL_FRAGMENT_SHADER, "../Shader/picking.fs"},
		{GL_NONE,NULL}
	};
	paramGL->pickingShader = LoadShaders(pickingShaders);


	ShaderInfo modelShaders[] = {
	{GL_VERTEX_SHADER, "../Shader/model.vs"},
	{GL_FRAGMENT_SHADER, "../Shader/model.fs"},
	{GL_NONE,NULL}
	};
	paramGL->modelShader = LoadShaders(modelShaders);

	this->Cursor = Cursors::AppStarting;
	//Load model
	if (model->Init("../Model/armadillo.obj"))
	{
		std::cout << "Load Model" << std::endl;
	}
	else
	{
		std::cout << "Load Model Failed" << std::endl;
	}
	this->Cursor = Cursors::Arrow;
	modelPath = "../Model/armadillo.obj";
	//Picking Texture (frame buffer)
	pickingTexture->Init(this->hkoglPanelControl1->Size.Width, this->hkoglPanelControl1->Size.Height);

	//Set matrix
	paramGL->projection = glm::perspective(80.0f, ((float)this->hkoglPanelControl1->Size.Width / (float)this->hkoglPanelControl1->Size.Height), 0.1f, 100.0f);
	paramGL->view = glm::lookAt(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
	glm::mat4 model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(100, 100, 100));
	paramGL->model = model;



	// Get and print used OpenGL version
	int ver[2] = { 0, 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
	glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
	std::cout << "OpenGL Version: " << ver[0] << "." << ver[1] << std::endl;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	addBrush = new Paintbrush();
	delBrush = new Paintbrush();
	currentBrush = addBrush;

}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	paramGL->view = glm::lookAt(glm::vec3(50 * sin(eye_x), 3, 50 * cos(eye_x)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	main_camera.SetWindowSize(this->hkoglPanelControl1->Size.Width, (float)this->hkoglPanelControl1->Size.Height);

	glm::mat4 model_matrix = glm::mat4(1);
	model_matrix = glm::scale(model_matrix, glm::vec3(100, 100, 100));
	paramGL->model = main_camera.GetModelMatrix() /* * model_matrix*/;
	paramGL->projection = main_camera.GetProjectionMatrix();
	paramGL->view = main_camera.GetViewMatrix();

	if (renderMode == WIREFRAME)
	{
		RenderToPickingTexture();
		RenderWireframe();
		RenderWireframeTexture();
		RenderSelectedFace();
		if (mouseState == MouseState::RELEASE) {
			glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), false);
			glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->hoverFaceColor.r, model->hoverFaceColor.g, model->hoverFaceColor.b, model->hoverFaceColor.a);
			model->RenderHoverSelectedFace();
		}
	}
	else
	{
		RenderModel();
		RenderModelTexture();
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	mouseState = MouseState::PRESS;
	if (e->Button == System::Windows::Forms::MouseButtons::Left) //select face
	{
		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			if (model->hoverSelectedFaces.size() == 0)
			{
				return;
			}
		
			if (isMoveMode) 
			{
				unsigned int id = model->hoverSelectedFaces[0];
				for (std::vector<Texture*>::iterator it = model->textures.begin(); it != model->textures.end(); it++) //model 1
				{
					std::vector<unsigned int> IDs = (*it)->faceIDs;
					bool flag = false;
					for (int i=0;i<IDs.size();i++)
					{
						if (id==IDs[i])
						{
							flag = true;
							break;
						}
					}
					if (flag) //remove texture
					{
						int ring = (*it)->ringNum;
						if (currentBrush->GetPaintbrushSize() < ring)
						{
							for (int i = currentBrush->GetPaintbrushSize(); i < ring; i++)
							{
								currentBrush->AddPaintbrushSize(1);
							}
						}
						else
						{
							for (int i = ring;i<currentBrush->GetPaintbrushSize(); i++)
							{
								currentBrush->AddPaintbrushSize(-1);
							}
						}

						moveTextureUvScale = (*it)->uvScale;
						moveTextureUvRotate = (*it)->uvRotate;
						moveTextureUvOffsetX = (*it)->uvOffsetX;
						moveTextureUvOffsetY = (*it)->uvOffsetY;
						it = model->textures.erase(it);
						currentTextureID--;
						isPaste = true;
						isMoveMode = false;
						hkoglPanelControl1->Invalidate();
						break;
					}
				}

				for (std::vector<Texture*>::iterator it = model2->textures.begin(); it != model2->textures.end(); it++) //model 2
				{
					std::vector<unsigned int> IDs = (*it)->faceIDs;
					bool flag = false;
					for (int i = 0; i < IDs.size(); i++)
					{
						if (id == IDs[i])
						{
							flag = true;
							break;
						}
					}
					if (flag) //remove texture
					{
						it = model2->textures.erase(it);
						break;
					}
				}
			}
			else
			{
				//check is inside
				for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
					model->AddSelectedFace(model->hoverSelectedFaces[i]);
				}
				if (isPaste)
				{
					SolveUV();
					SetUvScale();
					SetUvRotate();
					SetUvOffsetX();
					SetUvOffsetY();
					isPaste = false;
				}
			}
		}
		else if (selectMode == SelectMode::DELFACE) {
			std::cout << "Not yet to delete hover faces\n";
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->DeleteSelectedFace(model->hoverSelectedFaces[i]);
			}
		}

	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		//drag view
		main_camera.mouseEvents(GLUT_MIDDLE_BUTTON, GLUT_DOWN, e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		main_camera.mouseEvents(GLUT_LEFT_BUTTON, GLUT_DOWN, e->X, e->Y);
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		model->hoverSelectedFaces.clear();

		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, addBrush->GetPaintbrushSize() - 1);
		}
		else if (selectMode == SelectMode::DELFACE) {
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, delBrush->GetPaintbrushSize() - 1);
		}


		if (selectMode == SelectMode::PICKFACE) {
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->AddSelectedFace(model->hoverSelectedFaces[i]);
			}
		}
		else if (selectMode == SelectMode::DELFACE) {
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->DeleteSelectedFace(model->hoverSelectedFaces[i]);
			}
		}
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		main_camera.mouseMoveEvent(e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
		main_camera.mouseMoveEvent(e->X, e->Y);
	}
	else //just hover on it
	{
		model->hoverSelectedFaces.clear();

		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, addBrush->GetPaintbrushSize() - 1);
		}
		else if (selectMode == SelectMode::DELFACE) {
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, delBrush->GetPaintbrushSize() - 1);
		}
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	mouseState = MouseState::RELEASE;
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
		main_camera.mouseReleaseEvent(GLUT_MIDDLE_BUTTON, e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		main_camera.mouseReleaseEvent(GLUT_LEFT_BUTTON, e->X, e->Y);
	}
}

void OpenGLPanel_Example::MyForm::Load_Texture_Btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastTextureDirectory))
	{
		LastTextureDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->Filter = "Image Files (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|All Files(*.*)|*.*";
	openFileDialog1->FilterIndex = 1;
	try
	{
		openFileDialog1->InitialDirectory = LastTextureDirectory;
	}
	catch (Exception^)
	{
		openFileDialog1->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog1->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastTextureDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		this->CreatePictureBox(FileName);
	}
}

void OpenGLPanel_Example::MyForm::SolveUV_Click(System::Object^ sender, System::EventArgs^ e)
{
	SolveUV();
	SetUvScale();
	SetUvRotate();
	SetUvOffsetX();
	SetUvOffsetY();
}

void OpenGLPanel_Example::MyForm::Picture_Click(System::Object^ sender, System::EventArgs^ e)
{
	msclr::interop::marshal_context converter;
	std::string name = converter.marshal_as<std::string>(((PictureBox^)sender)->Name);
	auto it = textureRecord.find(name);
	if (it != textureRecord.end())
	{
		selectTextureImage = it->second;
		std::cout << "Select texture: " << it->second << std::endl;
	}
	else
	{
		std::cout << "Picture map error!" << std::endl;
	}
}

void OpenGLPanel_Example::MyForm::PaintbrushStripButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::ToolStripButton^ ctrl = (ToolStripButton^)sender;
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	std::cout << "Button type:" << tagInteger << std::endl;
	switch (tagInteger) {
	case 0: //add
		this->selectMode = SelectMode::PICKFACE; //set mode
		this->currentBrush = addBrush;
		this->PaintADDStripButton1->Checked = true;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		break;
	case 1: //del
		this->selectMode = SelectMode::DELFACE; //set mode
		this->currentBrush = delBrush;
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = true;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		break;
	case 2: //plus
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->currentBrush->AddPaintbrushSize(1);
		break;
	case 3: //shrink
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->currentBrush->AddPaintbrushSize(-1);
		break;
	case 4: //scale up by button
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->ScaleSelectedFaces();
		break;
	case 5: //shrink down by button
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->ShrinkSelectedFaces();
		break;
	default:
		std::cout << "Error button tag\n";
		break;
	}
}

void OpenGLPanel_Example::MyForm::RenderModeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::ToolStripMenuItem^ ctrl = (ToolStripMenuItem^)sender;
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	std::cout << "Item type:" << tagInteger << std::endl;
	switch (tagInteger) {
	case 0: //wireframe
		this->renderMode = RenderMode::WIREFRAME;
		break;
	case 1: //normal
		this->renderMode = RenderMode::NORMAL;
		break;
	default:
		std::cout << "Error Item tag\n";
		break;
	}
}

void OpenGLPanel_Example::MyForm::FaceColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (this->colorDialog1 == nullptr) {
		this->colorDialog1 = gcnew ColorDialog;
	}


	colorDialog1->AllowFullOpen = true;//allow custom color
	colorDialog1->ShowHelp = true; //idk

	if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		Color color = colorDialog1->Color;
		System::Windows::Forms::ToolStripMenuItem^ ctrl = (ToolStripMenuItem^)sender;
		int tagInteger = Int32::Parse(ctrl->Tag->ToString());
		std::cout << "Item type:" << tagInteger << std::endl;
		float MAXIMUMCOLOR = 255.0f;
		glm::vec4 colorFloatValue = glm::vec4(color.R / MAXIMUMCOLOR, color.G / MAXIMUMCOLOR, color.B / MAXIMUMCOLOR, color.A / MAXIMUMCOLOR);
		switch (tagInteger) {
		case 0: //selected faces color
			model->selectedFaceColor = colorFloatValue;
			break;
		case 1: //hover faces color
			model->hoverFaceColor = colorFloatValue;
			break;
		case 2: //model faces color
			model->modelFaceColor = colorFloatValue;
			break;
		case 3:
			model->wireFaceColor = colorFloatValue;
			break;
		case 4:
			model->wireColor = colorFloatValue;
			break;

		default:
			std::cout << "Error Item tag\n";
			break;
		}
	}
}


void OpenGLPanel_Example::MyForm::TextureRESETButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->TextureScaleTrackBar->Value = 1;
	this->TextureRotateTrackBar->Value = 0;
	this->TextureOffsetXTrackbar->Value = 0;
	this->TextureOffsetYTrackbar->Value = 0;
	this->TextureScalenumericUpDown1->Value= System::Decimal(1.0f);
	this->TextureRotatenumericUpDown1->Value = System::Decimal(0.0f);
	this->TextureOffsetXnumericUpDown1->Value = System::Decimal(0.0f);
	this->TextureOffsetYnumericUpDown1->Value = System::Decimal(0.0f);

}

void OpenGLPanel_Example::MyForm::Move_Click(System::Object^ sender, System::EventArgs^ e)
{
	isMoveMode = !isMoveMode;
	//throw gcnew System::NotImplementedException();
}

void OpenGLPanel_Example::MyForm::ModelImportToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastModelDirectory))
	{
		LastModelDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog2 = gcnew OpenFileDialog;
	openFileDialog2->Filter = "Object Files (*.obj)|*.obj|*.*| All Files(*.*)";
	try
	{
		openFileDialog2->InitialDirectory = LastModelDirectory;
	}
	catch (Exception^)
	{
		openFileDialog2->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog2->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastModelDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string fileName = converter.marshal_as<std::string>(FileName);
		std::cout << "Load model: " << fileName << std::endl;

		//New model
		currentTextureID = 0;
		delete model;
		delete model2;

		//Model 1
		model = new MeshObject();
		Texture* tex = new Texture();
		model->textures.push_back(tex);
		this->Cursor = Cursors::AppStarting;
		if (model->Init(fileName))
		{
			std::cout << "Load model successfully!" << std::endl;
		}
		else
		{
			std::cout << "Load model failed." << std::endl;
		}
		this->Cursor = Cursors::Arrow;
		modelPath = fileName;
		//Model 2
		hkoglPanelControl2->Refresh();
		model2 = new MeshObject();
		Texture* _tex = new Texture();
		model2->textures.push_back(_tex);
		model2->Init(fileName);

		hkoglPanelControl1->Refresh();
	}
}

void OpenGLPanel_Example::MyForm::ModelLoadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastSaveDirectory))
	{
		LastSaveDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog3 = gcnew OpenFileDialog;

	openFileDialog3->Filter = "Save Files (*.txt)|*.txt|All Files(*.*)|*.*";
	openFileDialog3->FilterIndex = 1;
	try
	{
		openFileDialog3->InitialDirectory = LastSaveDirectory;
	}
	catch (Exception^)
	{
		openFileDialog3->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog3->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog3->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastSaveDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string path = converter.marshal_as<std::string>(FileName);
		this->flowLayoutPanel1->Controls->Clear();
		this->LoadFile(path);
	}
}

void OpenGLPanel_Example::MyForm::ModelSaveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastSaveDirectory))
	{
		LastSaveDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;

	saveFileDialog1->Filter = "Save Files (*.txt)|*.txt|All Files(*.*)|*.*";
	saveFileDialog1->FilterIndex = 1;
	try
	{
		saveFileDialog1->InitialDirectory = LastSaveDirectory;
	}
	catch (Exception^)
	{
		saveFileDialog1->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = saveFileDialog1->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastSaveDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string path = converter.marshal_as<std::string>(FileName);
		this->SaveFile(path);
	}
}

void OpenGLPanel_Example::MyForm::CreatePictureBox(String^ FileName)
{
	msclr::interop::marshal_context converter;
	std::string fileName = converter.marshal_as<std::string>(FileName);
	std::cout << "Load texture: " << fileName << std::endl;
	AddTexture(fileName.c_str());
	System::Windows::Forms::PictureBox^ pictureBox = (gcnew System::Windows::Forms::PictureBox());
	pictureBox->Image = pictureBox->Image->FromFile(FileName);
	pictureBox->Location = System::Drawing::Point(30, 30);
	pictureBox->Name = gcnew String(fileName.c_str());
	pictureBox->Size = System::Drawing::Size(192, 192);
	pictureBox->TabStop = false;
	pictureBox->Click += gcnew System::EventHandler(this, &MyForm::Picture_Click);
	pictureBox->MouseLeave += gcnew System::EventHandler(this, &MyForm::PictureBoxMouseLeave);
	pictureBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::PictureBoxMouseMove);
	pictureBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::PictureBoxMouseClick);
	pictureBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::PictureBoxPaint);
	pictureBox->Tag = L"0";
	pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
	pictureBox->Cursor = Cursors::Hand;
	this->flowLayoutPanel1->Controls->Add(pictureBox);
	textureRecord[fileName] = textureRecord.size() - 1;
}

void OpenGLPanel_Example::MyForm::PictureBoxMouseLeave(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::PictureBox^ picturebox = (PictureBox^)sender;
	if (picturebox->Tag->Equals(L"2")) {
		picturebox->Tag = L"0";
		picturebox->Invalidate();
	}
}

void OpenGLPanel_Example::MyForm::PictureBoxMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	System::Windows::Forms::PictureBox^ picturebox = (PictureBox^)sender;
	if (picturebox->Tag->Equals(L"0")) {
		picturebox->Tag = L"2";
		picturebox->Invalidate();
	}
}

void OpenGLPanel_Example::MyForm::PictureBoxMouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	System::Windows::Forms::PictureBox^ picturebox = (PictureBox^)sender;
	if (this->pictureBoxPtr!=nullptr&&this->pictureBoxPtr!=picturebox) {
		this->pictureBoxPtr->Tag = L"0";
		this->pictureBoxPtr->Invalidate();
	}
	picturebox->Tag = L"1";
	this->pictureBoxPtr = picturebox;
	picturebox->Invalidate();
	
}

void OpenGLPanel_Example::MyForm::PictureBoxPaint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	
	PictureBox^ pb = (PictureBox^)sender;
	if (pb->Tag->Equals(L"1")) {
		e->Graphics->DrawRectangle(gcnew Pen(Color::Red, 5), 0, 0, pb->Width, pb->Height);
	}
	else if (pb->Tag->Equals(L"2")) {
		e->Graphics->DrawRectangle(gcnew Pen(Color::Black, 5), 0, 0, pb->Width, pb->Height);
	}
}


void OpenGLPanel_Example::MyForm::TextureBox_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::TrackBar^ ctrl = (TrackBar^)sender;
	float OneUnit;
	float value;
	//To Set numeric value
	//value is between 0.00~10.00
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	switch (tagInteger) {
	case 0://scale
		OneUnit = ctrl->LargeChange;
		value = ctrl->Value / OneUnit; //get floating point
		this->TextureScalenumericUpDown1->Value = System::Decimal(value);
		this->SetUvScale();
		break;
	case 1://rotate
		value = ctrl->Value;
		this->TextureRotatenumericUpDown1->Value = System::Int32(value);
		this->SetUvRotate();
		break;
	case 2://offsetX
		OneUnit = ctrl->LargeChange;
		value = ctrl->Value / OneUnit; //get floating point
		this->TextureOffsetXnumericUpDown1->Value = System::Decimal(value);
		this->SetUvOffsetX();
		break;
	case 3://offsetY
		OneUnit = ctrl->LargeChange;
		value = ctrl->Value / OneUnit; //get floating point
		this->TextureOffsetYnumericUpDown1->Value = System::Decimal(value);
		this->SetUvOffsetY();
		break;
	default:
		std::cout << "Error sender type\n";
		break;
	}
}


void OpenGLPanel_Example::MyForm::TextureBox_Numeric(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::NumericUpDown^ ctrl = (NumericUpDown^)sender;
	float value = (float)ctrl->Value;//get float value
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	//because the numeric value is between 0.00~10.00
	
	switch (tagInteger) {
	case 0://scale
		value = value * 100;//get rid of decimal part
		this->TextureScaleTrackBar->Value = System::Int32(value);
		this->SetUvScale();
		break;
	case 1://rotate
		this->TextureRotateTrackBar->Value = System::Int32(value);
		this->SetUvRotate();
		break;
	case 2://offsetX
		value = value * 100;//get rid of decimal part
		this->TextureOffsetXTrackbar->Value = System::Int32(value);
		this->SetUvOffsetX();
		break;
	case 3://offsetY
		value = value * 100;//get rid of decimal part
		this->TextureOffsetYTrackbar->Value = System::Int32(value);
		this->SetUvOffsetY();
		break;
	default:
		std::cout << "Error sender type\n";
		break;
	}
}

void OpenGLPanel_Example::MyForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
}

System::Void OpenGLPanel_Example::MyForm::hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e)
{

	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized!\n";
		return;
	}
	//Init 
	paramGL2 = new ParamGL();
	model2 = new MeshObject();
	Texture* tex = new Texture();
	model2->textures.push_back(tex);

	//Load shaders
	ShaderInfo simpleTextureShaders[] = {
	{GL_VERTEX_SHADER, "../Shader/drawTexture.vs"},
	{GL_FRAGMENT_SHADER, "../Shader/drawTexture.fs"},
	{GL_NONE,NULL}
	};
	paramGL2->simpleTextureShader = LoadShaders(simpleTextureShaders);
	ShaderInfo uvMapShaders[] =
	{
	 { GL_VERTEX_SHADER,  "../Shader/uvMap.vs" },
	 { GL_GEOMETRY_SHADER, "../Shader/uvMap.gs"},
	 { GL_FRAGMENT_SHADER, "../Shader/uvMap.fs" },
	 { GL_NONE, NULL }
	};
	paramGL2->uvMapShader = LoadShaders(uvMapShaders);

	ShaderInfo inversionShaders[] =
	{
	 { GL_VERTEX_SHADER,  "../Shader/PostEffect/inversion.vs" },
	 { GL_FRAGMENT_SHADER, "../Shader/PostEffect/inversion.fs" },
	 { GL_NONE, NULL }
	};
	paramGL2->postEffectInversionShader = LoadShaders(inversionShaders);

	//Load model
	this->Cursor = Cursors::AppStarting;
	if (model2->Init("../Model/armadillo.obj"))
	{
		std::cout << "Load Model" << std::endl;
	}
	else
	{
		std::cout << "Load Model Failed" << std::endl;
	}
	this->Cursor = Cursors::Arrow;
	modelPath = "../Model/armadillo.obj";
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


System::Void OpenGLPanel_Example::MyForm::hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{

	//hkoglPanelControl2->Refresh();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3, 0.3, 0.3, 0);

	if (currentTextureID == 0)
	{
		return;
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3, 0.3, 0.3, 0);
		//original
		glViewport(0, 0, this->hkoglPanelControl2->Size.Width, this->hkoglPanelControl2->Size.Height);

		float aspect = this->hkoglPanelControl2->Size.Width / (float)this->hkoglPanelControl2->Size.Height;
		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0));
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
		glm::mat4 projectMatrix = glm::ortho(-aspect * 1.5f, aspect * 1.5f, -1.5f, 1.5f, 0.1f, 30.0f);

		glUseProgram(paramGL2->simpleTextureShader);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "Model"), 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "Projection"), 1, GL_FALSE, &projectMatrix[0][0]);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "View"), 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "uvScale"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::vec3(model2->textures[currentTextureID - 1]->uvScale, model2->textures[currentTextureID - 1]->uvScale, model2->textures[currentTextureID - 1]->uvScale)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "uvRotate"), 1, GL_FALSE, glm::value_ptr(glm::rotate(model2->textures[currentTextureID - 1]->uvRotate, glm::vec3(0, 0, 1)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->simpleTextureShader, "uvOffset"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1), glm::vec3(model2->textures[currentTextureID - 1]->uvOffsetX, model2->textures[currentTextureID - 1]->uvOffsetY, 0)))
		);
		glBindTexture(GL_TEXTURE_2D, model2->textures[currentTextureID - 1]->textureID);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);



		glUseProgram(paramGL2->uvMapShader);

		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->uvMapShader, "Model"), 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->uvMapShader, "Projection"), 1, GL_FALSE, &projectMatrix[0][0]);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL2->uvMapShader, "View"), 1, GL_FALSE, &viewMatrix[0][0]);

		glUniform4f(glGetUniformLocation(paramGL2->uvMapShader, "faceColor"), 1,0,0,0);
		glUniform4f(glGetUniformLocation(paramGL2->uvMapShader, "wireColor"), 1,0,0,1);


		glBindVertexArray(model2->model.vao);

		glBindBuffer(GL_ARRAY_BUFFER, model2->textures[currentTextureID - 1]->textureVbo);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glMultiDrawElements(GL_TRIANGLES, &(model2->textures[currentTextureID - 1]->elementCount[0]), GL_UNSIGNED_INT, (const GLvoid**)&(model2->textures[currentTextureID - 1]->fvIDsPtr[0]), model2->textures[currentTextureID - 1]->elementCount.size());

		glBindVertexArray(0);
	}
}





void OpenGLPanel_Example::MyForm::LoadFile(std::string path)
{
	std::ifstream inStream;
	inStream.open(path);
	if (inStream.fail())
	{
		std::cout << "Open file failed." << std::endl;
		return;
	}
	this->Cursor = Cursors::AppStarting;

	std::string _modelPath;
	std::string str;
	std::stringstream ss;
	float uvRotate, uvScale, uvOffsetx, uvOffsetY;
	std::vector<unsigned int> faceIds;
	std::vector<int> textureImageIndeice;
	texturePaths.clear();
	textureImages.clear();
	uvMapTextures.clear();
	textureRecord.clear();
	selectedTextureRecord.clear();
	currentTextureID = 0;
	selectTextureImage = 0;
	delete model;
	delete model2;

	//Load Model
	getline(inStream, _modelPath);
	//Model 1
	model = new MeshObject();
	Texture* tex = new Texture();
	model->textures.push_back(tex);
	if (model->Init(_modelPath.c_str()))
	{
		std::cout << "Load model successfully!" << std::endl;
	}
	else
	{
		std::cout << "Load model failed." << std::endl;
	}
	modelPath = _modelPath;
	//Model 2
	hkoglPanelControl2->Refresh();
	model2 = new MeshObject();
	Texture* _tex = new Texture();
	model2->textures.push_back(_tex);
	model2->Init(_modelPath.c_str());
	hkoglPanelControl1->Refresh();

	//Load Texture Image
	std::getline(inStream, str); //Texture Image Start
	while (1)
	{
		getline(inStream, str);
		if (str == "Texture Image End")
		{
			break;
		}
		CreatePictureBox(gcnew String(str.c_str()));
	}

	std::getline(inStream, str);//Texture Image Index Record Start
	std::getline(inStream, str);
	ss << str;
	while (!ss.eof())
	{
		int index;
		ss >> index;
		textureImageIndeice.push_back(index);
	}
	ss.clear();
	ss.str("");
	std::getline(inStream, str);//Texture Image Index Record End

	std::getline(inStream, str); //Texture Start
	while (1)
	{
		getline(inStream, str);
		if (str == "Texture End")
		{
			break;
		}
		ss << str;
		ss >> uvRotate >> uvScale >> uvOffsetx >> uvOffsetY;
		getline(inStream, str); //Face Id Start
		getline(inStream, str);
		ss.clear();
		ss.str("");
		ss << str;
		faceIds.clear();
		while (!ss.eof())
		{
			int faceId;
			ss >> faceId;
			faceIds.push_back(faceId);
		}
		ss.clear();
		ss.str("");
		getline(inStream, str); //Face Id End

		model->selectedFace = faceIds;
		SolveUV();
		model->textures[currentTextureID - 1]->uvRotate = uvRotate;
		model->textures[currentTextureID - 1]->uvScale = uvScale;
		model->textures[currentTextureID - 1]->uvOffsetX = uvOffsetx;
		model->textures[currentTextureID - 1]->uvOffsetY = uvOffsetY;
		model2->textures[currentTextureID - 1]->uvRotate = uvRotate;
		model2->textures[currentTextureID - 1]->uvScale = uvScale;
		model2->textures[currentTextureID - 1]->uvOffsetX = uvOffsetx;
		model2->textures[currentTextureID - 1]->uvOffsetY = uvOffsetY;

		model->textures[currentTextureID - 1]->textureID = textureImages[textureImageIndeice[currentTextureID - 1]]->textureID;
		model2->textures[currentTextureID - 1]->textureID = uvMapTextures[textureImageIndeice[currentTextureID - 1]]->textureID;
	}
	selectedTextureRecord = textureImageIndeice;
	inStream.close();
	this->Cursor = Cursors::Arrow;
}

void OpenGLPanel_Example::MyForm::SaveFile(std::string path)
{

	std::ofstream outStream;
	outStream.open(path);
	if (outStream.fail())
	{
		std::cout << "Open file failed." << std::endl;
		return;
	}
	outStream << modelPath << std::endl;
	outStream << "Texture Image Start" << std::endl;
	for (int i = 0; i < texturePaths.size(); i++)
	{

		outStream << texturePaths[i] << std::endl;
	}
	outStream << "Texture Image End" << std::endl;
	outStream << "Texture Image Index Record Start" << std::endl;
	for (int i = 0; i < selectedTextureRecord.size(); i++)
	{
		if (i == 0)
		{
			outStream << selectedTextureRecord[i];
		}
		else
		{
			outStream << " " << selectedTextureRecord[i];
		}
	}
	outStream << std::endl;
	outStream << "Texture Image Index Record End" << std::endl;
	outStream << "Texture Start" << std::endl;
	for (int i = 0; i < model->textures.size() - 1; i++)
	{
		outStream << model->textures[i]->uvRotate << " " << model->textures[i]->uvScale << " " << model->textures[i]->uvOffsetX << " " << model->textures[i]->uvOffsetY << std::endl;
		outStream << "Face Id Start" << std::endl;
		for (int j = 0; j < model->textures[i]->faceIDs.size(); j++)
		{
			if (j == 0)
			{
				outStream << model->textures[i]->faceIDs[j];
			}
			else
			{
				outStream << " " << model->textures[i]->faceIDs[j];
			}
		}
		outStream << std::endl;
		outStream << "Face Id End" << std::endl;

	}
	outStream << "Texture End" << std::endl;
	outStream.close();
}

void OpenGLPanel_Example::MyForm::AddTexture(const char* fileName)
{
	this->Cursor = Cursors::AppStarting;
	hkoglPanelControl1->Refresh();
	TextureImage* tmp = new TextureImage();
	textureImages.push_back(tmp);
	textureImages.back()->Load_png(fileName);

	hkoglPanelControl2->Refresh();
	TextureImage* _tmp = new TextureImage();
	uvMapTextures.push_back(_tmp);
	uvMapTextures.back()->Load_png(fileName);
	hkoglPanelControl1->Refresh();
	std::string str = fileName;
	texturePaths.push_back(str);
	this->Cursor = Cursors::Arrow;
}

void OpenGLPanel_Example::MyForm::SolveUV()
{
	if (model->selectedFace.size() <= 0 || textureImages.size() == 0)
	{
		return;
	}
	std::sort(model->selectedFace.begin(), model->selectedFace.end());

	MyMesh mesh;

	//custom properties
	OpenMesh::HPropHandleT<double> weight;
	OpenMesh::VPropHandleT<int> row;
	mesh.add_property(weight, "weight");
	mesh.add_property(row, "row");

	//standard properties
	mesh.request_vertex_texcoords2D();
	mesh.request_vertex_normals();
	mesh.request_face_normals();

	//Copy selected faces to mesh
	std::vector<MyMesh::VertexHandle> vertexHandles;
	vertexHandles.reserve(3); //allocate space 

	std::map<int, int> usedVertex; //record used vertex
	for (std::vector<unsigned int>::iterator it = model->selectedFace.begin(); it != model->selectedFace.end(); ++it)
	{
		MyMesh::FaceHandle faceHandle = model->model.mesh.face_handle(*it); //get face handle
		for (MyMesh::FaceVertexIter faceVertexIt = model->model.mesh.fv_iter(faceHandle); faceVertexIt.is_valid(); ++faceVertexIt)
		{
			MyMesh::VertexHandle vertexHandle;
			MyMesh::Point point = model->model.mesh.point(*faceVertexIt);

			std::map<int, int>::iterator usedVertexIt = usedVertex.find(faceVertexIt->idx());

			if (usedVertexIt == usedVertex.end()) //vertex not used
			{
				vertexHandle = mesh.add_vertex(point);
				usedVertex[faceVertexIt->idx()] = vertexHandle.idx();
			}
			else //duplicated vertex
			{
				vertexHandle = mesh.vertex_handle(usedVertexIt->second);
			}
			vertexHandles.push_back(vertexHandle);
		}
		mesh.add_face(vertexHandles);
		vertexHandles.clear();
	}
	mesh.update_normals();


	//Weight Calculation
	MyMesh::HalfedgeHandle halfEdgeHandle;
	for (MyMesh::EdgeIter edgeIt = mesh.edges_begin(); edgeIt != mesh.edges_end(); ++edgeIt)
	{
		if (!mesh.is_boundary(*edgeIt)) //if this edge not boundary
		{

			MyMesh::HalfedgeHandle _halfEdgeHandle = mesh.halfedge_handle(*edgeIt, 0);
			MyMesh::Point pointFrom = mesh.point(mesh.from_vertex_handle(_halfEdgeHandle));
			MyMesh::Point pointTo = mesh.point(mesh.to_vertex_handle(_halfEdgeHandle));
			MyMesh::Point point1 = mesh.point(mesh.opposite_vh(_halfEdgeHandle));
			MyMesh::Point point2 = mesh.point(mesh.opposite_he_opposite_vh(_halfEdgeHandle));

			double edgeLength = (pointFrom - pointTo).length();

			//Weight from -> to
			OpenMesh::Vec3d vector1 = OpenMesh::Vec3d(pointTo - pointFrom);
			vector1.normalized();

			OpenMesh::Vec3d vector2 = OpenMesh::Vec3d(point1 - pointFrom);
			vector2.normalized();

			GLdouble angle1 = std::acos(OpenMesh::dot(vector1, vector2));

			OpenMesh::Vec3d vector3 = OpenMesh::Vec3d(point2 - pointFrom);
			vector3.normalized();

			GLdouble angle2 = std::acos(OpenMesh::dot(vector1, vector3));

			GLdouble _weight = (tan(angle1 / 2.0f) + tan(angle2 / 2.0f)) / edgeLength;

			mesh.property(weight, _halfEdgeHandle) = _weight;

			//Weight to -> from
			vector1 = -vector1;
			vector2 = OpenMesh::Vec3d(point1 - pointTo);
			vector2.normalized();

			angle1 = acos(OpenMesh::dot(vector1, vector2));

			vector3 = OpenMesh::Vec3d(point2 - pointTo);
			vector3.normalized();

			angle2 = acos(OpenMesh::dot(vector1, vector3));

			_weight = (tan(angle1 / 2.0f) + tan(angle2 / 2.0f)) / edgeLength;
			mesh.property(weight, mesh.opposite_halfedge_handle(_halfEdgeHandle)) = _weight;
		}
		else
		{
			if (!halfEdgeHandle.is_valid())
			{
				halfEdgeHandle = mesh.halfedge_handle(*edgeIt, 1);
			}
		}
	}

	//Calculate matrix size
	int count = 0;
	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
	{
		if (mesh.is_boundary(*vertexIt))
		{
			mesh.property(row, *vertexIt) = -1;
		}
		else
		{
			mesh.property(row, *vertexIt) = count++;
		}
	}

	double perimeter = 0;
	std::vector<double> segmentLength;
	std::vector<MyMesh::VertexHandle> _vertexHandles;
	MyMesh::HalfedgeHandle next = halfEdgeHandle;
	do
	{
		MyMesh::Point from = mesh.point(mesh.from_vertex_handle(next));
		MyMesh::Point to = mesh.point(mesh.to_vertex_handle(next));
		perimeter += (from - to).length();

		segmentLength.push_back(perimeter);
		_vertexHandles.push_back(mesh.from_vertex_handle(next));

		next = mesh.next_halfedge_handle(next);
	} while (halfEdgeHandle != next);


	MyMesh::TexCoord2D coord(1, 0.5);
	mesh.set_texcoord2D(_vertexHandles[0], coord);

	for (int i = 1; i < _vertexHandles.size(); i++)
	{
		double angle = 2 * M_PI * segmentLength[i - 1] / perimeter;

		coord[0] = (std::cos(angle) + 1) / 2;
		coord[1] = (std::sin(angle) + 1) / 2;

		mesh.set_texcoord2D(_vertexHandles[i], coord);
	}


	Eigen::SparseMatrix<double> A(count, count);
	Eigen::VectorXd BX(count);
	Eigen::VectorXd BY(count);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > linear;
	BX.setZero();
	BY.setZero();

	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
	{
		if (!mesh.is_boundary(*vertexIt))
		{
			int i = mesh.property(row, *vertexIt);
			double totalWeight = 0;

			for (MyMesh::VertexVertexIter vertexVertexIt = mesh.vv_iter(*vertexIt); vertexVertexIt.is_valid(); ++vertexVertexIt)
			{
				MyMesh::HalfedgeHandle _halfEdgeHandle = mesh.find_halfedge(*vertexIt, *vertexVertexIt);
				double _weight = mesh.property(weight, _halfEdgeHandle);

				if (mesh.is_boundary(*vertexVertexIt))
				{
					MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*vertexVertexIt);
					BX[i] += _weight * texCoord[0];
					BY[i] += _weight * texCoord[1];
				}
				else
				{
					int j = mesh.property(row, *vertexVertexIt);
					A.insert(i, j) = -_weight;
				}
				totalWeight += _weight;
			}
			A.insert(i, i) = totalWeight;
		}
	}
	A.makeCompressed();

	//Solve linear
	Eigen::SparseMatrix<double> At = A.transpose();
	linear.compute(At * A);
	Eigen::VectorXd TX = linear.solve(At * BX);
	Eigen::VectorXd TY = linear.solve(At * BY);

	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
	{
		if (!mesh.is_boundary(*vertexIt))
		{
			int i = mesh.property(row, *vertexIt);
			mesh.set_texcoord2D(*vertexIt, MyMesh::TexCoord2D(TX[i], TY[i]));
		}
	}

	if (!model->model.mesh.has_vertex_texcoords2D())
	{
		model->model.mesh.request_vertex_texcoords2D();
		for (MyMesh::VertexIter vertexIt = model->model.mesh.vertices_begin(); vertexIt != model->model.mesh.vertices_end(); ++vertexIt)
		{
			model->model.mesh.set_texcoord2D(*vertexIt, MyMesh::TexCoord2D(-1, -1));
		}
	}

	int index = 0;
	for (MyMesh::FaceIter faceIt = mesh.faces_begin(); faceIt != mesh.faces_end(); ++faceIt)
	{
		MyMesh::FaceHandle faceHandle = *faceIt;
		MyMesh::FaceHandle selectedFace_h = model->model.mesh.face_handle(model->selectedFace[index++]);

		MyMesh::FaceVertexIter faceVertexIt = mesh.fv_iter(faceHandle);
		MyMesh::FaceVertexIter selectedFaceVertexIt = model->model.mesh.fv_iter(selectedFace_h);
		for (; faceVertexIt.is_valid() && selectedFaceVertexIt.is_valid(); ++faceVertexIt, ++selectedFaceVertexIt)
		{
			MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*faceVertexIt);
			model->model.mesh.set_texcoord2D(*selectedFaceVertexIt, texCoord);
		}
	}
	if (model->model.mesh.has_vertex_texcoords2D())
	{
		std::vector<MyMesh::TexCoord2D> texCoords;
		texCoords.reserve(model->model.mesh.n_vertices());
		for (MyMesh::VertexIter vertexIt = model->model.mesh.vertices_begin(); vertexIt != model->model.mesh.vertices_end(); ++vertexIt)
		{
			MyMesh::TexCoord2D texCoord = model->model.mesh.texcoord2D(*vertexIt);
			texCoords.push_back(texCoord);
		}
		std::vector<MyMesh::TexCoord2D> texCoords2 = texCoords;
		//Model 1
		glBindVertexArray(model->model.vao);
		glGenBuffers(1, &model->textures[currentTextureID]->textureVbo);

		glBindBuffer(GL_ARRAY_BUFFER, model->textures[currentTextureID]->textureVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::TexCoord2D) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);


		hkoglPanelControl2->Refresh();
		//Model 2
		glBindVertexArray(model2->model.vao);
		glGenBuffers(1, &model2->textures[currentTextureID]->textureVbo);

		glBindBuffer(GL_ARRAY_BUFFER, model2->textures[currentTextureID]->textureVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::TexCoord2D) * texCoords2.size(), &texCoords2[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	hkoglPanelControl1->Refresh();
	//Model 1
	model->textures[currentTextureID]->fvIDsPtr.swap(std::vector<unsigned int*>(model->selectedFace.size()));
	for (int i = 0; i < model->textures[currentTextureID]->fvIDsPtr.size(); i++)
	{
		model->textures[currentTextureID]->fvIDsPtr[i] = (GLuint*)(model->selectedFace[i] * 3 * sizeof(GLuint));
	}
	model->textures[currentTextureID]->elementCount.swap(std::vector<int>(model->selectedFace.size(), 3));

	model->textures[currentTextureID]->faceIDs = model->selectedFace;
	model->textures[currentTextureID]->textureID = textureImages[selectTextureImage]->textureID;
	selectedTextureRecord.push_back((int)selectTextureImage);
	Texture* tmp = new Texture();
	model->textures.push_back(tmp);
	model->textures[currentTextureID]->ringNum = currentBrush->GetPaintbrushSize();
	//hkoglPanelControl1->Invalidate();

	hkoglPanelControl2->Refresh();
	//Model 2
	model2->textures[currentTextureID]->fvIDsPtr.swap(std::vector<unsigned int*>(model->selectedFace.size()));
	for (int i = 0; i < model2->textures[currentTextureID]->fvIDsPtr.size(); i++)
	{
		model2->textures[currentTextureID]->fvIDsPtr[i] = (GLuint*)(model->selectedFace[i] * 3 * sizeof(GLuint));
	}
	model2->textures[currentTextureID]->elementCount.swap(std::vector<int>(model->selectedFace.size(), 3));

	model2->textures[currentTextureID]->faceIDs = model->selectedFace;
	model2->textures[currentTextureID]->textureID = uvMapTextures[selectTextureImage]->textureID;
	Texture* _tmp = new Texture();
	model2->textures.push_back(_tmp);

	hkoglPanelControl1->Refresh();
	model->selectedFace.clear();
	currentTextureID++;
}

void OpenGLPanel_Example::MyForm::SetUvScale()
{
	if (isPaste)
	{
		model->textures[currentTextureID - 1]->uvScale = moveTextureUvScale;
		model2->textures[currentTextureID - 1]->uvScale = moveTextureUvScale;
		TextureScalenumericUpDown1->Value = System::Decimal(1.0f/ moveTextureUvScale);
	}
	else
	{
		float value = (float)TextureScalenumericUpDown1->Value;
		model->textures[currentTextureID - 1]->uvScale = 1 / value;
		model2->textures[currentTextureID - 1]->uvScale = 1 / value;
	}
}

void OpenGLPanel_Example::MyForm::SetUvRotate()
{
	if (isPaste)
	{
		model->textures[currentTextureID - 1]->uvRotate = moveTextureUvRotate;
		model2->textures[currentTextureID - 1]->uvRotate = moveTextureUvRotate;
		TextureRotateTrackBar->Value = moveTextureUvRotate/(M_PI / 180.0f);
	}
	else
	{
		int value = TextureRotateTrackBar->Value;
		model->textures[currentTextureID - 1]->uvRotate = value * (M_PI / 180.0f);
		model2->textures[currentTextureID - 1]->uvRotate = value * (M_PI / 180.0f);
	}
}

void OpenGLPanel_Example::MyForm::SetUvOffsetX()
{
	if (isPaste)
	{
		model->textures[currentTextureID - 1]->uvOffsetX = moveTextureUvOffsetX;
		model2->textures[currentTextureID - 1]->uvOffsetX = moveTextureUvOffsetX;
		TextureOffsetXnumericUpDown1->Value= System::Decimal(moveTextureUvOffsetX *10.0f);
	}
	else
	{
		float value = (float)this->TextureOffsetXnumericUpDown1->Value / 10.0f;
		model->textures[currentTextureID - 1]->uvOffsetX = value;
		model2->textures[currentTextureID - 1]->uvOffsetX = value;
	}
}

void OpenGLPanel_Example::MyForm::SetUvOffsetY()
{
	if (isPaste)
	{
		model->textures[currentTextureID - 1]->uvOffsetY = moveTextureUvOffsetY;
		model2->textures[currentTextureID - 1]->uvOffsetY = moveTextureUvOffsetY;
		TextureOffsetYnumericUpDown1->Value = System::Decimal(moveTextureUvOffsetY * 10.0f);
	}
	else
	{
		float value = (float)this->TextureOffsetYnumericUpDown1->Value / 10.0f;
		model->textures[currentTextureID - 1]->uvOffsetY = value;
		model2->textures[currentTextureID - 1]->uvOffsetY = value;
	}
}

void OpenGLPanel_Example::MyForm::RenderToPickingTexture()
{
	pickingTexture->Enable();
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(paramGL->pickingShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	model->Render();
	pickingTexture->Disable();
}

void OpenGLPanel_Example::MyForm::RenderWireframeTexture()
{
	glUseProgram(paramGL->wireframeShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), true);

	for (int i = 0; i < currentTextureID; i++)
	{
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvScale"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::vec3(model->textures[i]->uvScale, model->textures[i]->uvScale, model->textures[i]->uvScale)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvRotate"), 1, GL_FALSE, glm::value_ptr(glm::rotate(model->textures[i]->uvRotate, glm::vec3(0, 0, 1)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvOffset"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0), glm::vec3(model->textures[i]->uvOffsetX, model->textures[i]->uvOffsetY, 0)))
		);

		glBindTexture(GL_TEXTURE_2D, model->textures[i]->textureID);
		model->RenderTexture(i);
	}
}

void OpenGLPanel_Example::MyForm::RenderSelectedFace()
{
	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->selectedFaceColor.r, model->selectedFaceColor.g, model->selectedFaceColor.b, model->selectedFaceColor.a);
	model->RenderSelectedFace();
}

void OpenGLPanel_Example::MyForm::RenderModelTexture()
{
	glUseProgram(paramGL->modelShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), true);

	for (int i = 0; i < currentTextureID; i++)
	{
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->modelShader, "uvScale"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::vec3(model->textures[i]->uvScale, model->textures[i]->uvScale, model->textures[i]->uvScale)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->modelShader, "uvRotate"), 1, GL_FALSE, glm::value_ptr(glm::rotate(model->textures[i]->uvRotate, glm::vec3(0, 0, 1)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvOffset"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0), glm::vec3(model->textures[i]->uvOffsetX, model->textures[i]->uvOffsetY, 0)))
		);
		glBindTexture(GL_TEXTURE_2D, model->textures[i]->textureID);
		model->RenderTexture(i);
	}
}

void OpenGLPanel_Example::MyForm::RenderWireframe()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glUseProgram(paramGL->wireframeShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);

	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->wireFaceColor.r,model->wireFaceColor.g,model->wireFaceColor.b,model->wireFaceColor.a);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "wireColor"), model->wireColor.r,model->wireColor.g,model->wireColor.b,model->wireColor.a);
	model->Render();
}

void OpenGLPanel_Example::MyForm::RenderModel()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glUseProgram(paramGL->modelShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);

	glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->modelShader, "faceColor"), model->modelFaceColor.r,model->modelFaceColor.g,model->modelFaceColor.b,model->modelFaceColor.a);
	model->Render();
}

void OpenGLPanel_Example::MyForm::ScaleSelectedFaces()
{
	if (model->selectedFace.size() > 0) {
		int id = -1;
		Paintbrush::ScaleSelectedFaces(*model, id, 1);
	}

}

void OpenGLPanel_Example::MyForm::ShrinkSelectedFaces()
{
	if (model->selectedFace.size() > 0) {
		int id = -1;
		Paintbrush::ShrinkSelectedFaces(*model, id, 1);
	}

}

