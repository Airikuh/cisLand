//Program Name: CIS-Land Gets Internet Access	
//Programmer Name:Erika Valle-Baird
//Description:Make a program that will find the lowest cost to install fiber-optic cable lines.  
//These lines will connect to every town in CIS-Land and provide internet access to these towns. 
//This program will use a priority queue, along with Prim’s algorithm, to determine the minimum 
//spanning tree along with its cost.  This program will allow the user to connect any vertices missing from the graph, 
//add an edge, increase or decrease a weight, and determine the shortest path determined by Dijkstra’s algorithm.  
//Date Created:07/14/2020
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<queue>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <set>

using namespace std;

#define INFINITE 0x3f3f3f3f 

 //Adjacency List Class
class AdjacencyList {
public:
	//Constructor
	AdjacencyList();
	//Input File Reads
	bool readFromFile(string inFile,string outFile);
	void displayFile(string inFile, string outFile);
	//Display Methods
	void display(string outFile);
	void displayPath(int parent[], int j);
	//Calculate MST and Sum
	void prim(string outFile);
	//Calculate Shortest Path
	void dijkstra(int start, int end, string outFile);
	//Add Edges
	void addEdge( int u, int v, int w);

	//Determine if Edge exists
	bool findEdge(int u, int v);
	//Get methods
	int getSource() { return source; }
	int getDestination() { return destination; }
	int getEdgeWeight(int x, int y, int w);
	//User Weight Changes
	void increaseEdge(int u, int v, string outFile);
	void decreaseEdge(int u, int v, string outFile);
	//Determine if Connected Methods
	void traverseGraph(AdjacencyList const &al, int x, vector<bool>& inTree);
	bool isConnected();
	//Stream Operators
	friend ostream& operator<<(ostream& out, AdjacencyList& adj);
	friend ostream& operator<<(ostream& out, const vector<int>& v);
	friend ostream& operator<<(ostream& out, const pair<int, int>& v);
	friend ostream& operator<<(ostream& out, AdjacencyList& e);

private:
	int edges;
	int vertices;
	int source = 0;
	int destination = 0;
	bool inTree = false;
	list < pair<int, int> >* adj;
	void reDo(int vertices, int edges);


};

//Description: Constructor For Class
//Pre-condition: None
//Post-condition: Initializes private data memebers
AdjacencyList::AdjacencyList() {
	this->adj = new list < pair<int, int> >[vertices];
	this->edges = 0;
	this->source = 0;
	this->vertices = 0;
	this->inTree = false;

}


//Description: Adds an edge to the graph
//Pre-condition: Is called by another function, edge is not part of graph
//Post-condition: Edge is added to the graph
void AdjacencyList::addEdge( int u, int v, int w) {
	this->adj[u].push_back(make_pair(v, w));
	this->adj[v].push_back(make_pair(u, w));
}



//Description: Increases an Edge's Weight
//Pre-condition: Edge already exists
//Post-condition: Weight of edge is increased
void AdjacencyList::increaseEdge(int u, int v, string outFile) {
	//Variable Declarations
	ofstream oFile;
	oFile.open(outFile, ios::app);
	int add;
	//Get User Increase
	cout << "Please enter amount increase by: ";
	cin >> add;
	//if Invalid Input, Loop Until Valid Entered
	if ((!add) || (add <= 0)) {
		cout << "ERROR: a positive number must be entered: ";
		cin >> add;
	}

	//Find Vertices
	for (auto i = adj[u].begin(); i != adj[u].end(); i++) {
		if (i->first == v) {
			//Increase Edge Weight
			i->second = i->second + add;
			//Display Increment Message
			cout << "increment edge " << u << " " << v << " weight by " << i->second << endl;
			oFile << "increment edge " << u << " " << v << " weight by " << i->second << endl;
		}
	}
	//Close File
	oFile.close();
}

