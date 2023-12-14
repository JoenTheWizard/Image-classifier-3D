#ifndef MLP_NN_H
#define MLP_NN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "matrix.h"

//The Multilayer Perceptron struct
typedef struct {
    //Number of nodes on the Dense layers
    unsigned int num_inputs;
    unsigned int num_outputs;
    unsigned int* num_hidden;
    //Other MLP NN parameters
    double learning_rate;
    int epoch;
    //The MLP nodes, weights and neurons (might change this later)
    Matrix* neurons;
    Matrix* weights;
} MLP_NN;

//Read the data set
int read_dataset(const char* file_path, unsigned int num_inputs, unsigned int num_outputs, Matrix* input_nodes, Matrix* output_nodes);

//Our activation function to pass in
void sigmoid(Matrix* mat);

//Initialize the weights for the model
size_t initialize_rand_weights(MLP_NN* mlp, size_t num_of_hidden_layers);

//Forward propagate
void init_mlp_model(MLP_NN* mlp, Matrix* inputs_neurons, size_t num_of_hidden_layers);

//Print MLP model (its neurons with weights)
void print_mlp_nn(MLP_NN* mlp, size_t num_of_hidden_layers);

//Forward propgate (pass the inputs through the model)
void forward_propagate(MLP_NN* mlp, Matrix* inputs_neurons, size_t num_of_hidden_layers);

//Train the MLP model (backward and forward propgation)
void train_mlp_model(MLP_NN* mlp, Matrix* inputs_neurons_dataset, Matrix* outputs_neurons_dataset, size_t num_of_hidden_layers);

//Save the weights into a file
void save_mlp_weights(MLP_NN* mlp, const char* file_path, size_t num_of_hidden_layers);

//Will deallocate the matrix arrays and set to NULL
void free_mat_array(Matrix** weights, int num_weights);

#endif