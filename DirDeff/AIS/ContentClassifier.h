/*
 * ContentClassifier.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#ifndef CONTENTCLASSIFIER_H_
#define CONTENTCLASSIFIER_H_

#include "RandomNumberGenerator.h"
#include "packet_m.h"

enum class Class {
    BENIGN, MALICIOUS
};

/**
 * Class that is used to classify incoming packets into two classes.
 * For the purpose of this project the classification is mocked with an 80%
 * accuracy.
 */
class ContentClassifier {
    public:
        ContentClassifier();
        Class classify(Packet* p);
    private:
        RandomNumberGenerator generator;
};

#endif /* CONTENTCLASSIFIER_H_ */
