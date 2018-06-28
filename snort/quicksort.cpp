#include "stdafx.h"

template<typename T>
void quick_sort(T a[], int low, int high)
{
    int i = low;
    int j = high;
    T temp = a[low];

    while (i < j)
    {
        while (i < j && temp < a[j])j--;
        if (i < j)a[i++] = a[j];

        while (i < j && temp > a[i])i++;
        if (i < j)a[j--] = a[i];
    }
    a[i] = temp;

    if (low < i)quick_sort(a, low, i - 1);
    if (i < high)quick_sort(a, j + 1, high);
}

int main(){
    int arr[10] = {8,3,4,5,6,1,2,7,9,0};
    quick_sort(arr, 10);
    return 0;
}

