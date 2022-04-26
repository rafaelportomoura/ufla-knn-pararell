#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <typeinfo>

using namespace std;

#define SPECIES_QUANTITY 7
#define PARAMETERS_QUANTITY 17

string removeQuotes(string mStr) {
    mStr.erase(remove(mStr.begin(), mStr.end(), '"'), mStr.end());
    return mStr;
}

double removeQuotesAndAtof(string mStr) {
    mStr.erase(remove(mStr.begin(), mStr.end(), '"'), mStr.end());
    return atof(mStr.c_str());
}

int main() {

    ifstream readDataset ("Dry_Bean_Dataset.csv");
    string species[SPECIES_QUANTITY] = {
        "SEKER",
        "BARBUNYA",
        "BOMBAY",
        "CALI",
        "HOROZ",
        "SIRA",
        "DERMASON"
    };

    vector<vector<double>> lines;
    vector<string> header;

    string line;
    int counter = 0;
    while(getline(readDataset, line)) {
        // double area, perimeter, majorAxisLength, minorAxisLength, aspectRatio, eccentricity, convexArea, equivDiameter, extent, solidity, roundness, compactness, shapeFactor1, shapeFactor2, shapeFactor3, shapeFactor4, beanClass;
        string tempString;
        vector<double> tempVector;
        stringstream inputString(line);

        if (counter == 0) {
            for (int i = 0; i < PARAMETERS_QUANTITY; ++i) {
                getline(inputString, tempString, ',');
                header.push_back(removeQuotes(tempString));
            }
        }
        if (counter > 0) {
            for (int i = 0; i < PARAMETERS_QUANTITY - 1; ++i) {
                getline(inputString, tempString, ',');
                tempVector.push_back(removeQuotesAndAtof(tempString));
            }
            
            getline(inputString, tempString, ',');
            if (removeQuotes(tempString) == "SEKER") {
                tempVector.push_back(double(0));
            } else if (removeQuotes(tempString) == "BARBUNYA") {
                tempVector.push_back(double(1));
            } else if (removeQuotes(tempString) == "BOMBAY") {
                tempVector.push_back(double(2));
            } else if (removeQuotes(tempString) == "CALI") {
                tempVector.push_back(double(3));
            } else if (removeQuotes(tempString) == "HOROZ") {
                tempVector.push_back(double(4));
            } else if (removeQuotes(tempString) == "SIRA") {
                tempVector.push_back(double(5));
            } else if (removeQuotes(tempString) == "DERMASON") {
                tempVector.push_back(double(6));
            }

            lines.push_back(tempVector);
        }
        ++counter;
    }

    for (unsigned i = 0; i < header.size(); ++i) {
        cout << header[i] << " ";
    }

    for (unsigned i = 0; i < lines.size(); ++i) {
        for (unsigned j = 0; j < lines[i].size(); ++j) {
            cout << lines[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}