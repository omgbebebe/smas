FROM alt:sisyphus
RUN apt-get update && apt-get -y install apt rpm && apt-get -y dist-upgrade && apt-get clean
RUN apt-get -y install gcc libGLES-devel libxcb-devel cmake curl git make gcc-c++ libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel libGL-devel
RUN git clone https://github.com/omgbebebe/smas.git smas
RUN cd smas && git submodule update --init --recursive && mkdir build && cd build
RUN cd smas/build && cmake ../ && make
ENTRYPOINT []
