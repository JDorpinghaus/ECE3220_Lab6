//============================================================================
// Name        : lab6.cpp
// Author      : Jacob Dorpinghaus
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

class Signal {
public:
	Signal();
	Signal(int fileNumber);
	Signal(char* filename);
	void offset(double offsetValue);
	void scale(double scaleValue);
	void center(void);
	void normalize(void);
	void statistics(void);
	void Sig_info(void);
	void Save_file(char* filename);
private:
	void updateMaxValue(void);
	void updateMeanValue(void);
	void fileToArray(char* filename);
	char* buildFilename(int fileNumber);
	double* dataArray;
	int length;
	int maxNum;
	int currentMaxNum;
	double average;
};

int main() {
	Signal signal(3);
	signal.Sig_info();
	return 0;
}



Signal::Signal(){
	fileToArray(buildFilename(1)); //default to file #1
	statistics();
}

Signal::Signal(int fileNumber){
	fileToArray(buildFilename(fileNumber));
	statistics();
}

Signal::Signal(char* filename){
	fileToArray(filename);
	statistics();
}

char* Signal::buildFilename(int fileNumber){
	char* filename = (char*)calloc(16, sizeof(char));
	sprintf(filename, "Raw_data_%02d.txt", fileNumber);
	return filename;
}

void Signal::fileToArray(char* filename){
	FILE *fp;
	int x=0;
	fp=fopen(filename, "r");
	fscanf(fp, "%d %d", &length, &maxNum); //read in number of values and maximum value
	dataArray = (double*)calloc(length, sizeof(double)); //allocate memory for number of integers
	for(x=0;x<length;x++){
		fscanf(fp, "%lf", &dataArray[x]); //scan values into integer array
	}
	fclose(fp);
}

void Signal::updateMaxValue(void){
	int i;
	double max;
	max = dataArray[0];
	for(i=1;i<length;i++){
		if (dataArray[i] > max){
			max = dataArray[i];
		}
	}
	currentMaxNum = max;
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

void Signal::offset(double offsetValue){
	int i;
	for(i=0;i<length;i++){
		dataArray[i] += offsetValue;
	}
	maxNum += offsetValue;
	statistics();
}

void Signal::scale(double scaleValue){
	int i;
	for(i=0;i<length;i++){
		dataArray[i] = dataArray[i] * scaleValue;
	}
	maxNum *= scaleValue;
	statistics();
}

void Signal::center(void){
	offset(-1*average);
	statistics();
}

void Signal::normalize(void){
	scale((double)1/maxNum);
	statistics();
}

void Signal::statistics(void){
	updateMeanValue();
	updateMaxValue();
}

void Signal::Sig_info(void){
	cout << endl << "Length: " << length;
	cout << endl << "Average: " << average;
	cout << endl << "Maximum: " << maxNum << endl;
}

