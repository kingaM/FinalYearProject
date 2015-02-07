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

/**
 * A class containing methods that generate random numbers. This class reads
 * a seed from a csv file and based on that generates pseudo-random numbers.
 * Seeds are used in order to be able to replicate the results if needed.
 */
class RandomNumberGenerator {
    public:
        RandomNumberGenerator();
        /**
         * Initalises the generator with a seed from the specified file and row.
         * The class keeps count of how many times it was already initialised and
         * chooses the next column in the csv file each time.
         */
        RandomNumberGenerator(string filename, int row);
        /**
         * Returns a random number within the specified range. Numbers generated
         * follow uniform distribution.
         */
        int getNumber(int start, int end);
        /**
         * Returns true/false with the specified probability. The values are
         * generated using bernoulli distribution.
         */
        bool boolWithProbability(double p);
    private:
        default_random_engine generator;
        int getSeed(string filename, int row);
};

#endif /* RANDOMNUMBERGENERATOR_H_ */
