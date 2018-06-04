#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

class xy
{
public:
	double x, y;
};

int check_input_validity(int argc, char* argv[]) {
	if (argc != 3) {																			//checks if arg counter is correct
		cerr << "Error! Invalid syntax. Correct syntax: <program name> <option> <filename>";
		return 1;
	}
	if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-n") != 0) {									//checks if option is valid
		cerr << "Error! Invalid option. Valid options are -e or -n (not both)." << endl;
		//cout << argv[1] << endl;
		return 1;
	}
	return 0;
}

int read_file(char* filename, int* nodeNum, vector<xy>& coordinates) {
	string input = "";
	ifstream file(filename);
	string x_s;
	string y_s;
	double x_d;
	double y_d;
	int nodeNameCounter = 0;
	xy axis;

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

			axis.x = x_d;
			axis.y = y_d;
			coordinates.push_back(axis);
			
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

void generateMatrix(vector<vector<double>>& matrix, vector<xy>& coordinates, int nodeNum)
{
	double distance[2];
	for (int i = 0; i < nodeNum; i++)
	{
		vector<double> row;
		for (int j = 0; j < nodeNum; j++)
		{
			distance[0] = abs(coordinates[i].x - coordinates[j].x);
			distance[1] = abs(coordinates[i].y - coordinates[j].y);
			if (i == j) {
				row.push_back(numeric_limits<double>::max());
			}
			else {
				row.push_back(sqrt(pow(distance[0], 2) + pow(distance[1], 2)));
			}
			//matrix[i][j] = sqrt(pow(distance[0], 2) + pow(distance[1], 2)); WHAT REALLY HAPPENS
		}
		matrix.push_back(row);
	}
}

int fact(int num) {
	if (num > 0) {
		int count = num - 1;
		while (count > 0) {
			num *= count;
			count--;
		}
	}
	return num;
}

vector<int> get_combi(int nodeNum, int counter) {
	vector<int> combi;
	vector<int> order;
	int temp_order;
	int pos;
	vector<bool> used;
	int store_fact_of_current_pos;

	for (int i = 0; i < nodeNum; i++) {
		used.push_back(0);
		temp_order = 0;
		store_fact_of_current_pos = fact(nodeNum - i -1);
		while (counter >= store_fact_of_current_pos && store_fact_of_current_pos>0) {
			counter -= store_fact_of_current_pos;
			temp_order++;
		}
		order.push_back(temp_order);
	}

	for (int i = 0; i < nodeNum; i++) {
		pos = -1;
		for (int j = 0; j <= order[i]; j++) {
			pos++;
			while (used[pos] != 0) {
				pos++;
			}
		}
		used[pos] = 1;
		combi.push_back(pos);
	}

	return combi;
}

double get_route_len(int nodeNum, vector<int> combi, vector<vector<double>>& matrix) {
	double length = 0;
	for (int i = 0; i < nodeNum; i++) {
		if (nodeNum - 1 != i) {
			length += matrix[combi[i]][combi[i + 1]];
		}
		else {
			length += matrix[combi[i]][combi[0]];
		}
	}
	return length;
}
double route_enumeration(vector<vector<double>>& matrix, vector<xy>& coordinates, int nodeNum, vector<int>& best_combi) {
	int i_holder;
	vector<int> combi;
	double route_length;
	double shortest= numeric_limits<double>::max();

	int store_fact = fact(nodeNum);
	for (int i = 0; i < store_fact; i++) {
		combi = get_combi(nodeNum, i);
		route_length = get_route_len(nodeNum, combi, matrix);
		/*cout << i << ": ";
		for (int j = 0; j < nodeNum; j++) {
			 cout << combi[j];
		}
		cout << "; " << route_length << endl;*/
		if(route_length < shortest) {
			shortest = route_length;
			best_combi = combi;
		}
	}
	return shortest;
}

double nearestNeighbor(vector<vector<double>>& matrix, vector<xy>& coordinates)
{
	int index = 0; // starting at point 0
	int count = 0;
	int nextIndex;
	// creating a dynamic vector as for flagging used points
	vector<bool> check;
	for (int i = 0; i <= matrix.size(); i++)
	{
		check.push_back(false);
	}
	double distanceTravelled = 0;

	cout << "Visited Nodes: ";
	for (count; count < matrix.size() - 1; count++) // Last point => return to first point
	{
		double min = DBL_MAX;
		int j; // iterator
		for (j = 0; j < matrix[index].size(); j++)
		{
			if (check[j] != true && matrix[index][j] < min)
			{
				min = matrix[index][j];
				nextIndex = j;
			}

		}
		char printNodeName = 'A' + index;
		cout << printNodeName;

		distanceTravelled += min;

		check[index] = true;

		index = nextIndex;
	}
	char printNodeName = 'A' + nextIndex;
	cout << printNodeName;
	//Way back to 0
	return distanceTravelled += matrix[index][0];
}

int main(int argc, char* argv[]) {
	vector<vector<double>> matrix;
	vector<xy> coordinates;
	vector<int> best_combi;
	char* option;
	char* filename;
	int nodeNum;


	if (check_input_validity(argc, argv)) {
		return 1;
	}

	option = argv[1];

	filename = argv[2];

	cout << "Option: " << option << endl;
	cout << "Filename: " << filename << endl << endl;
	
	if (read_file(filename, &nodeNum, coordinates)) {
		return 1;
	}

	cout << endl << "Number of nodes: " << nodeNum << endl;
	cout << "Printing nodes:" << endl;
	for (int i = 0; i < coordinates.size(); i++)
	{
		cout << "(" << coordinates[i].x << ", " << coordinates[i].y << ")" << endl;
	}
	
	generateMatrix(matrix, coordinates, nodeNum);

	cout << endl << "Adjacency matrix:" << endl;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] == numeric_limits<double>::max()) {
				cout << "max" << "  ";
			}
			else {
				cout << matrix[i][j] << "  ";
			}
		}
		cout << endl;
	}

	clock_t c_start = std::clock();

	if (strcmp(option, "-e") == 0) {
		cout << endl << "Shortest route is: " << route_enumeration(matrix, coordinates, nodeNum, best_combi) << endl;
		cout << "Visited Nodes: ";
		for (int i = 0; i < nodeNum; i++) {
			char print_node_name = 'A' + best_combi[i];
			cout << print_node_name;
		}
		cout << endl;
	}
	else {
		cout << endl;
		cout << endl << "Shortest route is: " <<  nearestNeighbor(matrix, coordinates) << endl;
	}

	clock_t c_end = std::clock();

	long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;
	cout << endl << endl << "CPU time used: " << time_elapsed_ms << " ms\n";

	return 0;
}