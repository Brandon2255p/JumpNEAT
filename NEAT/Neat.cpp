#include "Neat.h"

NeuralNetwork::NeuralNetwork() {	
	
		
	playerForm = gcnew PlayerForm();
	playerForm->setThread(gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &NeuralNetwork::runPlayerForm)));
	playerForm->getThread()->Name = "PlayerForm Thread";
	playerForm->getThread()->Start();
	playerForm->getThread()->Sleep(10);

	srand(time(NULL));		

	//runGenome("generation0.xml");
	runNeuralNetwork();
}

void NeuralNetwork::runPlayerForm() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(playerForm);
}

void NeuralNetwork::runNeuralNetwork() {	
	initializePool();
	
	while (true) {		
		double timeoutBonus;
		double fitness;

		Species^ species = pool->getSpecies()[pool->getCurrentSpecies()];
		Genome^ genome = species->getGenome()[pool->getCurrentGenome()];

		if (pool->getCurrentFrame() == 1) {
			clearDisplay();
			displayGenome(genome);
			updateLabels();
			playerForm->updateDisplay();
		}
		
		// change to %5 ?
		if (pool->getCurrentFrame() % 1 == 0) {
			evaluateCurrent();
		}

		// if changed to %5, the keyboard in evaluateCurrent() must be set again here

		if (getCharacterX() > rightmost) {
			rightmost = getCharacterX();
			timeout = GlobalVariables::TimeoutConstant;
		}

		timeout = timeout - 1;
		timeoutBonus = pool->getCurrentFrame() / 4;

		if (timeout + timeoutBonus <= 0 || playerForm->getGameRunning() == false) {
			// change here to adjust fitness importance of distance and speed
			fitness = rightmost - pool->getCurrentFrame() * 0.5;
			if (rightmost > 962)
				fitness = fitness + 1000;
			else if (fitness == 0)
				fitness = -1;
			genome->setFitness(fitness);			

			if (fitness > pool->getCurrentMaxFitness()) {
				pool->setCurrentMaxFitness(fitness);
				pool->setMaxFitnessSpecies(pool->getCurrentSpecies());
				pool->setMaxFitnessGenome(pool->getCurrentGenome());
			}

			if (fitness > pool->getMaxFitness()) {
				pool->setMaxFitness(fitness);
				pool->setMaxFitnessSpecies(pool->getCurrentSpecies());
				pool->setMaxFitnessGenome(pool->getCurrentGenome());
				pool->setSaveGenome(true);
			}

			System::Diagnostics::Debug::WriteLine("Generation " + pool->getGeneration() + " species " + pool->getCurrentSpecies() + " genome " + pool->getCurrentGenome() + " fitness: " + fitness);
			pool->setCurrentGenome(1);
			pool->setCurrentSpecies(1);
			while (fitnessAlreadyMeasured())
				nextGenome();
			initializeRun();
		}

		pool->setCurrentFrame(pool->getCurrentFrame() + 1);
		playerForm->updateGame();
	}
}

void NeuralNetwork::runGenome(String^ filename) {
	double fitness;
	double timeoutBonus;
	int currentFrame = 0;
	bool running = true;
		
	createLabels();
	Genome^ genome = loadGenome(filename);
	genome->setParentPool(pool);
	genome->generateNetwork();	
	displayGenome(genome);
	
	while (running && playerForm->getGameRunning()) {
		evaluateGenome(genome);

		if (getCharacterX() > rightmost) {
			rightmost = getCharacterX();
			timeout = GlobalVariables::TimeoutConstant;
		}
			
		fitness = rightmost - currentFrame * 0.5;
		if (rightmost > 962) 
			fitness = fitness + 1000;
		
		playerForm->getLabelValues()[4] = fitness;

		timeout = timeout - 1;
		timeoutBonus = currentFrame / 4;

		if (timeout + timeoutBonus <= 0) 
			running = false;
		
		updateCellValues(genome);
		playerForm->updateDisplay();
		playerForm->updateGame();	

		currentFrame++;
		playerForm->getThread()->Sleep(10);			
	}	
}

void NeuralNetwork::initializePool() {
	pool = gcnew Pool();
	
	for (int i = 0; i < GlobalVariables::Population; i++) {
		Genome^ basicGenome = createBasicGenome();
		pool->addToSpecies(basicGenome);
	}	
	
	initializeRun();
}

void NeuralNetwork::initializeRun() {
	Species^ species;
	Genome^ genome;

	playerForm->restartGame();
	rightmost = 0;
	pool->setCurrentFrame(0);
	timeout = GlobalVariables::TimeoutConstant;
	// may have to set all keyboard inputs to 0?

	species = pool->getSpecies()[pool->getCurrentSpecies()];
	genome = species->getGenome()[pool->getCurrentGenome()];

	genome->generateNetwork();
	evaluateCurrent();
}

Genome^ NeuralNetwork::createBasicGenome() {
	Genome^ genome = gcnew Genome();
	// int innovation = 1; ?

	genome->setParentPool(pool);
	genome->setMaxNeuron(GlobalVariables::NumInputs);
	genome->createBasicGenes();
	genome->mutate();

	return genome;
}

void NeuralNetwork::evaluateCurrent() {
	Species^ species = pool->getSpecies()[pool->getCurrentSpecies()];
	Genome^ genome = species->getGenome()[pool->getCurrentGenome()];

	List<double>^ inputs = gcnew List<double>();
	inputs->Add(NULL);
	inputs->Add(getCharacterX() / 962.0);
	inputs->Add(getCharacterY() / 586.0);
	inputs->Add((getEnemy1() - 422) / (149.0 - 18));
	inputs->Add((getEnemy2() - 766) / (110.0 - 18));
	inputs->Add(1.0);

	// keyboard[0] = space, keyboard[1] = left, keyboard[2] = right
	List<bool>^ keyboard = genome->evaluateNetwork(inputs);

	if (keyboard[1] == true && keyboard[2] == true) {
		keyboard[1] = false;
		keyboard[2] = false;
	}

	playerForm->setControls(keyboard);
}

void NeuralNetwork::evaluateGenome(Genome^ genome) {
	List<double>^ inputs = gcnew List<double>();
	inputs->Add(NULL);
	inputs->Add(getCharacterX() / 962.0);
	inputs->Add(getCharacterY() / 586.0);
	inputs->Add((getEnemy1() - 422) / (149.0 - 18));
	inputs->Add((getEnemy2() - 766) / (110.0 - 18));
	inputs->Add(1.0);

	// keyboard[0] = space, keyboard[1] = left, keyboard[2] = right
	List<bool>^ keyboard = genome->evaluateNetwork(inputs);

	if (keyboard[1] == true && keyboard[2] == true) {
		keyboard[1] = false;
		keyboard[2] = false;
	}

	playerForm->setControls(keyboard);
}

// character x position (PlayerForm->Size = (962, 586))
double NeuralNetwork::getCharacterX() {
	//return playerForm->getCharacter()->Location.X / 962.0;
	return playerForm->getCharacter()->Location.X;
}

