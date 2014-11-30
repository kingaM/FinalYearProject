/*
 * RandomNumberGenerator.h
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#include <string>
#include <random>

#ifndef RANDOMNUMBERGENERATOR_H_
#define RANDOMNUMBERGENERATOR_H_

using namespace std;

class RandomNumberGenerator {
public:
    RandomNumberGenerator();
    RandomNumberGenerator(string filename, int row);
    virtual ~RandomNumberGenerator();
    int getNumber(int start, int end);

private:
    default_random_engine generator;
    int getSeed(string filename, int row);
};

#endif /* RANDOMNUMBERGENERATOR_H_ */
