/* Program 8B: Diffie-Hellman Key Exchange
   Alice and Bob derive the same shared secret using public parameters p and g */

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

    A    = mod_exp(g, a, p);   /* Alice's public key */
    B    = mod_exp(g, b, p);   /* Bob's   public key */
    keyA = mod_exp(B, a, p);   /* Alice derives shared secret */
    keyB = mod_exp(A, b, p);   /* Bob   derives shared secret */

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
