#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std; 
  
// maximum size of matrix 
#define MAX 2000 
  
// maximum number of threads 
#define MAX_THREAD 2


double get_time()
{
  struct timeval timeval_time;
  gettimeofday(&timeval_time,NULL);
  return (double)timeval_time.tv_sec + (double)timeval_time.tv_usec*1e-6;
}

// get RAM structure
struct rusage r_usage;
  
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int step_i = 0;

void* multi(void* arg)
{
    int core = step_i++;

    // Each thread computes 1/MAX_THREADth of matrix multiplication
    for (int i = core * MAX / MAX_THREAD; i < (core + 1) * MAX / MAX_THREAD; i++)
        for (int j = 0; j < MAX; j++)
            for (int k = 0; k < MAX; k++)
                matC[i][j] += matA[i][k] * matB[k][j];
}

// Driver Code
int main()
{
    // Generating random values in matA and matB
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    // Displaying matA
    cout << endl
         << "Matrix A" << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            cout << matA[i][j] << " ";
        cout << endl;
    }

    // Displaying matB
    cout << endl
         << "Matrix B" << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            cout << matB[i][j] << " ";
        cout << endl;
    }

    //r_usage.ru_maxrss = 0.0; //set memory to zero

    // declaring four threads
    pthread_t threads[MAX_THREAD];

    //time the matrix-multiplication kernel
    double t = get_time();



    // Creating four threads, each evaluating its own part
    for (int i = 0; i < MAX_THREAD; i++) {
        int* p;
        pthread_create(&threads[i], NULL, multi, (void*)(p));
    }

    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);
    

    printf("\nElapsed Time: [%g sec] \n",get_time()-t);
    // get memory usage
    getrusage(RUSAGE_SELF, &r_usage);
    printf("\nMemory Usage: [%lf MB] \n",r_usage.ru_maxrss*0.001);


    // Displaying the result matrix
    cout << endl
         << "Multiplication of A and B" << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            cout << matC[i][j] << " ";
        cout << endl;
    }
    return 0;
}
