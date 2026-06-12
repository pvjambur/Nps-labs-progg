/* Program 8A: RSA Encryption / Decryption
   Randomly picks two primes p, q; generates key pair; encrypts and decrypts a string */

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