//Description: Decrease an edge's weight
//Pre-condition: Edge already exists
//Post-condition: Edge's weight amount is decreased
void AdjacencyList::decreaseEdge(int u, int v, string outFile) {
	//Variable Declarations
	ofstream oFile;
	oFile.open(outFile, ios::app);
	int sub;
	//Get User Decrease
	cout << "Please enter decrement amount: ";
	cin >> sub;
	//if Invalid Input, Loop Until Valid Entered
	if ((!sub) || (sub <= 0)) {
		cout << "ERROR: a positive number must be entered: ";
		cin >> sub;
	}
	//Find Vertices
	for (auto i = adj[u].begin(); i != adj[u].end(); i++) {
		if (i->first == v) {
			//If Invalid Input, Loop Until Valid Entered
			if ((i->second = i->second - sub) >= 0) {
				i->second = i->second - sub;
			}
			else {
				cout << "ERROR: results in negative amount. ";
				break;
			}

			//Display Decrement Message
			cout << "decrement edge " << u << " " << v << " weight by " << i->second << endl;
			oFile << "decrement edge " << u << " " << v << " weight by " << i->second << endl;
		}
	}
	//Close File
	oFile.close();
}

//Description: Find an edge
//Pre-condition: Data of graph has already been read in
//Post-condition: Returns true if edge exists, otherwise false
bool AdjacencyList::findEdge(int u, int v) {
	if (u > vertices || v > vertices)
		return false;
	//Find Vertices
	for (auto i = adj[u].begin(); i != adj[u].end(); i++) {
		if (i->first == v)
			//Found Return True
			return true;
	}
	//Not Found Return False
	return false;
}

//Description: Resets graph to size determined in file
//Pre-condition: File exists with readable input
//Post-condition: Resizes graph to size determined in input file
void AdjacencyList::reDo(int vertices, int edges) {
	//Variable Declarations
	this->adj = new list < pair<int, int> >[vertices];
	this->edges = edges;
	this->vertices = vertices;
}

//Description: Display's file information to screen and output file
//Pre-condition: There is a file to read in 
//Post-condition: Input file's information printed to output file and screen
void AdjacencyList::display(string outFile) {
	//Variable Declarations
	ofstream oFile;
	oFile.open(outFile, ios::app);
	//For Loop to Display Each Vertex
	for (int u = 0; u < this->vertices; u++) {
		cout << "adj[" << u << "] ->";
		oFile << "adj[" << u << "] ->";
		//For Loop to Display Adjacent Information
		for (auto i = this->adj[u].begin(); i != this->adj[u].end(); i++) {
			auto v = i->first;
			auto w = i->second;
			cout << " (" << v << "," << w << ")";
			oFile << " (" << v << "," << w << ")";
		}
		cout << endl;
		oFile << endl;
	}
	//Close File
	oFile.close();
}

//Description: Minimum spanning tree and sum determined via Prim's algorithm and Priority Queue
//Pre-condition: Graph is fully connected
//Post-condition: Minimum spanning tree and sum printed to screen and output file
typedef pair<int, int> iip;
void AdjacencyList::prim(string outFile) {
	//Variable Declarations
	ofstream oFile;
	oFile.open(outFile,ios::app);
	priority_queue<iip, vector<iip >, greater<iip > >pq;
	int* mst = new int[this->vertices];
	int* parents = new int[this->vertices];
	bool* visitedVerts = new bool[this->vertices];
	//For Loop to Initialize Vectors
	for (int i = 0; i < this->vertices; i++) {
		mst[i] = INFINITE;
		visitedVerts[i] = false;
		parents[i] = 0;
	}
	//Use Source Vertex to Push First Pair into Queue
	pq.push(make_pair(0, 0));
	mst[0] = 0;
	//While Queue is Not Empty Get Adjacent Vertices
	while (!pq.empty()) {
		//Remove Next Vertex, Mark as Visited
		int u = pq.top().second;
		pq.pop();
		visitedVerts[u] = true;

		//For Loop to Get Destination and Weight
		for (auto i = this->adj[u].begin(); i != this->adj[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			//If Loop to Get Adjacent Vertices That Have Not Been Visited Yet
			if (visitedVerts[v] == false && mst[v] > weight) {
				mst[v] = weight;
				parents[v] = u;
				//Push Into Queue
				pq.push(make_pair(mst[v], v));
			}
		}
	}
	//Display Minimum Spanning Tree Information
	cout << "Minimum Spanning Tree" << endl;
	oFile << "Minimum Spanning Tree" << endl;
	int sum = 0;
	for (int i = 1; i < this->vertices; ++i) {
		cout << "adj[" << parents[i] << "]-> (" << i << "," << mst[i] << ") " << endl;
		oFile << "adj[" << parents[i] << "]-> (" << i << "," << mst[i] << ") " << endl;
		sum += mst[i];
	}
	//Display Sum of Tree
	cout << "Sum: " << sum << endl;
	oFile << "Sum: " << sum << endl;
	//Close File
	oFile.close();
}


//Description: Shortest Path Determined by Dijkstra's Algorithm and user's starting point
//Pre-condition: Fully connected graph, start and end vertices exist
//Post-condition: Shortest path and sums printed to screen and output file
void AdjacencyList::dijkstra(int start, int end, string outFile) {
	//Variable Declarations
	ofstream oFile;
	oFile.open(outFile,ios::app);
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* distance = new int[this->vertices];
	int* parents = new int[this->vertices];
	bool* visitedVerts = new bool[this->vertices];
	//For Loop to Initialize Vectors
	for (int i = 0; i < this->vertices; i++) {
		distance[i] = (end,INFINITE);
		parents[i] = -1;
		visitedVerts[i] = false;
	}
	//Use Source Vertex to Push First Pair Determined By User into Queue
	pq.push(make_pair(0, start));
	distance[start] = 0;
	//While Queue is Not Empty Get Adjacent Vertices
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		visitedVerts[u] = true;
		//For Loop to Get Destination and Weight
		for (auto i = this->adj[u].begin(); i != this->adj[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			//If Not Visited Get Data
			if (!visitedVerts[v] && distance[v] && distance[v] > distance[u] + weight) {
				distance[v] = distance[u] + weight;
				parents[v] = u;
				//Push Next Vertex Into Queue
				pq.push(make_pair(distance[v], v));
			}
		}
	}
	//For Statement to Display Shortest Path Information
	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		oFile << "Path " << start;
		//Display if Exists
		if (distance[i] != INFINITE) {
			displayPath(parents, i);
			cout << ", weighs: " << distance[i] << endl;
			oFile << ", weighs: " << distance[i] << endl;
		}
		//Else Display Does Not Exist
		else {
			cout << " !-> " << i << endl;
			oFile << " !-> " << i << endl;
		}
	}
	//Close File
	oFile.close();
}

