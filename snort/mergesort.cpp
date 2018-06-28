
#include "stdafx.h"

template<typename T>
void merge_do(T a[], int n, T swap[], int k){
   int m = 0 ;
   int i = 0, j = 0;
   int u1 = 0, l1 = 0;
   int u2 = 0, l2 = 0;
   while (l1 + k < n)
   {
       l2 = l1 + k;
       u1 = l2 - 1;
       u2 = (l2 + k - 1 < n) ? l2 + k - 1 : n - 1;

       for (i = l1, j = l2; i <= u1 && j <= u2; m++)
           swap[m] = a[i] <= a[j] ? a[i++] : a[j++];
       while (i <= u1)swap[m++] = a[i++];
       while (j <= u2)swap[m++] = a[j++];

       l1 = u2 + 1;
   }
   for (i = l1; i < n; i++, m++)swap[m] = a[i];
}

template<typename T>
void merge_snort(T a[], int n){
    int k = 1;
    T * swap = new T[n];
    while (k < n)
    {
        merge_do(a, n, swap, k);
        for (int i = 0; i < n; i++)a[i] = swap[i];
        k *= 2;
    }
    delete swap;
    swap = nullptr;
}

int main(){
    int arr[10] = {8,3,4,5,6,1,2,7,9,0};
    merge_snort(arr, 10);
    return 0;
}




