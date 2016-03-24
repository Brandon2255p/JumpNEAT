#include "PlayerForm.h"

using namespace GUI;

PlayerForm::PlayerForm() {	
	InitializeComponent();

	cellX = gcnew List<int>();
	cellY = gcnew List<int>();
	cellValue = gcnew List<double>();
	inCell = gcnew List<int>();
	outCell = gcnew List<int>();
	geneWeight = gcnew List<double>();
	labelValues = gcnew List<double>();

	startGame();
}

PlayerForm::~PlayerForm() {
	if (components)
		delete components;
}

System::Void PlayerForm::platform_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	PictureBox^ platform = (PictureBox^)sender;
	System::Drawing::Rectangle newRectangle = platform->ClientRectangle;
	e->Graphics->DrawRectangle(gcnew Pen(Color::White), newRectangle.Left, newRectangle.Top, newRectangle.Right - 1, newRectangle.Bottom - 1);
}

// commented out for AI
/*
System::Void PlayerForm::keyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	if (e->KeyCode == Keys::Right) {
		moveRight = true;
		moveLeft = false;
	}
	else if (e->KeyCode == Keys::Left) {
		moveLeft = true;
		moveRight = false;
	}
	else if (e->KeyCode == Keys::Space && checkCollisionDown(0) != nullptr && falling == false && gameRunning == true) {
		jumpStartHeight = character->Location.Y;
		jumping = true;
	}
	else if (e->KeyCode == Keys::Space && gameRunning == false)
		restartGame();
}

System::Void PlayerForm::keyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	if (e->KeyCode == Keys::Right)
		moveRight = false;
	else if (e->KeyCode == Keys::Left)
		moveLeft = false;
}
*/

System::Void PlayerForm::keyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	// do nothing
}

System::Void PlayerForm::keyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	// do nothing
}

void PlayerForm::setControls(Generic::List<bool>^ controls) {
	if (controls[2] == true) {
		moveRight = true;
		moveLeft = false;
	}
	
	if (controls[1] == true) {
		moveLeft = true;
		moveRight = false;
	}
	
	if (controls[2] == false && controls[1] == false) {
		moveRight = false;
		moveLeft = false;
	}	
	
	if (controls[0] == true && checkCollisionDownAI(0) != nullptr && falling == false && gameRunning == true) {
		jumpStartHeight = character->Location.Y;
		jumping = true;
	}

	return;
}

void PlayerForm::startGame() {
	moveRight = false;
	moveLeft = false;
	jumping = false;
	falling = false;
	gameRunning = true;
	movingRight1 = true;
	movingRight2 = true;
		
	//commented out for AI
	/*
	thread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &PlayerForm::gameClock));
	thread->Name = "Game Thread";
	thread->Start();
	*/	
}

void PlayerForm::gameClock() {
	while (gameRunning) {
		updateGame();
		thread->Sleep(10);
	}
}

void PlayerForm::updateGame() {	
	if (gameRunning) {
		move();

		if (jumping == true)
			jump();
		else if (falling == true)
			fall();
		else if (checkCollisionDown(0) == nullptr) {
			fallStartHeight = character->Location.Y - 2;
			falling = true;
			fall();
		}

		moveEnemies();
		checkCollisionEnemy();
	}
}

System::Void PlayerForm::formClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	thread->Abort();
}

void PlayerForm::move() {
	if (character->InvokeRequired) {
		CharacterDelegate^ d = gcnew CharacterDelegate(this, &PlayerForm::move);
		this->Invoke(d);
	}
	else {
		if (moveRight == true && checkCollisionRight(0) == nullptr) {
			if (checkCollisionRight(1) != nullptr)
				character->Location = System::Drawing::Point(checkCollisionRight(1)->Location.X - character->Size.Width, character->Location.Y);
			else
				character->Location = System::Drawing::Point(character->Location.X + 2, character->Location.Y);
		}
		else if (moveLeft == true && checkCollisionLeft(0) == nullptr) {
			if (checkCollisionLeft(1) != nullptr )
				character->Location = System::Drawing::Point((checkCollisionLeft(1)->Location.X + checkCollisionLeft(1)->Size.Width), character->Location.Y);
			else
				character->Location = System::Drawing::Point(character->Location.X - 2, character->Location.Y);
		}
	}
}

