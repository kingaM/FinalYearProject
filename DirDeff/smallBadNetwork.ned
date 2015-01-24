network SmallBadNetwork
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        source : SourceNode;
        sink : SinkNode;
        evil[1]: EvilNode;
    connections:
        sink.gate++ <--> Channel <--> source.gate++;
        source.gate++ <--> Channel <--> evil[0].gate++;
}
