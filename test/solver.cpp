#include <iostream>
#include <stack>
#include <queue>
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
        int first = dist(i, 1);
        int second = dist(i, 2);
        if (first == second) { // critical point exists at x = 0.5
            int third = dist(i, 3);
            directions[i] = third < first ? 1 : -1;
        } else {
            directions[i] = first < second ? 1 : -1;
        }

        int dirtracker = -directions[i];

        double pivot1 = i - (double) N / 2;
        double pivot3 = i + (double) N / 2;

        if (1 < pivot1 && pivot1 <= N) {
            if (N % 2 == 1) {
                sumarr[(int) pivot1] += dirtracker * A[i];
                sumarr[(int) pivot1 + 1] += dirtracker * A[i];
            } else {
                sumarr[(int) pivot1] += 2 * dirtracker * A[i];
            }
            dirtracker *= -1;
        }
        if (1 < i && i <= N) {
            sumarr[i] += 2 * dirtracker * A[i];
            dirtracker *= -1;
        }
        if (1 < pivot3 && pivot3 <= N) {
            if (N % 2 == 1) {
                sumarr[(int) pivot3] += dirtracker * A[i];
                sumarr[(int) pivot3 + 1] += dirtracker * A[i];
            } else {
                sumarr[(int) pivot3] += 2 * dirtracker * A[i];
            }
            dirtracker *= -1;
        }
    }

    int ans = 1e9;
    int cur = 0;

    for (int i = 1; i <= N; i++) {
        cur += dist(i, 1) * A[i];
        sumarr[0] += directions[i] * A[i];
    }

    for (int i = 1; i <= N; i++) {
        sumarr[i] = sumarr[i - 1] + sumarr[i];
    }

    sumarr[0] = cur;

    for (int i = 0; i < N; i++) {
        sumarr[i] = sumarr[i - 1] + sumarr[i];
        ans = min(ans, sumarr[i]);
    }

    cout << ans << endl;
}   