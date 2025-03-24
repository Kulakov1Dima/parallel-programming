#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tbb/parallel_invoke.h>
#include <tbb/global_control.h>

using namespace std;

vector<int> read_array_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return {};
    }
    vector<int> array;
    int value;
    while (file >> value) {
        array.push_back(value);
    }
    return array;
}

void save_array_to_file(const vector<int>& array, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening!" << endl;
        return;
    }

    for (int value : array) {
        file << value << "\n";
    }
    file.close();
}

int median_of_three(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    return arr[mid];
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = median_of_three(arr, low, high);
    int i = low - 1, j = high + 1;
    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

void parallel_quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high);

        tbb::parallel_invoke(
            [&] { parallel_quicksort(arr, low, pivot_index); },
            [&] { parallel_quicksort(arr, pivot_index + 1, high); }
        );
    }
}

int main() {
    tbb::global_control global_limit(tbb::global_control::max_allowed_parallelism, 3);  // Инициализация планировщика задач TBB

    vector<int> loaded_array = read_array_from_file("text.txt");

    if (loaded_array.empty()) {
        cerr << "File is empty or could not be read." << endl;
        return 1;
    }

    parallel_quicksort(loaded_array, 0, loaded_array.size() - 1);
    save_array_to_file(loaded_array, "c_task_bar/out.txt");
    return 0;
}