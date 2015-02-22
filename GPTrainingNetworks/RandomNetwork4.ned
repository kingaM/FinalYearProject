import Channel;
import Node;
import SourceNode;
import SinkNode;
import EvilNode;


//
// Generated network with random topology (16 nodes, 75 edges, seed=101).
//

network RandomNetwork4 {
    submodules:
        node0 : Node;
        node1 : Node;
        node2 : Node;
        node3 : SourceNode;
        			
        node4 : Node;
        node5 : Node;
        node6 : Node;
        node7 : EvilNode;
        			
        node8 : Node;
        node9 : Node;
        node10 : SinkNode;
        			
        node11 : Node;
        node12 : Node;
        node13 : Node;
        node14 : Node;
        node15 : Node;
    connections:
        node0.gate++ <--> Channel <--> node1.gate++;
        node0.gate++ <--> Channel <--> node2.gate++;
        node0.gate++ <--> Channel <--> node3.gate++;
        node0.gate++ <--> Channel <--> node4.gate++;
        node0.gate++ <--> Channel <--> node5.gate++;
        node0.gate++ <--> Channel <--> node6.gate++;
        node0.gate++ <--> Channel <--> node7.gate++;
        node0.gate++ <--> Channel <--> node8.gate++;
        node0.gate++ <--> Channel <--> node9.gate++;
        node0.gate++ <--> Channel <--> node10.gate++;
        node0.gate++ <--> Channel <--> node11.gate++;
        node0.gate++ <--> Channel <--> node12.gate++;
        node0.gate++ <--> Channel <--> node13.gate++;
        node0.gate++ <--> Channel <--> node14.gate++;
        node0.gate++ <--> Channel <--> node15.gate++;
        node1.gate++ <--> Channel <--> node2.gate++;
        node1.gate++ <--> Channel <--> node3.gate++;
        node1.gate++ <--> Channel <--> node4.gate++;
        node1.gate++ <--> Channel <--> node5.gate++;
        node1.gate++ <--> Channel <--> node6.gate++;
        node1.gate++ <--> Channel <--> node7.gate++;
        node1.gate++ <--> Channel <--> node8.gate++;
        node1.gate++ <--> Channel <--> node9.gate++;
        node1.gate++ <--> Channel <--> node10.gate++;
        node1.gate++ <--> Channel <--> node11.gate++;
        node1.gate++ <--> Channel <--> node12.gate++;
        node1.gate++ <--> Channel <--> node14.gate++;
        node1.gate++ <--> Channel <--> node15.gate++;
        node2.gate++ <--> Channel <--> node3.gate++;
        node2.gate++ <--> Channel <--> node4.gate++;
        node2.gate++ <--> Channel <--> node5.gate++;
        node2.gate++ <--> Channel <--> node6.gate++;
        node2.gate++ <--> Channel <--> node7.gate++;
        node2.gate++ <--> Channel <--> node9.gate++;
        node2.gate++ <--> Channel <--> node10.gate++;
        node2.gate++ <--> Channel <--> node11.gate++;
        node2.gate++ <--> Channel <--> node12.gate++;
        node2.gate++ <--> Channel <--> node13.gate++;
        node2.gate++ <--> Channel <--> node14.gate++;
        node2.gate++ <--> Channel <--> node15.gate++;
        node3.gate++ <--> Channel <--> node4.gate++;
        node3.gate++ <--> Channel <--> node6.gate++;
        node3.gate++ <--> Channel <--> node7.gate++;
        node3.gate++ <--> Channel <--> node8.gate++;
        node3.gate++ <--> Channel <--> node9.gate++;
        node3.gate++ <--> Channel <--> node10.gate++;
        node3.gate++ <--> Channel <--> node11.gate++;
        node3.gate++ <--> Channel <--> node12.gate++;
        node3.gate++ <--> Channel <--> node13.gate++;
        node3.gate++ <--> Channel <--> node14.gate++;
        node3.gate++ <--> Channel <--> node15.gate++;
        node4.gate++ <--> Channel <--> node5.gate++;
        node4.gate++ <--> Channel <--> node6.gate++;
        node4.gate++ <--> Channel <--> node8.gate++;
        node4.gate++ <--> Channel <--> node10.gate++;
        node4.gate++ <--> Channel <--> node11.gate++;
        node4.gate++ <--> Channel <--> node12.gate++;
        node4.gate++ <--> Channel <--> node13.gate++;
        node4.gate++ <--> Channel <--> node14.gate++;
        node4.gate++ <--> Channel <--> node15.gate++;
        node5.gate++ <--> Channel <--> node6.gate++;
        node5.gate++ <--> Channel <--> node8.gate++;
        node5.gate++ <--> Channel <--> node9.gate++;
        node5.gate++ <--> Channel <--> node11.gate++;
        node5.gate++ <--> Channel <--> node13.gate++;
        node5.gate++ <--> Channel <--> node14.gate++;
        node6.gate++ <--> Channel <--> node7.gate++;
        node7.gate++ <--> Channel <--> node8.gate++;
        node7.gate++ <--> Channel <--> node9.gate++;
        node7.gate++ <--> Channel <--> node12.gate++;
        node7.gate++ <--> Channel <--> node15.gate++;
        node8.gate++ <--> Channel <--> node9.gate++;
        node8.gate++ <--> Channel <--> node13.gate++;
        node10.gate++ <--> Channel <--> node11.gate++;
        node12.gate++ <--> Channel <--> node14.gate++;
}
