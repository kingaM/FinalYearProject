import Channel;
import Node;
import SourceNode;
import SinkNode;
import EvilNode;


//
// Generated network with random topology (8 nodes, 15 edges, seed=101).
//

network RandomNetwork3 {
    submodules:
        node0 : SinkNode;
        			
        node1 : Node;
        node2 : Node;
        node3 : Node;
        node4 : SourceNode;
        			
        node5 : EvilNode;
        			
        node6 : Node;
        node7 : Node;
    connections:
        node0.gate++ <--> Channel <--> node1.gate++;
        node0.gate++ <--> Channel <--> node2.gate++;
        node0.gate++ <--> Channel <--> node3.gate++;
        node0.gate++ <--> Channel <--> node4.gate++;
        node0.gate++ <--> Channel <--> node5.gate++;
        node0.gate++ <--> Channel <--> node6.gate++;
        node0.gate++ <--> Channel <--> node7.gate++;
        node1.gate++ <--> Channel <--> node2.gate++;
        node1.gate++ <--> Channel <--> node3.gate++;
        node1.gate++ <--> Channel <--> node4.gate++;
        node1.gate++ <--> Channel <--> node7.gate++;
        node2.gate++ <--> Channel <--> node3.gate++;
        node2.gate++ <--> Channel <--> node7.gate++;
        node3.gate++ <--> Channel <--> node7.gate++;
        node5.gate++ <--> Channel <--> node6.gate++;
}
