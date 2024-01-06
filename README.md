
# IRC server

This project is simple in essence: develop an `IRC server` following [RFC 1459](http://abcdrfc.free.fr/rfc-vf/rfc1459.html).

In this project I aspire to develop classes facilitating the use of `Sockets` but also to develop easy to understand and modular code. This project was also an occasion for me to go from c++98 to c++11.

# Installation

You'll need make and clang (or any traditionnal c++ compiler)
```
sudo apt-get update
sudo apt-get install make
sudo apt-get install clang 
```
or
```
sudo apt-get update
sudo apt-get install make
sudo apt-get install build-essential
```

Then simply write `make` (_optional arguments: debug, optimize, noflag, santhread, optimize_) in your console while being at the root of our repository to compile the program and launch by executing the newly generated executable.

# Sockets class (and extra)

Everything that's related to Sockets usage is in the `sockets` folder. In main.cpp, you can find a usage example.

You'll have to store in a `SocketInfo::SocketParamsList` one or multiple `SocketInfo::SocketParams` instances that'll serve building your sockets via the `Sockets` class.

___Example:___
```
// Build the parameters list. It's a std::vector<struct SocketParams>.

SocketInfo::SocketParamsList    parameters;

// Add options for one Socket to the list. Here we build a IPv4 Socket on address 127.0.0.1 and port 8080.

parameters.push_back(SocketInfo::SocketParams(AF_INET, SOCK_STREAM, IPPROTO_TCP, "127.0.0.1", "8080"));
```
