#ifndef SOURCENODE_H_
#define SOURCENODE_H_

#include "Node.h"

/**
 * SinkNode - a node that generates interests and gathers data from the source
 * nodes.
 */
class SinkNode : public Node {
    protected:
        virtual void initialize();
};

#endif /* SOURCENODE_H_ */
