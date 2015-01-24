simple Node
{
    parameters:
        @display("i=block/routing");
        @statistic[packetsSent](source=pktSent; record=count,histogram,vector);
        @statistic[dataGenerated](source=genData; record=count,histogram,vector);
        @statistic[recievedPackets](source=rcvdPkt; record=count,histogram,vector);
        @statistic[falseNegative](source=fn; record=count,histogram,vector);
        @statistic[falePositive](source=fp; record=count,histogram,vector);
    gates:
        inout gate[];
}

simple EvilNode
{
    parameters:
        @display("i=block/routing,red");
    gates:
        inout gate[];
}

simple SourceNode
{
    parameters:
        @display("i=block/source,grey75");
        @statistic[packetsSent](source=pktSent; record=count,histogram,vector);
        @statistic[dataGenerated](source=genData; record=count,histogram,vector);
        @statistic[recievedPackets](source=rcvdPkt; record=count,histogram,vector);
        @statistic[falseNegative](source=fn; record=count,histogram,vector);
        @statistic[falePositive](source=fp; record=count,histogram,vector);
    gates:
        inout gate[];
}

simple SinkNode
{
    parameters:
        @display("i=block/sink,grey90");
        @statistic[packetsSent](source=pktSent; record=count,histogram,vector);
        @statistic[dataGenerated](source=genData; record=count,histogram,vector);
        @statistic[recievedPackets](source=rcvdPkt; record=count,histogram,vector);
        @statistic[falseNegative](source=fn; record=count,histogram,vector);
        @statistic[falePositive](source=fp; record=count,histogram,vector);
    gates:
        inout gate[];
}
