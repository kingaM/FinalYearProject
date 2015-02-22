/*
 * ContentClassifier.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include "ContentClassifier.h"
#include "packet_m.h"
#include <stdio.h>

ContentClassifier::ContentClassifier(int row, int column) {
    generator = RandomNumberGenerator("seeds.csv", row, column);
}

Class ContentClassifier::classify(Packet* p) {
    bool result = generator.boolWithProbability(0.8);
    if (p->getMalicious()) {
        if (result) {
            return Class::MALICIOUS;
        } else {
            return Class::BENIGN;
        }
    } else {
        if (result) {
            return Class::BENIGN;
        } else {
            return Class::MALICIOUS;
        }
    }
}

