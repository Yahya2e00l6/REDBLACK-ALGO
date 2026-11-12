#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

void runBenchmark(int N, const string& label, const vector<int>& dataset) {
    map<int, int> mp;

    auto t_start_insert = chrono::high_resolution_clock::now();
    for (int x : dataset) mp.insert({x, 1});
    auto t_end_insert = chrono::high_resolution_clock::now();

    int testSamples = min(N, 1000);
    auto t_start_search = chrono::high_resolution_clock::now();
    for (int i = 0; i < testSamples; ++i) mp.find(dataset[i]);
    auto t_end_search = chrono::high_resolution_clock::now();

    auto t_start_delete = chrono::high_resolution_clock::now();
    for (int i = 0; i < testSamples; ++i) mp.erase(dataset[i]);
    auto t_end_delete = chrono::high_resolution_clock::now();

    auto t_start_vider = chrono::high_resolution_clock::now();
    mp.clear();
    auto t_end_vider = chrono::high_resolution_clock::now();

    double insert_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_insert - t_start_insert).count() / 1e6;
    double search_ns = (double)chrono::duration_cast<chrono::nanoseconds>(t_end_search - t_start_search).count() / testSamples;
    double delete_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_delete - t_start_delete).count() / 1e6;
    double vider_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_vider - t_start_vider).count() / 1e6;

    cout << "N = " << N << " | " << label << endl;
    cout << "  Insert time: " << insert_ms << " ms" << endl;
    cout << "  Search latency: " << search_ns << " ns/item" << endl;
    cout << "  Delete time (" << testSamples << " items): " << delete_ms << " ms" << endl;
    cout << "  Vider time: " << vider_ms << " ms" << endl << endl;
}

int main() {
    vector<int> sizes = {1000, 100000, 1000000};
    mt19937 rng(42);

    for (int N : sizes) {
        vector<int> sortedData(N);
        for (int i = 0; i < N; ++i) sortedData[i] = i;

        vector<int> almostSortedData = sortedData;
        int swaps = max(1, (int)(N * 0.05));
        for (int i = 0; i < swaps; ++i) {
            int a = rng() % N;
            int b = rng() % N;
            swap(almostSortedData[a], almostSortedData[b]);
        }

        vector<int> randomData = sortedData;
        shuffle(randomData.begin(), randomData.end(), rng);

        runBenchmark(N, "Sorted", sortedData);
        runBenchmark(N, "Almost Sorted", almostSortedData);
        runBenchmark(N, "Random", randomData);
        cout << "--------------------------------------------------------" << endl;
    }

    return 0;
}
