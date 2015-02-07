/*
 * RandomNumberGenerator.h
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#ifndef RANDOMNUMBERGENERATOR_H_
#define RANDOMNUMBERGENERATOR_H_

#include <string>
#include <random>

using namespace std;

class RandomNumberGenerator {
    public:
        RandomNumberGenerator();
        RandomNumberGenerator(string filename, int row);
        int getNumber(int start, int end);
        bool boolWithProbability(double p);
    private:
        default_random_engine generator;
        int getSeed(string filename, int row);
};

#endif /* RANDOMNUMBERGENERATOR_H_ */
