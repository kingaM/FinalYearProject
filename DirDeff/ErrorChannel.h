#ifndef ERRORCHANNEL_H_
#define ERRORCHANNEL_H_

#include <cdelaychannel.h>
#include "RandomNumberGenerator.h"

enum class State {
        GOOD, BAD
};

class ErrorChannel : public cDelayChannel {
    private:
        State state = State::GOOD;
        double pGood = 0.99;
        double pBad = 0.6;
        RandomNumberGenerator generator = RandomNumberGenerator("seeds.csv", 0);

        void discardMessage(const simtime_t& t, result_t& result,
                cMessage* msg);

    public:
        virtual void processMessage(cMessage *msg, simtime_t t, result_t& result);
};

#endif /* ERRORCHANNEL_H_ */
