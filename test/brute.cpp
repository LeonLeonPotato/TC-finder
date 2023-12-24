#include <iostream>
using namespace std;

const int MAXN = 1e5 + 10;

int N;
int A[MAXN];
int directions[MAXN];
int sumarr[MAXN];

inline int dist(int i, int x) {
    return min(abs(i - x), N - abs(i - x));
}

int main() {
    cin >> N;

    for (int i = 1; i <= N; i++) {
        cin >> A[i];
    }

    int ans = 1e9;

    for (int i = 1; i <= N; i++) {
        int s = 0;
        for (int j = 1; j <= N; j++) {
            s += dist(i, j) * A[j];
        }
        ans = min(s, ans);
    }

    cout << ans << endl;
}   