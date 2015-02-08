#ifndef SOURCENODE_H_
#define SOURCENODE_H_

#include "Node.h"

/**
 * SourceNode - node that generates data and sends it according to the interests
 * received.
 */
class SourceNode : public Node {
    protected:
        virtual void initialize();
};

#endif /* SOURCENODE_H_ */
