//============================================================================
// Name        : lab6.cpp
// Author      : Jacob Dorpinghaus
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

class Signal {
public:
	Signal();
	Signal(int fileNumber);
	Signal(char* filename);
	~Signal();
	void offset(double offsetValue);
	void scale(double scaleValue);
	void center(void);
	void normalize(void);
	void Sig_info(void);
	void Save_file(char* filename);
private:
	void updateMeanValue(void);
	void updateMaxValue(void);
	void fileToArray(char* filename);
	double* dataArray;
	int length;
	double maxNum;
	double average;
};

char* buildFilename(int fileNumber);
void printUsage(void);

int main(int argc, char* argv[]) {
	int i;
	Signal* signalObject;
	int fileNumber;
	int gotFile = 0;
	double offsetValue, scaleValue;
	char arg;
	for(i=0; i<argc; i++){
		if((argv[i][0] == '-')&&(argv[i][1] == 'n')){
			gotFile = 1;
			if(i+1 == argc){ //if no file number was provided after n, terminate the program
				printUsage();
				exit(0);
			}
			if(atoi(argv[i+1]) <= 11 && atoi(argv[i+1]) >= 1){ //if file number is between 1 and 11
				fileNumber = atoi(argv[i+1]);
				signalObject = new Signal(fileNumber);
				break;
			} else {
				printUsage();
				exit(0);
			}
			break;
		} else if((argv[i][0] == '-')&&(argv[i][1] == 'f')){
			gotFile = 1;
			if(i+1 == argc){ //if no file name was provided after f, terminate the program
				printUsage();
				exit(0);
			}
			signalObject = new Signal(argv[i+1]);
			break;
		}
	}
	for(i=0; i<argc; i++){
		if(argv[i][0] == '-'){
			arg = argv[i][1];
			switch(arg){
				case 'n':
				case 'f':
					break;
				case 'o':{
					if(i+1 == argc || !gotFile){ //if no additional arguments or no file, terminate the program
						printUsage();
						exit(0);
					}
					if (sscanf(argv[i+1], "%lf", &offsetValue) == 0){ //check that a valid float value was input
						printUsage();
						exit(0);
					}
					signalObject->offset(offsetValue);
					i++;
					break;
				}
				case 's':{
					if(i+1 == argc || !gotFile){ //if no additional arguments, terminate the program
						printUsage();
						exit(0);
					}
					if (sscanf(argv[i+1], "%lf", &scaleValue) == 0){ //check that a valid float value was input
						printUsage();
						exit(0);
					}
					signalObject->scale(scaleValue);
					i++;
					break;
				}

				case 'h':
					printUsage();
					exit(0);
					break;
				case 'S':{
					if(!gotFile){
						printUsage();
						exit(0);
					}
					signalObject->Sig_info();
				}
					break;
				case 'C':{
					if(!gotFile){
						printUsage();
						exit(0);
					}
					signalObject->center();
				}
					break;
				case 'N':{
					if(!gotFile){
						printUsage();
						exit(0);
					}
					signalObject->normalize();
				}
					break;
				case 'W':{
					if(i+1 == argc || !gotFile){ //if no additional arguments, terminate the program
						printUsage();
						exit(0);
					}
					signalObject->Save_file(argv[i+1]);
				}
					break;
				default:
					printUsage();
					exit(0);
					break;
			}
		}
	} // enter menu mode
	if(!gotFile){
		signalObject = new Signal();
	}
	char input[100]; //char array for ingesting user input
	int inputNum;
	while(inputNum != 7){
		do {
			cout << endl;
			cout << "Choose an option: " << endl;
			cout << "1: Offset data" << endl;
			cout << "2: Scale data" << endl;
			cout << "3: Center data" << endl;
			cout << "4: Normalize data" << endl;
			cout << "5: Print statistics" << endl;
			cout << "6: Save file" << endl;
			cout << "7: Exit program" << endl;
			cin >> input;
			inputNum = atoi(input);
		} while (inputNum > 7 || inputNum < 1);
		switch(inputNum){
		case 1:{
			double offsetNum;
			cout << endl << "Enter offset value: ";
			cin >> offsetNum;
			signalObject->offset(offsetNum);
			break;
		}
		case 2:{
			double scaleNum;
			cout << endl << "Enter scale value: ";
			cin >> scaleNum;
			signalObject->scale(scaleNum);
			break;
		}
		case 3:
			signalObject->center();
			break;
		case 4:
			signalObject->normalize();
			break;
		case 5:
			signalObject->Sig_info();
			break;
		case 6:
			char newFilename[100];
			cout << endl << "Enter filename: ";
			cin >> newFilename;
			signalObject->Save_file(newFilename);
			break;
		case 7:
			exit(0);
			break;
		default:
			cout << endl << "Invalid choice, ending program";
			exit(0);
			break;
		}
	}
	return EXIT_SUCCESS;
}



