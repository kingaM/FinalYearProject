#include <omnetpp.h>
#include "packet_m.h"
#include "RandomNumberGenerator.h"
#include "DataCache.h"
#include "NodeBase.h"

/**
 * The attacker that is modified by the GP. Provides the GP with a skeleton.
 */
class EvilNode : public NodeBase {
    private:
        /**
         * Sends a bogus interest packet, with a random 20-character type and
         * broadcasted to all neighbours.
         */
        void sendBogusInterests();
        /**
         * Contents of this function are changed using the GP. It's the main
         * attack function. Gets executed every time the node receives an
         * interest.
         * @param ttmsg The interest packet received.
         */
        void forwardInterestPacket(Packet* ttmsg);
        /**
         * Creates a random string of specified length.
         * @param len The length of the string.
         * @return Randomly generated string.
         */
        std::string getRandomString(const int len);
        /**
         * Adds a packet to data cache of the node. Makes sure that the message
         * is forwarded only once.
         * @param ttmsg
         */
        void addToDataCache(Packet* ttmsg);
        /**
         * The GP method, which sends an "INTEREST" self-message after the i
         * seconds.
         */
        int broadcastInterest(int i);
        /**
         * The GP method that increments i.
         */
        int wait(int i);
        /**
         * The GP method that sends a "SENSOR" self-message after i seconds. The
         * content of the self-message includes the gate at which the interest
         * was received from and the type of the interest.
         */
        int sendDataPacket(int i);

        RandomNumberGenerator generator;
        DataCache dataCache;
        /**
         * The gate on which the interest arrived. Used in sendDataPacket.
         * Can't be used directly because sendDataPacket is places by the GP.
         */
        int gate;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
