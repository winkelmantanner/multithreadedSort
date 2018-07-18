#ifndef THREADEDMERGESORT_H
#define THREADEDMERGESORT_H

#include "commonHeader.h"

template<typename T>
void myMerge(const T *const source, T *const dest, const long min, const long mid, const long max) {
  long destIndex = min;
  long lowerIndex = min;
  long upperIndex = mid;
  while( destIndex <= max ) {
   if( upperIndex > max || (lowerIndex < mid && source[lowerIndex] < source[upperIndex]) ) {
     dest[destIndex] = source[lowerIndex];
     lowerIndex++;
   }else{
     dest[destIndex] = source[upperIndex];
     upperIndex++;
   }
   destIndex++;
  }
}

template<typename T>
void myMergesortRecursive(T * target, T * auxiliary, const long min, const long max, const short numThreads){
  if( min == max ) {
     return;
  }
  long mid = (max + min + 1) / 2;
  if( numThreads >= 2 ) {
    thread f1 (&myMergesortRecursive<T>, auxiliary, target, min, mid - 1, numThreads / 2);
    thread f2 (&myMergesortRecursive<T>, auxiliary, target, mid, max, (numThreads + 1) / 2);
    f1.join();
    f2.join();
  } else {
    myMergesortRecursive(auxiliary, target, min, mid-1, 1);
    myMergesortRecursive(auxiliary, target, mid, max, 1);
  }
  myMerge(auxiliary, target, min, mid, max);
}

template<typename T>
void myMergesort(T * target, const long length, const short numThreads = 1 ) {
  T * auxiliary = new T[length];
  for( long k = 0; k < length; k++ ) {
    auxiliary[k] = target[k];
  }
  
  myMergesortRecursive(target,auxiliary,0,length-1,numThreads);

  delete [] auxiliary;
}

#endif

