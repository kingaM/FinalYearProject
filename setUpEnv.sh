#On VM
sudo apt-get install make flex bison g++ gcc tmux libboost-all-dev python-pip python-matplotlib r-base-core 
sudo easy_install pyevolve
pip install jinja2 networkx
tar xvfz omnetpp-4.6-src.tgz
cd omnetpp-4.6
 . setenv
nano ~/.bashrc
# put at the end of file: export PATH=$PATH:$HOME/omnetpp-4.6/bin
source ~/.bashrc
NO_TCL=1 ./configure
make -j4

sudo R CMD INSTALL omnetpp_0.2-1.tar.gz
sudo Rscript -e 'install.packages(c("plyr", "ggplot2", "reshape", "gridExtra"))'

#On host machine
scp -rp FinalYearProject/ azureuser@gpwsn.cloudapp.net:.
scp omnetpp-4.6-src.tgz azureuser@gpwsn.cloudapp.net:.
scp -r Downloads/omnetpp_0.2-1.tar.gz azureuser@gpawsn.cloudapp.net:.
