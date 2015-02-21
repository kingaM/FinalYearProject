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
        @statistic[packetsSent](source=pktSent; record=count,vector);
        @statistic[dataGenerated](source=genData; record=count,vector);
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
    gates:
        inout gate[];
}

simple SinkNode
{
    parameters:
        @display("i=block/sink,grey90");
        @statistic[recievedPackets](source=rcvdPkt; record=count,vector);
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
    gates:
        inout gate[];
}

channel ErrorChannel 
{
    parameters:
        double delay @unit(s);
        bool disabled = default(false);
}

channel Channel extends ErrorChannel
{
    delay = 100ms;
}
