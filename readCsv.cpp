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
            for (int i = 0; i < SPECIES_QUANTITY; ++i) {
                if (species[i] == removeQuotes(tempString)) {
                    tempVector.push_back(double(i));
                }
            }

            lines.push_back(tempVector);
        }
        ++counter;
    }

    for (unsigned i = 0; i < header.size(); ++i) {
        cout << header[i] << " ";
    }
    cout << endl;

    for (unsigned i = 0; i < lines.size(); ++i) {
        for (unsigned j = 0; j < lines[i].size(); ++j) {
            cout << lines[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}