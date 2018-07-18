#include "threadedMergesort.h"
#include "threadedQuicksort.h"

void getInput( short & alg, short & numThreads, long & dataSize ) {
  cout << "select sorting algorithm (0-quit 1-quicksort 2-mergesort): ";
  cin >> alg;
  if( alg > 0 ) {
    cout << "# threads (0 to quit): ";
    cin >> numThreads;
    if( numThreads > 0 ) {
      cout << "# data elements (0 to quit): ";
      cin >> dataSize;
    }
  }
}

int main() {
  short numThreads;
  long dataSize;
  short alg;
  getInput( alg, numThreads, dataSize );
  
  while(numThreads > 0 && dataSize > 0 && alg > 0) {
    long * bob = new long[dataSize];
    bool failure = false;
    for( long j = 0; j < dataSize; j++ ) {
      bob[j] = rand() % dataSize;
    }
    cout << endl;
    
    chrono::time_point<std::chrono::system_clock> start, end;
    
    start = chrono::system_clock::now();
    switch(alg){
      case 1:
        myQuicksort( bob, dataSize, numThreads ) ;
        break;
      case 2:
        myMergesort( bob, dataSize, numThreads);
        break;
    }
    end = chrono::system_clock::now();
    
    chrono::duration<double> elapsed_seconds = end - start;
    cout << elapsed_seconds.count() << " seconds" << endl;
    
    for( long k = 0; k < dataSize - 1; k++ ) {
      if( bob[k+1] < bob[k] ) {
        failure = true;
        break;
      }
    }
    cout << (failure ? "test failed" : "test passed") << endl;
    cout << endl;
    delete [] bob;
    
    getInput( alg, numThreads, dataSize );
  }

  return 0;
}


