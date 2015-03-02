/*
 * RandomNumberGenerator.cc
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#include "RandomNumberGenerator.h"
#include <string>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

RandomNumberGenerator::RandomNumberGenerator() {
    // Should not be used - just making the compiler happy
}

RandomNumberGenerator::RandomNumberGenerator(string filename, int row, int column) {
    generator.seed(getSeed(filename, row, column));
}

int RandomNumberGenerator::getNumber(int start, int end) {
    uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

bool RandomNumberGenerator::boolWithProbability(double p) {
    std::bernoulli_distribution distribution(p);
    return distribution(generator);
}

int RandomNumberGenerator::getSeed(string filename, int row, int column) {
    string line, csvItem;
    ifstream myfile(filename);
    int lineNumber = 0;
    int columnNumber = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (lineNumber == row) {
                istringstream myline(line);
                while (getline(myline, csvItem, ',')) {
                    if (columnNumber == column) {
                        return stoi(csvItem);
                    }
                    columnNumber++;
                }
            }
            lineNumber++;
        }
        myfile.close();
    }
    return 0;
}
