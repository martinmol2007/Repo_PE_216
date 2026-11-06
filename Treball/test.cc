#include <iostream>
using namespace std;

const long long MOD = 1000000007;

// Function returns {F(n), F(n + 1)}
pair<long long, long long> fastDoubling(long long n)
{
    // Base Case
    if (n == 0)
        return {0, 1};

    // Recursively find F(k) and F(k + 1)
    auto p = fastDoubling(n / 2);

    long long a = p.first;
    long long b = p.second;

    // Compute F(2k)
    long long c = (1LL * a * ((2LL * b % MOD - a + MOD) % MOD)) % MOD;

    // Compute F(2k + 1)
    long long d = (1LL * a * a + 1LL * b * b) % MOD;

    // If n is even
    if (n % 2 == 0)
        return {c, d};

    // If n is odd
    return {d, (c + d) % MOD};
}

long long nthFibonacci(long long n)
{
    // Return the n-th Fibonacci number
    return fastDoubling(n).first;
}

int main()
{
    long long n;
    cin >> n;
    cout << "F(" << n << ") = " << nthFibonacci(n) << "\n";
    return 0;
}