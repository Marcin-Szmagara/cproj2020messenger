# Messenger
Allows to send text messages between two application instantions (server and client).
## Building
Compile src folder with ```-lncurses```.
```server.c``` and ```client.c``` both contain ```main()```.
You can use provided ```Makefile``` as well as ```makes``` and ```makec``` scripts.
Alternatively use provided exectuables (linux amd64).
## Usage
Run server. Run client with IPV4 address of the server and port number (from server log).
### Example
On one machine:
```
./server
```
Then on another:
```
./client 186.32.11.2 27094
```
