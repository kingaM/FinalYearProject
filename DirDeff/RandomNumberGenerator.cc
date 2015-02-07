/*
 * RandomNumberGenerator.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#include "RandomNumberGenerator.h"
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

RandomNumberGenerator::RandomNumberGenerator() {
    // Should not be used - just making the compiler happy
}

RandomNumberGenerator::RandomNumberGenerator(string filename, int row) {
    // This will use a new seed for each node, thus each row needs to be substantial
    generator.seed(getSeed(filename, row));
}

RandomNumberGenerator::~RandomNumberGenerator() {
    // TODO Auto-generated destructor stub
}

int RandomNumberGenerator::getNumber(int start, int end) {
    uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

bool RandomNumberGenerator::boolWithProbability(double p) {
    std::bernoulli_distribution distribution(p);
    return distribution(generator);
}

int RandomNumberGenerator::getSeed(string filename, int row) {
    static int column = -1;
    column++;
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
