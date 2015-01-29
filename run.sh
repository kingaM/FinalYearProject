#!/bin/bash
for i in {2..8}
do
        ./DirDeff/DirDeff -u Cmdenv -f RandomNetworks/random.ini -c RandomNetwork$i -n RandomNetworks/
done 

