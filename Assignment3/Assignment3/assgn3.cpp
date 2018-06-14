#include <iostream>
#include <assert.h>

using namespace std;


int debug = false;
int verbose = false;

void number1() {
	int myarray[5];
	int check;
	for (int i = 0; i < 5; i++) {
		cout << "Give me an integer ";
		cin >> myarray[i];
	}
	cout << "Give me a number to check ";
	cin >> check;

	int found = 0;
	for (int i = 0; i < 5; i++) {
		if (check == myarray[i]) {
			cout << "FOUND " << check << " at offset " << i << endl;
			found++; break;
		}
	}
	if (!found)
		cout << "Did not find number" << endl;
}

void number2() {
	int numElements;
	cout << "How many numbers to input? ";
	cin >> numElements;

	int check;

	int *myarray = new int[numElements];
	for (int i = 0; i < numElements; i++) {
		cout << "Give me an integer ";
		cin >> myarray[i];
	}
	cout << "Give me a number to check ";
	cin >> check;

	int found = 0;
	for (int i = 0; i < numElements; i++) {
		if (check == myarray[i]) {
			cout << "FOUND " << check << " at offset " << i << endl;
			found++; break;
		}
	}
	if (!found)
		cout << "Did not find number" << endl;

	delete[] myarray;
}

class DataSeries {
private:
	double *valuesArray;
	int arraySize;
	int lastPosition = 0;

public:
	DataSeries(int arraySize) {
		this->arraySize = arraySize;
		valuesArray = new double[arraySize];
	}

	~DataSeries(void) {
		delete[] valuesArray;
	}

	void set(double value) {
		valuesArray[lastPosition++] = value;
	}


	double get(int index) {
		return valuesArray[index];
	}

	double absoluteVariation() {
		double a = 0;

		for (int i = 0; i < arraySize - 1; i++) {
			a += fabs(valuesArray[i + 1] - valuesArray[i]);
		}

		cout << "Absolute Variation is " << a << endl;
		return a;
	}

	int countChangesAboveThreshold(int threshold) {
		int *indexArray = new int[arraySize];
		int qCount = 0;
		int indexCount = 0;

		//look for threshold from the smallest gap to the biggest,
		// fabs(Xn+gap - Xn)
		for (int gap = 1; gap < arraySize; gap++) {
			bool addedEntries = false;

			//main

			for (int i= 0; i < arraySize - gap; i++) {
				
				//check if Xn and Xn+gap overlaps an existing point
				bool overlap = false;
				for (int f = 0; f < indexCount; f++) {
					if (i < indexArray[f] && i + gap >= indexArray[f]) {
						//cout << "overlap " << i << " " << indexArray[f] << " " << i + gap << endl;
						overlap = true;
						break;
					}
				}
				if (overlap) {
					continue;
				}

				//check if |Xn+gap - Xn| >= threshold
				//cout << "gap " << gap << " " << valuesArray[i + gap] << " " << valuesArray[i] << " " << fabs(valuesArray[i + gap] - valuesArray[i]) << endl;
				if (fabs(valuesArray[i + gap] - valuesArray[i]) >= threshold) {
					qCount++;

					if (debug)
						cout << "Added X" << i << " and X" << i + gap << " (gap=" << gap << ") |" << valuesArray[i] << " - " << valuesArray[i + gap] << "| = " << fabs(valuesArray[i + gap] - valuesArray[i]) << endl;

					// add Xn to the indexArray if it's not in there already
					bool exist = false;
					for (int g = 0; g < indexCount; g++) {
						if (indexArray[g] == i) {
							exist = true;
							break;
						}
					}
					if (!exist) {
						indexArray[indexCount++] = i;
						addedEntries = true;
						if (verbose)
							cout << "Added " << i << endl;
					}

					// add Xn+gap to the indexArray if it's not in there already
					exist = false;
					for (int g = 0; g < indexCount; g++) {
						if (indexArray[g] == i + gap) {
							exist = true;
							break;
						}
					}
					if (!exist) {
						indexArray[indexCount++] = i + gap;
						addedEntries = true;
						if (verbose)
							cout << "Added " << i + gap << endl;
					}
				}
				
			}

			//sort indexArray
			if (addedEntries) {
				/*cout << "Sort before" << endl;
				for (int z = 0; z < indexCount; z++) {
					cout << indexArray[z] << endl;
				}*/
				for (int zz = 0; zz < indexCount; zz++) {
					for (int z = 0; z < indexCount - zz - 1; z++) {
						if (indexArray[z] > indexArray[z + 1]) {
							int temp = indexArray[z];
							indexArray[z] = indexArray[z + 1];
							indexArray[z + 1] = temp;
						}
					}
				}
				/*cout << "Sort after" << endl;
				for (int z = 0; z < indexCount; z++) {
					cout << indexArray[z] << endl;
				}*/
			}

		}
		cout << "Q is " << qCount << endl;
		delete[] indexArray;
		return qCount;
	}
};

void runExercise(double array[], int length) {
	//define class DataSeries and load the values
	DataSeries myDataSeries(length);
	for (int i = 0; i < length; i++) {
		myDataSeries.set(array[i]);
	}

	myDataSeries.absoluteVariation();
	myDataSeries.countChangesAboveThreshold(1);
	cout << endl;
}

void number3() {
	debug = true;

	//double testArray[] = { 1,2,3,4,5,6,7,8,9,10,11 };
	//double testArray[] = {1,-2,3,-4,5,-6,7,-8,9,-10,11};
	//double testArray[] = { 0,0.7,0,-0.7,0,0.7,0 };
	double testArray[] = { 0,0.5,0,-0.7,0.1,0.6,0.8 };
	double testArray1[] = { 3,2.1,2.2,1.9,1.3,1,1.1,0.25 };

	double testArray2[1000];
	for (int i = 1; i <= 1000; i++) {
		testArray2[i - 1] = sin(i / 100.0) + cos(i / 50.0) + (i / 2000.0)*cos(i / 2.0);
	}


	cout << "{ 0,0.5,0,-0.7,0.1,0.6,0.8 }" << endl;
	runExercise(testArray, sizeof(testArray) / sizeof(double));

	cout << "{ 3,2.1,2.2,1.9,1.3,1,1.1,0.25 }" << endl;
	runExercise(testArray1, sizeof(testArray1) / sizeof(double));

	cout << "sin(i / 100.0) + cos(i / 50.0) + (i / 2000.0)*cos(i / 2.0)" << endl;
	runExercise(testArray2, sizeof(testArray2) / sizeof(double));

}


/*
* 
*/
int main() {

	cout << "Running #1" << endl;
	number1();
	cout << endl;
	cout << "Running #2" << endl;
	number2();
	cout << endl;
	cout << "Running #3" << endl;
	number3();

	return 0;
}