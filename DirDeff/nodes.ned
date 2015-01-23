simple Node
{
    parameters:
        @display("i=block/routing");
        @statistic[packetsSent](source=pktSent; record=count,histogram,vector);
        @statistic[dataGenerated](source=genData; record=count,histogram,vector);
        @statistic[recievedPackets](source=rcvdPkt; record=count,histogram,vector);
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