//Description: Display Shortest Path 
//Pre-condition: Called by dijkstra's function
//Post-condition: Shortest path printed to screen and output file
void AdjacencyList::displayPath(int* parents, int v) {
	//Variable Declarations
	ofstream oFile;
	string outFile;
	oFile.open(outFile, ios::app);
	//If No More Parents, Return 
	if (parents[v] == -1)
		return;
	//Recursive Call to Self
	displayPath(parents, parents[v]);
	//Display Information
	cout << " -> " << v;
	oFile << " -> " << v;
	//Close File
	oFile.close();

}

//Description: Read data from input file
//Pre-condition: File exists and has data
//Post-condition: Adjacency List and connected graph created and displayed to screen and output file
bool AdjacencyList::readFromFile(string fileName, string oFile) {
	//Variable Declarations
	ifstream file;
	file.open(fileName);
	ofstream outFile;
	outFile.open(oFile);
	//If File Exists, Execute
	if (file) {
		//Call to Display File Contents
		displayFile(fileName,oFile);
		//Initialize Number of Edges and Vertices
		int edges = 0;
		int vertices = 0;
		//Get Number of Vertices and Edges From First Two Numbers in File
		file >> vertices >> edges;
		//Push Information to Output File
		outFile << vertices << " " << edges << endl;
		cout << endl;
		//Size Graph to Vertices and Edges From Input File
		reDo(vertices, edges);
		//Intialize Variables 
		int source = 0, destination = 0, weight = 0;
		//Get Next Source, Destination, and Weight 
		file >> source >> destination >> weight;
		//Push Information to Output File
		outFile << source << " " << destination << " " <<  weight  << endl;
		//Push Information to Source Data Member
		this->source = source;
		//Add Edge to Graph
		addEdge(source, destination, weight);
		int i = 1;
		//While Loop to Get File Contents and Create Graph
		while (i < edges && file) {
			//Intialize Variables 
			int u = 0, v = 0, w = 0;
			//Get Next Source, Destination, and Weight 
			file >> u >> v >> w;
			//Push Information to Output File
			outFile <<  u << " " << v << " " << w << endl;
			//Get Source, Destination, and Weight As Long as Exists and is Valid Input
			if ((u < vertices) && (v<vertices) && (u > 0) && (v > 0) && (w > 0)) {
				addEdge(u, v, w);
				//Increment to Next Line
				i++;
			}
			//Else Do not Get Data and Just Increment to Next Line
			else {
				i++;
			}
			//If Less than 10 Nodes, Loop to Display Recursive Adds to Minimum Spanning Tree
			if (u < 10) {
				cout << "Added " << u << " " << v << " with weight " << w <<  endl;
			}
		}
	}
	//Else Error With File
	else if(!file){
		cerr << "Error File Does Not Exist" << endl;
		outFile << "Error File Does Not Exist" << endl;
		return false;
	}
	else {
		cerr << "Error File is empty" << endl;
		outFile << "Error File is empty" << endl;
	}

	//Initial Graph Complete Message
	cout << "Initial Graph Complete " << endl;
	outFile << "Initial Graph Complete " << endl;

	cout << endl;
	outFile << endl;
	//Close File
	file.close();
	outFile.close();
	return true;
}

