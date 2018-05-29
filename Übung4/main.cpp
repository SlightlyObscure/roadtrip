#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

class nodeList {
	struct node{
		double node_x;
		double node_y;
		int name;
		node* next;
	};
	
public:
	nodeList() {
		head = NULL;
	}
	~nodeList() {
		node* next = head;
		while (next) {
			node* deleteNode = next;
			next = next->next;
			delete deleteNode;
		}
	}
	void addNode(double input_x, double input_y, int input_name) {
		node* newNode = new node();
		newNode->node_x = input_x;
		newNode->node_y = input_y;
		newNode->name = input_name;
		newNode->next = head;
		head = newNode;
	}
	void printNodes() {
		node* next = head;
		while (next) {
			node* printNode = next;
			next = next->next;
			cout << "node" << printNode->name << ": " << printNode->node_x << " " << printNode->node_y << endl;
		}
	}

private:
	node* head;
};


int check_input_validity(int argc, char* argv[]) {
	if (argc != 3) {																			//checks if arg counter is correct
		cerr << "Error! Invalid syntax. Correct syntax: <program name> <option> <filename>";
		return 1;
	}
	if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-n")) {									//checks if option is valid
		cerr << "Error! Invalid option. Valid options are -e or -n (not both)." << endl;
		//cout << argv[1] << endl;
		return 1;
	}
	return 0;
}

int read_file(char* filename, int* nodeNum, nodeList* places) {
	string input = "";
	ifstream file(filename);
	string x_s;
	string y_s;
	double x_d;
	double y_d;
	int nodeNameCounter = 0;

	if (!file.is_open()) {								//looks for file
		cerr << "Error! File not found." << endl;
		return 1;
	}
	else {
		if (getline(file, input)) {						//trys to read number of nodes (first line)
			try {
				*nodeNum = stoi(input);
			}
			catch (...) {								//exception for when stoi fails
				cerr << "Error! Number of nodes in file could not be read." << endl;
				return 1;
			}
		}
		else {											//exception for when getline fails
			cerr << "Error! File appears to be empty." << endl;
			return 1;
		}

		cout << "Number of nodes: " << *nodeNum << endl;
		cout << "Content of file:" << endl;

		while (getline(file, input)) {
			if (nodeNameCounter+1 > *nodeNum) {
				break;
			}
			x_s = input.substr(0, 3);
			if(!(x_d = atof(x_s.c_str()))) {
				cerr << "Error! Tried to read coordinate, but failed." << endl;
				return 1;
			}
			y_s = input.substr(4);
			if (!(y_d = atof(y_s.c_str()))) {
				cerr << "Error! Tried to read coordinate, but failed." << endl;
				return 1;
			}

			places->addNode(x_d, y_d, nodeNameCounter);
			nodeNameCounter++;
			cout << x_s << " " << y_s << endl;
		}
		if (nodeNameCounter < *nodeNum) {
			*nodeNum = nodeNameCounter;
		}
	}
	file.close();
	return 0;
}

int main(int argc, char* argv[]) {
	clock_t c_start = std::clock();
	char* option;
	char* filename;
	int nodeNum;
	nodeList places;

	/*for (int i = 0; i < argc; i++) {    //for checking args
		cout << argv[i] << endl;
	}*/

	if (check_input_validity(argc, argv)) {
		return 1;
	}

	option = argv[1];

	filename = argv[2];

	cout << "Option: " << option << endl;
	cout << "Filename: " << filename << endl << endl;
	
	if (read_file(filename, &nodeNum, &places)) {
		return 1;
	}

	cout << endl << "Number of nodes: " << nodeNum << endl;
	cout << "Printing nodes:" << endl;
	places.printNodes();


	clock_t c_end = std::clock();

	long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;
	cout << endl << endl << "CPU time used: " << time_elapsed_ms << " ms\n";

	return 0;
}