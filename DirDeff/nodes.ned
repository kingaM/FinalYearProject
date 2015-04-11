simple Node
{
    parameters:
        @display("i=block/routing");
        @statistic[falseNegative](source=fn; record=count,vector);
        @statistic[falePositive](source=fp; record=count,vector);
        @statistic[trueNegative](source=tn; record=count,vector);
        @statistic[truePositive](source=tp; record=count,vector);
        @statistic[falseNegativeFilter](source=fnf; record=count,vector);
        @statistic[falePositiveFilter](source=fpf; record=count,vector);
        @statistic[trueNegativeFilter](source=tnf; record=count,vector);
        @statistic[truePositiveFilter](source=tpf; record=count,vector);
        @statistic[totalMalicious](source=totalm; record=count,vector);
        @statistic[totalBenign](source=totalb; record=count,vector);
        @statistic[packetsSent](source=pktSent; record=count,vector);
        int id = default(-1);
        int numOfNodes = default(1);
        int run = default(0);
        bool interestCacheFilter = default(true);
        bool dataFilter = default(true);
    gates:
        inout gate[];
}

simple EvilNode
{
    parameters:
        @display("i=block/routing,red");
        int id = default(-1);
        int numOfNodes = default(1);
        int run = default(0);
    gates:
        inout gate[];
}

simple SourceNode extends Node
{
    parameters:
        @display("i=block/source,grey75");
        @statistic[dataGenerated](source=genData; record=count,vector);
        @class(SourceNode);
}

simple SinkNode extends Node
{
    parameters:
        @display("i=block/sink,grey90");
        @statistic[receivedPackets](source=rcvdPkt; record=count,vector);
        @class(SinkNode);
}

channel ErrorChannel 
{
    parameters:
        double delay @unit(s);
        bool disabled = default(false);
        int id = default(-1);
        int run = default(0);
}

channel Channel extends ErrorChannel
{
    delay = 100ms;
}
