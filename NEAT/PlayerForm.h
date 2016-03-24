#pragma once

#include <cmath>

namespace GUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class PlayerForm : public System::Windows::Forms::Form
	{
	public:
		PlayerForm();

		System::Void platform_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		System::Void keyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
		System::Void keyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
		System::Void formClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
		System::Void drawGenome(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);

		void startGame();
		void gameClock();
		void updateGame();
		void move();
		void jump();
		void fall();
		void gameOver();
		void gameWin();
		void restartGame();
		void moveEnemies();
		void checkCollisionEnemy();
		bool getGameRunning();

		PictureBox^ getCharacter();
		PictureBox^ getEnemy1();
		PictureBox^ getEnemy2();

		PictureBox^ checkCollisionDown(int);
		PictureBox^ checkCollisionDownAI(int);
		PictureBox^ checkCollisionUp(int);
		PictureBox^ checkCollisionRight(int);
		PictureBox^ checkCollisionLeft(int);
		
		delegate void CharacterDelegate();
		delegate PictureBox^ CharacterDelegatePB(int);
		delegate void EnemyDelegate();
		delegate void LabelDelegate();	
		delegate void GenomeDelegate();

		System::Threading::Thread^ getThread();
		void setThread(System::Threading::Thread^);
		void setControls(Generic::List<bool>^);
		List<int>^ getCellX();
		List<int>^ getCellY();
		List<double>^ getCellValue();
		List<int>^ getInCell();
		List<int>^ getOutCell();
		List<double>^ getGeneWeight();
		void updateDisplay();
		List<double>^ getLabelValues();
				
	protected:
		~PlayerForm();

	private: 
		System::ComponentModel::Container ^components;
		System::Windows::Forms::PictureBox^  platform3;	
		System::Windows::Forms::PictureBox^  character;
		System::Windows::Forms::PictureBox^  leftBoundary;
		System::Windows::Forms::Label^  gameOverLabel;
		System::Windows::Forms::Label^  playAgainLabel;
		System::Windows::Forms::PictureBox^  platform2;
		System::Windows::Forms::PictureBox^  black1;
		System::Windows::Forms::PictureBox^  platform4;
		System::Windows::Forms::PictureBox^  platform5;
		System::Windows::Forms::PictureBox^  enemy1;
		System::Windows::Forms::PictureBox^  platform6;
		System::Windows::Forms::PictureBox^  platform7;
		System::Windows::Forms::PictureBox^  platform8;
		System::Windows::Forms::PictureBox^  enemy2;
		System::Windows::Forms::PictureBox^  platform9;
		System::Windows::Forms::Label^  youWinLabel;
		System::Windows::Forms::PictureBox^  platform1;
		System::Windows::Forms::PictureBox^  genome;
		System::Windows::Forms::Label^  playerX;
		System::Windows::Forms::Label^  playerY;
		System::Windows::Forms::Label^  enemy1X;
		System::Windows::Forms::Label^  npc2X;
		System::Windows::Forms::Label^  bias;
		System::Windows::Forms::Label^  space;
		System::Windows::Forms::Label^  left;
		System::Windows::Forms::Label^  right;
		System::Windows::Forms::Label^  population;
		System::Windows::Forms::Label^  species;
		System::Windows::Forms::Label^  genomeLabel;
		System::Windows::Forms::Label^  maxFitness;
	private: System::Windows::Forms::Label^  generation;
	private: System::Windows::Forms::Label^  fitness;


		
		bool moveRight;
		bool moveLeft;
		bool jumping;
		bool falling;
		bool gameRunning;
		bool movingRight1;
		bool movingRight2;
		int jumpStartHeight;
		int fallStartHeight;

		List<int>^ cellX;
		List<int>^ cellY;
		List<double>^ cellValue;
		List<int>^ inCell;
		List<int>^ outCell;
		List<double>^ geneWeight;
		List<double>^ labelValues;
	
		System::Threading::Thread^ thread;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->platform1 = (gcnew System::Windows::Forms::PictureBox());
			this->platform3 = (gcnew System::Windows::Forms::PictureBox());
			this->character = (gcnew System::Windows::Forms::PictureBox());
			this->leftBoundary = (gcnew System::Windows::Forms::PictureBox());
			this->gameOverLabel = (gcnew System::Windows::Forms::Label());
			this->playAgainLabel = (gcnew System::Windows::Forms::Label());
			this->platform2 = (gcnew System::Windows::Forms::PictureBox());
			this->black1 = (gcnew System::Windows::Forms::PictureBox());
			this->platform4 = (gcnew System::Windows::Forms::PictureBox());
			this->platform5 = (gcnew System::Windows::Forms::PictureBox());
			this->enemy1 = (gcnew System::Windows::Forms::PictureBox());
			this->platform6 = (gcnew System::Windows::Forms::PictureBox());
			this->platform7 = (gcnew System::Windows::Forms::PictureBox());
			this->platform8 = (gcnew System::Windows::Forms::PictureBox());
			this->enemy2 = (gcnew System::Windows::Forms::PictureBox());
			this->platform9 = (gcnew System::Windows::Forms::PictureBox());
			this->youWinLabel = (gcnew System::Windows::Forms::Label());
			this->genome = (gcnew System::Windows::Forms::PictureBox());
			this->playerX = (gcnew System::Windows::Forms::Label());
			this->playerY = (gcnew System::Windows::Forms::Label());
			this->enemy1X = (gcnew System::Windows::Forms::Label());
			this->npc2X = (gcnew System::Windows::Forms::Label());
			this->bias = (gcnew System::Windows::Forms::Label());
			this->space = (gcnew System::Windows::Forms::Label());
			this->left = (gcnew System::Windows::Forms::Label());
			this->right = (gcnew System::Windows::Forms::Label());
			this->population = (gcnew System::Windows::Forms::Label());
			this->species = (gcnew System::Windows::Forms::Label());
			this->genomeLabel = (gcnew System::Windows::Forms::Label());
			this->maxFitness = (gcnew System::Windows::Forms::Label());
			this->generation = (gcnew System::Windows::Forms::Label());
			this->fitness = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->character))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftBoundary))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->black1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enemy1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enemy2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->genome))->BeginInit();
			this->SuspendLayout();
			// 
			// platform1
			// 
			this->platform1->Location = System::Drawing::Point(-2, 544);
			this->platform1->Name = L"platform1";
			this->platform1->Size = System::Drawing::Size(129, 195);
			this->platform1->TabIndex = 0;
			this->platform1->TabStop = false;
			this->platform1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// platform3
			// 
			this->platform3->Location = System::Drawing::Point(284, 485);
			this->platform3->Name = L"platform3";
			this->platform3->Size = System::Drawing::Size(141, 254);
			this->platform3->TabIndex = 2;
			this->platform3->TabStop = false;
			this->platform3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// character
			// 
			this->character->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)),
				static_cast<System::Int32>(static_cast<System::Byte>(100)));
			this->character->Location = System::Drawing::Point(16, 519);
			this->character->Name = L"character";
			this->character->Size = System::Drawing::Size(25, 25);
			this->character->TabIndex = 3;
			this->character->TabStop = false;
			// 
			// leftBoundary
			// 
			this->leftBoundary->Location = System::Drawing::Point(-1, 0);
			this->leftBoundary->Name = L"leftBoundary";
			this->leftBoundary->Size = System::Drawing::Size(1, 1);
			this->leftBoundary->TabIndex = 4;
			this->leftBoundary->TabStop = false;
			// 
			// gameOverLabel
			// 
			this->gameOverLabel->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->gameOverLabel->AutoSize = true;
			this->gameOverLabel->BackColor = System::Drawing::Color::Transparent;
			this->gameOverLabel->Font = (gcnew System::Drawing::Font(L"Emulogic", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->gameOverLabel->ForeColor = System::Drawing::Color::White;
			this->gameOverLabel->Location = System::Drawing::Point(225, 288);
			this->gameOverLabel->Name = L"gameOverLabel";
			this->gameOverLabel->Size = System::Drawing::Size(762, 100);
			this->gameOverLabel->TabIndex = 6;
			this->gameOverLabel->Text = L"GAME OVER";
			this->gameOverLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->gameOverLabel->Visible = false;
			// 
			// playAgainLabel
			// 
			this->playAgainLabel->AutoSize = true;
			this->playAgainLabel->BackColor = System::Drawing::Color::Transparent;
			this->playAgainLabel->Font = (gcnew System::Drawing::Font(L"Emulogic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->playAgainLabel->ForeColor = System::Drawing::Color::White;
			this->playAgainLabel->Location = System::Drawing::Point(359, 405);
			this->playAgainLabel->Name = L"playAgainLabel";
			this->playAgainLabel->Size = System::Drawing::Size(512, 26);
			this->playAgainLabel->TabIndex = 7;
			this->playAgainLabel->Text = L"Press SPACE to play again";
			this->playAgainLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->playAgainLabel->Visible = false;
			// 
			// platform2
			// 
			this->platform2->Location = System::Drawing::Point(91, 485);
			this->platform2->Name = L"platform2";
			this->platform2->Size = System::Drawing::Size(88, 254);
			this->platform2->TabIndex = 8;
			this->platform2->TabStop = false;
			this->platform2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// black1
			// 
			this->black1->Location = System::Drawing::Point(83, 545);
			this->black1->Name = L"black1";
			this->black1->Size = System::Drawing::Size(20, 194);
			this->black1->TabIndex = 9;
			this->black1->TabStop = false;
			// 
			// platform4
			// 
			this->platform4->BackColor = System::Drawing::Color::Black;
			this->platform4->Location = System::Drawing::Point(386, 364);
			this->platform4->Name = L"platform4";
			this->platform4->Size = System::Drawing::Size(122, 30);
			this->platform4->TabIndex = 10;
			this->platform4->TabStop = false;
			this->platform4->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// platform5
			// 
			this->platform5->BackColor = System::Drawing::Color::Black;
			this->platform5->Location = System::Drawing::Point(563, 288);
			this->platform5->Name = L"platform5";
			this->platform5->Size = System::Drawing::Size(199, 30);
			this->platform5->TabIndex = 11;
			this->platform5->TabStop = false;
			this->platform5->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// enemy1
			// 
			this->enemy1->BackColor = System::Drawing::Color::Maroon;
			this->enemy1->Location = System::Drawing::Point(563, 263);
			this->enemy1->Name = L"enemy1";
			this->enemy1->Size = System::Drawing::Size(25, 25);
			this->enemy1->TabIndex = 12;
			this->enemy1->TabStop = false;
			// 
			// platform6
			// 
			this->platform6->BackColor = System::Drawing::Color::Black;
			this->platform6->Location = System::Drawing::Point(795, 228);
			this->platform6->Name = L"platform6";
			this->platform6->Size = System::Drawing::Size(40, 30);
			this->platform6->TabIndex = 13;
			this->platform6->TabStop = false;
			this->platform6->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// platform7
			// 
			this->platform7->BackColor = System::Drawing::Color::Black;
			this->platform7->Location = System::Drawing::Point(939, 445);
			this->platform7->Name = L"platform7";
			this->platform7->Size = System::Drawing::Size(40, 30);
			this->platform7->TabIndex = 14;
			this->platform7->TabStop = false;
			this->platform7->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// platform8
			// 
			this->platform8->BackColor = System::Drawing::Color::Black;
			this->platform8->Location = System::Drawing::Point(1021, 381);
			this->platform8->Name = L"platform8";
			this->platform8->Size = System::Drawing::Size(146, 30);
			this->platform8->TabIndex = 15;
			this->platform8->TabStop = false;
			this->platform8->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// enemy2
			// 
			this->enemy2->BackColor = System::Drawing::Color::Maroon;
			this->enemy2->Location = System::Drawing::Point(1021, 357);
			this->enemy2->Name = L"enemy2";
			this->enemy2->Size = System::Drawing::Size(25, 25);
			this->enemy2->TabIndex = 16;
			this->enemy2->TabStop = false;
			// 
			// platform9
			// 
			this->platform9->BackColor = System::Drawing::Color::Black;
			this->platform9->Location = System::Drawing::Point(1201, 331);
			this->platform9->Name = L"platform9";
			this->platform9->Size = System::Drawing::Size(171, 365);
			this->platform9->TabIndex = 17;
			this->platform9->TabStop = false;
			this->platform9->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::platform_Paint);
			// 
			// youWinLabel
			// 
			this->youWinLabel->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->youWinLabel->BackColor = System::Drawing::Color::Transparent;
			this->youWinLabel->Font = (gcnew System::Drawing::Font(L"Emulogic", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->youWinLabel->ForeColor = System::Drawing::Color::White;
			this->youWinLabel->Location = System::Drawing::Point(225, 288);
			this->youWinLabel->Name = L"youWinLabel";
			this->youWinLabel->Size = System::Drawing::Size(762, 100);
			this->youWinLabel->TabIndex = 18;
			this->youWinLabel->Text = L"YOU WIN";
			this->youWinLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->youWinLabel->Visible = false;
			// 
			// genome
			// 
			this->genome->BackColor = System::Drawing::Color::White;
			this->genome->Location = System::Drawing::Point(9, 9);
			this->genome->Name = L"genome";
			this->genome->Size = System::Drawing::Size(495, 223);
			this->genome->TabIndex = 19;
			this->genome->TabStop = false;
			this->genome->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlayerForm::drawGenome);
			// 
			// playerX
			// 
			this->playerX->BackColor = System::Drawing::Color::White;
			this->playerX->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->playerX->Location = System::Drawing::Point(9, 18);
			this->playerX->Name = L"playerX";
			this->playerX->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->playerX->Size = System::Drawing::Size(56, 16);
			this->playerX->TabIndex = 20;
			this->playerX->Text = L"playerX";
			this->playerX->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// playerY
			// 
			this->playerY->BackColor = System::Drawing::Color::White;
			this->playerY->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->playerY->Location = System::Drawing::Point(9, 60);
			this->playerY->Name = L"playerY";
			this->playerY->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->playerY->Size = System::Drawing::Size(56, 16);
			this->playerY->TabIndex = 21;
			this->playerY->Text = L"playerY";
			this->playerY->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// enemy1X
			// 
			this->enemy1X->BackColor = System::Drawing::Color::White;
			this->enemy1X->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->enemy1X->Location = System::Drawing::Point(9, 102);
			this->enemy1X->Name = L"enemy1X";
			this->enemy1X->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->enemy1X->Size = System::Drawing::Size(56, 16);
			this->enemy1X->TabIndex = 22;
			this->enemy1X->Text = L"npc1X";
			this->enemy1X->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// npc2X
			// 
			this->npc2X->BackColor = System::Drawing::Color::White;
			this->npc2X->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->npc2X->Location = System::Drawing::Point(9, 144);
			this->npc2X->Name = L"npc2X";
			this->npc2X->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->npc2X->Size = System::Drawing::Size(56, 16);
			this->npc2X->TabIndex = 23;
			this->npc2X->Text = L"npc2X";
			this->npc2X->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// bias
			// 
			this->bias->BackColor = System::Drawing::Color::White;
			this->bias->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->bias->Location = System::Drawing::Point(9, 202);
			this->bias->Name = L"bias";
			this->bias->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->bias->Size = System::Drawing::Size(56, 16);
			this->bias->TabIndex = 24;
			this->bias->Text = L"bias";
			this->bias->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// space
			// 
			this->space->BackColor = System::Drawing::Color::White;
			this->space->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->space->Location = System::Drawing::Point(455, 37);
			this->space->Name = L"space";
			this->space->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->space->Size = System::Drawing::Size(49, 16);
			this->space->TabIndex = 25;
			this->space->Text = L"Space";
			// 
			// left
			// 
			this->left->BackColor = System::Drawing::Color::White;
			this->left->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->left->Location = System::Drawing::Point(455, 108);
			this->left->Name = L"left";
			this->left->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->left->Size = System::Drawing::Size(49, 16);
			this->left->TabIndex = 26;
			this->left->Text = L"Left";
			// 
			// right
			// 
			this->right->BackColor = System::Drawing::Color::White;
			this->right->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->right->Location = System::Drawing::Point(455, 188);
			this->right->Name = L"right";
			this->right->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->right->Size = System::Drawing::Size(49, 16);
			this->right->TabIndex = 27;
			this->right->Text = L"Right";
			// 
			// population
			// 
			this->population->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->population->ForeColor = System::Drawing::Color::White;
			this->population->Location = System::Drawing::Point(511, 9);
			this->population->Name = L"population";
			this->population->Size = System::Drawing::Size(400, 20);
			this->population->TabIndex = 28;
			this->population->Text = L"Population: ";
			// 
			// species
			// 
			this->species->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->species->ForeColor = System::Drawing::Color::White;
			this->species->Location = System::Drawing::Point(512, 69);
			this->species->Name = L"species";
			this->species->Size = System::Drawing::Size(400, 20);
			this->species->TabIndex = 30;
			this->species->Text = L"Species: ";
			// 
			// genomeLabel
			// 
			this->genomeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->genomeLabel->ForeColor = System::Drawing::Color::White;
			this->genomeLabel->Location = System::Drawing::Point(512, 89);
			this->genomeLabel->Name = L"genomeLabel";
			this->genomeLabel->Size = System::Drawing::Size(400, 20);
			this->genomeLabel->TabIndex = 31;
			this->genomeLabel->Text = L"Genome: ";
			// 
			// maxFitness
			// 
			this->maxFitness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->maxFitness->ForeColor = System::Drawing::Color::White;
			this->maxFitness->Location = System::Drawing::Point(512, 149);
			this->maxFitness->Name = L"maxFitness";
			this->maxFitness->Size = System::Drawing::Size(400, 20);
			this->maxFitness->TabIndex = 33;
			this->maxFitness->Text = L"Max Fitness: ";
			// 
			// generation
			// 
			this->generation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->generation->ForeColor = System::Drawing::Color::White;
			this->generation->Location = System::Drawing::Point(512, 49);
			this->generation->Name = L"generation";
			this->generation->Size = System::Drawing::Size(400, 20);
			this->generation->TabIndex = 34;
			this->generation->Text = L"Generation: ";
			// 
			// fitness
			// 
			this->fitness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->fitness->ForeColor = System::Drawing::Color::White;
			this->fitness->Location = System::Drawing::Point(512, 129);
			this->fitness->Name = L"fitness";
			this->fitness->Size = System::Drawing::Size(400, 20);
			this->fitness->TabIndex = 35;
			this->fitness->Text = L"Fitness: ";
			// 
			// PlayerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(1262, 673);
			this->Controls->Add(this->character);
			this->Controls->Add(this->fitness);
			this->Controls->Add(this->generation);
			this->Controls->Add(this->maxFitness);
			this->Controls->Add(this->genomeLabel);
			this->Controls->Add(this->species);
			this->Controls->Add(this->population);
			this->Controls->Add(this->right);
			this->Controls->Add(this->left);
			this->Controls->Add(this->space);
			this->Controls->Add(this->bias);
			this->Controls->Add(this->npc2X);
			this->Controls->Add(this->enemy1X);
			this->Controls->Add(this->playerY);
			this->Controls->Add(this->playerX);
			this->Controls->Add(this->genome);
			this->Controls->Add(this->youWinLabel);
			this->Controls->Add(this->platform9);
			this->Controls->Add(this->enemy2);
			this->Controls->Add(this->platform8);
			this->Controls->Add(this->playAgainLabel);
			this->Controls->Add(this->gameOverLabel);
			this->Controls->Add(this->platform7);
			this->Controls->Add(this->platform6);
			this->Controls->Add(this->enemy1);
			this->Controls->Add(this->platform5);
			this->Controls->Add(this->black1);
			this->Controls->Add(this->platform2);
			this->Controls->Add(this->leftBoundary);
			this->Controls->Add(this->platform3);
			this->Controls->Add(this->platform1);
			this->Controls->Add(this->platform4);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"PlayerForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"NEAT";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &PlayerForm::formClosing);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &PlayerForm::keyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &PlayerForm::keyUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->character))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftBoundary))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->black1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enemy1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enemy2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->platform9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->genome))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

};
}
