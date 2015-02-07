#ifndef SOURCENODE_H_
#define SOURCENODE_H_

#include "node.h"

class SinkNode : public Node {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif /* SOURCENODE_H_ */
