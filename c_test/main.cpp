#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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
        cerr << "Error oppening!" << endl;
        return;
    }

    for (int value : array) {
        file << value << " ";
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

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high);

        quicksort(arr, low, pivot_index);
        quicksort(arr, pivot_index + 1, high);
    }
}


int main() {
    vector<int> loaded_array = read_array_from_file("text.txt");

    if (loaded_array.empty()) {
        cerr << "File is empty or could not be read." << endl;
        return 1;
    }

    quicksort(loaded_array, 0, loaded_array.size() - 1);
    save_array_to_file(loaded_array, "c_test/out.txt");
    return 0;
}