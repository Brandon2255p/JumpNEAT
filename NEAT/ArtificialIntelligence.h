#pragma once

using namespace System::Collections::Generic;

static ref class GlobalVariables {
public:
	double compatibilityThreshold;
	double c1;
	double c2;
	double c3;
};

public ref class Neat {
public: 
	Neat();
private:
	List<Genome^>^ genomes;
	List<Genome^>^ species;
};

public ref class Species {
public: 
	Species();
private:
	List<Genome^>^ genomes;
	Genome^ repGenome;
};

public ref class Genome {
public: 
	Genome();
private:
	List<NodeGene^>^ nodeGenes;
	List<ConnectionGene^>^ connectionGenes;
	int nextInnovNum; 
	double fitness;
	double adjustedFitness;
};

public ref class NodeGene {
public:
	NodeGene();
private:
	int nodeType; // -1 is input, 0 is hidden, 1 is output
};

public ref class ConnectionGene {
public:
	ConnectionGene();
private: 
	NodeGene^ inNode;
	NodeGene^ outNode;
	double weight;
	bool isEnabled;
	int innovationNum;
};

