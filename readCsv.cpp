#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <typeinfo>


#define SPECIES_QUANTITY 7
#define PARAMETERS_QUANTITY 17

std::string removeQuotes(std::string mStr) {
    mStr.erase(remove(mStr.begin(), mStr.end(), '"'), mStr.end());
    return mStr;
}

double removeQuotesAndAtof(std::string mStr) {
    mStr.erase(remove(mStr.begin(), mStr.end(), '"'), mStr.end());
    return atof(mStr.c_str());
}

int readCsv(std::vector<std::vector<double>> &lines) {

    std::ifstream readDataset ("Dry_Bean_Dataset.csv");
    std::string species[SPECIES_QUANTITY] = {
        "SEKER",
        "BARBUNYA",
        "BOMBAY",
        "CALI",
        "HOROZ",
        "SIRA",
        "DERMASON"
    };

    std::vector<std::string> header;

    std::string line;
    int counter = 0;
    while(getline(readDataset, line)) {
        std::string tempString;
        std::vector<double> tempVector;
        std::stringstream inputString(line);

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


    return 0;
}