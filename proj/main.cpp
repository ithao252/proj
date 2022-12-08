// proj.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;


const clock_t timeBegin = clock();
unsigned int Size = 500000;

string INPUT_FILE = "Books_rating.csv"; // Input file
string SPLIT_CHUNKS_FILE = "Split_file_no_";  // Splited to n files
string SORT_CHUNKS_FILE = "Sorted_file_no_";    // Sorted children files
string OUTPUT_FILE = "Books_rating_sorted.csv"; // Output sorted file 



// Quicksort Algorithm
void quickSort(string* arr, int l, int r)
{
    if (l >= r)
        return;
    int i = l, j = r;
    string mid = arr[(l + r) / 2];
    do
    {
        while (arr[i] < mid)
            i++;
        while (arr[j] > mid)
            j--;
        if (i <= j)
            swap(arr[i++], arr[j--]);
    } while (i <= j);
    quickSort(arr, l, j);
    quickSort(arr, i, r);
}

//Idea:
// We split very big file to many equal file
// Quick Sort each smaller file by the book ID
int splitAndSortFile(string inputFilename, int sizeOfFile)
{
    fstream file_input;
    file_input.open(inputFilename, ios_base::in | ios_base::binary);
    int countingFiles = 0;
    bool flag = true;
    string tmp;
    getline(file_input, tmp);
    while (flag && file_input.good())
    {
        string* arr = new string[sizeOfFile];
        int i = 0;
        for (i; i < sizeOfFile; i++)
        {
            if (file_input.good())
                getline(file_input, arr[i]);
            else
            {
                flag = false;
                break;
            }
        }
        // Sort this array
        quickSort(arr, 0, i - 1);
        string fileOutputName = SPLIT_CHUNKS_FILE + to_string(countingFiles) + ".csv";
        fstream file_output;
        file_output.open(fileOutputName, ios_base::out | ios_base::binary);
        for (int j = 0; j < i; j++)
        {
            file_output << arr[j];
            if (j != i)
                file_output << endl;
        }
        countingFiles++;
        delete[] arr;
        file_output.close();
    }
    file_input.close();
    return countingFiles;
}

// Function to compare 2 id of a book
int compareBookId(string book_id_1, string book_id_2)
{
    int a = 0;
    string s_1 = "";
    while (book_id_1[a] != ',' && a < book_id_1.length())
    {
        s_1.push_back(book_id_1[a]);
        a++;
    }
    int b = 0;
    string s_2 = "";
    while (book_id_2[b] != ',' && b < book_id_2.length())
    {
        s_2.push_back(book_id_2[b]);
        b++;
    }
    // compare book id
    return s_1.compare(s_2);
}



// Merge two file into one file 
void mergeSplitedFile(string inputFileName_1, string inputFileName_2, string outputFile)
{
    fstream tmpFile1, tmpFile2, outputFile_f;
    tmpFile1.open(inputFileName_1, ios::in | ios::binary);
    tmpFile2.open(inputFileName_2, ios::in | ios::binary);
    outputFile_f.open(outputFile, ios::out | ios::binary);
    string x;
    getline(tmpFile1, x);
    string y;
    getline(tmpFile2, y);
    while (!tmpFile1.eof() && !tmpFile2.eof())
    {
        if (compareBookId(x, y) != 1)
        {
            outputFile_f << x << endl;
            getline(tmpFile1, x);
        }
        else
        {
            outputFile_f << y << endl;
            getline(tmpFile2, y);
        }
    }
    while (!tmpFile1.eof())
    {
        outputFile_f << x << endl;
        getline(tmpFile1, x);
    }
    while (!tmpFile2.eof())
    {
        outputFile_f << y << endl;
        getline(tmpFile2, y);
    }
    tmpFile1.close();
    tmpFile2.close();
    outputFile_f.close();
}

