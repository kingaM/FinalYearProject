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
        int id;
        int run;
        RandomNumberGenerator generator;

        void discardMessage(const simtime_t& t, result_t& result,
                cMessage* msg);

    public:
        virtual void processMessage(cMessage *msg, simtime_t t,
                result_t& result);
        virtual void initialize();
};

#endif /* ERRORCHANNEL_H_ */
