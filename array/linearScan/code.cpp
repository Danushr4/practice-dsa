#include <iostream>
using namespace std;

void Single_traversal(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void Reverse_traversal(int arr[], int size) {
    for (int i = size - 1; i >= 0; i--) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void Running_state(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    cout << "Sum: " << sum << endl;
}

void Simultaneous_tracking(int arr[], int size) {
    int maxVal = arr[0];
    int minVal = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }

        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }

    cout << "Max: " << maxVal
         << " Min: " << minVal << endl;
}

void Counting(int arr[], int size) {
    int evenCount = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i] % 2 == 0) {
            evenCount++;
        }
    }

    cout << "Even Count: " << evenCount << endl;
}

void LinearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            cout << "Found at index: " << i << endl;
            return;
        }
    }

    cout << "Not found" << endl;
}

void TrackIndex(int arr[], int size) {
    int maxIdx = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxIdx]) {
            maxIdx = i;
        }
    }

    cout << "Max value at index: " << maxIdx << endl;
}

void CheckSorted(int arr[], int size) {
    bool sorted = true;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            sorted = false;
            break;
        }
    }

    cout << (sorted ? "Sorted" : "Not Sorted") << endl;
}

void RunningBest(int arr[], int size) {
    int best = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] > best) {
            best = arr[i];
        }
    }

    cout << "Best so far: " << best << endl;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    Single_traversal(arr, size);
    Reverse_traversal(arr, size);
    Running_state(arr, size);
    Simultaneous_tracking(arr, size);
    Counting(arr, size);
    LinearSearch(arr, size, 3);
    TrackIndex(arr, size);
    CheckSorted(arr, size);
    RunningBest(arr, size);

    return 0;
}
