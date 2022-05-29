### Installation libraries
```
sudo apt install libx11-dev
```

## Execution
### Server
```
g++ server.cpp -o server
./server
```
### Terminal 1
```
g++ window.cpp -o window -lX11
./window
```
### Terminal 2
```
g++ window.cpp -o window -lX11
./window
```
