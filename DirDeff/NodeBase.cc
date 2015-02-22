#include "NodeBase.h"
#include <omnetpp.h>
#include <string.h>
#include "packet_m.h"

using namespace std;

NodeBase::NodeBase() {

}

NodeBase::~NodeBase() {

}

Packet *NodeBase::generateMessage(int interval, int type, string dataType,
        double psConc) {
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setExpiresAt((simTime() + 1000).raw());
    msg->setInterval(interval);
    msg->setType(type);
    msg->setTimestamp(simTime());
    msg->setDataType(dataType.c_str());
    msg->setPsConc(psConc);
    msg->setMalicious(malicious);
    return msg;
}

Packet *NodeBase::generateMessage(int type, string dataType) {
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setType(type);
    msg->setDataType(dataType.c_str());
    return msg;
}

void NodeBase::setParameters() {
    id = par("id");
    numOfNodes = par("numOfNodes");
    run = par("run");
}
