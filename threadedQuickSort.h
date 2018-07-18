#ifndef THREADEDQUICKSORT_H
#define THREADEDQUICKSORT_H

#include "commonHeader.h"

template<typename T>
void quicksortRecursive( T *const arr, const long min, const long max, const short numThreads = 1 ) {
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
    quicksortRecursive( arr, min, pivotIndex - 1, 1 );
    quicksortRecursive( arr, pivotIndex + 1, max, 1 );
  }else {
    short numThreadsOnFirstHalf = 1;
    if( numThreads > 2 ){
      long problemSize1 = pivotIndex - min;
      long problemSize2 = max - pivotIndex;
      numThreadsOnFirstHalf = ((numThreads * problemSize1) + (problemSize2 >> 1)) / (problemSize2 + problemSize1);
      if( numThreadsOnFirstHalf <= 0 ) {
        numThreadsOnFirstHalf = 1;
      }else if( numThreadsOnFirstHalf >= numThreads - 1 ) {
        numThreadsOnFirstHalf = numThreads - 1;
      }
    }
    thread t1 (&quicksortRecursive<T>, arr, min, pivotIndex - 1, numThreadsOnFirstHalf );
    thread t2 (&quicksortRecursive<T>, arr, pivotIndex + 1, max, numThreads - numThreadsOnFirstHalf );
    t1.join();
    t2.join();
  }
}

template<typename T>
void myQuicksort( T *const arr, const long length, const short numThreads = 1 ) {
  quicksortRecursive( arr, 0, length - 1, numThreads );
}

#endif