void PlayerForm::jump() {
	if (character->InvokeRequired) {
		CharacterDelegate^ d = gcnew CharacterDelegate(this, &PlayerForm::jump);
		this->Invoke(d);
	}
	else {
		int offset = ceil(10 * (10000 - pow(jumpStartHeight - character->Location.Y, 2)) / 10000.0);
		character->Location = System::Drawing::Point(character->Location.X, character->Location.Y - offset);

		if (checkCollisionUp(offset - 1) != nullptr) {
			character->Location = System::Drawing::Point(character->Location.X, checkCollisionUp(offset-1)->Location.Y + checkCollisionUp(offset - 1)->Size.Height);
			jumping = false;
		}
		else if (offset == 0) 
			jumping = false;
	}
}

void PlayerForm::fall() {
	if (character->InvokeRequired) {
		CharacterDelegate^ d = gcnew CharacterDelegate(this, &PlayerForm::fall);
		this->Invoke(d);
	}
	else {
		int offset = ceil(10 * (10000 - pow(fallStartHeight + 100 - character->Location.Y, 2)) / 10000.0);

		if (character->Location.Y - fallStartHeight > 100)
			offset = 10;
		
		if (checkCollisionDown(offset-1) != nullptr) {
			character->Location = System::Drawing::Point(character->Location.X, checkCollisionDown(offset-1)->Location.Y - character->Size.Height);
			falling = false;
		}
		else
			character->Location = System::Drawing::Point(character->Location.X, character->Location.Y + offset);		
	}
}

PictureBox^ PlayerForm::checkCollisionDownAI(int y) {
	if (character->Location.X + character->Size.Width > platform1->Location.X && character->Location.X < platform1->Location.X + platform1->Size.Width && character->Location.Y + character->Size.Height + y >= platform1->Location.Y)
		return platform1;
	else if (character->Location.X + character->Size.Width > platform2->Location.X && character->Location.X < platform2->Location.X + platform2->Size.Width && character->Location.Y + character->Size.Height + y >= platform2->Location.Y)
		return platform2;
	else if (character->Location.X + character->Size.Width > platform3->Location.X && character->Location.X < platform3->Location.X + platform3->Size.Width && character->Location.Y + character->Size.Height + y >= platform3->Location.Y)
		return platform3;
	else if (character->Location.X + character->Size.Width > platform4->Location.X && character->Location.X < platform4->Location.X + platform4->Size.Width && character->Location.Y + character->Size.Height + y >= platform4->Location.Y && character->Location.Y < platform4->Location.Y + platform4->Size.Height)
		return platform4;
	else if (character->Location.X + character->Size.Width > platform5->Location.X && character->Location.X < platform5->Location.X + platform5->Size.Width && character->Location.Y + character->Size.Height + y >= platform5->Location.Y && character->Location.Y < platform5->Location.Y + platform5->Size.Height)
		return platform5;
	else if (character->Location.X + character->Size.Width > platform6->Location.X && character->Location.X < platform6->Location.X + platform6->Size.Width && character->Location.Y + character->Size.Height + y >= platform6->Location.Y && character->Location.Y < platform6->Location.Y + platform6->Size.Height)
		return platform6;
	else if (character->Location.X + character->Size.Width > platform7->Location.X && character->Location.X < platform7->Location.X + platform7->Size.Width && character->Location.Y + character->Size.Height + y >= platform7->Location.Y && character->Location.Y < platform7->Location.Y + platform7->Size.Height)
		return platform7;
	else if (character->Location.X + character->Size.Width > platform8->Location.X && character->Location.X < platform8->Location.X + platform8->Size.Width && character->Location.Y + character->Size.Height + y >= platform8->Location.Y && character->Location.Y < platform8->Location.Y + platform8->Size.Height)
		return platform8;
	else if (character->Location.X + character->Size.Width > platform9->Location.X && character->Location.X < platform9->Location.X + platform9->Size.Width && character->Location.Y + character->Size.Height + y >= platform9->Location.Y)
		return platform9;
	else if (character->Location.Y > 548)
		gameOver();
	else
		return nullptr;
}

