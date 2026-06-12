# Program 3 – Sample Execution Output

---

## 3A – IP Checksum

**Input (sender – 10 header fields):**
```
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7
```

**Output:**
```
Computed Checksum at sender: b861
```

**Input (receiver – same 10 fields + checksum):**
```
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7 b861
```

**Output:**
```
Computed Checksum at receiver: 0000

No error in Data received
```

---

## 3B – Hamming Code (No Error)

**Input:**
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
```

**Output:**
```
The Data encoded (7 bits) is: 0110011
```

**Input:**
```
Enter received 7 bits (space-separated): 0 1 1 0 0 1 1
```

**Output:**
```
No error detected in received data.
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

---

## 3B – Hamming Code (With Error at bit 5)

**Input (same data):**
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
The Data encoded (7 bits) is: 0110011
```

**Input (bit 5 flipped: 1 → 0):**
```
Enter received 7 bits (space-separated): 0 1 1 0 1 1 1
```

**Output:**
```
Error detected at position 5. Correcting...
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

---

## Notes

- Hamming positions: 1, 2, 4 are parity bits; 3, 5, 6, 7 are data bits.
- Syndrome = 0 → no error; Syndrome = k → error at position k.
- Checksum uses 1's complement addition with carry wrap-around.
