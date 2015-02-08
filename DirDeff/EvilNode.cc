#include "EvilNode.h"
#include <string.h>
#include <omnetpp.h>
#include "packet_m.h"
#include "RandomNumberGenerator.h"
#include "DataCache.h"
#include "NodeBase.h"

using namespace std;

Define_Module(EvilNode);

void EvilNode::initialize() {
    generator = RandomNumberGenerator("seeds.csv", 0);
    dataCache = DataCache();
    malicious = true;
}

void EvilNode::forwardInterestPacket(Packet* ttmsg) {
    // GP input here
}

int EvilNode::wait(int i) {
    return ++i;
}

int EvilNode::broadcastInterest(int i) {
    scheduleAt(simTime() + i, generateMessage(BROADCAST, "sensor"));
    return i;
}

void EvilNode::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        delete ttmsg;
        return;
    }
    if (ttmsg->getType() == INTEREST) {
        addToDataCache(ttmsg);
        forwardInterestPacket(ttmsg);
    }
    if (ttmsg->getType() == BROADCAST) {
        sendBogusInterests();
    }
    delete msg;
}

void EvilNode::sendBogusInterests() {
    string type = getRandomString(20);
    EV << "EVIL NODE TYPE: " << type;
    Packet* msg = generateMessage(20, INTEREST, type, 0);
    int n = gateSize("gate");
    for (int i = 0; i < n; i++) {
        Packet *dup = msg->dup();
        send(dup, "gate$o", i);
        addToDataCache(dup);
        EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
    }
}

string EvilNode::getRandomString(const int len) {
    static const char alphanum[] = "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string s = "";
    for (int i = 0; i < len; ++i) {
        s += alphanum[generator.getNumber(0, (sizeof(alphanum) - 1))];
    }
    return s;
}

void EvilNode::addToDataCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prevHop, ttmsg->getType(),
            ttmsg->getDataType(), simTime().raw());
}
