#!/bin/bash
make
for i in {2..6}
do
        ./DirDeff -u Cmdenv -f ../RandomNetworks/random.ini -c RandomNetwork$i -n ../RandomNetworks/ 
done 

