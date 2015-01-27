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

//
// Generated network with random topology (12 nodes, 20 edges, seed=100).
//
network Network {
    submodules:
        node0 : SourceNode;
        			
        node1 : Node;
        node2 : Node;
        node3 : Node;
        node4 : Node;
        node5 : Node;
        node6 : Node;
        node7 : SinkNode;
        			
        node8 : Node;
        node9 : Node;
        node10 : EvilNode;
        			
        node11 : Node;
    connections:
        node0.gate++ <--> Channel <--> node1.gate++;
        node0.gate++ <--> Channel <--> node2.gate++;
        node0.gate++ <--> Channel <--> node4.gate++;
        node0.gate++ <--> Channel <--> node6.gate++;
        node0.gate++ <--> Channel <--> node11.gate++;
        node1.gate++ <--> Channel <--> node2.gate++;
        node1.gate++ <--> Channel <--> node3.gate++;
        node1.gate++ <--> Channel <--> node5.gate++;
        node1.gate++ <--> Channel <--> node8.gate++;
        node2.gate++ <--> Channel <--> node4.gate++;
        node2.gate++ <--> Channel <--> node8.gate++;
        node3.gate++ <--> Channel <--> node5.gate++;
        node3.gate++ <--> Channel <--> node7.gate++;
        node3.gate++ <--> Channel <--> node10.gate++;
        node4.gate++ <--> Channel <--> node11.gate++;
        node5.gate++ <--> Channel <--> node9.gate++;
        node6.gate++ <--> Channel <--> node10.gate++;
        node6.gate++ <--> Channel <--> node11.gate++;
        node7.gate++ <--> Channel <--> node9.gate++;
        node8.gate++ <--> Channel <--> node9.gate++;
}
