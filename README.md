# Build and run

```sh
git clone https://github.com/omgbebebe/smas.git smas && cd smas
git submodule update --init --recursive
mkdir -p build && cd build
cmake ../ && make
./smas_gui
```

## Docker build

```sh
docker build -t smas .
docker run --rm -it smas /bin/bash
# compiled binary will be located in `/smas/build/smas_gui`
```
