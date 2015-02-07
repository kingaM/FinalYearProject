#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "packet_m.h"
#include "Cache.h"
#include "DataCache.h"
#include "RandomNumberGenerator.h"
#include "MultiLevelFeedbackQueue.h"
#include <set>
#include <map>
#include <utility>
#include <SignalMatrix.h>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include <DendricCells.h>
#include <AIS/ContentClassifier.h>
#include <AIS/PacketFilter.h>

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define EXP_INT 6
#define DATA_RETRY 4
#define TIC 5
#define EXPLORATORY_INT 10
#define INT 2

using namespace std;
using namespace boost::accumulators;

typedef accumulator_set<int, stats<tag::rolling_sum>> SumAcc;

class Node : public cSimpleModule {
    protected:
        void addToCache(Packet* ttmsg);
        void generateSensor();
        void saveToDataCache(Packet* ttmsg);
        void forwardInterestPacket(Packet* ttmsg, Class classification);
        void forwardDataPacket(Packet* ttmsg);
        void generateNewInterval(string dataType, int interval);
        void deleteDataCacheEntries();
        void saveToBuffer(Packet* ttmsg);

        Cache cache;
        DataCache dataCache;
        map<string, MultiLevelFeedbackQueue> buffer;
        simtime_t lastSent;
        RandomNumberGenerator generator;
        SignalMatrix* matrix;
        SumAcc acc;
        int numOfUpdates = 0;
        map<string, int> numRcvd;
        map<string, int> numExp;
        DendricCells* dcs = NULL;
        ContentClassifier classifier;
        PacketFilter* filter;
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
        virtual Packet *generateMessage(simtime_t expiresAt, int interval,
                int type, simtime_t timestamp, string dataType, double psConc);
        virtual Packet *generateMessage(int type, string dataType);
        virtual void forwardMessage(Packet *msg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Node() : acc(SumAcc(tag::rolling_window::window_size = 10)) {};
};
