# Program 1 – Sample Execution Output

> **Setup:** Create a test file `hello.txt` in the same directory as the server binary.
>
> ```bash
> echo "Hello from server file!" > hello.txt
> ```

---

## Server Terminal

```
Socket created successfully
Binding successful
Waiting for client...
Client connected: 127.0.0.1
Requested file: hello.txt
```

---

## Client Terminal

```
Socket created
Connected to server
Enter the filename: hello.txt

--- File Contents ---
Hello from server file!
--- End of File ---
```

---

## Notes

- The server exits after serving one client (no loop). Restart it for a new connection.
- If the file does not exist on the server, `open()` fails and the server exits with an error.
- Port used: **15001** (TCP).
