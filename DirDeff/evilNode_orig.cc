#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "packet_m.h"
#include <set>
#include <map>
#include <utility>
#include "RandomNumberGenerator.h"
#include "Cache.h"
#include "DataCache.h"

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define DATA_RETRY 4
#define TIC 5

using namespace std;

typedef vector<char> char_array;

class EvilNode_Orig : public cSimpleModule {
    private:
        simsignal_t arrivalSignal;
        void sendBogusInterests();
        void forwardInterestPacket(Packet* ttmsg);
        string getRandomString(const int len);
        void addToDataCache(Packet* ttmsg);

        simtime_t lastSent;
        RandomNumberGenerator generator;
        DataCache dataCache;

    protected:
        virtual Packet *generateMessage(simtime_t expiresAt, int interval,
                int type, simtime_t timestamp, string dataType, double psConc);
        virtual Packet *generateMessage(int type, string dataType);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        EvilNode_Orig() {};
};

Define_Module(EvilNode_Orig);

void EvilNode_Orig::initialize() {
    arrivalSignal = registerSignal("arrival");
    lastSent = simTime();
    generator = RandomNumberGenerator("seeds.csv", 0);
    dataCache = DataCache();
//    scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
}

void EvilNode_Orig::forwardInterestPacket(Packet* ttmsg) {
    // We need to forward the message.
    EV << "FORWARDING INTEREST PACKET with interval " << ttmsg->getInterval()
            << endl;
    sendBogusInterests();
}

void EvilNode_Orig::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        delete ttmsg;
        return;
    }
    if (ttmsg->getType() == INTEREST) {
        addToDataCache(ttmsg);
        forwardInterestPacket(ttmsg);
    }
    if (ttmsg->getType() == TIC) {
        scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
    }
    delete msg;
}

Packet *EvilNode_Orig::generateMessage(simtime_t expiresAt, int interval, int type,
        simtime_t timestamp, string dataType, double psConc) {
    int src = getIndex();

    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setSource(src);
    msg->setExpiresAt((simTime() + 1000).raw());
    msg->setInterval(interval);
    msg->setType(type);
    msg->setTimestamp(timestamp);
    msg->setDataType(dataType.c_str());
    msg->setPsConc(psConc);
    msg->setMalicious(true);
    return msg;
}

Packet *EvilNode_Orig::generateMessage(int type, string dataType) {
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setType(type);
    msg->setDataType(dataType.c_str());
    msg->setMalicious(true);
    return msg;
}

void EvilNode_Orig::sendBogusInterests() {
    for (int i = 0; i < Cache::SIZE; i++) {
        string type = getRandomString(20);
        EV << "EVIL NODE TYPE: " << type;
        Packet* msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
                type, 0);
        int n = gateSize("gate");
        for (int i = 0; i < n; i++) {
            Packet *dup = msg->dup();
            send(dup, "gate$o", i);
            addToDataCache(dup);
            EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
        }
    }
}

string EvilNode_Orig::getRandomString(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string s = "";
    for (int i = 0; i < len; ++i) {
        s += alphanum[generator.getNumber(0,(sizeof(alphanum) - 1))];
    }
    return s;
}

void EvilNode_Orig::addToDataCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prevHop, ttmsg->getType(),
            ttmsg->getDataType(), simTime().raw());
}
