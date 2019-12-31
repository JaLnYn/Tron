# Tron
Tasks to be done:

Server:
- Game loop and Client communication loop timings [COMPLETE]
- Game info encoding [COMPLETE]
- Initial connection with client [COMPLETE]
- Set up the timer (forked) for the server [COMPLETE]
- Redoing the gameloop with epoll [COMPLETE]
- Work on reading the input [COMPLETE]



Client:
- Simple ncureses window [COMPLETE]
- Reading and translating data sent from the client [COMPLETE]
- Initial connection with client [COMPLETE]
- Client Game Loop [COMPLETE]
- Reading userinput [COMPLETE]
- Sending userinput to server [COMPLETE]

Tron is a recreation of the classic Tron game for 4 players.
The server is built in c++.
This client is also built in c++ and uses the ncurses library.

The game:
In this game you are an 'x' character and every tick you must move in one direction (up, down, left, right). Your x character leaves a trail everywhere you go. If you run into another person or a trail of any character (including your own) you lose. The last person alive wins.

The server:
The server splits into two programs (using fork). The game is real time so the child program is a timer that sends a message through to the parent (through a pipe) every 0.125 seconds. The parent uses epoll to monitor the socket and the pipe. The program will read from the sockfd or to "tick" the game when something is recieved in the pipe.

The client:
The client uses the ncurses to draw everything out. This program uses epoll to monitor the keyboard and the socket. If anything is recieved from the socket the recieved message is used to load the game. If a key is pressed a command is sent to the server.
