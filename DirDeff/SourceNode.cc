#include "SourceNode.h"

Define_Module(SourceNode);

void SourceNode::initialize() {
    Node::initialize();
    generateSensor();
}
