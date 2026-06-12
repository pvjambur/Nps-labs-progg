# Program 4 – Sample Execution Output

---

## Sender Terminal (server)

```
Sending multicast messages to 225.0.0.37:12345 ...
Sent: RVCE-CSE
Sent: RVCE-CSE
Sent: RVCE-CSE
...
```
*(One line per second until Ctrl+C)*

---

## Receiver Terminal (client)

```
Listening for multicast on 225.0.0.37:12345 ...
Received from 127.0.0.1: RVCE-CSE
Received from 127.0.0.1: RVCE-CSE
Received from 127.0.0.1: RVCE-CSE
...
```
*(One message per second until Ctrl+C)*

---

## Notes

- Start the **receiver before** the sender so it doesn't miss early messages.
- Multiple receivers can run simultaneously — all receive the same multicast stream.
- Multicast address range: `224.0.0.0` to `239.255.255.255` (Class D).
- The `IP_ADD_MEMBERSHIP` socket option tells the kernel to join the multicast group.
