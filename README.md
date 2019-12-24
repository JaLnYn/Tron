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

I while trying to figure out the game loop I came across a solution with epoll which is much better. Now I will rewrite the Client and server using this solution
