#include "SinkNode.h"

Define_Module(SinkNode);

void SinkNode::initialize() {
    Node::initialize();
    Packet *msg = generateMessage(EXP_INT, "sensor");
    scheduleAt(simTime() + 2, msg);
    msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
            msg->getDataType(), 0);
    forwardInterestPacket(msg, classifier.classify(msg));
}

void SinkNode::handleMessage(cMessage *msg) {
    Node::handleMessage(msg);
}