PictureBox^ PlayerForm::checkCollisionDown(int y) {
	if (character->InvokeRequired) {
		CharacterDelegatePB^ d = gcnew CharacterDelegatePB(this, &PlayerForm::checkCollisionDown);
		this->Invoke(d, y);
	}
	else {		
		if (character->Location.X + character->Size.Width > platform1->Location.X && character->Location.X < platform1->Location.X + platform1->Size.Width && character->Location.Y + character->Size.Height + y >= platform1->Location.Y)
			return platform1;
		else if (character->Location.X + character->Size.Width > platform2->Location.X && character->Location.X < platform2->Location.X + platform2->Size.Width && character->Location.Y + character->Size.Height + y >= platform2->Location.Y)
			return platform2;
		else if (character->Location.X + character->Size.Width > platform3->Location.X && character->Location.X < platform3->Location.X + platform3->Size.Width && character->Location.Y + character->Size.Height + y >= platform3->Location.Y)
			return platform3;
		else if (character->Location.X + character->Size.Width > platform4->Location.X && character->Location.X < platform4->Location.X + platform4->Size.Width && character->Location.Y + character->Size.Height + y >= platform4->Location.Y && character->Location.Y < platform4->Location.Y + platform4->Size.Height)
			return platform4;
		else if (character->Location.X + character->Size.Width > platform5->Location.X && character->Location.X < platform5->Location.X + platform5->Size.Width && character->Location.Y + character->Size.Height + y >= platform5->Location.Y && character->Location.Y < platform5->Location.Y + platform5->Size.Height)
			return platform5;	
		else if (character->Location.X + character->Size.Width > platform6->Location.X && character->Location.X < platform6->Location.X + platform6->Size.Width && character->Location.Y + character->Size.Height + y >= platform6->Location.Y && character->Location.Y < platform6->Location.Y + platform6->Size.Height)
			return platform6;
		else if (character->Location.X + character->Size.Width > platform7->Location.X && character->Location.X < platform7->Location.X + platform7->Size.Width && character->Location.Y + character->Size.Height + y >= platform7->Location.Y && character->Location.Y < platform7->Location.Y + platform7->Size.Height)
			return platform7;
		else if (character->Location.X + character->Size.Width > platform8->Location.X && character->Location.X < platform8->Location.X + platform8->Size.Width && character->Location.Y + character->Size.Height + y >= platform8->Location.Y && character->Location.Y < platform8->Location.Y + platform8->Size.Height)
			return platform8;
		else if (character->Location.X + character->Size.Width > platform9->Location.X && character->Location.X < platform9->Location.X + platform9->Size.Width && character->Location.Y + character->Size.Height + y >= platform9->Location.Y)
			return platform9;
		else if (character->Location.Y > 548)
			gameOver();		
		else
			return nullptr;
	}
}

PictureBox^ PlayerForm::checkCollisionUp(int y) {
	if (character->InvokeRequired) {
		CharacterDelegatePB^ d = gcnew CharacterDelegatePB(this, &PlayerForm::checkCollisionUp);
		this->Invoke(d, y);
	}
	else {
		if (character->Location.X + character->Size.Width > platform4->Location.X && character->Location.X < platform4->Location.X + platform4->Size.Width && character->Location.Y + character->Size.Height > platform4->Location.Y && character->Location.Y - y <= platform4->Location.Y + platform4->Size.Height)
			return platform4;
		else if (character->Location.X + character->Size.Width > platform5->Location.X && character->Location.X < platform5->Location.X + platform5->Size.Width && character->Location.Y + character->Size.Height > platform5->Location.Y && character->Location.Y - y <= platform5->Location.Y + platform5->Size.Height)
			return platform5;
		else if (character->Location.X + character->Size.Width > platform6->Location.X && character->Location.X < platform6->Location.X + platform6->Size.Width && character->Location.Y + character->Size.Height > platform6->Location.Y && character->Location.Y - y <= platform6->Location.Y + platform6->Size.Height)
			return platform6;
		else if (character->Location.X + character->Size.Width > platform7->Location.X && character->Location.X < platform7->Location.X + platform7->Size.Width && character->Location.Y + character->Size.Height > platform7->Location.Y && character->Location.Y - y <= platform7->Location.Y + platform7->Size.Height)
			return platform7;
		else if (character->Location.X + character->Size.Width > platform8->Location.X && character->Location.X < platform8->Location.X + platform8->Size.Width && character->Location.Y + character->Size.Height > platform8->Location.Y && character->Location.Y - y <= platform8->Location.Y + platform8->Size.Height)
			return platform8;
		else
			return nullptr;
	}
}

