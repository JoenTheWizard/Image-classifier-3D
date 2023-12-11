#include "matrix.h"

//Responsible for inititalizing the matrix
void init_matrix(Matrix* mat, int rows, int columns) {
    mat->rows = rows;
    mat->columns = columns;
    mat->data = (double**)malloc(rows * sizeof(double*));
    //Use calloc to initialize each values to 0
    for (int i = 0; i < rows; i++)
        mat->data[i] = (double*)calloc(columns, sizeof(double));
}

//Pass in the matrix to set random weight values (ranging from 0 to 1)
void set_rand_weights(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->columns; j++) {
            mat->data[i][j] = ((double)rand() / (double)RAND_MAX) - 0.5;
        }
    }
}

//Method to calculate the dot product of two matricies
void dot_product(Matrix* mat1, Matrix* mat2, Matrix* result) {
    //# of cols of mat1 == # of rows of mat2
    if (mat1->columns != mat2->rows) {
        printf("ERROR: Number of columns (mat1) is not the same as number of rows (mat2)");
        return;
    }

    //Dot product of matrix (m,n) and matrix (n,k) results in (m,k)
    init_matrix(result, mat1->rows, mat2->columns);

    //Dot product calculation
    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat2->columns; j++) {
            for (int k = 0; k < mat1->columns; k++) {
                result->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
            }
        }
   }
}

//Subtract the two matricies and obtain the result
void subtract_matrix(Matrix* mat1, Matrix* mat2, Matrix* result) {
   //Check if the matrices have the same dimensions
   if (mat1->rows != mat2->rows || mat1->columns != mat2->columns) {
       printf("ERROR: Matrices must have the same dimensions to be subtracted.\n");
       return;
   }

   //Initialize the result matrix with the same dimensions as the input matrices
   init_matrix(result, mat1->rows, mat1->columns);

   //Subtract the two matrices
   for (int i = 0; i < mat1->rows; i++) {
       for (int j = 0; j < mat1->columns; j++) {
           result->data[i][j] = mat1->data[i][j] - mat2->data[i][j];
       }
   }
}

//This calculates the transpose matrix (interchange row and columns of matrix)
void transpose_matrix(Matrix* mat1, Matrix* result) {
    //Initialize resulting matrix
    init_matrix(result, mat1->columns, mat1->rows);

    //Transpose mat1 matrix and store into result
    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat1->columns; j++)
            result->data[j][i] = mat1->data[i][j];
    }
}

//Copy memory from one matrix to another
void copy_matrix(Matrix* dest, Matrix* src) {
   //Free any existing data in the destination matrix
   free_matrix(dest);

   //Initialize the destination matrix with the same dimensions as the source matrix
   init_matrix(dest, src->rows, src->columns);

   //Copy data from the source matrix to the destination matrix
   for (int i = 0; i < src->rows; i++) {
       for (int j = 0; j < src->columns; j++) {
           dest->data[i][j] = src->data[i][j];
       }
   }
}

//Obtain a one-row matrix by obtaining from a specified row index
Matrix* get_row_matrix(Matrix* mat, int row) {
    //Check if the row index is valid
    if (row < 0 || row >= mat->rows) {
        printf("ERROR: Invalid row index.\n");
        return NULL;
    }

    //Create the result matrix
    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    if (!result) {
        return NULL;
    }
    //Create the result matrix with 1 row and the same number of columns as the original matrix
    init_matrix(result, 1, mat->columns);

    //Copy the selected row from the original matrix to the result matrix
    for (int j = 0; j < mat->columns; j++) {
        result->data[0][j] = mat->data[row][j];
    }

   return result;
}

//Print the matrix in a nice format
void print_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->columns; j++) {
            printf("%f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

//Free memory from matrix
void free_matrix(Matrix* mat) {
    if (mat == NULL || mat->data == NULL) {
        return;
    }

    for (int i = 0; i < mat->rows; i++)
        free(mat->data[i]);
    free(mat->data);
}