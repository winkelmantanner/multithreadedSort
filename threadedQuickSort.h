#ifndef THREADEDQUICKSORT_H
#define THREADEDQUICKSORT_H

#include "commonHeader.h"

template<typename T>
void quickSortRecursive( T *const arr, const long min, const long max, const short numThreads = 1 ) {
  if( max <= min ) {
    return;
  }
  long high = max;
  long low = min;
  long pivotIndex = (max + min + 1) / 2;
  T pivotValue = arr[pivotIndex];
  while( high > low ) {
    if( high > pivotIndex ) {
      if( arr[high] < pivotValue ) {
        arr[pivotIndex] = arr[high];
        pivotIndex++;
        arr[high] = arr[pivotIndex];
      }else{
        high--;
      }
    }
    if( low < pivotIndex ) {
      if( pivotValue < arr[low] ) {
        arr[pivotIndex] = arr[low];
        pivotIndex--;
        arr[low] = arr[pivotIndex];
      }else{
        low++;
      }
    } 
  }
  arr[pivotIndex] = pivotValue;

  if( numThreads == 1 ){
    quickSortRecursive( arr, min, pivotIndex - 1, 1 );
    quickSortRecursive( arr, pivotIndex + 1, max, 1 );
  }else if( numThreads == 2 ) {
    thread f1 (&quickSortRecursive<T>, arr, min, pivotIndex - 1, 1 );
    thread f2 (&quickSortRecursive<T>, arr, pivotIndex + 1, max, 1 );
    f1.join();
    f2.join();
  }else{ // numThreads >= 3
    long problemSize1 = pivotIndex - min;
    long problemSize2 = max - pivotIndex;
    short numThreadsOnFirstHalf = ((numThreads * problemSize1) + (problemSize2 >> 1)) / (problemSize2 + problemSize1);
    
    if( numThreadsOnFirstHalf <= 0 ) {
      numThreadsOnFirstHalf = 1;
    }else if( numThreadsOnFirstHalf >= numThreads - 1 ) {
      numThreadsOnFirstHalf = numThreads - 1;
    }
    // I found that combining this with the threading above makes the program much slower
    thread t1 (&quickSortRecursive<T>, arr, min, pivotIndex - 1, numThreadsOnFirstHalf );
    thread t2 (&quickSortRecursive<T>, arr, pivotIndex + 1, max, numThreads - numThreadsOnFirstHalf );
    t1.join();
    t2.join();
  }
}

template<typename T>
void myQuicksort( T *const arr, const long length, const short numThreads = 1 ) {
  quickSortRecursive( arr, 0, length - 1, numThreads );
}

#endif
