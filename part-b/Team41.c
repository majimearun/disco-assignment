// Arunachala Amuda Murugan (2021A7PS0205H)
// Shailesh Chandra Rao (2021A7PS2064H)

// DISCO Assignment Part B

#include <stdio.h>
#include <stdlib.h>

struct matrixDetails
{
    // number of nodes = number of rows
    int n;
    // number of edges
    int e;
    // adj matrix
    int **matrix;
};

struct matrixDetails* createMatrxiFromFile(char *filename){
    FILE *fp = fopen(filename, "r");
    int n, e;
    fscanf(fp, "%d", &n);
    fscanf(fp, "%d", &e);
    if(n == 0){
        printf("No vertices in the graph\n");
    }
    else if(n < 0){
        printf("Invalid number of vertices, only positive integers are allowed\n");
        exit(0);
    }
    if(e == 0){
        printf("No edges in the graph\n");
    }
    else if(e < 0){
        printf("Invalid number of edges, only positive integers are allowed\n");
        exit(0);
    }


    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < e; i++)
    {
        int u, v;
        fscanf(fp, "%d", &u);
        fscanf(fp, "%d", &v);
        // subtracting 1 as indexes start from 0
        // set to 1 to show edge exists
        matrix[u - 1][v - 1] = 1;
        matrix[v - 1][u - 1] = 1;
    }
    fclose(fp);
    struct matrixDetails *mat = (struct matrixDetails *)malloc(sizeof(struct matrixDetails));
    mat->n = n;
    mat->matrix = matrix;
    mat->e = e;
    return mat;
}

int *degreeSequence(int **matrix, int n){
    int *degree = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                if(i == j){
                    // adding 1 extra for self loops
                    count++;
                }
                count++;
            }
        }
        degree[i] = count;
    }

    // bubble sort
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (degree[i] < degree[j])
            {
                int temp = degree[i];
                degree[i] = degree[j];
                degree[j] = temp;
            }
        }
    }
    return degree;

}


int matricesAreEqual(int **matrix1, int **matrix2, int n){
    int flag = 1;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(matrix1[i][j] != matrix2[i][j]){
                flag = 0;
                break;
            }
        }
    }
    return flag;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int findIndex(int *l, int elem){
    for(int i = 0; i < sizeof(l); i++){
        if(l[i] == elem){
            return i;
        }
    }
    return -1;
}

void isomorphicCheck(int *a, int l, int r, int **matrix1, int **matrix2, int n){

    // when l == r we have reached the ending of the swaps so a permuation has been formed
    if(l == r){
        int **newMatrix = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            newMatrix[i] = (int *)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++)
            {
                newMatrix[i][j] = 0;
            }
        }
        
        // creating new matrix in the new row and column order (both same) 
        // from the specified permutation
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(matrix1[i][j] == 1){
                    // finding new position of the row and column where the 1 has to be
                    // in the new matrix 
                    // this will be the position of the {index + 1} element in the permuation
                    // +1 because node count starts from 1, but index from 0
                    int x = findIndex(a, i + 1);
                    int y = findIndex(a, j + 1);
                    newMatrix[x][y] = 1;
                }
            }
        }
        if(matricesAreEqual(newMatrix, matrix2, n)){
            printf("Isomorphic\n");
            for(int i = 0; i < n; i++){
                printf("%d %d\n", i + 1, a[i]);
            }
            exit(0);
        }

    }
    else{
        for(int i = l; i <= r; i++){
            // swapping first element with each element of the array
            swap((a+l), (a+i));
            // conitnuing the permutation  for subsequent elements once earlier element is fixed
            isomorphicCheck(a, l+1, r, matrix1, matrix2, n);
            // swapping back inital element to its original place
            swap((a+l), (a+i));
        }
    }
}

int main(int argc, char **argv){
    // NOTE: Output depends on the order u pass the test files
    // the graph node order from the first text file is kept constant and we find a corresponding bijection
    // to a permuation of the graph formed from the second text file
    // constructing the matrix
    struct matrixDetails *matrix1 = createMatrxiFromFile(argv[1]);
    struct matrixDetails *matrix2 = createMatrxiFromFile(argv[2]);

    // checking if the number of nodes and edges are the same
    if(matrix1->n != matrix2->n || matrix1->e != matrix2->e){
        printf("Not isomorphic\n");
        exit(0);
    }

    // checking if the degree sequences are the same
    int *degree1 = degreeSequence(matrix1->matrix, matrix1->n);
    int *degree2 = degreeSequence(matrix2->matrix, matrix2->n);
    for (int i = 0; i < matrix1->n; i++)
    {
        if(degree1[i] != degree2[i]){
            printf("Not isomorphic\n");
            exit(0);
        }
    }

    // setting node order for graph 2
    // initial order {1, 2, 3, 4, ..., n}
    int *a = (int *)malloc(matrix1->n * sizeof(int));
    for(int i = 0; i < matrix1->n; i++){
        a[i] = i + 1;
    }

    // finding bijection (if exists) recursively
    isomorphicCheck(a, 0, matrix1->n - 1, matrix2->matrix, matrix1->matrix, matrix1->n);

    // bijection doesnt exist
    printf("Not isomorphic\n");
    return 0;
}
