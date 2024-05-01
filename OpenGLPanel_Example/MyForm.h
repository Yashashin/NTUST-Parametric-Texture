#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Param.h"
#include "LoadShaders.h"
#include "PickingTexture.h"
#include "TextureImage.h"
#include "Texture.h"
#include "MeshObject.h"
#include <Eigen/Sparse>
#include "./camera/ViewManager.h"


#include "Paintbrush.h"

#include <windows.h>
#include <msclr\marshal_cppstd.h>

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimpleBuffer.h"


std::vector<TextureImage*> textureImages;
std::map<std::string, int> textureRecord;
std::vector<TextureImage*> uvMapTextures;
std::vector<std::string> texturePaths;
std::string modelPath;
std::vector<int> selectedTextureRecord;



ViewManager main_camera, tex_camera;
enum RenderMode {
	WIREFRAME,
	NORMAL
};

enum SelectMode {
	PICKFACE,
	DELFACE,
	NONE
};

enum MouseState {
	PRESS,
	RELEASE,
};


namespace OpenGLPanel_Example
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::IO;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Button^ button1;
	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;






	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl2;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ modelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelImportToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^ textureToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ TextureImportToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripButton^ PaintADDStripButton1;

	private: System::Windows::Forms::ToolStrip^ toolStrip1;
	private: System::Windows::Forms::ToolStripButton^ PaintDELStripButton2;




	private: System::Windows::Forms::ToolStripButton^ PaintSizeScaleStripButton3;
	private: System::Windows::Forms::ToolStripButton^ PaintSizeShrinkStripButton4;


	private: System::Windows::Forms::ToolStripDropDownButton^ toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^ wireframeToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ normalToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripButton^ PaintFaceScaleStripButton2;
	private: System::Windows::Forms::ToolStripButton^ PaintFaceShrinkStripButton1;



	private: System::Windows::Forms::ToolStripMenuItem^ FaceColorToolStripMenuItem;

	private: System::Windows::Forms::ColorDialog^ colorDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^ selectedFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ hoverFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ wireFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ wireToolStripMenuItem;

	private: System::Windows::Forms::NumericUpDown^ TextureScalenumericUpDown1;

	private: System::Windows::Forms::Label^ ScaleLabel;
	private: System::Windows::Forms::TrackBar^ TextureScaleTrackBar;
	private: System::Windows::Forms::NumericUpDown^ TextureRotatenumericUpDown1;
	private: System::Windows::Forms::Label^ RotateLabel;
	private: System::Windows::Forms::TrackBar^ TextureRotateTrackBar;
	private: System::Windows::Forms::NumericUpDown^ TextureOffsetYnumericUpDown1;

	private: System::Windows::Forms::Label^ OffsetYLabel;
	private: System::Windows::Forms::TrackBar^ TextureOffsetYTrackbar;

	private: System::Windows::Forms::NumericUpDown^ TextureOffsetXnumericUpDown1;

	private: System::Windows::Forms::Label^ OffsetXLabel;
	private: System::Windows::Forms::TrackBar^ TextureOffsetXTrackbar;

	private: System::Windows::Forms::Button^ TextureRESETButton;
	private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog2;
	private: System::Windows::Forms::ToolStripMenuItem^ modelSaveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelLoadStripMenuItem1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog3;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Button^ button2;







	protected:

		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code

	private:
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->modelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelImportToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelLoadStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelSaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TextureImportToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PaintADDStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->PaintDELStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintSizeScaleStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintSizeShrinkStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintFaceScaleStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintFaceShrinkStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->wireframeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normalToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FaceColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->selectedFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hoverFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wireFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wireToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->TextureRESETButton = (gcnew System::Windows::Forms::Button());
			this->TextureOffsetYnumericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->OffsetYLabel = (gcnew System::Windows::Forms::Label());
			this->TextureOffsetYTrackbar = (gcnew System::Windows::Forms::TrackBar());
			this->TextureOffsetXnumericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->OffsetXLabel = (gcnew System::Windows::Forms::Label());
			this->TextureOffsetXTrackbar = (gcnew System::Windows::Forms::TrackBar());
			this->TextureRotatenumericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->RotateLabel = (gcnew System::Windows::Forms::Label());
			this->TextureRotateTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->TextureScalenumericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->ScaleLabel = (gcnew System::Windows::Forms::Label());
			this->TextureScaleTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog3 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetYnumericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetYTrackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetXnumericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetXTrackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureRotatenumericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureRotateTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureScalenumericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureScaleTrackBar))->BeginInit();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 16;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->button1->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(932, 302);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(270, 34);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Show Texture";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::SolveUV_Click);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Cursor = System::Windows::Forms::Cursors::Cross;
			this->hkoglPanelControl1->Location = System::Drawing::Point(10, 24);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(916, 521);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnMouseWheel);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// hkoglPanelControl2
			// 
			hkcoglPanelCameraSetting2->Far = 1000;
			hkcoglPanelCameraSetting2->Fov = 45;
			hkcoglPanelCameraSetting2->Near = -1000;
			hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting2;
			this->hkoglPanelControl2->Location = System::Drawing::Point(932, 24);
			this->hkoglPanelControl2->Margin = System::Windows::Forms::Padding(2);
			this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
			hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat2;
			this->hkoglPanelControl2->Size = System::Drawing::Size(270, 274);
			this->hkoglPanelControl2->TabIndex = 11;
			this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl2_Load);
			this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl2_Paint);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->modelToolStripMenuItem,
					this->textureToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1213, 24);
			this->menuStrip1->TabIndex = 12;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// modelToolStripMenuItem
			// 
			this->modelToolStripMenuItem->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->modelToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->modelImportToolStripMenuItem,
					this->modelLoadStripMenuItem1, this->modelSaveToolStripMenuItem
			});
			this->modelToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->modelToolStripMenuItem->Name = L"modelToolStripMenuItem";
			this->modelToolStripMenuItem->Size = System::Drawing::Size(57, 20);
			this->modelToolStripMenuItem->Text = L"Model";
			this->modelToolStripMenuItem->ToolTipText = L"Model tool";
			// 
			// modelImportToolStripMenuItem
			// 
			this->modelImportToolStripMenuItem->BackColor = System::Drawing::SystemColors::Control;
			this->modelImportToolStripMenuItem->ForeColor = System::Drawing::Color::Black;
			this->modelImportToolStripMenuItem->Name = L"modelImportToolStripMenuItem";
			this->modelImportToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->modelImportToolStripMenuItem->Text = L"Import Obj";
			this->modelImportToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ModelImportToolStripMenuItem_Click);
			// 
			// modelLoadStripMenuItem1
			// 
			this->modelLoadStripMenuItem1->BackColor = System::Drawing::SystemColors::Control;
			this->modelLoadStripMenuItem1->ForeColor = System::Drawing::Color::Black;
			this->modelLoadStripMenuItem1->Name = L"modelLoadStripMenuItem1";
			this->modelLoadStripMenuItem1->Size = System::Drawing::Size(136, 22);
			this->modelLoadStripMenuItem1->Text = L"Load File";
			this->modelLoadStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::ModelLoadToolStripMenuItem_Click);
			// 
			// modelSaveToolStripMenuItem
			// 
			this->modelSaveToolStripMenuItem->BackColor = System::Drawing::SystemColors::Control;
			this->modelSaveToolStripMenuItem->ForeColor = System::Drawing::Color::Black;
			this->modelSaveToolStripMenuItem->Name = L"modelSaveToolStripMenuItem";
			this->modelSaveToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->modelSaveToolStripMenuItem->Text = L"Save File";
			this->modelSaveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ModelSaveToolStripMenuItem_Click);
			// 
			// textureToolStripMenuItem
			// 
			this->textureToolStripMenuItem->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->textureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->TextureImportToolStripMenuItem1 });
			this->textureToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->textureToolStripMenuItem->Name = L"textureToolStripMenuItem";
			this->textureToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->textureToolStripMenuItem->Text = L"Texture";
			this->textureToolStripMenuItem->ToolTipText = L"Texture tool";
			// 
			// TextureImportToolStripMenuItem1
			// 
			this->TextureImportToolStripMenuItem1->BackColor = System::Drawing::SystemColors::Control;
			this->TextureImportToolStripMenuItem1->ForeColor = System::Drawing::Color::Black;
			this->TextureImportToolStripMenuItem1->Name = L"TextureImportToolStripMenuItem1";
			this->TextureImportToolStripMenuItem1->Size = System::Drawing::Size(112, 22);
			this->TextureImportToolStripMenuItem1->Text = L"Import";
			this->TextureImportToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::Load_Texture_Btn_Click);
			// 
			// PaintADDStripButton1
			// 
			this->PaintADDStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintADDStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintADDStripButton1.Image")));
			this->PaintADDStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintADDStripButton1->Name = L"PaintADDStripButton1";
			this->PaintADDStripButton1->Size = System::Drawing::Size(24, 24);
			this->PaintADDStripButton1->Tag = L"0";
			this->PaintADDStripButton1->Text = L"toolStripButton1";
			this->PaintADDStripButton1->ToolTipText = L"pick faces brush";
			this->PaintADDStripButton1->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->toolStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->toolStrip1->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->PaintADDStripButton1,
					this->PaintDELStripButton2, this->PaintSizeScaleStripButton3, this->PaintSizeShrinkStripButton4, this->PaintFaceScaleStripButton2,
					this->PaintFaceShrinkStripButton1, this->toolStripDropDownButton1
			});
			this->toolStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->toolStrip1->Location = System::Drawing::Point(106, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->toolStrip1->Size = System::Drawing::Size(178, 27);
			this->toolStrip1->TabIndex = 13;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// PaintDELStripButton2
			// 
			this->PaintDELStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintDELStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintDELStripButton2.Image")));
			this->PaintDELStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintDELStripButton2->Name = L"PaintDELStripButton2";
			this->PaintDELStripButton2->Size = System::Drawing::Size(24, 24);
			this->PaintDELStripButton2->Tag = L"1";
			this->PaintDELStripButton2->Text = L"toolStripButton2";
			this->PaintDELStripButton2->ToolTipText = L"erase faces brush";
			this->PaintDELStripButton2->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintSizeScaleStripButton3
			// 
			this->PaintSizeScaleStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintSizeScaleStripButton3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintSizeScaleStripButton3.Image")));
			this->PaintSizeScaleStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintSizeScaleStripButton3->Name = L"PaintSizeScaleStripButton3";
			this->PaintSizeScaleStripButton3->Size = System::Drawing::Size(24, 24);
			this->PaintSizeScaleStripButton3->Tag = L"2";
			this->PaintSizeScaleStripButton3->Text = L"toolStripButton3";
			this->PaintSizeScaleStripButton3->ToolTipText = L"scale current brush size";
			this->PaintSizeScaleStripButton3->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintSizeShrinkStripButton4
			// 
			this->PaintSizeShrinkStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintSizeShrinkStripButton4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintSizeShrinkStripButton4.Image")));
			this->PaintSizeShrinkStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintSizeShrinkStripButton4->Name = L"PaintSizeShrinkStripButton4";
			this->PaintSizeShrinkStripButton4->Size = System::Drawing::Size(24, 24);
			this->PaintSizeShrinkStripButton4->Tag = L"3";
			this->PaintSizeShrinkStripButton4->Text = L"toolStripButton4";
			this->PaintSizeShrinkStripButton4->ToolTipText = L"shrink brush size";
			this->PaintSizeShrinkStripButton4->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintFaceScaleStripButton2
			// 
			this->PaintFaceScaleStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintFaceScaleStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintFaceScaleStripButton2.Image")));
			this->PaintFaceScaleStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintFaceScaleStripButton2->Name = L"PaintFaceScaleStripButton2";
			this->PaintFaceScaleStripButton2->Size = System::Drawing::Size(24, 24);
			this->PaintFaceScaleStripButton2->Tag = L"4";
			this->PaintFaceScaleStripButton2->Text = L"toolStripButton2";
			this->PaintFaceScaleStripButton2->ToolTipText = L"scale selected faces";
			this->PaintFaceScaleStripButton2->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintFaceShrinkStripButton1
			// 
			this->PaintFaceShrinkStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintFaceShrinkStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintFaceShrinkStripButton1.Image")));
			this->PaintFaceShrinkStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintFaceShrinkStripButton1->Name = L"PaintFaceShrinkStripButton1";
			this->PaintFaceShrinkStripButton1->Size = System::Drawing::Size(24, 24);
			this->PaintFaceShrinkStripButton1->Tag = L"5";
			this->PaintFaceShrinkStripButton1->Text = L"toolStripButton1";
			this->PaintFaceShrinkStripButton1->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// toolStripDropDownButton1
			// 
			this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripDropDownButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->wireframeToolStripMenuItem1,
					this->normalToolStripMenuItem1, this->FaceColorToolStripMenuItem
			});
			this->toolStripDropDownButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripDropDownButton1.Image")));
			this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->Size = System::Drawing::Size(33, 24);
			this->toolStripDropDownButton1->Text = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->ToolTipText = L"ViewTool";
			// 
			// wireframeToolStripMenuItem1
			// 
			this->wireframeToolStripMenuItem1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->wireframeToolStripMenuItem1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"wireframeToolStripMenuItem1.Image")));
			this->wireframeToolStripMenuItem1->Name = L"wireframeToolStripMenuItem1";
			this->wireframeToolStripMenuItem1->Size = System::Drawing::Size(152, 26);
			this->wireframeToolStripMenuItem1->Tag = L"0";
			this->wireframeToolStripMenuItem1->Text = L"Wireframe";
			this->wireframeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::RenderModeToolStripMenuItem_Click);
			// 
			// normalToolStripMenuItem1
			// 
			this->normalToolStripMenuItem1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->normalToolStripMenuItem1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"normalToolStripMenuItem1.Image")));
			this->normalToolStripMenuItem1->Name = L"normalToolStripMenuItem1";
			this->normalToolStripMenuItem1->Size = System::Drawing::Size(152, 26);
			this->normalToolStripMenuItem1->Tag = L"1";
			this->normalToolStripMenuItem1->Text = L"Normal";
			this->normalToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::RenderModeToolStripMenuItem_Click);
			// 
			// FaceColorToolStripMenuItem
			// 
			this->FaceColorToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->selectedFaceToolStripMenuItem,
					this->hoverFaceToolStripMenuItem, this->modelFaceToolStripMenuItem, this->wireFaceToolStripMenuItem, this->wireToolStripMenuItem
			});
			this->FaceColorToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"FaceColorToolStripMenuItem.Image")));
			this->FaceColorToolStripMenuItem->Name = L"FaceColorToolStripMenuItem";
			this->FaceColorToolStripMenuItem->Size = System::Drawing::Size(152, 26);
			this->FaceColorToolStripMenuItem->Text = L"Color Setting";
			// 
			// selectedFaceToolStripMenuItem
			// 
			this->selectedFaceToolStripMenuItem->Name = L"selectedFaceToolStripMenuItem";
			this->selectedFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->selectedFaceToolStripMenuItem->Tag = L"0";
			this->selectedFaceToolStripMenuItem->Text = L"Selected Face";
			this->selectedFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// hoverFaceToolStripMenuItem
			// 
			this->hoverFaceToolStripMenuItem->Name = L"hoverFaceToolStripMenuItem";
			this->hoverFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->hoverFaceToolStripMenuItem->Tag = L"1";
			this->hoverFaceToolStripMenuItem->Text = L"Hover Face";
			this->hoverFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// modelFaceToolStripMenuItem
			// 
			this->modelFaceToolStripMenuItem->Name = L"modelFaceToolStripMenuItem";
			this->modelFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->modelFaceToolStripMenuItem->Tag = L"2";
			this->modelFaceToolStripMenuItem->Text = L"Model Face";
			this->modelFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// wireFaceToolStripMenuItem
			// 
			this->wireFaceToolStripMenuItem->Name = L"wireFaceToolStripMenuItem";
			this->wireFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->wireFaceToolStripMenuItem->Tag = L"3";
			this->wireFaceToolStripMenuItem->Text = L"Wire Face";
			this->wireFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// wireToolStripMenuItem
			// 
			this->wireToolStripMenuItem->Name = L"wireToolStripMenuItem";
			this->wireToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->wireToolStripMenuItem->Tag = L"4";
			this->wireToolStripMenuItem->Text = L"Wire Line";
			this->wireToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// colorDialog1
			// 
			this->colorDialog1->FullOpen = true;
			// 
			// TextureRESETButton
			// 
			this->TextureRESETButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->TextureRESETButton->FlatAppearance->BorderSize = 0;
			this->TextureRESETButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TextureRESETButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->TextureRESETButton->ForeColor = System::Drawing::Color::White;
			this->TextureRESETButton->Location = System::Drawing::Point(12, 110);
			this->TextureRESETButton->Margin = System::Windows::Forms::Padding(2);
			this->TextureRESETButton->Name = L"TextureRESETButton";
			this->TextureRESETButton->Size = System::Drawing::Size(258, 22);
			this->TextureRESETButton->TabIndex = 12;
			this->TextureRESETButton->Text = L"RESET";
			this->TextureRESETButton->UseVisualStyleBackColor = false;
			this->TextureRESETButton->Click += gcnew System::EventHandler(this, &MyForm::TextureRESETButton_Click);
			// 
			// TextureOffsetYnumericUpDown1
			// 
			this->TextureOffsetYnumericUpDown1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureOffsetYnumericUpDown1->DecimalPlaces = 2;
			this->TextureOffsetYnumericUpDown1->ForeColor = System::Drawing::Color::White;
			this->TextureOffsetYnumericUpDown1->Location = System::Drawing::Point(216, 86);
			this->TextureOffsetYnumericUpDown1->Margin = System::Windows::Forms::Padding(2);
			this->TextureOffsetYnumericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->TextureOffsetYnumericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->TextureOffsetYnumericUpDown1->Name = L"TextureOffsetYnumericUpDown1";
			this->TextureOffsetYnumericUpDown1->Size = System::Drawing::Size(54, 22);
			this->TextureOffsetYnumericUpDown1->TabIndex = 11;
			this->TextureOffsetYnumericUpDown1->Tag = L"3";
			this->TextureOffsetYnumericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::TextureBox_Numeric);
			// 
			// OffsetYLabel
			// 
			this->OffsetYLabel->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->OffsetYLabel->ForeColor = System::Drawing::Color::White;
			this->OffsetYLabel->Location = System::Drawing::Point(9, 83);
			this->OffsetYLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->OffsetYLabel->Name = L"OffsetYLabel";
			this->OffsetYLabel->Size = System::Drawing::Size(52, 18);
			this->OffsetYLabel->TabIndex = 10;
			this->OffsetYLabel->Text = L"Offset Y";
			this->OffsetYLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// TextureOffsetYTrackbar
			// 
			this->TextureOffsetYTrackbar->AutoSize = false;
			this->TextureOffsetYTrackbar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureOffsetYTrackbar->LargeChange = 100;
			this->TextureOffsetYTrackbar->Location = System::Drawing::Point(61, 86);
			this->TextureOffsetYTrackbar->Margin = System::Windows::Forms::Padding(2);
			this->TextureOffsetYTrackbar->Maximum = 1000;
			this->TextureOffsetYTrackbar->Minimum = -1000;
			this->TextureOffsetYTrackbar->Name = L"TextureOffsetYTrackbar";
			this->TextureOffsetYTrackbar->Size = System::Drawing::Size(151, 17);
			this->TextureOffsetYTrackbar->SmallChange = 10;
			this->TextureOffsetYTrackbar->TabIndex = 9;
			this->TextureOffsetYTrackbar->Tag = L"3";
			this->TextureOffsetYTrackbar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->TextureOffsetYTrackbar->Scroll += gcnew System::EventHandler(this, &MyForm::TextureBox_Scroll);
			// 
			// TextureOffsetXnumericUpDown1
			// 
			this->TextureOffsetXnumericUpDown1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureOffsetXnumericUpDown1->DecimalPlaces = 2;
			this->TextureOffsetXnumericUpDown1->ForeColor = System::Drawing::Color::White;
			this->TextureOffsetXnumericUpDown1->Location = System::Drawing::Point(216, 61);
			this->TextureOffsetXnumericUpDown1->Margin = System::Windows::Forms::Padding(2);
			this->TextureOffsetXnumericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->TextureOffsetXnumericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->TextureOffsetXnumericUpDown1->Name = L"TextureOffsetXnumericUpDown1";
			this->TextureOffsetXnumericUpDown1->Size = System::Drawing::Size(54, 22);
			this->TextureOffsetXnumericUpDown1->TabIndex = 8;
			this->TextureOffsetXnumericUpDown1->Tag = L"2";
			this->TextureOffsetXnumericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::TextureBox_Numeric);
			// 
			// OffsetXLabel
			// 
			this->OffsetXLabel->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->OffsetXLabel->ForeColor = System::Drawing::Color::White;
			this->OffsetXLabel->Location = System::Drawing::Point(9, 58);
			this->OffsetXLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->OffsetXLabel->Name = L"OffsetXLabel";
			this->OffsetXLabel->Size = System::Drawing::Size(52, 18);
			this->OffsetXLabel->TabIndex = 7;
			this->OffsetXLabel->Text = L"Offset X";
			this->OffsetXLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// TextureOffsetXTrackbar
			// 
			this->TextureOffsetXTrackbar->AutoSize = false;
			this->TextureOffsetXTrackbar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureOffsetXTrackbar->LargeChange = 100;
			this->TextureOffsetXTrackbar->Location = System::Drawing::Point(61, 61);
			this->TextureOffsetXTrackbar->Margin = System::Windows::Forms::Padding(2);
			this->TextureOffsetXTrackbar->Maximum = 1000;
			this->TextureOffsetXTrackbar->Minimum = -1000;
			this->TextureOffsetXTrackbar->Name = L"TextureOffsetXTrackbar";
			this->TextureOffsetXTrackbar->Size = System::Drawing::Size(151, 17);
			this->TextureOffsetXTrackbar->SmallChange = 10;
			this->TextureOffsetXTrackbar->TabIndex = 6;
			this->TextureOffsetXTrackbar->Tag = L"2";
			this->TextureOffsetXTrackbar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->TextureOffsetXTrackbar->Scroll += gcnew System::EventHandler(this, &MyForm::TextureBox_Scroll);
			// 
			// TextureRotatenumericUpDown1
			// 
			this->TextureRotatenumericUpDown1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureRotatenumericUpDown1->ForeColor = System::Drawing::Color::White;
			this->TextureRotatenumericUpDown1->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 30, 0, 0, 0 });
			this->TextureRotatenumericUpDown1->Location = System::Drawing::Point(216, 36);
			this->TextureRotatenumericUpDown1->Margin = System::Windows::Forms::Padding(2);
			this->TextureRotatenumericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, 0 });
			this->TextureRotatenumericUpDown1->Name = L"TextureRotatenumericUpDown1";
			this->TextureRotatenumericUpDown1->Size = System::Drawing::Size(54, 22);
			this->TextureRotatenumericUpDown1->TabIndex = 5;
			this->TextureRotatenumericUpDown1->Tag = L"1";
			this->TextureRotatenumericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::TextureBox_Numeric);
			// 
			// RotateLabel
			// 
			this->RotateLabel->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->RotateLabel->ForeColor = System::Drawing::Color::White;
			this->RotateLabel->Location = System::Drawing::Point(9, 34);
			this->RotateLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->RotateLabel->Name = L"RotateLabel";
			this->RotateLabel->Size = System::Drawing::Size(52, 18);
			this->RotateLabel->TabIndex = 4;
			this->RotateLabel->Text = L"Rotate";
			this->RotateLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// TextureRotateTrackBar
			// 
			this->TextureRotateTrackBar->AutoSize = false;
			this->TextureRotateTrackBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureRotateTrackBar->LargeChange = 90;
			this->TextureRotateTrackBar->Location = System::Drawing::Point(61, 36);
			this->TextureRotateTrackBar->Margin = System::Windows::Forms::Padding(2);
			this->TextureRotateTrackBar->Maximum = 360;
			this->TextureRotateTrackBar->Name = L"TextureRotateTrackBar";
			this->TextureRotateTrackBar->Size = System::Drawing::Size(151, 17);
			this->TextureRotateTrackBar->SmallChange = 10;
			this->TextureRotateTrackBar->TabIndex = 3;
			this->TextureRotateTrackBar->Tag = L"1";
			this->TextureRotateTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->TextureRotateTrackBar->Scroll += gcnew System::EventHandler(this, &MyForm::TextureBox_Scroll);
			// 
			// TextureScalenumericUpDown1
			// 
			this->TextureScalenumericUpDown1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureScalenumericUpDown1->DecimalPlaces = 2;
			this->TextureScalenumericUpDown1->ForeColor = System::Drawing::Color::White;
			this->TextureScalenumericUpDown1->Location = System::Drawing::Point(216, 11);
			this->TextureScalenumericUpDown1->Margin = System::Windows::Forms::Padding(2);
			this->TextureScalenumericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->TextureScalenumericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->TextureScalenumericUpDown1->Name = L"TextureScalenumericUpDown1";
			this->TextureScalenumericUpDown1->Size = System::Drawing::Size(54, 22);
			this->TextureScalenumericUpDown1->TabIndex = 2;
			this->TextureScalenumericUpDown1->Tag = L"0";
			this->TextureScalenumericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->TextureScalenumericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::TextureBox_Numeric);
			// 
			// ScaleLabel
			// 
			this->ScaleLabel->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->ScaleLabel->ForeColor = System::Drawing::Color::White;
			this->ScaleLabel->Location = System::Drawing::Point(9, 9);
			this->ScaleLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->ScaleLabel->Name = L"ScaleLabel";
			this->ScaleLabel->Size = System::Drawing::Size(52, 18);
			this->ScaleLabel->TabIndex = 1;
			this->ScaleLabel->Text = L"Scale";
			this->ScaleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// TextureScaleTrackBar
			// 
			this->TextureScaleTrackBar->AutoSize = false;
			this->TextureScaleTrackBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->TextureScaleTrackBar->LargeChange = 100;
			this->TextureScaleTrackBar->Location = System::Drawing::Point(61, 11);
			this->TextureScaleTrackBar->Margin = System::Windows::Forms::Padding(2);
			this->TextureScaleTrackBar->Maximum = 1000;
			this->TextureScaleTrackBar->Minimum = 1;
			this->TextureScaleTrackBar->Name = L"TextureScaleTrackBar";
			this->TextureScaleTrackBar->Size = System::Drawing::Size(151, 17);
			this->TextureScaleTrackBar->SmallChange = 10;
			this->TextureScaleTrackBar->TabIndex = 0;
			this->TextureScaleTrackBar->Tag = L"0";
			this->TextureScaleTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->TextureScaleTrackBar->Value = 100;
			this->TextureScaleTrackBar->Scroll += gcnew System::EventHandler(this, &MyForm::TextureBox_Scroll);
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->AutoScroll = true;
			this->flowLayoutPanel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)), static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->flowLayoutPanel1->Location = System::Drawing::Point(9, 550);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(1192, 162);
			this->flowLayoutPanel1->TabIndex = 17;
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->FileName = L"openFileDialog3";
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->TextureOffsetYnumericUpDown1);
			this->panel1->Controls->Add(this->TextureRESETButton);
			this->panel1->Controls->Add(this->TextureOffsetXnumericUpDown1);
			this->panel1->Controls->Add(this->ScaleLabel);
			this->panel1->Controls->Add(this->TextureRotatenumericUpDown1);
			this->panel1->Controls->Add(this->TextureScaleTrackBar);
			this->panel1->Controls->Add(this->TextureOffsetYTrackbar);
			this->panel1->Controls->Add(this->OffsetYLabel);
			this->panel1->Controls->Add(this->TextureScalenumericUpDown1);
			this->panel1->Controls->Add(this->TextureOffsetXTrackbar);
			this->panel1->Controls->Add(this->RotateLabel);
			this->panel1->Controls->Add(this->OffsetXLabel);
			this->panel1->Controls->Add(this->TextureRotateTrackBar);
			this->panel1->Location = System::Drawing::Point(932, 339);
			this->panel1->Margin = System::Windows::Forms::Padding(2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(281, 206);
			this->panel1->TabIndex = 18;
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->button2->FlatAppearance->BorderSize = 0;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->button2->ForeColor = System::Drawing::Color::White;
			this->button2->Location = System::Drawing::Point(11, 136);
			this->button2->Margin = System::Windows::Forms::Padding(2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(258, 22);
			this->button2->TabIndex = 13;
			this->button2->Text = L"MOVE\r\n\r\n";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::Move_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->ClientSize = System::Drawing::Size(1213, 720);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->hkoglPanelControl2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Texture Editor";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetYnumericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetYTrackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetXnumericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureOffsetXTrackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureRotatenumericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureRotateTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureScalenumericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureScaleTrackBar))->EndInit();
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
		//--------------EVENT--------------------
		//form1
		void OnMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e);
		void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

		//form2
		void hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e);
		void hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);

		//component
		//button
		void Load_Texture_Btn_Click(System::Object^ sender, System::EventArgs^ e);//load texture
		void SolveUV_Click(System::Object^ sender, System::EventArgs^ e);//solve uv button
		void Picture_Click(System::Object^ sender, System::EventArgs^ e);//picture box setting
		void PaintbrushStripButton_Click(System::Object^ sender, System::EventArgs^ e);//brush mode
		void RenderModeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);//render mode
		void FaceColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);//custom color
		void TextureRESETButton_Click(System::Object^ sender, System::EventArgs^ e);
		void Move_Click(System::Object^ sender, System::EventArgs^ e);
		void ModelImportToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		void ModelLoadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		void ModelSaveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		//picture box
		void CreatePictureBox(String^ FileName);
		void PictureBoxMouseLeave(System::Object^ sender, System::EventArgs^ e);
		void PictureBoxMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void PictureBoxMouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void PictureBoxPaint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
