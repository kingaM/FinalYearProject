network GoodNetwork
{
    submodules:
        node[2]: Node;
        sink : SinkNode;
        source : SourceNode;
    connections:
        node[0].gate++ <--> Channel <--> node[1].gate++;
        node[0].gate++ <--> Channel <--> source.gate++;
        node[1].gate++ <--> Channel <--> sink.gate++;
}
