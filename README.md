# Messenger
Allows to send text messages between two application instantions (server and client).
## Building
Compile src folder with ```-lncurses```.
```server.c``` and ```client.c``` both contain main().
Alternatively use provided exectuables (linux amd64).
## Usage
Run server. Run server with IPV4 address and host number (from server log).
### Example
On one machine:
```
./server
```
Then on another:
```
./client 127.0.0.1 27094
```
