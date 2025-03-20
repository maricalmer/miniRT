// CHAT GPT ==> change that !!!

#include "minirt.h"

// Partition function for QuickSelect
int partition(float arr[], int left, int right, int pivotIndex) {
    float pivotValue = arr[pivotIndex];
    ft_swap(&arr[pivotIndex], &arr[right]);  // Move pivot to end
    int storeIndex = left;
    
    for (int i = left; i < right; i++) {
        if (arr[i] < pivotValue) {
            ft_swap(&arr[i], &arr[storeIndex]);
            storeIndex++;
        }
    }
    ft_swap(&arr[storeIndex], &arr[right]);  // Move pivot to its final place
    return storeIndex;
}

// QuickSelect function to find k-th smallest element
float quickSelect(float arr[], int left, int right, int k) {
    if (left == right)
        return arr[left];
    
    int pivotIndex = left + rand() % (right - left + 1);
    pivotIndex = partition(arr, left, right, pivotIndex);
    
    if (k == pivotIndex)
        return arr[k];
    else if (k < pivotIndex)
        return quickSelect(arr, left, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, right, k);
}

// Function to find the median
float findMedian(float arr[], int n) {
    if (n % 2 == 1)
        return quickSelect(arr, 0, n - 1, n / 2);  // Odd case
    else {
        float leftMid = quickSelect(arr, 0, n - 1, n / 2 - 1);
        float rightMid = quickSelect(arr, 0, n - 1, n / 2);
        return (leftMid + rightMid) / 2.0f;  // Even case
    }
}