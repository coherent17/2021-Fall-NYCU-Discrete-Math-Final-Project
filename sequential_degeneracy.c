#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 1000

//allocate adjancencyMatrix and initialize it
bool **adjacencyMatrix(){
    bool **Matrix = (bool **)malloc(sizeof(bool *) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE;++i){
        Matrix[i] = (bool *)malloc(sizeof(bool) * MAX_SIZE);
    }
    //init
    /*
    for (int i = 0; i < MAX_SIZE;++i){
        for (int j = i+1; j < MAX_SIZE;++j){
            Matrix[i][j] = 0;
        }
    }
    */
    
    return Matrix;
}

void freeMatrix(bool **Matrix){
    for(int i = 0; i < MAX_SIZE;++i){
        free(Matrix[i]);
    }
    free(Matrix);
}

int readfile(bool **Matrix){
    int a, b;
    int max_vertex = 0;
    while(scanf("%d %d",&a, &b)!=EOF){
        //printf("%d %d\n", a, b);
        max_vertex = max_vertex > b ? max_vertex : b;
        Matrix[a][b] = 1;
        //Matrix[b][a] = 1;
    }
    return max_vertex;
}

int *getDegree(bool **Matrix, int length){
    int *degree = malloc(sizeof(int) * (length+1));
    for (int i = 0; i <= length;i++){
        degree[i] = 0;
    }
    // using row-wise access to be a cache friendly function
    for (int i = 0; i <= length; ++i){
        for (int j = i+1; j <= length; ++j){
            if (Matrix[i][j] == 1){
                degree[i]++;
                degree[j]++;
            }
        }
    }
    return degree;
}

bool **deleteVertex(bool **Matrix, int vertextodelete, int max_vertex){
    for (int i = 0; i <= max_vertex;++i){
        Matrix[i][vertextodelete] = 0;
    }
    for (int j = 0; j <= max_vertex;++j){
        Matrix[vertextodelete][j] = 0;
    }
    return Matrix;
}

bool allzero(bool **matrix, int max_vertex){
    for (int i = 0; i <= max_vertex;++i){
        for (int j = i+1; j <= max_vertex;++j){
            if(matrix[i][j]!=0){
                return false;
            }
        }
    }
    return true;
}


bool **k_core_degeneracy(int k, bool **Matrix, int max_vertex){

    bool **resultMatrix = malloc(sizeof(bool *) * (max_vertex + 1));
    for (int i = 0; i <= max_vertex;++i){
        resultMatrix[i] = malloc(sizeof(bool) * (max_vertex + 1));
    }

    //copy the matrix
    for (int i = 0; i <= max_vertex;++i){
        for (int j = i+1; j <= max_vertex;++j){
            resultMatrix[i][j] = Matrix[i][j];
        }
    }

    for(;;){
        int *degree = getDegree(resultMatrix, max_vertex);

        int count = 0;
        for (int i = 0; i < max_vertex;++i){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            return resultMatrix;
        }

        for (int i = 0; i <= max_vertex; ++i){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i, max_vertex);
            }
        }
        free(degree);
    }
    return resultMatrix;
}

bool **sequential_degeneracy(int k, bool **resultMatrix, int max_vertex){
    for(;;){
        int *degree = getDegree(resultMatrix, max_vertex);

        int count = 0;
        for (int i = 0; i < max_vertex;++i){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            return resultMatrix;
        }

        for (int i = 0; i <= max_vertex; ++i){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i, max_vertex);
            }
        }
        free(degree);
    }
    return resultMatrix;
}

void outputformat(bool **matrix, int max_vertex){
    for (int i = 0; i <= max_vertex;++i){
        for (int j = i+1; j <= max_vertex; ++j){
            if(matrix[i][j]!=0 && j>i){
                printf("%d %d\n", i, j);
            }
        }
    }
}

void freeResultMatrix(bool **Matrix, int max_vertex){
    for (int i = 0; i <= max_vertex;++i){
        free(Matrix[i]);
    }
    free(Matrix);
}

void printMatrix(bool **Matrix, int max_vertex){
    for (int i = 0; i < max_vertex;i++){
        for (int j = 0; j < max_vertex;j++){
            printf("%d ", Matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    bool **Matrix = adjacencyMatrix();
    int max_vertex = readfile(Matrix);
    //printMatrix(Matrix, max_vertex);
    bool **resultMatrix;
    resultMatrix = k_core_degeneracy(3, Matrix, max_vertex);
    for (int i = 4;;++i){
        resultMatrix = sequential_degeneracy(i, resultMatrix, max_vertex);
        if(allzero(resultMatrix,max_vertex)){
            freeResultMatrix(resultMatrix, max_vertex);
            resultMatrix = k_core_degeneracy(i-1, Matrix, max_vertex);
            printf("%d-core\n", i - 1);
            break;
        }
    }
    outputformat(resultMatrix, max_vertex);
    freeMatrix(Matrix);
    freeResultMatrix(resultMatrix,max_vertex);
    return 0;
}