// character y position
double NeuralNetwork::getCharacterY() {
	//return playerForm->getCharacter()->Location.Y / 586.0;
	return playerForm->getCharacter()->Location.Y;
}

// enemy1 x position (platform5->Size.Width = 149, platform5->Location.X = 422, character->Size.Width = 19)
double NeuralNetwork::getEnemy1() {
	//return (playerForm->getEnemy1()->Location.X - 422) / (149.0 - 18);
	return playerForm->getEnemy1()->Location.X;
}

// enemy2 x position (platform8->Size.Width = 110, platform8->Location.X = 766)
double NeuralNetwork::getEnemy2() {
	//return (playerForm->getEnemy2()->Location.X - 766) / (110.0 - 18);
	return playerForm->getEnemy2()->Location.X;
}

bool NeuralNetwork::fitnessAlreadyMeasured() {
	Species^ species = pool->getSpecies()[pool->getCurrentSpecies()];
	Genome^ genome = species->getGenome()[pool->getCurrentGenome()];

	return (genome->getFitness() != 0);
}

void NeuralNetwork::nextGenome() {
	pool->setCurrentGenome(pool->getCurrentGenome() + 1);
	if (pool->getCurrentGenome() >= pool->getSpecies()[pool->getCurrentSpecies()]->getGenome()->Count) {
		pool->setCurrentGenome(1);
		pool->setCurrentSpecies(pool->getCurrentSpecies() + 1);
		if (pool->getCurrentSpecies() >= pool->getSpecies()->Count) {
			saveGenome();
			pool->setCurrentMaxFitness(0);
			pool->setSaveGenome(false);
			pool->newGeneration();
			pool->setCurrentSpecies(1);
		}
	}
	return;
}

