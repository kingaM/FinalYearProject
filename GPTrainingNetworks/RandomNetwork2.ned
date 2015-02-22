import Channel;
import Node;
import SourceNode;
import SinkNode;
import EvilNode;


//
// Generated network with random topology (4 nodes, 5 edges, seed=101).
//

network RandomNetwork2 {
    submodules:
        node0 : Node;
        node1 : EvilNode;
        			
        node2 : SourceNode;
        			
        node3 : SinkNode;
        			
    connections:
        node0.gate++ <--> Channel <--> node1.gate++;
        node0.gate++ <--> Channel <--> node2.gate++;
        node0.gate++ <--> Channel <--> node3.gate++;
        node1.gate++ <--> Channel <--> node2.gate++;
        node1.gate++ <--> Channel <--> node3.gate++;
}
