# Tron
Tasks to be done:

Server:
- Game loop and Client communication loop timings [Complete]
- Game info encoding [Complete]
- Initial connection with client [Work in progress]

Client:
- Simple ncureses window [Complete]
- Reading and translating data sent from the client [Work in progress]
- initial connection with client [Work in progress]

The current problem is that when I add the line: game *g = new game(); to the server, the server is no longer able to send data to the client. evernything works perfectly fine if I remove that line. I can't see the connection between creating a "game" object and the UDP connection. I can't seem to find a solution to this.
