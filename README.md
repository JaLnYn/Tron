# Tron
Tasks to be done:

Server:
- Game loop and Client communication loop timings [Complete]
- Game info encoding [Complete]
- Initial connection with client [Complete]
- Set up the timer (forked) for the server [Complete]
- Redoing the gameloop with epoll [Working..]


Client:
- Simple ncureses window [Complete]
- Reading and translating data sent from the client [Complete]
- Initial connection with client [Complete]
- Client Game Loop [TODO]
- Reading userinput [TODO]
- Sending userinput [TODO]

I while trying to figure out the game loop I came across a solution with epoll which is much better. Now I will rewrite ThClient and server using this solution
