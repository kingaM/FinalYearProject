simple Node
{
    parameters:
        @signal[arrival](type="long");
        @statisnode[hopCount](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);

        @display("i=block/routing");
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