#include "Header.h"
#include <algorithm>
#include <unordered_set>
#include <map>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;


//Description: Display Message For User Options
//Pre-condition: Is called by main program
//Post-condition: Character choice returned to calling program
char DisplayMessage() {
	char option{};
	cout << endl;

	cout << " What would you like to do? " << endl;
	cout << "E) Add an Edge " << endl;
	cout << "I) Increase an Edge's Weight" << endl;
	cout << "D) Decrease an Edge's Weight" << endl;
	cout << "S) Get the Shortest Path " << endl;
	cout << "Q) Quit " << endl;
	cin >> option;
	while (!option) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid Input, Please Choose Input From List: " << endl;
		DisplayMessage();
		cin >> option;
	}
	cout << endl;
	return option;
}





//Description: Main Program
//Pre-condition: None
//Post-condition: File input, output, graph, adjacency list, shortest path, user input, completed
//and pushed out to screen and output file
int main() {
	cout << "Welcome to CIS-Land! We will be installing fiber-optic lines!" << endl;
	//Variable Declarations
	string fileName;
	string outName;
	ofstream outFile;
	AdjacencyList* adj = new AdjacencyList();
	int start,end;
	ifstream inFile;
	int quit = 0;
	int a, b,w;


	//Get Input File Name
	cout << "Please Enter the File That Has the Information: ";
	cin >> fileName;
	//Get Output File Name
	cout << "Please Enter the File That You Would Like to Write Information to: ";
	cin >> outName;
	//Explicit File Opens
	inFile.open(fileName);
	outFile.open(outName);

	//If Statement Occurs For Bad Input File
	if (!adj->readFromFile(fileName,outName)) {
		return -1;
	}

	cout << endl;
	//Display File Contents to Output File and Screen
	adj->display(outName);
	cout << endl;

	if (!adj->isConnected()) {
		//Not Connected Message
		cout << "Graph Not Connected, Add Edge: " << endl;
		//Get Vertex From User
		cout << "First Vertex: " << endl;
		cin >> a;
		cout << "Next Vertex: " << endl;
		cin >> b;
		//Get Weight From User
		cout << "Please Enter Weight: " << endl;
		cin >> w;
		//If Invalid User Input Loop Until Valid Recieved
		if (!(cin >> w)) {
			cout << "ERROR: a number must be entered: ";
			cin >> w;
		}
		//Add Edge
		adj->addEdge(a, b, w);

	}
	
	//Display File Contents to Output File and Screen
	adj->display(outName);
	cout << endl;

	//Display Minimum Spanning Tree and Sum to Output File and Screen
	adj->prim(outName);
	cout << endl;

	//While Statement to Loop While User Does Not Choose Quit
	while (quit != 1) {
		//Display Message For User Choice
		char option{ DisplayMessage() };
		//Switch Statement to Execute Depending on User Choice
		switch (option) {
			//Case For Add Edge
		case 'E':
		case 'e': {
			//Get Edge Information
			cout << "Please enter the first vertex: ";
			cin >> a;
			cout << "Please enter the second vertex: ";
			cin >> b;
			cout << "Please Enter Weight: " << endl;
			cin >> w;
			if ((! w)|| (w<=0)) {
				cout << "ERROR: a positive number must be entered: ";
				cin >> w;
			}
			if (!a || !b) {
				cout << "Invalid Entry";
				break;
		}
			//if Loop to Check if Edge Already Exists
			if (adj->findEdge(a, b)) {
				cout << "Error, Duplicate Edge" << endl;
				break;
			}
			//Add User Input Edge to Graph
			adj->addEdge(a, b, w);
			//Edge Added Message
			cout << "Add Edge " << a << " " << b << " With Weight " << w << endl;
			outFile << "Add Edge " << a << " " << b << " With Weight " << w << endl;
			//Display New Adjacency List 
			adj->display(outName);
			cout << endl;
			//Display New Minimum Spanning Tree and Sum
			adj->prim(outName);
			//Break Statement to Return to User Choice
			break;
		}
		//Case For Increment Edge
		case 'I':
		case 'i': {
			//Get Edge Information
			cout << "Please Enter the First Vertex: " << endl;
			cin >> a;
			cout << "Please enter the second vertex: " << endl;
			cin >> b;
			if ((a<0)||(b<0)) {
				cout << "Invalid Vertex";
				break;
			}
			//While Loop to Check if Edge Already Exists, If it Does Not Get New Information
			while (!adj->findEdge(a, b)) {
				cout << "Please reenter vertices: " << endl;
				cout << "Please Enter the First Vertex: " << endl;
				cin >> a;
				cout << "Please enter the second vertex: " << endl;
				cin >> b;
			}

			//If Edge Does Exist, Increment Weight
			adj->increaseEdge(a, b, outName);
	
			//Display New Adjacency List 
			adj->display(outName);
			cout << endl;
			//Display New Minimum Spanning Tree and Sum
			adj->prim(outName);
			//Break Statement to Return to User Choice
			break;
		}
		//Case For Decrement Edge
		case 'D':
		case'd': {
			//Get Edge Information
			cout << "Please enter the first vertex: ";
			cin >> a;
			cout << "Please enter the second vertex: ";
			cin >> b;
			if (!a || !b) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Error, No such edge";
				break;
			}
			if (!adj->findEdge(a, b)) {
				cout << "Error, No such edge";
				break;

			}
			//If Edge Does Exist, Decrement Weight
			adj->decreaseEdge(a, b, outName);

			//Display New Adjacency List 
			adj->display(outName);
			cout << endl;
			//Display New Minimum Spanning Tree and Sum
			adj->prim(outName);
			//Break Statement to Return to User Choice
			break;
		}
		//Case For Shortest Path
		case 'S':
		case 's': {
			//Get Source Vertice Information
			cout << "Enter source vertex: ";
			cin >> start;

			cout << "Enter destination vertex: ";
			cin >> end;

			//Display Shortest Path Message
			cout << "Determine Shortest Path From " << start << " to " << end <<  endl;
			outFile << "Determine Shortest Path From " << start << " to " << end << endl;
			//Call Shortest Path Method
			adj->dijkstra(start, end, outName);
			//Break Statement to Return to User Choice
			break;

		}
		//Quit Case
		case 'Q':
		case 'q': {
			quit = 1;
			//Break Statement Makes While Loop False and Exits Program
			break;
		}
		//Invalid User Selection
		default: {
			cout << "Invalid Operation Selected " << endl;
		}
		}
	}
	//Exit Program
	return 0;
}
