#pragma once

#include "PlayerForm.h"

#include <algorithm>
#include <stdlib.h>   
#include <time.h> 
#include <iostream>

using namespace System::Collections::Generic;
using namespace GUI;
using namespace Xml;

ref class GlobalVariables;
ref class NeuralNetwork;
ref class Pool;
ref class Species;
ref class Genome;
ref class Network;
ref class Gene;
ref class Neuron;

static ref class GlobalVariables {
public:
	static int Population = 300;

	static int NumInputs = 4+1;
	static int NumOutputs = 3;
	static int MaxNodes = 1000;

	static double MutateConnectionsChance = 0.25;
	static double PerturbChance = 0.90;
	static double CrossoverChance = 0.75; // set to 1 if you want all new organisms in new generation to be bred, rather than some to be cloned
	static double LinkMutationChance = 2.0;
	static double BiasMutationChance = 0.4;
	static double NodeMutationChance = 0.5;
	static double EnableMutationChance = 0.2;
	static double DisableMutationChance = 0.4;
	static double StepSize = 0.1;

	static double SurvivalThreshold = 0.2;

	static int StaleSpecies = 15;
	static int TimeoutConstant = 20;

	static double CompatibilityThreshold = 3.0;
	static double C1 = 2.0;
	static double C2 = 2.0;
	static double C3 = 1.0;

	//static double SigmoidBias = 0;
};

public ref class NeuralNetwork {
public:
	NeuralNetwork();
	void runPlayerForm();
	void runNeuralNetwork();
	void runGenome(String^);
	void initializePool();
	void initializeRun();
	Genome^ createBasicGenome();
	void evaluateCurrent();
	void evaluateGenome(Genome^);
	bool fitnessAlreadyMeasured();
	void nextGenome();
	void saveGenome();
	Genome^ loadGenome(String^);
	void displayGenome(Genome^);
	void updateCellValues(Genome^);	
	void clearDisplay();
	void updateLabels();
	void createLabels();
				
	double getCharacterX(); // character x position
	double getCharacterY(); // character y position
	double getEnemy1(); // enemy1 x position
	double getEnemy2(); // enemy2 x position
private:
	PlayerForm^ playerForm;
	Pool^ pool;

	int rightmost;
	int timeout;
};

public ref class Pool {
public:
	Pool();
	List<Species^>^ getSpecies();
	int getCurrentSpecies();
	void setCurrentSpecies(int);
	int getCurrentGenome();
	void setCurrentGenome(int);
	int getCurrentFrame();
	void setCurrentFrame(int);
	double getMaxFitness();
	void setMaxFitness(double);
	double getCurrentMaxFitness();
	void setCurrentMaxFitness(double);
	int getGeneration();
	int getMaxFitnessSpecies();
	void setMaxFitnessSpecies(int);
	int getMaxFitnessGenome();
	void setMaxFitnessGenome(int);
	bool getSaveGenome();
	void setSaveGenome(bool);

	void addToSpecies(Genome^);
	bool isSameSpecies(Genome^, Genome^);
	int numExcessGenes(Genome^, Genome^);
	int numDisjointGenes(Genome^, Genome^);
	int numDisjointExcessGenes(Genome^, Genome^);
	double weightDifferences(Genome^, Genome^);
	int newInnovation();
	void newGeneration();
	void cullSpecies(bool);
	void cullWeakGenomes();
	void rankGlobally();
	void removeStaleSpecies();
	void removeWeakSpecies();
	void removeWeakSpecies2();
	double totalAdjustedFitness();
	double totalAverageFitness();

private:
	List<Species^>^ species;
	int generation;
	int innovation;
	int currentSpecies;
	int currentGenome;
	int currentFrame; 
	double maxFitness;
	double currentMaxFitness;
	int maxFitnessSpecies;
	int maxFitnessGenome;
	bool saveGenome;
};

public ref class Species {
public:
	Species();
	List<Genome^>^ getGenome();
	double getTotalAdjustedFitness();
	double getTopFitness();
	void setTopFitness(double);
	int getStaleness();
	void setStaleness(int);
	double getAverageFitness();

	void sortGenomesAscending(int, int);
	void sortGenomesDescending(int, int);
	void calculateAdjustedFitness();	
	void calculateAverageFitness();
	Genome^ breedChild();
	Genome^ crossover(Genome^, Genome^);
private:
	List<Genome^>^ genomes;

	double topFitness;
	int staleness;
	double totalAdjustedFitness;	
	double averageFitness;
};

public ref class Genome {
public:
	Genome();
	int getMaxNeuron();
	void setMaxNeuron(int);
	List<Gene^>^ getGenes();
	Pool^ getParentPool();
	void setParentPool(Pool^);
	double getFitness();
	void setFitness(double);
	double getAdjustedFitness();
	void setAdjustedFitness(double);
	int getGlobalRank();
	void setGlobalRank(int);
	Network^ getNetwork();

	double getConnectionsMR();
	void setConnectionsMR(double);
	double getLinkMR();
	void setLinkMR(double);
	double getBiasMR();
	void setBiasMR(double);
	double getNodeMR();
	void setNodeMR(double);
	double getEnableMR();
	void setEnableMR(double);
	double getDisableMR();
	void setDisableMR(double);
	double getStepMR();
	void setStepMR(double);

	void createBasicGenes();
	void mutate();
	void randomizeRates();
	void pointMutate();
	void linkMutate(bool);
	void nodeMutate();
	void enableDisableMutate(bool);
	int randomNeuron(bool);
	bool containsLink(Gene^);	
	void generateNetwork();
	void sortGenes(int, int);
	List<bool>^ evaluateNetwork(List<double>^);
	double sigmoid(double);
	Genome^ copyGenome();
private:
	List<Gene^>^ genes; 

	Network^ network;
	Pool^ parentPool;

	double fitness;
	double adjustedFitness;
	int maxNeuron;
	int globalRank;

	// mutation rates
	double connectionsMR;
	double linkMR;
	double biasMR;
	double nodeMR;
	double enableMR;
	double disableMR;
	double stepMR;
};

public ref class Network {
public:
	Network();
	List<Neuron^>^ getNeurons();
private:
	List<Neuron^>^ neurons;
};

public ref class Neuron { // node gene
public:
	Neuron();
	List<Gene^>^ getIncomingGenes();
	double getValue();
	void setValue(double);
private:
	List<Gene^>^ incomingGenes;
	double value;
};

public ref class Gene { // connection gene
public:
	Gene();
	int getInnovNum();
	void setInnovNum(int);
	double getWeight();
	void setWeight(double);
	bool getEnabled();
	void setEnabled(bool);
	int getInNode();
	void setInNode(int);
	int getOutNode();
	void setOutNode(int);

	Gene^ copyGene();
private:
	int inNode;
	int outNode;
	double weight;
	bool isEnabled;
	int innovationNum;
};
