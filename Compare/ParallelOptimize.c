#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

int NUM_THREAD;
int MAX_SIZE;
int VERTEX_COUNT = 0;

//allocate adjancencyMatrix and initialize it
bool **adjacencyMatrix(){
    bool **Matrix = (bool **)malloc(sizeof(bool *) * MAX_SIZE);
    int i,j;
    #pragma omp parallel for private(i) num_threads(NUM_THREAD)
    for(i = 0; i < MAX_SIZE;++i){
        Matrix[i] = (bool *)malloc(sizeof(bool) * MAX_SIZE);
    }
    //init
    #pragma omp parallel for private(i,j) num_threads(NUM_THREAD)
    for (i = 0; i < MAX_SIZE;++i){
        for (j = i+1; j < MAX_SIZE;++j){
            Matrix[i][j] = 0;
        }
    }
    
    return Matrix;
}

void freeMatrix(bool **Matrix){
    int i;
    for(i = 0; i < MAX_SIZE;++i){
        free(Matrix[i]);
    }
    free(Matrix);
}

void readfile(bool **Matrix){
    int a, b;
    while(scanf("%d %d",&a, &b)!=EOF){
        VERTEX_COUNT = VERTEX_COUNT > b ? VERTEX_COUNT : b;
        Matrix[a][b] = 1;
        Matrix[b][a] = 1;
    }
    VERTEX_COUNT ++;
}

int *getDegree(bool **Matrix){
    int *degree = malloc(sizeof(int) * VERTEX_COUNT);
    int i,j;
    #pragma omp parallel for private(i) num_threads(NUM_THREAD)
    for (i = 0; i < VERTEX_COUNT;i++){
        degree[i] = 0;
    }
    // using row-wise access to be a cache friendly function
    for (i = 0; i < VERTEX_COUNT; ++i){
        for (j = i+1; j <VERTEX_COUNT; ++j){
            if (Matrix[i][j] == 1){
                degree[i]++;
                degree[j]++;
            }
        }
    }
    return degree;
}

bool **deleteVertex(bool **Matrix, int vertextodelete){
    int i;
    #pragma omp parallel for private(i) num_threads(NUM_THREAD)
    for (i = 0; i < VERTEX_COUNT;i++){
        Matrix[i][vertextodelete] = 0;
    }
    int j;
    #pragma omp parallel for private(j) num_threads(NUM_THREAD)
    for (j = 0; j < VERTEX_COUNT;j++){
        Matrix[vertextodelete][j] = 0;
    }
    return Matrix;
}


bool allzero(bool **matrix){
    for (int i = 0; i < VERTEX_COUNT;i++){
        for (int j = i+1; j < VERTEX_COUNT;j++){
            if(matrix[i][j]!=0){
                return false;
            }
        }
    }
    return true;
}


bool **k_core_degeneracy(int k, bool **Matrix){

    bool **resultMatrix = malloc(sizeof(bool *) * VERTEX_COUNT);
    int i, j;
    #pragma omp parallel for private(i) num_threads(NUM_THREAD)
    for (i = 0; i < VERTEX_COUNT;i++){
        resultMatrix[i] = malloc(sizeof(bool) * VERTEX_COUNT);
    }

    //copy the matrix
    #pragma omp parallel for private(i,j) num_threads(NUM_THREAD)
    for (i = 0; i < VERTEX_COUNT;i++){
        for (j = 0; j < VERTEX_COUNT;j++){
            resultMatrix[i][j] = Matrix[i][j];
        }
    }

    while(1){
        int *degree = getDegree(resultMatrix);

        int count = 0;
        int i;
        #pragma omp parallel for private(i) num_threads(NUM_THREAD)
        for (i = 0; i < VERTEX_COUNT;i++){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            break;
        }

        #pragma omp parallel for private(i) num_threads(NUM_THREAD)
        for (int i = 0; i <VERTEX_COUNT; i++){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i);
            }
        }
        free(degree);
    }
    return resultMatrix;
}

bool **sequential_degeneracy(int k, bool **resultMatrix){
    for(;;){
        int *degree = getDegree(resultMatrix);

        int count = 0;
        int i;
        #pragma omp parallel for private(i) num_threads(NUM_THREAD)
        for (i = 0; i < VERTEX_COUNT;++i){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            return resultMatrix;
        }

        #pragma omp parallel for private(i) num_threads(NUM_THREAD)
        for (int i = 0; i < VERTEX_COUNT; ++i){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i);
            }
        }
        free(degree);
    }
    return resultMatrix;
}

void outputformat(bool **matrix){
    for (int i = 0; i < VERTEX_COUNT;i++){
        for (int j = i+1; j < VERTEX_COUNT; j++){
            if(matrix[i][j]!=0){
                //printf("%d %d\n", i, j);
            }
        }
    }
}

void freeResultMatrix(bool **Matrix){
    int i;
    for (i = 0; i < VERTEX_COUNT;i++){
        free(Matrix[i]);
    }
    free(Matrix);
}

int main(int argc, char *argv[]){

    MAX_SIZE = atoi(*(argv + 1));
    NUM_THREAD = atoi(*(argv + 2));

    bool **Matrix = adjacencyMatrix();
    readfile(Matrix);
    bool **resultMatrix;
    resultMatrix = k_core_degeneracy(3, Matrix);
    for (int i = 4;;++i){
        resultMatrix = sequential_degeneracy(i, resultMatrix);
        if(allzero(resultMatrix)){
            freeResultMatrix(resultMatrix);
            resultMatrix = k_core_degeneracy(i-1, Matrix);
            printf("%d-core\n", i - 1);
            break;
        }
    }
    outputformat(resultMatrix);
    freeMatrix(Matrix);
    freeResultMatrix(resultMatrix);
    return 0;
}