PictureBox^ PlayerForm::checkCollisionRight(int x) {
	if (character->InvokeRequired) {
		CharacterDelegatePB^ d = gcnew CharacterDelegatePB(this, &PlayerForm::checkCollisionRight);
		this->Invoke(d, x);
	}
	else {
		if (character->Location.Y + character->Size.Height > platform2->Location.Y && character->Location.Y < platform2->Location.Y + platform2->Size.Height && character->Location.X + character->Size.Width + x >= platform2->Location.X && character->Location.X < platform2->Location.X + platform2->Size.Width)
			return platform2;
		else if (character->Location.Y + character->Size.Height > platform3->Location.Y && character->Location.Y < platform3->Location.Y + platform3->Size.Height && character->Location.X + character->Size.Width + x >= platform3->Location.X && character->Location.X < platform3->Location.X + platform3->Size.Width)
			return platform3;
		else if (character->Location.Y + character->Size.Height > platform4->Location.Y && character->Location.Y < platform4->Location.Y + platform4->Size.Height && character->Location.X + character->Size.Width + x >= platform4->Location.X && character->Location.X < platform4->Location.X + platform4->Size.Width)
			return platform4;
		else if (character->Location.Y + character->Size.Height > platform5->Location.Y && character->Location.Y < platform5->Location.Y + platform5->Size.Height && character->Location.X + character->Size.Width + x >= platform5->Location.X && character->Location.X < platform5->Location.X + platform5->Size.Width)
			return platform5;		
		else if (character->Location.Y + character->Size.Height > platform6->Location.Y && character->Location.Y < platform6->Location.Y + platform6->Size.Height && character->Location.X + character->Size.Width + x >= platform6->Location.X && character->Location.X < platform6->Location.X + platform6->Size.Width)
			return platform6;
		else if (character->Location.Y + character->Size.Height > platform7->Location.Y && character->Location.Y < platform7->Location.Y + platform7->Size.Height && character->Location.X + character->Size.Width + x >= platform7->Location.X && character->Location.X < platform7->Location.X + platform7->Size.Width)
			return platform7;
		else if (character->Location.Y + character->Size.Height > platform8->Location.Y && character->Location.Y < platform8->Location.Y + platform8->Size.Height && character->Location.X + character->Size.Width + x >= platform8->Location.X && character->Location.X < platform8->Location.X + platform8->Size.Width)
			return platform8;
		else if (character->Location.Y + character->Size.Height > platform9->Location.Y && character->Location.Y < platform9->Location.Y + platform9->Size.Height && character->Location.X + character->Size.Width + x >= platform9->Location.X && character->Location.X < platform9->Location.X + platform9->Size.Width)
			return platform9;
		else if (character->Location.X > this->Size.Width)
			gameWin();
		else
			return nullptr;
	}
}

PictureBox^ PlayerForm::checkCollisionLeft(int x) {
	if (character->InvokeRequired) {
		CharacterDelegatePB^ d = gcnew CharacterDelegatePB(this, &PlayerForm::checkCollisionLeft);
		this->Invoke(d, x);
	}
	else {		
		if (character->Location.Y + character->Size.Height > platform2->Location.Y && character->Location.Y < platform2->Location.Y + platform2->Size.Height && character->Location.X + character->Size.Width > platform2->Location.X && character->Location.X - x <= platform2->Location.X + platform2->Size.Width)
			return platform2;
		else if (character->Location.Y + character->Size.Height > platform3->Location.Y && character->Location.Y < platform3->Location.Y + platform3->Size.Height && character->Location.X + character->Size.Width > platform3->Location.X && character->Location.X - x <= platform3->Location.X + platform3->Size.Width)
			return platform3;
		else if (character->Location.Y + character->Size.Height > platform4->Location.Y && character->Location.Y < platform4->Location.Y + platform4->Size.Height && character->Location.X + character->Size.Width > platform4->Location.X && character->Location.X - x <= platform4->Location.X + platform4->Size.Width)
			return platform4;
		else if (character->Location.Y + character->Size.Height > platform5->Location.Y && character->Location.Y < platform5->Location.Y + platform5->Size.Height && character->Location.X + character->Size.Width > platform5->Location.X && character->Location.X - x <= platform5->Location.X + platform5->Size.Width)
			return platform5;	
		else if (character->Location.Y + character->Size.Height > platform6->Location.Y && character->Location.Y < platform6->Location.Y + platform6->Size.Height && character->Location.X + character->Size.Width > platform6->Location.X && character->Location.X - x <= platform6->Location.X + platform6->Size.Width)
			return platform6;
		else if (character->Location.Y + character->Size.Height > platform7->Location.Y && character->Location.Y < platform7->Location.Y + platform7->Size.Height && character->Location.X + character->Size.Width > platform7->Location.X && character->Location.X - x <= platform7->Location.X + platform7->Size.Width)
			return platform7;
		else if (character->Location.Y + character->Size.Height > platform8->Location.Y && character->Location.Y < platform8->Location.Y + platform8->Size.Height && character->Location.X + character->Size.Width > platform8->Location.X && character->Location.X - x <= platform8->Location.X + platform8->Size.Width)
			return platform8;
		else if (character->Location.X - x <= 0)
			return leftBoundary; 
		else
			return nullptr;
	}
}

