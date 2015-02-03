//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <SinkNode.h>

Define_Module(SinkNode);

void SinkNode::initialize() {
    Node::initialize();
    Packet *msg = generateMessage(EXP_INT, "sensor");
    scheduleAt(simTime() + 2, msg);
    msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
            msg->getDataType(), 0);
    forwardInterestPacket(msg, classifier.classify(msg));
}

void SinkNode::handleMessage(cMessage *msg)
{
    Node::handleMessage(msg);
}
