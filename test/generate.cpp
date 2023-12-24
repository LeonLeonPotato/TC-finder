#include <iostream>
#include <random>
using namespace std;

const int MAXN = 100;
const int MAXAi = 100;

thread_local mt19937 generator(random_device{}());

int main() {
    uniform_int_distribution<int> n_chooser(1, MAXN);
    uniform_int_distribution<int> ai_chooser(1, MAXAi);

    int N = n_chooser(generator);

    cout << N << endl;
    for (int i = 0; i < N; i++) {
        cout << ai_chooser(generator) << ' ';
    }
    cout << endl;
}