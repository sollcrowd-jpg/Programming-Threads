#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

// Matrices
int matA[MAX][MAX]; 
int matB[MAX][MAX]; 
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// Struct to pass cell coordinates to threads
typedef struct {
    int row;
    int col;
} ThreadData;

// Function to fill a matrix with random values
void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

// Function to print a matrix
void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Thread function to compute sum of two matrices
void* computeSum(void* args) {
    ThreadData *data = (ThreadData*) args;
    int row = data->row;
    int col = data->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(data);  // Free allocated memory
    pthread_exit(0);
}

// Thread function to compute difference of two matrices
void* computeDiff(void* args) {
    ThreadData *data = (ThreadData*) args;
    int row = data->row;
    int col = data->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(data);  // Free allocated memory
    pthread_exit(0);
}

// Thread function to compute dot product of two matrices
void* computeProduct(void* args) {
    ThreadData *data = (ThreadData*) args;
    int row = data->row;
    int col = data->col;
    matProductResult[row][col] = 0;
    for (int k = 0; k < MAX; k++) {
        matProductResult[row][col] += matA[row][k] * matB[k][col];
    }
    free(data);  // Free allocated memory
    pthread_exit(0);
}

int main() {
    srand(time(0));  // Seed for random number generation

    // Fill matrices A and B with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    pthread_t threads[NUM_THREADS];
    int thread_count = 0;

    // Matrix Sum
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadData *data = (ThreadData*) malloc(sizeof(ThreadData));
            data->row = i;
            data->col = j;
            pthread_create(&threads[thread_count++], NULL, computeSum, data);
            if (thread_count == NUM_THREADS) {
                for (int t = 0; t < NUM_THREADS; t++) {
                    pthread_join(threads[t], NULL);
                }
                thread_count = 0;
            }
        }
    }

    // Matrix Difference
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadData *data = (ThreadData*) malloc(sizeof(ThreadData));
            data->row = i;
            data->col = j;
            pthread_create(&threads[thread_count++], NULL, computeDiff, data);
            if (thread_count == NUM_THREADS) {
                for (int t = 0; t < NUM_THREADS; t++) {
                    pthread_join(threads[t], NULL);
                }
                thread_count = 0;
            }
        }
    }

    // Matrix Product
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadData *data = (ThreadData*) malloc(sizeof(ThreadData));
            data->row = i;
            data->col = j;
            pthread_create(&threads[thread_count++], NULL, computeProduct, data);
            if (thread_count == NUM_THREADS) {
                for (int t = 0; t < NUM_THREADS; t++) {
                    pthread_join(threads[t], NULL);
                }
                thread_count = 0;
            }
        }
    }

    // Print the results
    printf("Sum of matrices:\n");
    printMatrix(matSumResult);
    printf("Difference of matrices:\n");
    printMatrix(matDiffResult);
    printf("Product of matrices:\n");
    printMatrix(matProductResult);

    return 0;
}
