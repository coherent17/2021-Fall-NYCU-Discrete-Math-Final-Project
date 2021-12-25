#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 1000

bool **adjacencyMatrix(){
    bool **Matrix = (bool **)malloc(sizeof(bool *) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE;i++){
        Matrix[i] = (bool *)malloc(sizeof(bool) * MAX_SIZE);
    }
    //init
    for (int i = 0; i < MAX_SIZE;i++){
        for (int j = 0; j < MAX_SIZE;j++){
            Matrix[i][j] = 0;
        }
    }
    return Matrix;
}

int readfile(bool **Matrix){
    int a, b;
    int max_ab = 0;
    int max_vertex = 0;
    while(scanf("%d %d",&a, &b)!=EOF){
        printf("%d %d\n", a, b);
        max_ab = a > b ? a : b;
        max_vertex = max_vertex > max_ab ? max_vertex : max_ab;
        Matrix[a][b] = 1;
        Matrix[b][a] = 1;
    }
    return max_vertex;
}

void printMatrix(bool **Matrix, int max_vertex){
    for (int i = 0; i <= max_vertex;i++){
        for (int j = 0; j <= max_vertex;j++){
            printf("%d ", Matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    bool **Matrix = adjacencyMatrix();
    int max_vertex = readfile(Matrix);
    printf("%d\n" ,max_vertex);
    printMatrix(Matrix, max_vertex);
    return 0;
}