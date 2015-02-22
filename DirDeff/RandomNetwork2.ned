import Channel;
import Node;
import SourceNode;
import SinkNode;
import EvilNode;


//
// Generated network with random topology (4 nodes, 4 edges, seed=2063356125).
//

network RandomNetwork2 {
submodules:
        node0 : Node {
          id = 0;
        };
        node1 : SinkNode {
          id = 1;
        };
        node2 : EvilNode {
          id = 2;
        };
        node3 : SourceNode {
          id = 3;
        };
    connections:
        node0.gate++ <--> Channel {
          id = 4;
        } <--> node1.gate++;
        node0.gate++ <--> Channel {
          id = 5;
        } <--> node2.gate++;
        node0.gate++ <--> Channel {
          id = 6;
        } <--> node3.gate++;
        node1.gate++ <--> Channel {
          id = 7;
        } <--> node2.gate++;
}