void PlayerForm::gameOver() {
	if (gameOverLabel->InvokeRequired) {
		LabelDelegate^ d = gcnew LabelDelegate(this, &PlayerForm::gameOver);
		this->Invoke(d);
	}
	else {
		gameRunning = false;
		gameOverLabel->Visible = true;
		playAgainLabel->Visible = true;
	}
}

void PlayerForm::gameWin() {
	gameRunning = false;
	youWinLabel->Visible = true;
	playAgainLabel->Visible = true;
}

void PlayerForm::restartGame() {	
	if (enemy1->InvokeRequired || enemy2->InvokeRequired) {
		EnemyDelegate^ d = gcnew EnemyDelegate(this, &PlayerForm::restartGame);
		this->Invoke(d);
	}
	else {
		gameOverLabel->Visible = false;
		youWinLabel->Visible = false;
		playAgainLabel->Visible = false;
		character->Location = System::Drawing::Point(12, 422);
		enemy1->Location = System::Drawing::Point(platform5->Location.X, platform5->Location.Y - enemy1->Size.Height);
		enemy2->Location = System::Drawing::Point(platform8->Location.X, platform8->Location.Y - enemy2->Size.Height);
		startGame();
	}
}

void PlayerForm::moveEnemies() {
	if (enemy1->InvokeRequired || enemy2->InvokeRequired) {
		EnemyDelegate^ d = gcnew EnemyDelegate(this, &PlayerForm::moveEnemies);
		this->Invoke(d);
	}
	else {
		if (movingRight1 == true)
			enemy1->Location = System::Drawing::Point(enemy1->Location.X + 1, enemy1->Location.Y);
		else
			enemy1->Location = System::Drawing::Point(enemy1->Location.X - 1, enemy1->Location.Y);

		if (enemy1->Location.X + enemy1->Size.Width >= platform5->Location.X + platform5->Size.Width)
			movingRight1 = false;
		else if (enemy1->Location.X <= platform5->Location.X)
			movingRight1 = true;

		if (movingRight2 == true)
			enemy2->Location = System::Drawing::Point(enemy2->Location.X + 2, enemy2->Location.Y);
		else
			enemy2->Location = System::Drawing::Point(enemy2->Location.X - 2, enemy2->Location.Y);

		if (enemy2->Location.X + enemy2->Size.Width >= platform8->Location.X + platform8->Size.Width)
			movingRight2 = false;
		else if (enemy2->Location.X <= platform8->Location.X)
			movingRight2 = true;
	}
}

void PlayerForm::checkCollisionEnemy() {
	if (character->Location.X + character->Size.Width > enemy1->Location.X && character->Location.X < enemy1->Location.X + enemy1->Size.Width && character->Location.Y + character->Size.Height >= enemy1->Location.Y && character->Location.Y < enemy1->Location.Y + enemy1->Size.Height)
		gameOver();
	else if (character->Location.Y + character->Size.Height > enemy1->Location.Y && character->Location.Y < enemy1->Location.Y + enemy1->Size.Height && character->Location.X + character->Size.Width >= enemy1->Location.X && character->Location.X < enemy1->Location.X + enemy1->Size.Width)
		gameOver();
	else if (character->Location.Y + character->Size.Height > enemy1->Location.Y && character->Location.Y < enemy1->Location.Y + enemy1->Size.Height && character->Location.X + character->Size.Width > enemy1->Location.X && character->Location.X <= enemy1->Location.X + enemy1->Size.Width)
		gameOver();
	else if (character->Location.X + character->Size.Width > enemy2->Location.X && character->Location.X < enemy2->Location.X + enemy2->Size.Width && character->Location.Y + character->Size.Height >= enemy2->Location.Y && character->Location.Y < enemy2->Location.Y + enemy2->Size.Height)
		gameOver();
	else if (character->Location.Y + character->Size.Height > enemy2->Location.Y && character->Location.Y < enemy2->Location.Y + enemy2->Size.Height && character->Location.X + character->Size.Width >= enemy2->Location.X && character->Location.X < enemy2->Location.X + enemy2->Size.Width)
		gameOver();
	else if (character->Location.Y + character->Size.Height > enemy2->Location.Y && character->Location.Y < enemy2->Location.Y + enemy2->Size.Height && character->Location.X + character->Size.Width > enemy2->Location.X && character->Location.X <= enemy2->Location.X + enemy2->Size.Width)
		gameOver();
}