//Description: Displays input file's data 
//Pre-condition: Input file exists
//Post-condition: Contents printed to screen and output file
void AdjacencyList::displayFile(string inFile, string outFile) {
	//Variable Declarations
	ifstream file;
	file.open(inFile);
	ofstream oFile;
	oFile.open(outFile, ios::app);
	//If File Exists, Execute
	if (file) {
		//Initialize Number of Edges and Vertices
		int vertices = 0;
		int edges = 0;
		//Get Number of Vertices and Edges From First Two Numbers in File
		file >> vertices >> edges;
		//Push Information to Display to Screen and Output File
		cout << vertices << " " << edges << endl;
		oFile << vertices << " " << edges << endl;
		//Intialize Variables
		int vertice = 0, destination = 0, weight = 0;
		//Get Next Source, Destination, and Weight 
		file >> vertice >> destination >> weight;
		//Push Information to Screen and Output File
		cout << vertice << " " << destination << " " << weight << endl;
		oFile << vertice << " " << destination << " " << weight << endl;
		int i = 1;
		//While Loop to Get File Contents and Display Graph
		while (i < edges && file) {
			//Intialize Variables
			int u = 0, v = 0, w = 0;
			//Get Next Source, Destination, and Weight 
			file >> u >> v >> w;
			//Push Information to Screen and Output File
			cout << u << " " << v << " " << w << endl;
			oFile << u << " " << v << " " << w << endl;
			i++;
		}
		cout << endl;
		oFile << endl;
	}
	//Else Display Error Message to Screen and File
	else {
		cerr << "Error while reading file" << endl;
		oFile << "Error while reading file" << endl;
	}
	//Close File
	file.close();
	oFile.close();
}

//Description: Traverse's Graph Checking connections
//Pre-condition: Graph exists, called by isConnected
//Post-condition: Traverses graph checking if each vertex is connected in graph
void AdjacencyList::traverseGraph(AdjacencyList const &al,int x, vector<bool> &inTree) {
	//Variable Declarations
	inTree[x] = true;
	for (auto i = al.adj[x].begin(); i != al.adj[x].end(); i++) {
		if (!inTree[x])
			traverseGraph(al, x, inTree);
}
	
}

//Description: Determines if graph is fully connected
//Pre-condition: Graph exists
//Post-condition: Graph is fully connected, missing connections inputted via user
bool AdjacencyList::isConnected() {
	int u = 0;
	int v = vertices;
	//Find Vertices
	for (auto i = adj[u].begin(); i != adj[u].end(); i++) {
		if (i->first == v)
			//Found Return True
			return true;
	}
	//Not Found Return False
	return false;


}

//Description: Stream operator for output
//Pre-condition: Graph exists
//Post-condition: Allows output access
ostream& operator<<(ostream& out, AdjacencyList& a) {
	for (int i = 0; i < a.edges; i++)
		out << i <<  a.adj << endl;
	return out;
}

//Description: Stream operator for output
//Pre-condition: Graph exists
//Post-condition:Allows output access
ostream& operator<<(ostream& out, const vector<int>& v) {
	int len = static_cast<int>(v.size());
	for (int i = 0; i < len; i++) {
		if (i < len - 1) {
			out << v[i] << ", ";
		}
		else {
			out << v[i];
		}
	}
	return out;
}

//Description: Stream operator for output
//Pre-condition: Graph exists
//Post-condition:Allows output access
ostream& operator<<(ostream& out, const pair<int, int>& v) {
	out << "(";
	out << v.first << ", "
		<< v.second << ")";
	return out;
}

