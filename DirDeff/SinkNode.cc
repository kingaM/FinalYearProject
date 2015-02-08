#include "SinkNode.h"

Define_Module(SinkNode);

void SinkNode::initialize() {
    Node::initialize();
    Packet *msg = generateMessage(EXP_INT, "sensor");
    scheduleAt(simTime(), msg);
}
