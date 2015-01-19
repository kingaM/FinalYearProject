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
    string line, csvItem;
    ifstream myfile(filename);
    int lineNumber = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (lineNumber == row) {
                istringstream myline(line);
                while (getline(myline, csvItem, ',')) {
                    return stoi(csvItem);
                }
            }
            lineNumber++;
        }
        myfile.close();
    }
    return 0;
}
