#include <iostream>
using namespace std;

void binarySearch(int arr[], int size, int key) {
    int left =0;
    int right = size - 1;
    while(left <= right) {
        int mid = ((right-left)+left)/2;
        if(arr[mid] == key) {
            cout << "Found at index: " << mid << endl;
            return;
        } else if (arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    cout << "Not found" << endl;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(arr) / sizeof(arr[0]);
    int key = 7;
    binarySearch(arr, size, key);
    return 0;
}