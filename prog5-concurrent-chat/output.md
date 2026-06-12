# Program 5 – Sample Execution Output

---

## Server Terminal

```
Socket created
Binding done
Server listening on port 15001...
New client connected from 127.0.0.1:54321
From client (port 54321): hello world
Reply: hi from server
From client (port 54321): how are you
Reply: fine, thanks
Client disconnected (port 54321)
```

---

## Client Terminal

```
Socket created
Connected
hello world
Server replied: hi from server
how are you
Server replied: fine, thanks
^D

EOF
```

> `^D` = Ctrl+D triggers EOF in the client loop.

---

## Notes

- The server uses `fork()` so each client is handled in a separate child process.
- The parent process continues accepting new clients immediately after forking.
- Port used: **15001** (TCP).
- To test concurrency, open two client terminals simultaneously — the server will handle both.