void NeuralNetwork::saveGenome() {
	// remove if statement to save genomes for every generation
	if (pool->getSaveGenome() == true) {
		Genome^ genome = pool->getSpecies()[pool->getMaxFitnessSpecies()]->getGenome()[pool->getMaxFitnessGenome()];

		String^ filename = "generation" + pool->getGeneration() + ".xml";
		XmlWriterSettings^ xmlSettings = gcnew XmlWriterSettings();
		xmlSettings->Indent = true;
		
		XmlWriter^ xmlFile = XmlWriter::Create(filename, xmlSettings);
		xmlFile->WriteStartDocument(true);
		xmlFile->WriteStartElement("genome");

		// write genome information
		xmlFile->WriteStartElement("info");
		xmlFile->WriteStartElement("generation");
		xmlFile->WriteString(Convert::ToString(pool->getGeneration()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("species");
		xmlFile->WriteString(Convert::ToString(pool->getMaxFitnessSpecies()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("genome");
		xmlFile->WriteString(Convert::ToString(pool->getMaxFitnessGenome()));
		xmlFile->WriteEndElement();
		xmlFile->WriteEndElement();

		// write genome object
		xmlFile->WriteStartElement("object");
		// properties
		xmlFile->WriteStartElement("fitness");
		xmlFile->WriteString(Convert::ToString(genome->getFitness()));
		//xmlFile->WriteEndElement();
		//xmlFile->WriteStartElement("adjustedfitness");
		//xmlFile->WriteString(Convert::ToString(genome->getAdjustedFitness()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("maxneuron");
		xmlFile->WriteString(Convert::ToString(genome->getMaxNeuron()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("globalrank");
		xmlFile->WriteString(Convert::ToString(genome->getGlobalRank()));
		xmlFile->WriteEndElement();
		// mutation rates
		xmlFile->WriteStartElement("connectionsmr");
		xmlFile->WriteString(Convert::ToString(genome->getConnectionsMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("linkmr");
		xmlFile->WriteString(Convert::ToString(genome->getLinkMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("biasmr");
		xmlFile->WriteString(Convert::ToString(genome->getBiasMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("nodemr");
		xmlFile->WriteString(Convert::ToString(genome->getNodeMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("enablemr");
		xmlFile->WriteString(Convert::ToString(genome->getEnableMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("disablemr");
		xmlFile->WriteString(Convert::ToString(genome->getDisableMR()));
		xmlFile->WriteEndElement();
		xmlFile->WriteStartElement("stepmr");
		xmlFile->WriteString(Convert::ToString(genome->getStepMR()));
		xmlFile->WriteEndElement();
		// genes
		xmlFile->WriteStartElement("genes");
		for (int i = 1; i < genome->getGenes()->Count; i++) {
			xmlFile->WriteStartElement("gene" + Convert::ToString(i));
			xmlFile->WriteStartElement("innode");
			xmlFile->WriteString(Convert::ToString(genome->getGenes()[i]->getInNode()));
			xmlFile->WriteEndElement();
			xmlFile->WriteStartElement("outnode");
			xmlFile->WriteString(Convert::ToString(genome->getGenes()[i]->getOutNode()));
			xmlFile->WriteEndElement();
			xmlFile->WriteStartElement("weight");
			xmlFile->WriteString(Convert::ToString(genome->getGenes()[i]->getWeight()));
			xmlFile->WriteEndElement();
			xmlFile->WriteStartElement("isenabled");
			xmlFile->WriteString(Convert::ToString(genome->getGenes()[i]->getEnabled()));
			xmlFile->WriteEndElement();
			xmlFile->WriteStartElement("innovationnum");
			xmlFile->WriteString(Convert::ToString(genome->getGenes()[i]->getInnovNum()));
			xmlFile->WriteEndElement();
			xmlFile->WriteEndElement();
		}
		xmlFile->WriteEndElement();
		xmlFile->WriteEndElement();

		xmlFile->WriteEndElement();
		xmlFile->WriteEndDocument();
		xmlFile->Close();
	}
	return;
}

Genome^ NeuralNetwork::loadGenome(String^ filename) {
	Genome^ genome = gcnew Genome();	

	XmlDocument^ xmlFile = gcnew XmlDocument();
	xmlFile->Load(IO::Directory::GetCurrentDirectory() + "\\" + filename);	
	XmlNode^ root = xmlFile->DocumentElement;

	playerForm->getLabelValues()[1] = Convert::ToDouble(root->SelectSingleNode("info")->SelectSingleNode("generation")->InnerText);
	playerForm->getLabelValues()[2] = Convert::ToDouble(root->SelectSingleNode("info")->SelectSingleNode("species")->InnerText);
	playerForm->getLabelValues()[3] = Convert::ToDouble(root->SelectSingleNode("info")->SelectSingleNode("genome")->InnerText);

	genome->setFitness(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("fitness")->InnerText));
	//genome->setAdjustedFitness(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("adjustedfitness")->InnerText));
	genome->setMaxNeuron(Convert::ToInt32(root->SelectSingleNode("object")->SelectSingleNode("maxneuron")->InnerText));
	genome->setGlobalRank(Convert::ToInt32(root->SelectSingleNode("object")->SelectSingleNode("globalrank")->InnerText));

	genome->setConnectionsMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("connectionsmr")->InnerText));
	genome->setLinkMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("linkmr")->InnerText));
	genome->setBiasMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("biasmr")->InnerText));
	genome->setNodeMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("nodemr")->InnerText));
	genome->setEnableMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("enablemr")->InnerText));
	genome->setDisableMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("disablemr")->InnerText));
	genome->setStepMR(Convert::ToDouble(root->SelectSingleNode("object")->SelectSingleNode("stepmr")->InnerText));

	for each (XmlNode^ gene in root->SelectSingleNode("object")->SelectSingleNode("genes")->ChildNodes) {
		Gene^ tempGene = gcnew Gene();
		tempGene->setInNode(Convert::ToInt32(gene->SelectSingleNode("innode")->InnerText));
		tempGene->setOutNode(Convert::ToInt32(gene->SelectSingleNode("outnode")->InnerText));
		tempGene->setWeight(Convert::ToDouble(gene->SelectSingleNode("weight")->InnerText));
		tempGene->setEnabled(Convert::ToBoolean(gene->SelectSingleNode("isenabled")->InnerText));
		tempGene->setInnovNum(Convert::ToInt32(gene->SelectSingleNode("innovationnum")->InnerText));
		genome->getGenes()->Add(tempGene);
	}

	return genome;
}

void NeuralNetwork::displayGenome(Genome^ genome) {
	List<int>^ neurons = gcnew List<int>();
	bool exists;
	int tempNeuron;
	double xSpacing;

	for (int i = 0; i <= GlobalVariables::MaxNodes + GlobalVariables::NumOutputs; i++) {
		playerForm->getCellX()->Add(0);
		playerForm->getCellY()->Add(0);
		playerForm->getCellValue()->Add(0);
	}	

	// hidden nodes
	for (int i = 1; i < genome->getGenes()->Count; i++) {
		Gene^ gene = genome->getGenes()[i];
		if (gene->getInNode() > GlobalVariables::NumInputs && gene->getInNode() <= GlobalVariables::MaxNodes) {
			exists = false;
			for (int j = 0; j < neurons->Count; j++)
				if (neurons[j] == gene->getInNode())
					exists = true;
			if (exists == false)
				neurons->Add(gene->getInNode());
		}
		if (gene->getOutNode() > GlobalVariables::NumInputs && gene->getOutNode() <= GlobalVariables::MaxNodes) {
			exists = false;
			for (int j = 0; j < neurons->Count; j++)
				if (neurons[j] == gene->getOutNode())
					exists = true;
			if (exists == false)
				neurons->Add(gene->getOutNode());
		}
	}

	for (int i = 1; i < genome->getGenes()->Count; i++) {
		Gene^ gene = genome->getGenes()[i];
		if (gene->getInNode() > GlobalVariables::NumInputs && gene->getInNode() <= GlobalVariables::MaxNodes && gene->getOutNode() > GlobalVariables::NumInputs && gene->getOutNode() <= GlobalVariables::MaxNodes) {
			if (neurons->IndexOf(gene->getInNode()) > neurons->IndexOf(gene->getOutNode())) {
				tempNeuron = neurons[neurons->IndexOf(gene->getInNode())];
				neurons[neurons->IndexOf(gene->getInNode())] = neurons[neurons->IndexOf(gene->getOutNode())];
				neurons[neurons->IndexOf(gene->getOutNode())] = tempNeuron;
			}
		}
	}

	xSpacing = (315.0 - 51.0) / (neurons->Count + 1);
	
	for (int i = 0; i < neurons->Count; i++) {
		playerForm->getCellX()[neurons[i]] = xSpacing*(i + 1) + 51;
		playerForm->getCellY()[neurons[i]] = rand() % (152 - 19) + 19;
	}

	// inputs
	for (int i = 0; i < 4; i++) {
		playerForm->getCellX()[i+1] = 42;
		playerForm->getCellY()[i+1] = 10 + (25 * i) + (i * 9);
	}
	playerForm->getCellX()[5] = 42;
	playerForm->getCellY()[5] = 161;
	
	// outputs
	playerForm->getCellX()[GlobalVariables::MaxNodes + 1] = 324;
	playerForm->getCellY()[GlobalVariables::MaxNodes + 1] = 25;
	playerForm->getCellX()[GlobalVariables::MaxNodes + 2] = 324;
	playerForm->getCellY()[GlobalVariables::MaxNodes + 2] = 83;
	playerForm->getCellX()[GlobalVariables::MaxNodes + 3] = 324;
	playerForm->getCellY()[GlobalVariables::MaxNodes + 3] = 148;

	// genes
	for (int i = 1; i < genome->getGenes()->Count; i++) {		
		playerForm->getInCell()->Add(genome->getGenes()[i]->getInNode());
		playerForm->getOutCell()->Add(genome->getGenes()[i]->getOutNode());		
		if (genome->getGenes()[i]->getEnabled() == false)
			playerForm->getGeneWeight()->Add(0);
		else
			playerForm->getGeneWeight()->Add(genome->getGenes()[i]->getWeight());

	}
}

void NeuralNetwork::updateCellValues(Genome^ genome) {
	for (int i = 1; i < genome->getNetwork()->getNeurons()->Count; i++) {
		if (genome->getNetwork()->getNeurons()[i] != nullptr)
			playerForm->getCellValue()[i] = genome->getNetwork()->getNeurons()[i]->getValue();
	}
}

void NeuralNetwork::clearDisplay() {
	playerForm->getCellX()->RemoveRange(0, playerForm->getCellX()->Count);
	playerForm->getCellY()->RemoveRange(0, playerForm->getCellY()->Count);
	playerForm->getCellValue()->RemoveRange(0, playerForm->getCellValue()->Count);
	playerForm->getInCell()->RemoveRange(0, playerForm->getInCell()->Count);
	playerForm->getOutCell()->RemoveRange(0, playerForm->getOutCell()->Count);
	playerForm->getGeneWeight()->RemoveRange(0, playerForm->getGeneWeight()->Count);
}

void NeuralNetwork::updateLabels() {
	if (playerForm->getLabelValues()->Count == 0)
		for (int i = 0; i < 6; i++)
			playerForm->getLabelValues()->Add(0);

	playerForm->getLabelValues()[0] = GlobalVariables::Population;
	playerForm->getLabelValues()[1] = pool->getGeneration();
	playerForm->getLabelValues()[2] = pool->getCurrentSpecies();
	playerForm->getLabelValues()[3] = pool->getCurrentGenome();
	playerForm->getLabelValues()[4] = 0;
	playerForm->getLabelValues()[5] = pool->getMaxFitness();
}

void NeuralNetwork::createLabels() {
	if (playerForm->getLabelValues()->Count == 0)
		for (int i = 0; i < 6; i++)
			playerForm->getLabelValues()->Add(0);
}

////////////////////////////// Pool //////////////////////////////

Pool::Pool() {
	species = gcnew List<Species^>();
	species->Add(nullptr);
	generation = 0;
	//innovation = GlobalVariables::NumOutputs; 
	innovation = 15;
	currentSpecies = 1; 
	currentGenome = 1; 
	maxFitness = 0;
	currentMaxFitness = 0;
	currentFrame = 0;
	maxFitnessSpecies = 0;
	maxFitnessGenome = 0;
	saveGenome = false;
}

List<Species^>^ Pool::getSpecies() {
	return species;
}

int Pool::getCurrentSpecies() {
	return currentSpecies;
}

void Pool::setCurrentSpecies(int num) {
	currentSpecies = num;
	return;
}

int Pool::getCurrentGenome() {
	return currentGenome;
}

void Pool::setCurrentGenome(int num) {
	currentGenome = num;
	return;
}

void Pool::addToSpecies(Genome^ genome) {
	for (int i = 1; i < species->Count; i++) {
		if (isSameSpecies(genome, species[i]->getGenome()[1])) {
			species[i]->getGenome()->Add(genome);
			return;
		}
	}

	Species^ newSpecies = gcnew Species();
	newSpecies->getGenome()->Add(genome);
	species->Add(newSpecies);
	return;
}

bool Pool::isSameSpecies(Genome^ genome1, Genome^ genome2) {
	int maxGenes = std::max(genome1->getGenes()->Count - 1, genome2->getGenes()->Count - 1);
	//double numDisjoint = numDisjointGenes(genome1, genome2);
	//double numExcess = numExcessGenes(genome1, genome2);
	double numDisjointExcess = numDisjointExcessGenes(genome1, genome2);
	double weight = weightDifferences(genome1, genome2);
	
	double compatibility = ((GlobalVariables::C1*numDisjointExcess) / maxGenes) + (GlobalVariables::C3*weight);
	
	if (compatibility < GlobalVariables::CompatibilityThreshold)
		return true;
	else
		return false;
}

int Pool::numDisjointExcessGenes(Genome^ genome1, Genome^ genome2) {
	List<bool>^ innovation1 = gcnew List<bool>();
	List<bool>^ innovation2 = gcnew List<bool>();
	int numDisjoint = 0;

	int maxInnov = 0;
	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (genome1->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome1->getGenes()[i]->getInnovNum();
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (genome2->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome2->getGenes()[i]->getInnovNum();

	for (int i = 0; i <= maxInnov; i++) {
		innovation1->Add(false);
		innovation2->Add(false);
	}

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		innovation1[genome1->getGenes()[i]->getInnovNum()] = true;
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		innovation1[genome2->getGenes()[i]->getInnovNum()] = true;

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (innovation2[genome1->getGenes()[i]->getInnovNum()] == false)
			numDisjoint++;

	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (innovation1[genome2->getGenes()[i]->getInnovNum()] == false)
			numDisjoint++;

	return numDisjoint;
}

// needs to be fixed
int Pool::numDisjointGenes(Genome^ genome1, Genome^ genome2) {
	List<bool>^ innovation1 = gcnew List<bool>();
	List<bool>^ innovation2 = gcnew List<bool>();
	int numDisjoint = 0;
	int bigGenomeMaxInnov = std::max(genome1->getGenes()[genome1->getGenes()->Count - 1]->getInnovNum(), genome2->getGenes()[genome2->getGenes()->Count - 1]->getInnovNum());
	int smallGenomeMaxInnov = std::min(genome1->getGenes()[genome1->getGenes()->Count - 1]->getInnovNum(), genome2->getGenes()[genome2->getGenes()->Count - 1]->getInnovNum());

	for (int i = 0; i <= bigGenomeMaxInnov; i++) {
		innovation1->Add(false);
		innovation2->Add(false);
	}

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		innovation1[genome1->getGenes()[i]->getInnovNum()] = true;
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		innovation1[genome2->getGenes()[i]->getInnovNum()] = true;

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (innovation2[genome1->getGenes()[i]->getInnovNum()] == false && genome1->getGenes()[i]->getInnovNum() <= smallGenomeMaxInnov)
			numDisjoint++;

	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (innovation1[genome2->getGenes()[i]->getInnovNum()] == false && genome2->getGenes()[i]->getInnovNum() <= smallGenomeMaxInnov)
			numDisjoint++;

	return numDisjoint;
}

// needs to be fixed
int Pool::numExcessGenes(Genome^ genome1, Genome^ genome2) {
	List<bool>^ innovation1 = gcnew List<bool>();
	List<bool>^ innovation2 = gcnew List<bool>();
	int numExcess = 0;
	int bigGenomeMaxInnov = std::max(genome1->getGenes()[genome1->getGenes()->Count - 1]->getInnovNum(), genome2->getGenes()[genome2->getGenes()->Count - 1]->getInnovNum());
	int smallGenomeMaxInnov = std::min(genome1->getGenes()[genome1->getGenes()->Count - 1]->getInnovNum(), genome2->getGenes()[genome2->getGenes()->Count - 1]->getInnovNum());

	for (int i = 0; i <= bigGenomeMaxInnov; i++) {
		innovation1->Add(false);
		innovation2->Add(false);
	}

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		innovation1[genome1->getGenes()[i]->getInnovNum()] = true;
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		innovation1[genome2->getGenes()[i]->getInnovNum()] = true;

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (innovation2[genome1->getGenes()[i]->getInnovNum()] == false && genome1->getGenes()[i]->getInnovNum() > smallGenomeMaxInnov)
			numExcess++;

	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (innovation1[genome2->getGenes()[i]->getInnovNum()] == false && genome2->getGenes()[i]->getInnovNum() > smallGenomeMaxInnov)
			numExcess++;

	return numExcess;
}

double Pool::weightDifferences(Genome^ genome1, Genome^ genome2) {
	List<Gene^>^ genes1 = gcnew List<Gene^>();
	double totalWeight = 0;
	int numMatching = 0;

	int maxInnov = 0;
	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (genome1->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome1->getGenes()[i]->getInnovNum();
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (genome2->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome2->getGenes()[i]->getInnovNum();

	for (int i = 0; i <= maxInnov; i++)
		genes1->Add(nullptr);

	for (int i = 1; i < genome1->getGenes()->Count; i++)
		genes1[genome1->getGenes()[i]->getInnovNum()] = genome1->getGenes()[i];

	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (genes1[genome2->getGenes()[i]->getInnovNum()] != nullptr) {
			totalWeight = totalWeight + abs(genes1[genome2->getGenes()[i]->getInnovNum()]->getWeight() - genome2->getGenes()[i]->getWeight());
			numMatching++;
		}

	if (totalWeight == 0)
		return 0;
	else
		return (totalWeight / numMatching);
}


int Pool::newInnovation() {
	innovation++;
	return innovation;
}

int Pool::getCurrentFrame() {
	return currentFrame;
}

void Pool::setCurrentFrame(int num) {
	currentFrame = num;
	return;
}

double Pool::getMaxFitness() {
	return maxFitness;
}

void Pool::setMaxFitness(double num) {
	maxFitness = num;
	return;
}

double Pool::getCurrentMaxFitness() {
	return currentMaxFitness;
}

void Pool::setCurrentMaxFitness(double num) {
	currentMaxFitness = num;
	return;
}

int Pool::getGeneration() {
	return generation;
}

int Pool::getMaxFitnessSpecies() {
	return maxFitnessSpecies;
}

void Pool::setMaxFitnessSpecies(int num) {
	maxFitnessSpecies = num;
	return;
}

int Pool::getMaxFitnessGenome() {
	return maxFitnessGenome;
}

void Pool::setMaxFitnessGenome(int num) {
	maxFitnessGenome = num;
	return;
}

bool Pool::getSaveGenome() {
	return saveGenome;
}

void Pool::setSaveGenome(bool willBeSaved) {
	saveGenome = willBeSaved;
	return;
}

void Pool::newGeneration() {	
	
	double sum;
	int breed;
	List<Genome^>^ offspring = gcnew List<Genome^>();
	
	cullWeakGenomes();
	cullSpecies(false);

	for (int i = 1; i < species->Count; i++) 
		species[i]->calculateAdjustedFitness();		
			
	removeWeakSpecies2();
	
	sum = totalAdjustedFitness();	
	for (int i = 1; i < species->Count; i++) {
		breed = floor((species[i]->getTotalAdjustedFitness() / (double)sum) * GlobalVariables::Population);
		for (int j = 0; j < breed; j++)
			offspring->Add(species[i]->breedChild());  
	}
	while (offspring->Count + species->Count - 1 < GlobalVariables::Population)
		offspring->Add(species[(rand() % (species->Count - 1)) + 1]->breedChild());
	
	cullSpecies(true);		
			
	for (int i = 0; i < offspring->Count; i++)
		addToSpecies(offspring[i]);

	generation = generation + 1;
	return;
	
	/*
	double sum;
	int breed;
	List<Genome^>^ offspring = gcnew List<Genome^>();

	cullSpecies(false);
	rankGlobally();
	removeStaleSpecies();
	rankGlobally();
	
	for (int i = 1; i < species->Count; i++)
		species[i]->calculateAverageFitness();

	removeWeakSpecies();

	sum = totalAverageFitness();
	for (int i = 1; i < species->Count; i++) {
		breed = floor((species[i]->getAverageFitness() / (double)sum) * GlobalVariables::Population) - 1;
		for (int j = 0; j < breed; j++)
			offspring->Add(species[i]->breedChild());
	}

	cullSpecies(true);

	while (offspring->Count + species->Count - 1 < GlobalVariables::Population)
		offspring->Add(species[(rand() % (species->Count - 1)) + 1]->breedChild());

	for (int i = 0; i < offspring->Count; i++)
		addToSpecies(offspring[i]);

	generation++;
	return;
	*/
}

// if cullAllButOne = false then cull bottom 80%, if cullAllButOne = true then cull all but the top member
void Pool::cullSpecies(bool cullAllButOne) {
	int remaining;
	for (int i = 1; i < species->Count; i++) {

		species[i]->sortGenomesDescending(1, species[i]->getGenome()->Count - 1);
		
		if (cullAllButOne == true)
			remaining = 1 + 1;
		else
			remaining = ceil((species[i]->getGenome()->Count - 1) * GlobalVariables::SurvivalThreshold) + 1;
		
		while (species[i]->getGenome()->Count > remaining)
			species[i]->getGenome()->RemoveAt(species[i]->getGenome()->Count - 1);
	}
	return;
}

void Pool::cullWeakGenomes() {	
	List<int>^ speciesIndex = gcnew List<int>();

	for (int i = 1; i < species->Count; i++) {
		List<int>^ genomeIndex = gcnew List<int>();

		for (int j = 1; j < species[i]->getGenome()->Count; j++)
			if (species[i]->getGenome()[j]->getFitness() < 0)
				genomeIndex->Add(j);

		genomeIndex->Reverse();

		for (int j = 0; j < genomeIndex->Count; j++)
				species[i]->getGenome()->RemoveAt(genomeIndex[j]);

		if (species[i]->getGenome()->Count < 2)
			speciesIndex->Add(i);
	}

	speciesIndex->Reverse();

	for (int i = 0; i < speciesIndex->Count; i++)
		species->RemoveAt(speciesIndex[i]);

	return;
}

double Pool::totalAdjustedFitness() {
	double total = 0;
	for (int i = 1; i < species->Count; i++)
		total = total + species[i]->getTotalAdjustedFitness();
	return total;
}

void Pool::rankGlobally() {
	int count = 1;
	Species^ globalSpecies = gcnew Species();

	for (int i = 1; i < species->Count; i++)
		for (int j = 1; j < species[i]->getGenome()->Count; j++) 
			globalSpecies->getGenome()->Add(nullptr);

	for (int i = 1; i < species->Count; i++)
		for (int j = 1; j < species[i]->getGenome()->Count; j++) {
			globalSpecies->getGenome()[count] = species[i]->getGenome()[j];
			count++;
		}
		
	globalSpecies->sortGenomesAscending(1, globalSpecies->getGenome()->Count - 1);

	for (int i = 1; i < globalSpecies->getGenome()->Count; i++)
		globalSpecies->getGenome()[i]->setGlobalRank(i);

	return;
}

void Pool::removeStaleSpecies() {
	List<Species^>^ survived = gcnew List<Species^>();
	survived->Add(nullptr);
	for (int i = 1; i < species->Count; i++) {
		species[i]->sortGenomesDescending(1, species[i]->getGenome()->Count - 1);
		if (species[i]->getGenome()[1]->getFitness() > species[i]->getTopFitness()) {
			species[i]->setTopFitness(species[i]->getGenome()[1]->getFitness());
			species[i]->setStaleness(0);
		}
		else {
			species[i]->setStaleness(species[i]->getStaleness() + 1);
		}
		if (species[i]->getStaleness() < GlobalVariables::StaleSpecies || species[i]->getTopFitness() >= maxFitness)
			survived->Add(species[i]);
	}
	species = survived;
	return;
}

void Pool::removeWeakSpecies() {
	int breed;
	List<Species^>^ survived = gcnew List<Species^>();
	survived->Add(nullptr);
	for (int i = 1; i < species->Count; i++) {
		breed = floor(species[i]->getAverageFitness() / totalAverageFitness() * GlobalVariables::Population);
		if (breed >= 1)
			survived->Add(species[i]);
	}
	species = survived;
	return;
}

void Pool::removeWeakSpecies2() {
	List<int>^ speciesIndex = gcnew List<int>();

	double sum = totalAdjustedFitness();
	for (int i = 1; i < species->Count; i++)
		if (floor((species[i]->getTotalAdjustedFitness() / (double)sum) * GlobalVariables::Population) < 1)
			speciesIndex->Add(i);

	speciesIndex->Reverse();

	for (int i = 0; i < speciesIndex->Count; i++)
		species->RemoveAt(speciesIndex[i]);

	return;
}

double Pool::totalAverageFitness() {
	double total = 0;
	for (int i = 1; i < species->Count; i++)
		total = total + species[i]->getAverageFitness();
	return total;
}

////////////////////////////// Species //////////////////////////////

Species::Species() {
	genomes = gcnew List<Genome^>();
	genomes->Add(nullptr);
	topFitness = 0;
	staleness = 0;
	totalAdjustedFitness = 0;
	averageFitness = 0;
}

List<Genome^>^ Species::getGenome() {
	return genomes;
}

double Species::getTotalAdjustedFitness() {
	return totalAdjustedFitness;
}

double Species::getTopFitness() {
	return topFitness;
}

void Species::setTopFitness(double num) {
	topFitness = num;
	return;
}

int Species::getStaleness() {
	return staleness;
}

void Species::setStaleness(int num) {
	staleness = num;
	return;
}

double Species::getAverageFitness() {
	return averageFitness;
}

void Species::sortGenomesDescending(int first, int last) {
	if (last - first > 0) {		
		// generate random pivot
		double pivot = genomes[rand() % (last - first + 1) + first]->getFitness();
		int left = first;
		int right = last;
		Genome^ tempGenome;
		// exit loop when both pointers point to pivot (this occurs when they are equal)
		while (left <= right) {			
			// find left element that is less than or equal to pivot
			while (genomes[left]->getFitness() > pivot)
				left++;
			// find right element that is greater than or equal to pivot
			while (genomes[right]->getFitness() < pivot)
				right--;
			// swap left and right elements
			if (left <= right) {
				tempGenome = genomes[left];
				genomes[left] = genomes[right];
				genomes[right] = tempGenome;
				left++;
				right--;
			}
		}
		// call function recursively to continue sorting 
		sortGenomesDescending(first, right);
		sortGenomesDescending(left, last);
	}
	return;
}

void Species::sortGenomesAscending(int first, int last) {
	if (last - first > 0) {
		// generate random pivot
		double pivot = genomes[rand() % (last - first + 1) + first]->getFitness();
		int left = first;
		int right = last;
		Genome^ tempGenome;
		// exit loop when both pointers point to pivot (this occurs when they are equal)
		while (left <= right) {
			// find left element that is greater than or equal to pivot
			while (genomes[left]->getFitness() < pivot)
				left++;
			// find right element that is less than or equal to pivot
			while (genomes[right]->getFitness() > pivot)
				right--;
			// swap left and right elements
			if (left <= right) {
				tempGenome = genomes[left];
				genomes[left] = genomes[right];
				genomes[right] = tempGenome;
				left++;
				right--;
			}
		}
		// call function recursively to continue sorting 
		sortGenomesDescending(first, right);
		sortGenomesDescending(left, last);
	}
	return;
}

void Species::calculateAdjustedFitness() {
	double total = 0;
	for (int i = 1; i < genomes->Count; i++) {		
		genomes[i]->setAdjustedFitness(genomes[i]->getFitness() / (genomes->Count - 1));
		total = total + genomes[i]->getAdjustedFitness();
	}
	totalAdjustedFitness = total;
	return;
}

void Species::calculateAverageFitness() {
	int total = 0;
	for (int i = 1; i < genomes->Count; i++)
		total = total + genomes[i]->getGlobalRank();
	averageFitness = (double)total / genomes->Count;
	return;
}

Genome^ Species::breedChild() {
	Genome^ child;
	Genome^ genome1 = genomes[(rand() % (genomes->Count - 1)) + 1];
	Genome^ genome2 = genomes[(rand() % (genomes->Count - 1)) + 1];
			
	if (((double)rand() / (RAND_MAX)) < GlobalVariables::CrossoverChance)
		child = crossover(genome1, genome2);
	else
		child = genome1->copyGenome();

	child->mutate();

	return child;
}

Genome^ Species::crossover(Genome^ genome1, Genome^ genome2) {
	Genome^ tempGenome;
	List<Gene^>^ innovation2 = gcnew List<Gene^>();
	Genome^ child = gcnew Genome();
	
	int maxInnov = 0;
	for (int i = 1; i < genome1->getGenes()->Count; i++)
		if (genome1->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome1->getGenes()[i]->getInnovNum();
	for (int i = 1; i < genome2->getGenes()->Count; i++)
		if (genome2->getGenes()[i]->getInnovNum() > maxInnov)
			maxInnov = genome2->getGenes()[i]->getInnovNum();


	// make sure genome1 has the higher fitness
	if (genome2->getFitness() > genome2->getFitness()) {
		tempGenome = genome1;
		genome1 = genome2;
		genome2 = tempGenome;
	}

	for (int i = 0; i <= maxInnov; i++)
		innovation2->Add(nullptr);
	
	for (int i = 1; i < genome2->getGenes()->Count; i++) 
		innovation2[genome2->getGenes()[i]->getInnovNum()] = genome2->getGenes()[i];

	for (int i = 1; i < genome1->getGenes()->Count; i++) {
		// if it is a matching gene, and genome2->gene is enabled, and 50% chance
		if (innovation2[genome1->getGenes()[i]->getInnovNum()] != nullptr && rand() % 2 == 0 && innovation2[genome1->getGenes()[i]->getInnovNum()]->getEnabled() == true)
			child->getGenes()->Add(innovation2[genome1->getGenes()[i]->getInnovNum()]->copyGene());
		else
			child->getGenes()->Add(genome1->getGenes()[i]);
	}

	child->setParentPool(genome1->getParentPool());
	child->setMaxNeuron(std::max(genome1->getMaxNeuron(), genome2->getMaxNeuron()));

	child->setConnectionsMR(genome1->getConnectionsMR());
	child->setLinkMR(genome1->getLinkMR());
	child->setBiasMR(genome1->getBiasMR());
	child->setNodeMR(genome1->getNodeMR());
	child->setEnableMR(genome1->getEnableMR());
	child->setDisableMR(genome1->getDisableMR());
	child->setStepMR(genome1->getStepMR());

	return child;
}

////////////////////////////// Genome //////////////////////////////

Genome::Genome() {
	genes = gcnew List<Gene^>();
	genes->Add(nullptr);
	network = gcnew Network();
	fitness = 0;
	adjustedFitness = 0;
	maxNeuron = 0;
	globalRank = 0;
	connectionsMR = GlobalVariables::MutateConnectionsChance;
	linkMR = GlobalVariables::LinkMutationChance;
	biasMR = GlobalVariables::BiasMutationChance;
	nodeMR = GlobalVariables::NodeMutationChance;
	enableMR = GlobalVariables::EnableMutationChance;
	disableMR = GlobalVariables::DisableMutationChance;
	stepMR = GlobalVariables::StepSize;
}

int Genome::getMaxNeuron() {
	return maxNeuron;
}

void Genome::setMaxNeuron(int num) {
	maxNeuron = num;
	return;
}

List<Gene^>^ Genome::getGenes() {
	return genes;
}

Pool^ Genome::getParentPool() {
	return parentPool;
}

void Genome::setParentPool(Pool^ pool) {
	parentPool = pool;
	return;
}

double Genome::getFitness() {
	return fitness;
}

void Genome::setFitness(double num) {
	fitness = num;
	return;
}

double Genome::getAdjustedFitness() {
	return adjustedFitness;
}

void Genome::setAdjustedFitness(double num) {
	adjustedFitness = num;
	return;
}

int Genome::getGlobalRank() {
	return globalRank;
}

void Genome::setGlobalRank(int num) {
	globalRank = num;
	return;
}

Network^ Genome::getNetwork() {
	return network;
}

double Genome::getConnectionsMR() {
	return connectionsMR;
}

void Genome::setConnectionsMR(double num) {
	connectionsMR = num;
	return;
}

double Genome::getLinkMR() {
	return linkMR;
}

void Genome::setLinkMR(double num) {
	linkMR = num;
	return;
}

double Genome::getBiasMR() {
	return biasMR;
}

void Genome::setBiasMR(double num) {
	biasMR = num;
	return;
}

double Genome::getNodeMR() {
	return nodeMR;
}

void Genome::setNodeMR(double num) {
	nodeMR = num;
	return;
}

double Genome::getEnableMR() {
	return enableMR;
}

void Genome::setEnableMR(double num) {
	enableMR = num;
	return;
}

double Genome::getDisableMR() {
	return disableMR;
}

void Genome::setDisableMR(double num) {
	disableMR = num;
	return;
}

double Genome::getStepMR() {
	return stepMR;
}

void Genome::setStepMR(double num) {
	stepMR = num;
	return;
}

void Genome::createBasicGenes() {
	for (int i = 1; i <= GlobalVariables::NumInputs; i++) {
		for (int j = 1; j <= GlobalVariables::NumOutputs; j++) {
			Gene^ gene = gcnew Gene();
			gene->setInNode(i);
			gene->setOutNode(GlobalVariables::MaxNodes + j);
			gene->setEnabled(true);
			gene->setInnovNum(3*(i-1) + j);
			gene->setWeight(((double)rand() / (RAND_MAX)) * 4 - 2);
			genes->Add(gene);
		}
	}
	return;
}

void Genome::mutate() {
	double p;

	randomizeRates();

	if (((double)rand() / (RAND_MAX)) < connectionsMR)
		pointMutate();

	p = linkMR;
	while (p > 0) {
		if (((double)rand() / (RAND_MAX)) < p) 
			linkMutate(false);
		p = p - 1;
	}

	p = biasMR;
	while (p > 0) {
		if (((double)rand() / (RAND_MAX)) < p)
			linkMutate(true);
		p = p - 1;
	}
		
	p = nodeMR;
	while (p > 0) {
		if (((double)rand() / (RAND_MAX)) < p)
			nodeMutate();
		p = p - 1;
	}

	p = enableMR;
	while (p > 0) {
		if (((double)rand() / (RAND_MAX)) < p)
			enableDisableMutate(true);
		p = p - 1;
	}

	p = disableMR;
	while (p > 0) {
		if (((double)rand() / (RAND_MAX)) < p)
			enableDisableMutate(false);
		p = p - 1;
	}
}

void Genome::randomizeRates() {
	if (rand() % 2 == 0)
		connectionsMR = connectionsMR * 0.95;
	else
		connectionsMR = connectionsMR * 1.05263;

	if (rand() % 2 == 0)
		linkMR = linkMR * 0.95;
	else
		linkMR = linkMR * 1.05263;

	if (rand() % 2 == 0)
		biasMR = biasMR * 0.95;
	else
		biasMR = biasMR * 1.05263;

	if (rand() % 2 == 0)
		nodeMR = nodeMR * 0.95;
	else
		nodeMR = nodeMR * 1.05263;

	if (rand() % 2 == 0)
		enableMR = enableMR * 0.95;
	else
		enableMR = enableMR * 1.05263;

	if (rand() % 2 == 0)
		disableMR = disableMR * 0.95;
	else
		disableMR = disableMR * 1.05263;

	if (rand() % 2 == 0)
		stepMR = stepMR * 0.95;
	else
		stepMR = stepMR * 1.05263;
}

void Genome::pointMutate() {
	for (int i = 1; i < genes->Count; i++)
		if (((double)rand() / (RAND_MAX)) < GlobalVariables::PerturbChance)
			genes[i]->setWeight(genes[i]->getWeight() + ((double)rand() / (RAND_MAX)) * stepMR * 2 - stepMR);
		else
			genes[i]->setWeight(((double)rand() / (RAND_MAX)) * 4 - 2);
}

void Genome::linkMutate(bool forceBias) {
	int neuron1 = randomNeuron(false);
	int neuron2 = randomNeuron(true);
	int temp;
	
	Gene^ newLink = gcnew Gene();

	if (neuron1 <= GlobalVariables::NumInputs && neuron2 <= GlobalVariables::NumInputs) // both are input nodes
		return;

	if (neuron2 <= GlobalVariables::NumInputs) {
		temp = neuron1;
		neuron1 = neuron2;
		neuron2 = temp;
	}

	newLink->setInNode(neuron1);
	newLink->setOutNode(neuron2);
	if (forceBias == true)
		newLink->setInNode(GlobalVariables::NumInputs);

	if (containsLink(newLink) == true)
		return;

	newLink->setInnovNum(parentPool->newInnovation());
	newLink->setWeight(((double)rand() / (RAND_MAX)) * 4 - 2);
	
	genes->Add(newLink);
	return;
}

void Genome::nodeMutate() {
	Gene^ gene;
	Gene^ gene1;
	Gene^ gene2;

	if (genes->Count == 0)
		return;

	maxNeuron = maxNeuron + 1;

	gene = genes[(rand() % (genes->Count-1)) + 1];
	if (gene->getEnabled() == false)
		return;
	gene->setEnabled(false);

	gene1 = gene->copyGene();
	gene1->setOutNode(maxNeuron);
	gene1->setWeight(1.0);
	gene1->setInnovNum(parentPool->newInnovation());
	gene1->setEnabled(true);
	genes->Add(gene1);

	gene2 = gene->copyGene();
	gene2->setInNode(maxNeuron);
	gene2->setInnovNum(parentPool->newInnovation());
	gene2->setEnabled(true);
	genes->Add(gene2);

	return;
}

void Genome::enableDisableMutate(bool enable) {
	List<Gene^>^ candidates = gcnew List<Gene^>();

	for (int i = 1; i < genes->Count; i++)
		if (genes[i]->getEnabled() != enable)
			candidates->Add(genes[i]);

	if (candidates->Count == 0)
		return;
	else {
		candidates[rand() % candidates->Count]->setEnabled(!enable);
		return;
	}
}

int Genome::randomNeuron(bool nonInput) {
	List<bool>^ neurons = gcnew List<bool>();
	int count = 0;
	int n;

	for (int i = 0; i <= GlobalVariables::MaxNodes + GlobalVariables::NumOutputs; i++)
		neurons->Add(false);

	if (nonInput == false)
		for (int i = 1; i <= GlobalVariables::NumInputs; i++) 
			neurons[i] = true;
			
	for (int i = 1; i <= GlobalVariables::NumOutputs; i++) 
		neurons[GlobalVariables::MaxNodes + i] = true;
		
	for (int i = 1; i < genes->Count; i++) {
		if (nonInput == false || genes[i]->getInNode() > GlobalVariables::NumInputs) 
			neurons[genes[i]->getInNode()] = true;			
		if (nonInput == false || genes[i]->getOutNode() > GlobalVariables::NumInputs) 
			neurons[genes[i]->getOutNode()] = true;
	}

	for (int i = 0; i < neurons->Count; i++)
		if (neurons[i] == true)
			count++;

	n = rand() % count + 1;
			
	for (int i = 1; i <= GlobalVariables::MaxNodes + GlobalVariables::NumOutputs; i++) {
		if (neurons[i] == true) 
			n = n - 1;
		if (n == 0) 
			return i;
	}
	
	return 0; 
}

bool Genome::containsLink(Gene^ link) { 
	for (int i = 1; i < genes->Count; i++) 
		if (genes[i]->getInNode() == link->getInNode() && genes[i]->getOutNode() == link->getOutNode())
			return true;
	return false;
}

void Genome::generateNetwork() {
	for (int i = 1; i <= GlobalVariables::MaxNodes + GlobalVariables::NumOutputs; i++)
		network->getNeurons()->Add(nullptr);
	
	for (int i = 1; i <= GlobalVariables::NumInputs; i++)
		network->getNeurons()[i] = gcnew Neuron();
	
	for (int i = 1; i <= GlobalVariables::NumOutputs; i++)
		network->getNeurons()[GlobalVariables::MaxNodes + i] = gcnew Neuron();
	
	sortGenes(1, genes->Count - 1);

	for (int i = 1; i < genes->Count; i++) 
		if (genes[i]->getEnabled() == true) {
			if (network->getNeurons()[genes[i]->getOutNode()] == nullptr)
				network->getNeurons()[genes[i]->getOutNode()] = gcnew Neuron();
			network->getNeurons()[genes[i]->getOutNode()]->getIncomingGenes()->Add(genes[i]);
			if (network->getNeurons()[genes[i]->getInNode()] == nullptr)
				network->getNeurons()[genes[i]->getInNode()] = gcnew Neuron();
		}
}

void Genome::sortGenes(int first, int last) {
	if (last - first > 0) {
		// generate random pivot
		int pivot = genes[rand() % (last - first + 1) + first]->getOutNode();
		int left = first;
		int right = last;
		Gene^ tempGene;
		// exit loop when both pointers point to pivot (this occurs when they are equal)
		while (left <= right) {
			// find left element that is greater than or equal to pivot
			while (genes[left]->getOutNode() < pivot)
				left++;
			// find right element that is less than or equal to pivot
			while (genes[right]->getOutNode() > pivot)
				right--;
			// swap left and right elements
			if (left <= right) {
				tempGene = genes[left];
				genes[left] = genes[right];
				genes[right] = tempGene;
				left++;
				right--;
			}
		}
		// call function recursively to continue sorting 
		sortGenes(first, right);
		sortGenes(left, last);
	}
	return;
}

List<bool>^ Genome::evaluateNetwork(List<double>^ inputs) {
	double sum;
	Gene^ incomingGene;
	Neuron^ incomingNeuron;
	
	for (int i = 1; i <= GlobalVariables::NumInputs; i++)
		network->getNeurons()[i]->setValue(inputs[i]);
		
	for (int i = 1; i < network->getNeurons()->Count; i++) {
		sum = 0;			
		if (network->getNeurons()[i] != nullptr) {
			for (int j = 1; j < network->getNeurons()[i]->getIncomingGenes()->Count; j++) {
				incomingGene = network->getNeurons()[i]->getIncomingGenes()[j];
				incomingNeuron = network->getNeurons()[incomingGene->getInNode()];
				sum = sum + incomingGene->getWeight() * incomingNeuron->getValue();
			}
			if (network->getNeurons()[i]->getIncomingGenes()->Count > 1)
				network->getNeurons()[i]->setValue(sigmoid(sum));
		}
	}
	
	List<bool>^ outputs = gcnew List<bool>();
	for (int i = 1; i <= GlobalVariables::NumOutputs; i++) {
		if (network->getNeurons()[GlobalVariables::MaxNodes + i]->getValue() > 0)
			outputs->Add(true);
		else
			outputs->Add(false);
	}

	return outputs;
}

double Genome::sigmoid(double x) {
	//return (1 / (1 + exp(-x - GlobalVariables::SigmoidBias)));
	return (2 / (1 + exp(-4.9 * x)) - 1);
}

Genome^ Genome::copyGenome() {
	Genome^ newGenome = gcnew Genome();

	for (int i = 1; i < genes->Count; i++)
		newGenome->getGenes()->Add(genes[i]->copyGene());

	newGenome->setParentPool(parentPool);
	newGenome->setMaxNeuron(maxNeuron);
	newGenome->setConnectionsMR(connectionsMR);
	newGenome->setLinkMR(linkMR);
	newGenome->setBiasMR(biasMR);
	newGenome->setNodeMR(nodeMR);
	newGenome->setEnableMR(enableMR);
	newGenome->setDisableMR(disableMR);
	newGenome->setStepMR(stepMR);

	return newGenome;
}

////////////////////////////// Network //////////////////////////////

Network::Network() {
	neurons = gcnew List<Neuron^>();
	neurons->Add(nullptr);
}

List<Neuron^>^ Network::getNeurons() {
	return neurons;
}

////////////////////////////// Gene //////////////////////////////

Gene::Gene() {
	inNode = 0;
	outNode = 0;
	weight = 0;
	isEnabled = true;
	innovationNum = 0;
}

int Gene::getInnovNum() {
	return innovationNum;
}

void Gene::setInnovNum(int num) {
	innovationNum = num;
	return;
}

double Gene::getWeight() {
	return weight;
}

void Gene::setWeight(double num) {
	weight = num;
	return;
}

bool Gene::getEnabled() {
	return isEnabled;
}

void Gene::setEnabled(bool enable) {
	isEnabled = enable;
	return;
}

int Gene::getInNode() {
	return inNode;
}

void Gene::setInNode(int num) {
	inNode = num;
	return;
}

int Gene::getOutNode() {
	return outNode;
}
void Gene::setOutNode(int num) {
	outNode = num;
	return;
}

Gene^ Gene::copyGene() {
	Gene^ newGene = gcnew Gene();
	newGene->setInNode(inNode);
	newGene->setOutNode(outNode);
	newGene->setWeight(weight);
	newGene->setEnabled(isEnabled);
	newGene->setInnovNum(innovationNum);
	return newGene;
}

////////////////////////////// Neuron //////////////////////////////

Neuron::Neuron() {
	incomingGenes = gcnew List<Gene^>();
	incomingGenes->Add(nullptr);
	value = 0.0;
}

List<Gene^>^ Neuron::getIncomingGenes() {
	return incomingGenes;
}

double Neuron::getValue() {
	return value;
}

void Neuron::setValue(double num) {
	value = num;
	return;
}
