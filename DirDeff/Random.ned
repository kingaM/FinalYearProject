//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

import Node;

channel Link extends ned.DatarateChannel {
    parameters:
        int cost = default(0);
}

//
// Generated network with random topology (10 nodes, 20 edges, seed=93).
//
network RandomNetwork {
    submodules:
        node0 : Node;
        node1 : Node;
        node2 : Node;
        node3 : Node;
        node4 : SourceNode;
        			
        node5 : Node;
        node6 : Node;
        node7 : Node;
        node8 : SinkNode;
        			
        node9 : EvilNode;
        			
    connections:
        node0.gate++ <--> Link <--> node1.gate++;
        node0.gate++ <--> Link <--> node2.gate++;
        node0.gate++ <--> Link <--> node3.gate++;
        node0.gate++ <--> Link <--> node6.gate++;
        node0.gate++ <--> Link <--> node8.gate++;
        node0.gate++ <--> Link <--> node9.gate++;
        node1.gate++ <--> Link <--> node2.gate++;
        node1.gate++ <--> Link <--> node4.gate++;
        node1.gate++ <--> Link <--> node5.gate++;
        node1.gate++ <--> Link <--> node8.gate++;
        node2.gate++ <--> Link <--> node3.gate++;
        node2.gate++ <--> Link <--> node4.gate++;
        node3.gate++ <--> Link <--> node6.gate++;
        node4.gate++ <--> Link <--> node5.gate++;
        node4.gate++ <--> Link <--> node6.gate++;
        node4.gate++ <--> Link <--> node7.gate++;
        node4.gate++ <--> Link <--> node9.gate++;
        node6.gate++ <--> Link <--> node7.gate++;
        node6.gate++ <--> Link <--> node9.gate++;
        node8.gate++ <--> Link <--> node9.gate++;
}
