network SmallBadNetwork
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        node[2]: Node;
        evil[1]: EvilNode;
    connections:
        node[0].gate++ <--> Channel <--> node[1].gate++;
        node[1].gate++ <--> Channel <--> evil[0].gate++;
}
