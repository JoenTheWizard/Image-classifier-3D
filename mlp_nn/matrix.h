#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

//This is a redefinition of the Matrix struct for a better and nicer format

typedef struct {
    int rows;
    int columns;
    double** data;
} Matrix;

//Initialize matrix with rows and columns
void init_matrix(Matrix* mat, int rows, int columns);

//Neural network specific function: Set random weight values
void set_rand_weights(Matrix* mat);

//Matrix multiplication - dot product
void dot_product(Matrix* mat1, Matrix* mat2, Matrix* result); 

//Subtract two matricies
void subtract_matrix(Matrix* mat1, Matrix* mat2, Matrix* result);

//Obtain matrix transpose
void transpose_matrix(Matrix* mat1, Matrix* result);

//Copy matrix from mat1 to mat2
void copy_matrix(Matrix* dest, Matrix* src);

//Obtain a row from the matrix
Matrix* get_row_matrix(Matrix* mat, int row);

//Print matrix
void print_matrix(Matrix* mat);

//Free the matrix
void free_matrix(Matrix* mat);


#endif