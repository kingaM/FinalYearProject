#ifndef NODEBASE_H_
#define NODEBASE_H_

#include <omnetpp.h>
#include "packet_m.h"
#include <string>

// Message types
#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define EXP_INT 6
#define TIC 5
#define BROADCAST 7

// Intervals
#define EXPLORATORY_INT 10
#define INT 2

/**
 * A base class for all nodes in the network. Contains all common functions.
 */
class NodeBase : public cSimpleModule {
    protected:
        /**
         * Generates a packet with the given specification.
         * @param interval The rate at which data should be sent. Used only for
         *  interest packets
         * @param type The type of the packet. Can be one of INTEREST 1,
         *  DATA 2
         *  SENSOR 0
         *  INTERVAL 3
         *  EXP_INT 6
         *  TIC 5
         *  BROADCAST 7
         * @param dataType Type of data requested/sent.
         * @param psConc PAMP signal concentration. Used for intrusion
         *  detection.
         *  @see MatrixEntry
         * @return A pointer to the generated packet.
         */
        Packet* generateMessage(int interval, int type, std::string dataType,
                double psConc);
        /**
         * Generates a packet with the given specification. Used for
         * self-messages.
         * @param type The type of the packet. Can be one of INTEREST 1,
         *  DATA 2
         *  SENSOR 0
         *  INTERVAL 3
         *  EXP_INT 6
         *  TIC 5
         *  BROADCAST 7
         * @param dataType Type of data requested/sent.
         * @return A pointer to the generated packet.
         */
        Packet* generateMessage(int type, std::string dataType);
        virtual void setParameters();
        bool malicious = false;
        int id;
        int numOfNodes;
        int run;
    public:
        NodeBase();
        virtual ~NodeBase();
};

#endif /* NODEBASE_H_ */
