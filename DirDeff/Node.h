#include <string.h>
#include <omnetpp.h>
#include <map>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include "packet_m.h"
#include "Cache.h"
#include "DataCache.h"
#include "RandomNumberGenerator.h"
#include "MultiLevelFeedbackQueue.h"
#include "DendricCells.h"
#include "AIS/ContentClassifier.h"
#include "AIS/PacketFilter.h"
#include "AIS/SignalMatrix.h"
#include "AIS/InterestCacheFilter.h"
#include "NodeBase.h"

typedef boost::accumulators::accumulator_set<int,
        boost::accumulators::stats<boost::accumulators::tag::rolling_sum>> SumAcc;

class Node : public NodeBase {
    protected:
        void generateSensor();
        virtual void forwardMessage(Packet *msg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Node();
        virtual ~Node();
    private:
        void addToCache(Packet* ttmsg);
        void saveToDataCache(Packet* ttmsg);
        void forwardInterestPacket(Packet* ttmsg, Class classification);
        void forwardDataPacket(Packet* ttmsg);
        void generateNewInterval(std::string dataType, int interval);
        void deleteDataCacheEntries();
        void saveToBuffer(Packet* ttmsg);
        void handleInterestPacket(Packet *ttmsg);
        void sinkDataPackets(Packet* ttmsg);

        Cache cache;
        DataCache dataCache;
        std::map<std::string, MultiLevelFeedbackQueue> buffer;
        simtime_t lastSent;
        RandomNumberGenerator generator;
        SignalMatrix* matrix;
        SumAcc acc;
        int numOfUpdates = 0;
        std::map<std::string, int> numRcvd;
        std::map<std::string, int> numExp;
        DendricCells* dcs = NULL;
        ContentClassifier classifier;
        PacketFilter* filter;
        InterestCacheFilter* icf;
        simsignal_t packetsSentSignal;
        simsignal_t generatedDataSignal;
        simsignal_t receievedPacketsSignal;
        simsignal_t fnfSignal;
        simsignal_t fpfSignal;
        simsignal_t tpfSignal;
        simsignal_t tnfSignal;
        simsignal_t totalMalSignal;
        simsignal_t totalBenSignal;
        bool first = true;
};
