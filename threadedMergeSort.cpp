#include <iostream>
#include <cstdlib>
#include <future> // C++11
#include <chrono> // C++11
using namespace std;

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
  short numThreads;
  long dataSize;
  cout << "# threads: ";
  cin >> numThreads;
  cout << "# data elements: ";
  cin >> dataSize;
  
  while(numThreads > 0 && dataSize > 0) {
    unsigned long long * bob = new unsigned long long[dataSize];
    bool failure = false;
    for( long j = 0; j < dataSize; j++ ) {
      bob[j] = rand() % dataSize;
    }
    cout << endl;
    
    chrono::time_point<std::chrono::system_clock> start, end;
    
    start = chrono::system_clock::now();
    myMergesort( bob, dataSize, numThreads);
    end = chrono::system_clock::now();
    
    chrono::duration<double> elapsed_seconds = end - start;
    cout << elapsed_seconds.count() << " seconds" << endl;
    
    for( long k = 0; k < dataSize - 1; k++ ) {
      if( bob[k] > bob[k+1] ) {
        failure = true;
        break;
      }
    }
    
    cout << (failure?"test failed":"test passed") << endl;
    cout << endl;
    delete [] bob;

    cout << "# threads (0 to quit): ";
    cin >> numThreads;
    if( numThreads > 0 ) {
      cout << "# data elements (0 to quit): ";
      cin >> dataSize;
    }
    
  }

  return 0;
}

