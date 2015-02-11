#include <omnetpp.h>
#include "packet_m.h"
#include "RandomNumberGenerator.h"
#include "DataCache.h"
#include "NodeBase.h"

class EvilNode : public NodeBase {
    private:
        void sendBogusInterests();
        void forwardInterestPacket(Packet* ttmsg);
        std::string getRandomString(const int len);
        void addToDataCache(Packet* ttmsg);
        int broadcastInterest(int i);
        int wait(int i);
        int sendDataPacket(int i);

        RandomNumberGenerator generator;
        DataCache dataCache;
        int gate;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
