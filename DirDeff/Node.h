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

/**
 * A routing node in the network. Also acts as a base for SourceNode and
 * SinkNode
 */
class Node : public NodeBase {
    protected:
        /**
         * Generates a "SENSOR" self-message between 2 and 5 seconds from now.
         */
        void generateSensor();
        /**
         * Forwards an interest message, using the criteria defined in the
         * directed diffusion protocol.
         * @param msg The interest message to forward.
         */
        virtual void forwardMessage(Packet *msg);
        /**
         * Initialises objects, parameters, etc.
         */
        virtual void initialize();
        /**
         * Handles any message that is received by the node.
         * @param msg The received message.
         */
        virtual void handleMessage(cMessage *msg);

    public:
        Node();
        virtual ~Node();
    private:
        /**
         * Adds the packet to interest cache.
         */
        void addToCache(Packet* ttmsg);
        /**
         * Adds the packet to data cache.
         */
        void saveToDataCache(Packet* ttmsg);
        /**
         * Saves interest to cache and forwards the interest message.
         */
        void forwardInterestPacket(Packet* ttmsg, Class classification);
        /**
         * Forwards a data packet, as described in the directed diffusion
         * protocol.
         */
        void forwardDataPacket(Packet* ttmsg);
        /**
         * Generates a new exploratory or reinforced interest message.
         * @param dataType The type of interest to generate
         * @param interval EXPLORATORY_INT (10) or INT (2). Indicates what kind
         *  of interval to generate.
         */
        void generateNewInterval(std::string dataType, int interval);
        /**
         * Removes stale entires from the data cache and sends negative
         * reinforcement to the nodes that did not send a data packet recently.
         */
        void deleteDataCacheEntries();
        /**
         * Saves a data packet to the buffer.
         * @see MultiLevelFeedbackQueue
         */
        void saveToBuffer(Packet* ttmsg);
        /**
         * Handles the interest packet when it first arrives at the node.
         */
        void handleInterestPacket(Packet *ttmsg);

        Cache cache;
        DataCache dataCache;
        /**
         * Buffer holding data messages not yet sent.
         * Key = interest type, Value = MultiLevelFeedbackQueue
         */
        std::map<std::string, MultiLevelFeedbackQueue> buffer;
        simtime_t lastSent;
        RandomNumberGenerator generator;
        SignalMatrix* matrix;
        /**
         * @name PAMP Signal
         * Used for PAMP Signal concentration calculation.
         * @see MatrixEntry
         */
        //{@
        SumAcc acc;
        int numOfUpdates = 0;
        std::map<std::string, int> numRcvd;
        std::map<std::string, int> numExp;
        //@}
        DendricCells* dcs = NULL;
        ContentClassifier classifier;
        PacketFilter* filter;
        InterestCacheFilter* icf;
        /**
         * @name Signals
         * Signals gathered from the node.
         */
        //@{
        simsignal_t packetsSentSignal;
        simsignal_t generatedDataSignal;
        simsignal_t receivedPacketsSignal;
        simsignal_t fnfSignal;
        simsignal_t fpfSignal;
        simsignal_t tpfSignal;
        simsignal_t tnfSignal;
        simsignal_t totalMalSignal;
        simsignal_t totalBenSignal;
        //@}
        /**
         * Indicates if a node received a data packet.
         */
        bool first = true;
        /**
         * Enables/disables data filtering, as specified in the ini file.
         */
        bool dataFilteringEnabled = false;
};