// Merge 2 file CSV into fstream
void  mergeSlicetoFstream(string fileName_1, string fileName_2, fstream& fileOutput)
{
    fstream tmpFile_1, tmpFile_2;
    tmpFile_1.open(fileName_1, ios::in | ios::binary);
    tmpFile_2.open(fileName_2, ios::in | ios::binary);
    string x;
    getline(tmpFile_1, x);
    string y;
    getline(tmpFile_2, y);
    while (!tmpFile_1.eof() && !tmpFile_2.eof())
    {
        if (compareBookId(x, y) != 1)
        {
            fileOutput << x << endl;
            getline(tmpFile_1, x);
        }
        else
        {
            fileOutput << y << endl;
            getline(tmpFile_2, y);
        }
    }
    while (!tmpFile_1.eof())
    {
        fileOutput << x << endl;
        getline(tmpFile_1, x);
    }
    while (!tmpFile_2.eof())
    {
        fileOutput << y << endl;
        getline(tmpFile_2, y);
    }
    tmpFile_1.close();
    tmpFile_2.close();
    fileOutput.close();
}

int countingNumberOfFiles(int sizeOfFile)
{
    int nRunways = splitAndSortFile(INPUT_FILE, sizeOfFile);
    return nRunways - 1;
}


// Merge all sorted file into a final file CSV
void mergeFile(int numRunWays)
{
    fstream* readAllFile = new fstream[numRunWays];
    fstream outputFile;
    for (int i = 0; i < numRunWays; i++)
    {
        string s = SPLIT_CHUNKS_FILE + to_string(i);
        readAllFile[i].open(s, ios_base::in | ios_base::binary);
    }
    string file_tmp_0 = SPLIT_CHUNKS_FILE + to_string(0) + ".csv";
    string file_tmp_1 = SPLIT_CHUNKS_FILE + to_string(1) + ".csv";
    string file_a = SORT_CHUNKS_FILE + to_string(0) + ".csv";
    mergeSplitedFile(file_tmp_0, file_tmp_1, file_a);
    for (int i = 2; i < numRunWays; i++)
    {
        string file_b = SORT_CHUNKS_FILE + to_string(i) + ".csv";
        string file = SPLIT_CHUNKS_FILE + to_string(i) + ".csv";
        if (i != numRunWays - 1)
        {
            mergeSplitedFile(file_a, file, file_b);
            file_a = file_b;
        }
        else
        {
            outputFile.open(file_b, ios::out | ios::binary);
            outputFile << "Id,Title,Price,User_id,profileName,review/helpfulness,review/score,review/time,review/summary,review/text" << endl;
            mergeSlicetoFstream(file_a, file, outputFile);
        }
    }
    string result = SORT_CHUNKS_FILE + to_string(numRunWays - 1) + ".csv";
    rename(result.c_str(), OUTPUT_FILE.c_str());
    delete[] readAllFile;
    outputFile.close();
}

// After sort large file, remove all of small file to save memory of computer
void removeSmallerFiles(int numRunways)
{
    for (int i = 0; i <= numRunways; i++)
    {
        string split_file = SPLIT_CHUNKS_FILE + to_string(i);
        string fileName = split_file.append(".csv");
        remove(fileName.c_str());
    }

    for (int i = 0; i < numRunways - 1; i++)
    {
        if (i != 1)
        {
            string file = SORT_CHUNKS_FILE + to_string(i);
            string fileSorting = file.append(".csv");
            remove(fileSorting.c_str());
        }
    }
}


int main()
{
    cout << "Sorting file in processing...\n";

    int numberofRunway = countingNumberOfFiles(Size);
    clock_t start = clock();
    cout << "Alert: Splited to " << numberofRunway << " sorted smaller files\n";

    cout << "Alert: Start merging sorted smaller file into sorted 1 output file....\n";

    mergeFile(numberofRunway);

    removeSmallerFiles(numberofRunway);
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    int phut = time / 60;
    int giay = time - phut * 60;

    cout << "Finish sorting file!!!\n";
    cout << "Final sorted file name is: sorted_books_rating.csv in"<< phut << " phut " << giay <<"giay"<< std::endl;;

    return 0;
}