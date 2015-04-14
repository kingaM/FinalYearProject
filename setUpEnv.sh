# Download Omnetpp-4.6 from http://www.omnetpp.org/omnetpp/download/30-omnet-releases/2290-omnet-4-6-source-ide-tgz
# Download Omnetpp_0.2-1 https://github.com/downloads/omnetpp/omnetpp-resultfiles/omnetpp_0.2-1.tar.gz
sudo apt-get install make flex bison g++ gcc tmux libboost-all-dev python-pip python-matplotlib r-base-core 
sudo easy_install pyevolve
pip install jinja2 networkx
tar xvfz omnetpp-4.6-src.tgz
cd omnetpp-4.6
 . setenv
echo "export PATH=$PATH:$HOME/omnetpp-4.6/bin" >> ~/.bashrc
source ~/.bashrc
NO_TCL=1 ./configure
make -j4

sudo R CMD INSTALL omnetpp_0.2-1.tar.gz
# Note: this requires for the mirrors in R CRAN to be set
sudo Rscript -e 'install.packages(c("plyr", "ggplot2", "reshape", "gridExtra"))'
