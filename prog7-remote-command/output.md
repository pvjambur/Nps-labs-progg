# Program 7 – Sample Execution Output

---

## Server Terminal

```
The socket is created successfully
Binding Successful
The server is listening on port 15001...
The client is connected: 127.0.0.1
Inside Child
Port: 54322 | Command: ls -l
Port: 54322 | Command: whoami
```

*(The commands `ls -l` and `whoami` also execute on the server machine.)*

---

## Client Terminal

```
Socket created
Connected to server 127.0.0.1:15001
ls -l
ls -l
whoami
whoami
^D

EOF
```

> The client receives the echoed command string (not the command output). The `system()` call runs on the server side and prints to the **server's** terminal.

---

## Notes

- The client receives an **echo** of each command it sent — not the command's output.
- The command executes on the **server's** terminal via `system()`.
- Port used: **15001** (TCP).
- The server uses `fork()` to handle multiple clients concurrently.
