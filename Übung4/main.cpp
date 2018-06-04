#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <vector>

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
	if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-n")) {									//checks if option is valid
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
			row.push_back(sqrt(pow(distance[0], 2) + pow(distance[1], 2)));
			//matrix[i][j] = sqrt(pow(distance[0], 2) + pow(distance[1], 2)); WHAT REALLY HAPPENS
		}
		matrix.push_back(row);
	}
}

void nearestNeighbor(vector<vector<double>>& matrix, vector<xy>& coordinates)
{
	//cout << "Starting point: (" << coordinates[0].x << ", " << coordinates[0].y << ")" << endl;

	int index = 0;
	int count = 0;
	double distanceTravelled = 0;

	for (count; count < matrix.size(); count++)
	{
		for (int j = 0; j < matrix[index].size(); j++)
		{
			double min = DBL_MAX;
			if (matrix[index][j] < min)
			{
				min = matrix[index][j];
				cout << matrix[index][j] << endl;
				matrix.erase (matrix.begin() + index);
				//cout << "Point " << index+1 << ": (" << coordinates[index].x << ", " << coordinates[index].y << ")" << endl;
				index = j;

				//cout << "Point " << j << ": (" << coordinates[index].x << ", " << coordinates[index].y << ")" << endl;
				//cout << "matrix size: " << matrix.size() << endl;
				//cout << "matrix[j] size: " << matrix[j].size() << endl;


				cout << endl << endl << endl << "Adjacency matrix:" << endl;
				for (int i = 0; i < matrix.size(); i++)
				{
					for (int j = 0; j < matrix[i].size(); j++)
					{
						cout << matrix[i][j] << " | ";
					}
					cout << endl;
				}
			}

		}
	}
		
	
	//matrix[0][0];
}

int main(int argc, char* argv[]) {
	vector<vector<double>> matrix;
	vector<xy> coordinates;
	clock_t c_start = std::clock();
	char* option;
	char* filename;
	int nodeNum;

	

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
			cout << matrix[i][j] << " | ";
		}
		cout << endl;
	}


	cout << endl << "Nearest Neighbor Heuristic:" << endl;
	nearestNeighbor(matrix, coordinates);

	clock_t c_end = std::clock();

	long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;
	cout << endl << endl << "CPU time used: " << time_elapsed_ms << " ms\n";

	return 0;
}