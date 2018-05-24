#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	char* option;
	char* filename;
	string input = "";
	/*for (int i = 0; i < argc; i++) {    //for checking args
		cout << argv[i] << endl;
	}*/
	if (argc != 3) {																			//checks if arg counter is correct
		cerr << "Error! Invalid syntax. Correct syntax: <program name> <option> <filename>";
		return 1;
	}
	if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-n")) {									//checks if option is valid
		cerr << "Error! Invalid option. Valid options are -e or -n (not both)." << endl;
		//cout << argv[1] << endl;
		return 1;
	}
	else {
		option = argv[1];
	}

	filename = argv[2];

	cout << "Option: " << option << endl;
	cout << "Filename: " << filename << endl;
	cout << endl;
	
	ifstream file(filename);
	if (!file.is_open()) {								//looks for file
		cerr << "Error! File not found." << endl;
		return 1;
	}
	else {												//just outputs file...for now			
		cout << "Content of file:" << endl;
		while (getline(file, input)) {
			cout << input << endl;
		}
	}
	file.close();

	return 0;
}