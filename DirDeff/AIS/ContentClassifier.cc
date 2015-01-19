/*
 * ContentClassifier.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include <ContentClassifier.h>
#include <packet_m.h>

ContentClassifier::ContentClassifier() {
    generator = RandomNumberGenerator("seeds.csv", 0);
}

ContentClassifier::~ContentClassifier() {
    // TODO Auto-generated destructor stub
}

Class ContentClassifier::classify(Packet* p) {
    bool result = generator.boolWithProbability(0.8);
    if(p->getMalicious()) {
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

