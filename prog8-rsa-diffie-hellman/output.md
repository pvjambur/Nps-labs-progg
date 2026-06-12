# Program 8 – Sample Execution Output

---

## 8A – RSA Encryption / Decryption

**Input:**
```
Enter the text to be encrypted: hello
```

**Output:**
```
Two prime numbers (p and q): 17 and 19
n = 323
phi = 288
Public key  (n, e): (323, 5)
Private key (n, d): (323, 173)
Encrypted message:
104 149 27 27 67
Decrypted message:
hello
```

> Keys are randomly chosen each run — output will differ. Use short lowercase strings; `n` must exceed the ASCII value of each character (e.g. 'z' = 122, so `n > 122`).

---

## 8B – Diffie-Hellman Key Exchange

**Input:**
```
Enter the prime no (p)            : 23
Enter the primitive root of p (g) : 5
Enter Alice's private key (a)     : 6
Enter Bob's   private key (b)     : 15
```

**Output:**
```
Alice Public Key = 8
Bob   Public Key = 19

Alice Secret Key = 2
Bob   Secret Key = 2

Shared secret established successfully: 2
```

> Both secret keys equal **2** — shared secret established correctly without ever transmitting private keys.

---

## Notes

**RSA**
- Works best with short strings. Larger primes give more secure n values.
- `mod_exp` uses fast exponentiation by squaring to avoid overflow.

**Diffie-Hellman**
- `p` should be a prime; `g` should be a primitive root of `p`.
- Classic example: `p = 23`, `g = 5`.
- The shared secret `keyA == keyB` is the verification of correct implementation.
