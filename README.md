# Ads.chain C++ library

C++ library with classes and functions to implement the *Ads.chain* protocol

## Building

### Dependencies

The library uses conan for the dependency management and cmake for building. The recommended way of installing conan is with python3 pip.

```bash
# Install python3-pip and cmake (in debian based systems)
$ sudo apt update && sudo apt install cmake python3-pip
# Install and configure conan
$ sudo pip3 install conan
$ conan profile new default --detect
$ conan profile update settings.compiler.libcxx=libstdc++11 default
```


```bash
$ conan remote add google-fruit https://api.bintray.com/conan/google/fruit
$ conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

$ cd <path/to/ads-chain-cpp-library> 
# Build type can be Release or Debug 
$ conan install . -s build_type=Release -s 'compiler.libcxx=libstdc++11' --install-folder=cmake-build-release --build poco --build fruit
```

### build and install
```bash
$ cd <path/to/ads-chain-cpp-library>/cmake-build-release
$ cmake ..
$ make
$ sudo make install
```

You can run also the unit and integration tests. For the network tests you need to change the destination server name in the test code to a running server or assign the alias `dsp.com` of your server in the local dns or in the `/etc/hosts` file.
```bash
$ bin/adauditing_tests [unit],[integration]
```