PictureBox^ PlayerForm::getCharacter() {
	return character;
}

PictureBox^ PlayerForm::getEnemy1() {
	return enemy1;
}

PictureBox^ PlayerForm::getEnemy2() {
	return enemy2;
}

System::Threading::Thread^ PlayerForm::getThread() {
	return thread;
}

void PlayerForm::setThread(System::Threading::Thread^ t) {
	thread = t;
	return;
}

bool PlayerForm::getGameRunning() {
	return gameRunning;
}

List<int>^ PlayerForm::getCellX() {
	return cellX;
}

List<int>^ PlayerForm::getCellY() {
	return cellY;
}

List<double>^ PlayerForm::getCellValue() {
	return cellValue;
}

List<int>^ PlayerForm::getInCell() {
	return inCell;
}

List<int>^ PlayerForm::getOutCell() {
	return outCell;
}

List<double>^ PlayerForm::getGeneWeight() {
	return geneWeight;
}

List<double>^ PlayerForm::getLabelValues() {
	return labelValues;
}

// top left = (0,0) / bottom right = (370,180)
System::Void PlayerForm::drawGenome(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	int width = 9;
	int height = 9;

	Pen^ blackPen = gcnew Pen(Color::Black, 1);
	Pen^ greenPen = gcnew Pen(Color::Green, 1);
	Pen^ redPen = gcnew Pen(Color::Red, 1);
	Pen^ greyPen = gcnew Pen(Color::LightGray, 1);
	SolidBrush^ whiteBrush = gcnew SolidBrush(System::Drawing::Color::White);
	SolidBrush^ blackBrush = gcnew SolidBrush(System::Drawing::Color::Black);
	
	for (int i = 0; i < inCell->Count; i++) {
		Point point1 = Point(cellX[inCell[i]] + 9, cellY[inCell[i]] + 4);
		Point point2 = Point(cellX[outCell[i]], cellY[outCell[i]] + 4);
		if (geneWeight[i] == 0)
			e->Graphics->DrawLine(greyPen, point1, point2);		
	}

	for (int i = 0; i < inCell->Count; i++) {
		Point point1 = Point(cellX[inCell[i]] + 9, cellY[inCell[i]] + 4);
		Point point2 = Point(cellX[outCell[i]], cellY[outCell[i]] + 4);
		if (geneWeight[i] > 0)
			e->Graphics->DrawLine(greenPen, point1, point2);
		else if (geneWeight[i] < 0)
			e->Graphics->DrawLine(redPen, point1, point2);		
	}	

	
	for (int i = 0; i < cellX->Count; i++) {
		if (cellX[i] > 0) {		
			if (cellValue[i] > 0)
				e->Graphics->FillRectangle(blackBrush, cellX[i], cellY[i], width, height);
			else
				e->Graphics->FillRectangle(whiteBrush, cellX[i], cellY[i], width, height);
			e->Graphics->DrawRectangle(blackPen, cellX[i], cellY[i], width, height);
		}
	}
}

void PlayerForm::updateDisplay() {
	if (genome->InvokeRequired) {
		GenomeDelegate^ d = gcnew GenomeDelegate(this, &PlayerForm::updateDisplay);
		this->Invoke(d);
	}
	else {
		genome->Invalidate();

		if (labelValues[0] == 0)
			population->Text = "Population: N/A";
		else
			population->Text = "Population: " + Convert::ToString(labelValues[0]);

		generation->Text = "Generation: " + Convert::ToString(labelValues[1]);
		species->Text = "Species: " + Convert::ToString(labelValues[2]);
		genomeLabel->Text = "Genome: " + Convert::ToString(labelValues[3]);

		if (labelValues[4] == 0)
			fitness->Text = "Fitness: N/A";
		else 
			fitness->Text = "Fitness: " + Convert::ToString(labelValues[4]);

		if (labelValues[5] == 0)
			maxFitness->Text = "Max Fitness: N/A";
		else
			maxFitness->Text = "Max Fitness: " + Convert::ToString(labelValues[5]);
	}
}
