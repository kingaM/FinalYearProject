/*
 * ContentClassifier.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#ifndef CONTENTCLASSIFIER_H_
#define CONTENTCLASSIFIER_H_

#include "RandomNumberGenerator.h"
#include <packet_m.h>

enum class Class {
    BENIGN, MALICIOUS
};

class ContentClassifier {
    public:
        ContentClassifier();
        virtual ~ContentClassifier();
        Class classify(Packet* p);
    private:
        RandomNumberGenerator generator;
};

#endif /* CONTENTCLASSIFIER_H_ */
