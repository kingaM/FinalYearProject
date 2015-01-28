import Node;
import SinkNode;
import SourceNode;

network BadNetwork
{
    submodules:
        node[2]: Node;
        source: SourceNode;
        sink: SinkNode;
        evil: EvilNode;
    connections:
        sink.gate++ <--> Channel <--> node[0].gate++;
        node[0].gate++ <--> Channel <--> node[1].gate++;
        node[1].gate++ <--> Channel <--> evil.gate++;
        evil.gate++ <--> Channel <--> source.gate++;
        node[0].gate++ <--> Channel <--> source.gate++;
}
