#include "ErrorChannel.h"
#include <omnetpp.h>
#include "RandomNumberGenerator.h"
#include "debug.h"

Define_Channel(ErrorChannel);

void ErrorChannel::discardMessage(const simtime_t& t, result_t& result,
        cMessage* msg) {
    DEBUG_MSG("DISCARDING MESSAGE");
    result.discard = true;
    cTimestampedValue tmp(t, msg);
    emit(messageDiscardedSignal, &tmp);
}

void ErrorChannel::processMessage(cMessage* msg, simtime_t t,
        result_t& result) {
    if (!disabled) {
        switch (state) {
            case State::GOOD:
                if (!generator.boolWithProbability(pGood)) {
                    state = State::BAD;
                    discardMessage(t, result, msg);
                    return;
                }
                break;
            case State::BAD:
                if (generator.boolWithProbability(pBad)) {
                    discardMessage(t, result, msg);
                    return;
                } else {
                    state = State::GOOD;
                }
                break;
        }
    }
    cDelayChannel::processMessage(msg, t, result);
}

void ErrorChannel::initialize() {
    run = par("run");
    id = par("id");
    disabled = par("disabled");
    generator = RandomNumberGenerator("seeds.csv", run, id);
    cDelayChannel::initialize();
}