//		void PictureBox
		//scroll bar
		void TextureBox_Scroll(System::Object^ sender, System::EventArgs^ e);
		//numericdown
		void TextureBox_Numeric(System::Object^ sender, System::EventArgs^ e);
		//timer
		void timer1_Tick(System::Object^ sender, System::EventArgs^ e);
		//---------------END--------------------

		//---------------CUSTOM-----------------
		//file custom
		void LoadFile(std::string path);
		void SaveFile(std::string path);
		//Texture
		void AddTexture(const char* fileName);
		void SolveUV();
		void SetUvScale();
		void SetUvRotate();
		void SetUvOffsetX();
		void SetUvOffsetY();
		//Render method
		void RenderToPickingTexture();
		void RenderWireframeTexture();
		void RenderSelectedFace();
		void RenderModelTexture();
		void RenderWireframe();
		void RenderModel();

		//paintbrush
		//1. paint brush size -> recurrence times
		//2. first iterate current selected face
		//3. put it into already selected face -> will cause only one texture on it.
		void ScaleSelectedFaces();
		void ShrinkSelectedFaces();
		//---------------END--------------------

	//member field (variable)
	private:
		//picturebox pointer
		System::Windows::Forms::PictureBox^ pictureBoxPtr = nullptr;

		//record last path
		String^ LastTextureDirectory;
		String^ LastModelDirectory;
		String^ LastSaveDirectory;

		//form2
		ParamGL* paramGL2 = nullptr;
		MeshObject* model2 = nullptr;

		//form1
		ParamGL* paramGL = nullptr;
		MeshObject* model = nullptr;
		PickingTexture* pickingTexture = nullptr;
		int currentTextureID;
		int selectTextureImage;
		RenderMode renderMode; //to indicate render mode initize it by load function
		SelectMode selectMode = SelectMode::PICKFACE;//to indicate picking faces or delete faces mode
		MouseState mouseState = MouseState::RELEASE; //to indicate mouse state
		float eye_x = 0.0;
		bool isMoveMode=false;
		bool isPaste = false;
		unsigned int moveTextureId;
		float moveTextureUvRotate;
		float moveTextureUvScale;
		float moveTextureUvOffsetX;
		float moveTextureUvOffsetY;

		//paint brush
		Paintbrush* addBrush = NULL; //pick faces brush
		Paintbrush* delBrush = NULL; //delete faces brush
		Paintbrush* currentBrush = NULL; //brush control to trigger event 




};

}
