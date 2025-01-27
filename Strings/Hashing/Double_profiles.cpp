#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    mt19937_64 rng(42);
    vector<ull> hash_val(n + 1);
    for (int i = 1; i <= n; ++i) {
        hash_val[i] = rng();
    }

    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    edges.reserve(m);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }

    vector<ull> sum_h(n + 1);
    for (int i = 1; i <= n; ++i) {
        ull s = 0;
        for (int neighbor : adj[i]) {
            s += hash_val[neighbor];
        }
        sum_h[i] = s; // each profile hash is the sum of the hash values of its neighbors
    }
    
    // to make sum_h zero based index
    vector<ull> sh(n);
    for (int i = 1; i <= n; ++i) {
        sh[i-1] = sum_h[i];
    }
    
    // sort the sum_h values to count the number of groups
    // ex: [1,2,2,3,3,3,4,4,4,4] => 4 groups	
    sort(sh.begin(), sh.end());
    
    // count the number of groups 
    long long total_group_pairs = 0;
    int cnt = 1; // cnt is 1 because we have at least one group
    for (int i = 1; i < n; ++i) {
        if (sh[i] == sh[i-1]) {
            cnt++;
        } else {
            total_group_pairs += (long long) cnt * (cnt - 1) / 2; // in each group of k, we can choose kC2=k(k-1)/2  pairs, here k is cnt
            cnt = 1;
        }
    }
    total_group_pairs += (long long) cnt * (cnt - 1) / 2; // for the last group, because we don't have a chance to add it in the loop
    
    // count the number of friend pairs in groups
    long long friend_pairs_in_groups = 0;
    for (auto& e : edges) {
        int u = e.first, v = e.second;
        if (sum_h[u] == sum_h[v]) {
            friend_pairs_in_groups++;
        }
    }
    // we need to subtract the friend pairs in groups from the total group pairs, because there is seperate condition to check them 
    long long group_pairs = total_group_pairs - friend_pairs_in_groups;
    // seperate condition: if selected ordered pair are frds with each other their freind set is 
   
   /*
    ex: let i and j are friends
    
    Friend list of i: {j, A, B, C}

    Friend list of j: {i, A, B, C}

    All other profiles (A, B, C) are friends with both i and j.

    Are i and j doubles?
    ✅ Yes!
    Even though i and j are friends, their friend lists are identical except for the inclusion of each other.
    For every k ≠ i, j (i.e., A, B, C), k is friends with both i and j.

    Except for the inclusion of each other: this is the key point to check the condition

    so subrtacting the hash of i and j from the sum of hash of i and j, such that now sum_hash(i)-hash_val(j) is just hash of (A,B,C)
    and sum_hash(j)-hash_val(i) is just hash of (A,B,C) 
    if both are equal then i and j are doubles

   */
    long long edge_pairs = 0;
    for (auto& e : edges) {
        int u = e.first, v = e.second;
        ull a = sum_h[u] - hash_val[v];
        ull b = sum_h[v] - hash_val[u];
        if (a == b) {
            edge_pairs++;
        }
    }

    cout << group_pairs + edge_pairs << '\n';

    return 0;
}

/*
Key Insight:

By representing a profile’s friends as a hash sum, we can compare friend sets in O(1) time. This reduces the problem to:

Grouping profiles by their friend set hash (O(n log n)).

Linear scans over edges (O(m)).

Final Complexity: O(n log n + m), which handles 1e6 profiles/edges efficiently.
*/