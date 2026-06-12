# Program 8 – RSA Encryption & Diffie-Hellman Key Exchange

Two cryptography programs demonstrating public-key encryption (RSA) and key-exchange (Diffie-Hellman).

---

## Files

| File | Role |
|------|------|
| `rsa.c` | RSA: generates key pair, encrypts and decrypts a string |
| `dh.c`  | Diffie-Hellman: Alice and Bob derive the same shared secret |

---

## 8A – RSA Encryption / Decryption

### Compile & Run

```bash
gcc rsa.c -o rsa
./rsa
```

### How It Works

1. Randomly picks two small primes `p` and `q`.
2. Computes `n = p*q`, `phi = (p-1)*(q-1)`.
3. Picks public exponent `e` such that `gcd(phi, e) = 1`.
4. Finds private key `d` such that `(d*e) mod phi = 1`.
5. Encrypts each character using `M^e mod n` and decrypts with `M^d mod n`.

> **Note:** Use short lowercase strings. `n` must exceed the ASCII value of every character.

### rsa.c

```c
/* Program 8A: RSA Encryption / Decryption */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long int gcd(long int a, long int b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

long int isprime(long int a)
{
    int i;
    if (a < 2) return 0;
    for (i = 2; i * i <= a; i++)
        if ((a % i) == 0) return 0;
    return 1;
}

long int mod_exp(long int base, long int exp, long int mod)
{
    long int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

long int encrypt(char ch, long int n, long int e)
{
    return mod_exp((long int)(unsigned char)ch, e, n);
}

char decrypt(long int ch, long int n, long int d)
{
    return (char)mod_exp(ch, d, n);
}

int main()
{
    long int i, len;
    long int p, q, n, phi, e, d, cipher[50];
    char text[50];

    srand((unsigned int)time(0));

    printf("Enter the text to be encrypted: ");
    scanf("%49s", text);
    len = strlen(text);

    do { p = rand() % 30; } while (!isprime(p));
    do { q = rand() % 30; } while (!isprime(q) || q == p);

    n   = p * q;
    phi = (p - 1) * (q - 1);

    do { e = rand() % phi; } while (e <= 1 || gcd(phi, e) != 1);
    do { d = rand() % phi; } while ((d * e) % phi != 1);

    printf("Two prime numbers (p and q): %ld and %ld\n", p, q);
    printf("n = %ld\n", n);
    printf("phi = %ld\n", phi);
    printf("Public key  (n, e): (%ld, %ld)\n", n, e);
    printf("Private key (n, d): (%ld, %ld)\n", n, d);

    printf("Encrypted message:\n");
    for (i = 0; i < len; i++) { cipher[i] = encrypt(text[i], n, e); printf("%ld ", cipher[i]); }

    printf("\nDecrypted message:\n");
    for (i = 0; i < len; i++) { text[i] = decrypt(cipher[i], n, d); printf("%c", text[i]); }
    printf("\n");
    return 0;
}
```

---

## 8B – Diffie-Hellman Key Exchange

### Compile & Run

```bash
gcc dh.c -o dh
./dh
```

### How It Works

1. Public parameters: prime `p` and primitive root `g`.
2. Alice picks private key `a`; computes public key `A = g^a mod p`.
3. Bob picks private key `b`; computes public key `B = g^b mod p`.
4. Alice's secret = `B^a mod p`; Bob's secret = `A^b mod p` — both are equal.

### dh.c

```c
/* Program 8B: Diffie-Hellman Key Exchange */

#include <stdio.h>

long long mod_exp(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main()
{
    long long p, g, a, b, A, B, keyA, keyB;

    printf("Enter the prime no (p)            : "); scanf("%lld", &p);
    printf("Enter the primitive root of p (g) : "); scanf("%lld", &g);
    printf("Enter Alice's private key (a)     : "); scanf("%lld", &a);
    printf("Enter Bob's   private key (b)     : "); scanf("%lld", &b);

    A    = mod_exp(g, a, p);
    B    = mod_exp(g, b, p);
    keyA = mod_exp(B, a, p);
    keyB = mod_exp(A, b, p);

    printf("\nAlice Public Key = %lld\n", A);
    printf("Bob   Public Key = %lld\n",   B);
    printf("\nAlice Secret Key = %lld\n", keyA);
    printf("Bob   Secret Key = %lld\n",   keyB);

    if (keyA == keyB)
        printf("\nShared secret established successfully: %lld\n", keyA);
    else
        printf("\nKey mismatch — check parameters.\n");

    return 0;
}
```
