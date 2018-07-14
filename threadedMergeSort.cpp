#include <iostream>
#include <cstdlib>
#include <thread> // C++11
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

const long WEIGHT = 10000;
struct Heavy {
  char s[WEIGHT];
  unsigned long long comparable;
  Heavy(unsigned long long value) {
    comparable = value;
    for( long k = 0; k < WEIGHT; k++ ) {
      s[k] = 'a';
    }
  }
  Heavy(){
    *this = Heavy(0);
  }
  Heavy & operator=(const unsigned long long rhs);
  Heavy & operator=(const Heavy & rhs);
};

Heavy & Heavy::operator=(const unsigned long long rhs) {
  this->comparable = rhs;
  return *this;
}

Heavy & Heavy::operator=(const Heavy & rhs) {
  this->comparable = rhs.comparable;
  for( long k = 0; k < WEIGHT; k++ ) {
    this->s[k] = rhs.s[k];
  }
  return *this;
}

bool operator<(const Heavy & lhs, const Heavy & rhs) {
  return lhs.comparable < rhs.comparable;
}

int main() {
  short numThreads;
  long dataSize;
  cout << "# threads: ";
  cin >> numThreads;
  cout << "# data elements (warning: data elements are about " << WEIGHT << " bytes in size): ";
  cin >> dataSize;
  
  while(numThreads > 0 && dataSize > 0) {
    Heavy * bob = new Heavy[dataSize];
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
      if( bob[k+1] < bob[k] ) {
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
      cout << "# data elements (about " << WEIGHT << " bytes each) (0 to quit): ";
      cin >> dataSize;
    }
    
  }

  return 0;
}

