network BadNetwork
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        node[7]: Node;
        evil[1]: EvilNode;
    connections:
        node[0].gate++ <--> Channel <--> node[1].gate++;
        node[1].gate++ <--> Channel <--> node[2].gate++;
        node[1].gate++ <--> Channel <--> node[4].gate++;
        node[3].gate++ <--> Channel <--> evil[0].gate++;
        node[4].gate++ <--> Channel <--> evil[0].gate++;
        node[3].gate++ <--> Channel <--> node[2].gate++;
}
