#On VM
sudo apt-get install make flex bison g++ gcc tmux libboost-all-dev python-pip
sudo easy_install pyevolve
tar xvfz omnetpp-4.6-src.tgz
cd omnetpp-4.6
 . setenv
nano ~/.bashrc
# put at the end of file: export PATH=$PATH:$HOME/omnetpp-4.6/bin
source ~/.bashrc
NO_TCL=1 ./configure
make -j4

#On host machine
scp -rp FinalYearProject/ azureuser@gpwsn.cloudapp.net:.
scp omnetpp-4.6-src.tgz azureuser@gpwsn.cloudapp.net:.

