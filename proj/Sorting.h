#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;



int compareBookId(string book_id_1, string book_id_2);

void mergeSplitedFile(string inputFileName_1, string inputFileName_2, string outputFile);

void mergeSlicetoFstream(string fileName_1, string fileName_2, fstream& fileOutput);

void quickSort(string* arr, int l, int r);

int splitAndSortFile(string inputFilename, int sizeOfFile);

int countingNumberOfFiles(int size);

void mergeFile(int numRunWays);

void removeSmallerFiles(int numRunways);