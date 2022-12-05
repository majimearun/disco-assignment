// Arunachala Amuda Murugan (2021A7PS0205H)
// Shailesh Chandra Rao (2021A7PS2064H)

#include <stdio.h>
#include <stdlib.h>

struct matrixDetails
{
    int n;
    int **matrix;
};

struct matrixDetails* createMatrxiFromFile(char *filename){
    FILE *fp = fopen(filename, "r");
    int n, e;
    fscanf(fp, "%d", &n);
    fscanf(fp, "%d", &e);
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
        matrix[u-1][v-1] = 1;
        matrix[v-1][u-1] = 1;
    }
    fclose(fp);
    struct matrixDetails *mat = (struct matrixDetails *)malloc(sizeof(struct matrixDetails));
    mat->n = n;
    mat->matrix = matrix;
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
                count++;
            }
        }
        degree[i] = count;
    }
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

int main(int argc, char **argv){
    struct matrixDetails *matrix = createMatrxiFromFile(argv[1]);
    int *degree = degreeSequence(matrix->matrix, matrix->n);
    for (int i = 0; i < matrix->n; i++)
    {
        printf("%d ", degree[i]);
    }
    printf("\n");
    
    return 0;
}