Signal::Signal(){
	fileToArray(buildFilename(1)); //default to file #1
	updateMeanValue();
	updateMaxValue();
}

Signal::Signal(int fileNumber){
	fileToArray(buildFilename(fileNumber));
	updateMeanValue();
	updateMaxValue();
}

Signal::Signal(char* filename){
	fileToArray(filename);
	updateMeanValue();
	updateMaxValue();
}

Signal::~Signal(){
	free(dataArray);
}

char* buildFilename(int fileNumber){
	char* filename = (char*)calloc(16, sizeof(char));
	sprintf(filename, "Raw_data_%02d.txt", fileNumber);
	return filename;
}

void Signal::fileToArray(char* filename){
	FILE *fp;
	int x=0;
	fp=fopen(filename, "r");
	fscanf(fp, "%d %lf", &length, &maxNum); //read in number of values and maximum value
	dataArray = (double*)calloc(length, sizeof(double)); //allocate memory for number of integers
	for(x=0;x<length;x++){
		fscanf(fp, "%lf", &dataArray[x]); //scan values into integer array
	}
	fclose(fp);
}

void Signal::updateMeanValue(){
	int i;
	double mean;
	mean = dataArray[0];
	for(i=1;i<length;i++){
		mean += dataArray[i];
	}
	average = (mean/(double)length);
}

void Signal::updateMaxValue(){
	int i;
	double max;
	max = dataArray[0];
	for(i=1;i<length;i++){
		if(dataArray[i] > max){
			max = dataArray[i];
		}
	}
	maxNum = max;
}

void Signal::offset(double offsetValue){
	int i;
	for(i=0;i<length;i++){
		dataArray[i] += offsetValue;
	}
	maxNum += offsetValue;
	average += offsetValue;
}

void Signal::scale(double scaleValue){
	int i;
	for(i=0;i<length;i++){
		dataArray[i] = dataArray[i] * scaleValue;
	}
	maxNum *= scaleValue;
	average *= scaleValue;
}

void Signal::center(void){
	offset((-1)*average);
}

void Signal::normalize(void){
	scale((double)1/maxNum);
}

void Signal::Sig_info(void){
	cout << endl << "Length: " << length;
	cout << endl << "Average: " << average;
	cout << endl << "Maximum: " << maxNum << endl;
}

void Signal::Save_file(char* newFilename){
	FILE* fp;
	int i;
	fp = fopen(newFilename, "w+");
	fprintf(fp, "%d %lf\n", length, maxNum);
	for(i=0;i<length;i++){
		fprintf(fp, "%lf\n", dataArray[i]);
	}
	fclose(fp);
}

void printUsage(){
	printf("\nUsage:\n");
	printf("-n <file number>\tNumber of file to open, between 1 and 11\n");
	printf("-o <offset value>\tOffset the data by the given value\n");
	printf("-s <scale factor>\tScale the data by the given scaling factor\n");
	printf("-h\t\t\tDisplay usage instructions\n");
	printf("-S\t\t\tDisplay data statistics\n");
	printf("-C\t\t\tCenter the data\n");
	printf("-N\t\t\tNormalize the data\n");
	printf("-W <new filename>\tSave data to file\n");
}

