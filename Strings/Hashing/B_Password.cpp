// topics: hashing+binary_search
// Problem: https://codeforces.com/contest/126/problem/B
// time complexity: O(n*log(n))
// space complexity: O(n)

// Problem: Given a string s, find the longest substring that appears both at the beginning, middle and at the end of the string. 
// If there is no such substring, print "Just a legend". Otherwise, print the substring.

// idea: we can get the hash of any substring in a given string, from prefix hash using polynomial rolling hash function in O(1) time.
// we can binary search the length of the substring that appears at the beginning, middle and at the end of the string.
// if we find a valid length, we can check if there is a substring that appears in the middle of the string.


// employed double hashing to avoid hash collision.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DoubleHash {
    ll MOD1, MOD2, BASE;
    vector<ll> prefix_hash1, prefix_hash2;
    vector<ll> power1, power2;

    DoubleHash(const string &s, ll base, ll mod1, ll mod2) // constructor of struct DoubleHash
        : MOD1(mod1), MOD2(mod2), BASE(base) {
        int n = s.size();
        prefix_hash1.resize(n+1, 0);
        prefix_hash2.resize(n+1, 0);
        power1.resize(n+1, 1);
        power2.resize(n+1, 1);

        for (int i = 0; i < n; ++i) {
            prefix_hash1[i+1] = (prefix_hash1[i] * BASE + s[i]) % MOD1;
            prefix_hash2[i+1] = (prefix_hash2[i] * BASE + s[i]) % MOD2;

            power1[i+1] = (power1[i] * BASE) % MOD1;
            power2[i+1] = (power2[i] * BASE) % MOD2;
        }
    }

    pair<ll, ll> get_hash(int start, int len) {
        ll part1 = (prefix_hash1[start] * power1[len]) % MOD1;
        ll hash1 = (prefix_hash1[start + len] - part1) % MOD1;
        if (hash1 < 0) hash1 += MOD1;

        ll part2 = (prefix_hash2[start] * power2[len]) % MOD2;
        ll hash2 = (prefix_hash2[start + len] - part2) % MOD2;
        if (hash2 < 0) hash2 += MOD2;

        return {hash1, hash2};
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    if (n == 0) {
        cout << "Just a legend\n";
        return 0;
    }

    ll base = 911382629;
    ll mod1 = 1e9 + 7;
    ll mod2 = 1e9 + 9;

    DoubleHash dh(s, base, mod1, mod2);

    vector<int> valid_l;
    for (int l = 1; l < n; ++l) {
        auto prefix_hash = dh.get_hash(0, l);
        auto suffix_hash = dh.get_hash(n - l, l);
        if (prefix_hash == suffix_hash) {
            valid_l.push_back(l);
        }
    }

    if (valid_l.empty()) {
        cout << "Just a legend\n";
        return 0;
    }

    sort(valid_l.begin(), valid_l.end());

    int left = 0, right = valid_l.size() - 1;
    int best_l = -1;

    while (left <= right) {
        int mid = (left + right) / 2;
        int l = valid_l[mid];

        auto target = dh.get_hash(0, l);

        bool found = false;
        int max_i = n - l - 1;

        for (int i = 1; i <= max_i; ++i) {
            auto current = dh.get_hash(i, l);
            if (current == target) {
                found = true;
                break;
            }
        }

        if (found) {
            best_l = l;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (best_l != -1) {
        cout << s.substr(0, best_l) << '\n';
    } else {
        cout << "Just a legend\n";
    }

    return 0;
}

