#include <iostream>
#include <cstdlib>
#include <future>
using namespace std;

const long SIZE = 32767000;

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
    future<void> f1 = async(&myMergesortRecursive<T>, auxiliary, target, min, mid - 1, numThreads / 2);
    future<void> f2 = async(&myMergesortRecursive<T>, auxiliary, target, mid, max, (numThreads + 1) / 2);
    f1.get();
    f2.get();
  } else {
    myMergesortRecursive(auxiliary, target, min, mid-1, 1);
    myMergesortRecursive(auxiliary, target, mid, max, 1);
  }
  myMerge(auxiliary, target, min, mid, max);
}

template<typename T>
int myMergesort(T * target, const long length, const short numThreads = 1 ) {
  T * auxiliary = new T[length];
  for( long k = 0; k < length; k++ ) {
    auxiliary[k] = target[k];
  }
  
  myMergesortRecursive(target,auxiliary,0,length-1,numThreads);

  delete [] auxiliary;
  return 0;
}



int main() {
  unsigned long long * bob = new unsigned long long[SIZE];
  bool failure = false;
  for( long j = 0; j < SIZE; j++ ) {
    bob[j] = rand() % SIZE;
  }
  cout << endl;
  
  myMergesort( bob, SIZE, 4);
  
  for( long k = 0; k < SIZE - 1; k++ ) {
    if( bob[k] > bob[k+1] ) {
      failure = true;
      break;
    }
  }
  cout << "failure:" << (failure?"true":"false") << endl;
  cout << endl;
  delete [] bob;
  return 0;
}

