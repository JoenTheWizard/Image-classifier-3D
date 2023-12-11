#include "mlp_nn.h"

//Used to remove whitespace (newlines, spaces and tabs) for later use
void
remove_whitespace(char* str) {
   int i = 0, j = 0;
   while (str[i] != '\0') {
        //Pad to the left with index j
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j] = str[i];
            j++;
        }
        i++;
   }
   //Null-terminate the string
   str[j] = '\0';
}

//Get line count of file
int
get_file_line_count(const char* file_path) {
    int count = 1;
    char ch;
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("ERROR: Cannot read file %s\n", file_path);
        return 0;
    }

    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n')
            count++;
    }
    fclose(file);
    return count;
}

//Read the data set and obtain the corresponding matricies from it
int
read_dataset(const char* file_path, unsigned int num_inputs, unsigned int num_outputs,
            Matrix* input_nodes, Matrix* output_nodes) {
    //Initialize file
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Cannot read file %s\n", file_path);
        return 0;
    }

    //Get the line count from the file
    int line_count = get_file_line_count(file_path);

    //Store the matricies
    init_matrix(input_nodes, line_count, num_inputs);
    init_matrix(output_nodes, line_count, num_outputs);

    //Read each line
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    //Read and store character, j stores each line
    unsigned int j = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        char* l = line;
        char *toks, *saveptr;
        unsigned int i;
        //Read each token by its respective delimiter
        for (toks = strtok_r(l, ",", &saveptr), i = 0;
                toks != NULL && i < num_inputs + num_outputs;
                toks = strtok_r(NULL, ",", &saveptr), i++) {
            //Remove all whitespace in token (strtod() should automatically remove it but I'm keeping this for later)
            remove_whitespace(toks);
            //Convert token to double
            char* endptr;
            double value = strtod(toks, &endptr);
            if (endptr == toks) {
                fprintf(stderr, "ERROR: No valid digits were found in %s\n", toks);
                //Close file steam and free memory on error 
                fclose(file);
                if (line)
                    free(line);
                return 0;
            }
            //Store the respective values in the input and output matricies
            if (i < num_inputs)
                input_nodes->data[j][i] = value;
            else if (i >= num_inputs && i < num_inputs + num_outputs) {
                output_nodes->data[j][i - num_inputs] = value;
            }
        }
        j++;
    }

    //Print the matricies
    // printf("== INPUT MATRIX ==\n");
    // print_matrix(input_nodes);
    // printf("== OUTPUT MATRIX ==\n");
    // print_matrix(output_nodes);

    //Close file buffer and free other memory
    fclose(file);
    if (line)
        free(line);

    return 1;
}

//Sigmoid activation function (1 / (1+e^(-x)))
void sigmoid(Matrix* mat) {
    if (mat == NULL || mat->data == NULL) {
        printf("Error: Matrix data is not initialized\n");
        return;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->columns; j++)
            mat->data[i][j] = 1 / (1 + exp(-mat->data[i][j]));
    }
}

//Initialize the random weights for the parsed in model (will modify the weights attribute)
size_t
initialize_rand_weights(MLP_NN* mlp, size_t num_of_hidden_layers) {
    //Construct number of nodes on each layer and store into array
    int size_of_mlp_model = num_of_hidden_layers + 2;
    int* layers = (int*)malloc(size_of_mlp_model * sizeof(int));

    layers[0] = mlp->num_inputs;
    for (int i = 0; i < num_of_hidden_layers; i++) {
        layers[i + 1] = mlp->num_hidden[i];
    }
    layers[size_of_mlp_model - 1] = mlp->num_outputs;

    //Initialize the random weights for the passed in model (will set for the 'weights' paramter)
    srand(time(NULL));
    //printf("===== INITIALIZE RANDOM WEIGHTS =====\n");
    int num_weights = size_of_mlp_model - 1;
    mlp->weights = (Matrix*)malloc(num_weights * sizeof(Matrix));

    for (int i = 0; i < num_weights; i++) {
        init_matrix(&mlp->weights[i], layers[i], layers[i+1]);
        set_rand_weights(&mlp->weights[i]);
        //Print the matricies
        // print_matrix(&mlp->weights[i]);
        // printf("\n\n\n");
    }

    //Free unused memory
    free(layers);

    return num_weights;
}

//Forward propagate the MLP neural network
void
init_mlp_model(MLP_NN* mlp, Matrix* inputs_neurons, size_t num_of_hidden_layers) {
    //Copy inputs
    Matrix input;
    init_matrix(&input, inputs_neurons->rows, inputs_neurons->columns);
    copy_matrix(&input, inputs_neurons);

    mlp->neurons = (Matrix*)malloc((num_of_hidden_layers + 1) * sizeof(Matrix));
    init_matrix(&mlp->neurons[0], input.rows, input.columns);
    copy_matrix(&mlp->neurons[0], &input);

    //Loop calculate nodes for forward propagation
    // printf("===== INITIALIZE NEURONS (FORWARD PROPAGATION) =====\n");
    // print_matrix(&mlp->neurons[0]);
    for (int i = 0; i < num_of_hidden_layers; i++) {
        Matrix res;
        //Solve for the dot product of the net input, iterate for each weights on each layer
        dot_product(&input, &mlp->weights[i], &res);
        //Copy the result to the inputs
        copy_matrix(&input, &res);
        //Pass the activation function, sigmoid
        sigmoid(&input);
        //Print the matrix
        //print_matrix(&input);
        //Store the result into nodes matrix array
        init_matrix(&mlp->neurons[i+1], input.rows, input.columns);
        copy_matrix(&mlp->neurons[i+1], &input);
        //Free unnecessary memory
        free_matrix(&res);
    }

    free_matrix(&input);
}

//Print MLP model (its neurons with weights)
void
print_mlp_nn(MLP_NN* mlp, size_t num_of_hidden_layers) {
    printf("\n==== MLP MODEL ====\n");
    for (int i = 0; i < num_of_hidden_layers + 1; i++) {
        printf("Neurons %d:\n", i);
        print_matrix(&mlp->neurons[i]);
        printf("\n");
        if (i < num_of_hidden_layers) {
            printf("Weights %d:\n", i);
            print_matrix(&mlp->weights[i]);
            printf("\n");
        }
        printf("\n\n");
    }
}

void
forward_propagate(MLP_NN* mlp, Matrix* inputs_neurons, size_t num_of_hidden_layers) {
    //== PASS IN FORWARD PROPAGATION (Might make this a separate function) ==
    //Obtain the input again then forward pass
    printf("=== FORWARD PROPAGATION ===\n");
    Matrix input;
    init_matrix(&input, inputs_neurons->rows, inputs_neurons->columns);
    copy_matrix(&input, inputs_neurons);
    printf("Neurons 0\n");
    print_matrix(&input);
    for (int i = 0; i < num_of_hidden_layers; i++) {
        Matrix res;
        //Solve for the dot product of the net input, iterate for each weights on each layer
        dot_product(&input, &mlp->weights[i], &res);
        //Copy the result to the inputs
        copy_matrix(&input, &res);
        //Pass the activation function, sigmoid
        sigmoid(&input);
        //Print the output of the activation matrix
        printf("\nNeurons %i\n", i + 1);
        print_matrix(&input);
        //Store the result into nodes matrix array
        free_matrix(&mlp->neurons[i+1]);
        init_matrix(&mlp->neurons[i+1], input.rows, input.columns);
        copy_matrix(&mlp->neurons[i+1], &input);
        //Free unnecessary memory
        free_matrix(&res);
    }

    free_matrix(&input);
}

//Train the model. This will do a backpropagation and forward propagation pass modifying the weights
//of the passed in MLP.
void
train_mlp_model(MLP_NN* mlp, Matrix* inputs_neurons_dataset, Matrix* outputs_neurons_dataset, size_t num_of_hidden_layers) {
    for (int e = 0; e < mlp->epoch; e++) {
        printf("Epoch %i\n", e);
        Matrix error;
        Matrix old_weights;
        init_matrix(&old_weights, mlp->weights[num_of_hidden_layers - 1].rows, mlp->weights[num_of_hidden_layers - 1].columns);

        //Obtain inputs and outputs from random indexes
        unsigned int rand_index = rand() % (inputs_neurons_dataset->rows);
        Matrix* inputs = get_row_matrix(inputs_neurons_dataset, rand_index);
        Matrix* outputs = get_row_matrix(outputs_neurons_dataset, rand_index);

        //== PASS IN FORWARD PROPAGATION (Might make this a separate function) ==
        //Obtain the input again then forward pass
        Matrix input_neurons;
        init_matrix(&input_neurons, inputs->rows, inputs->columns);
        copy_matrix(&input_neurons, inputs);
        copy_matrix(&mlp->neurons[0], inputs);
        for (int i = 0; i < num_of_hidden_layers; i++) {
            Matrix res;
            //Solve for the dot product of the net input, iterate for each weights on each layer
            dot_product(&input_neurons, &mlp->weights[i], &res);
            //Copy the result to the inputs
            copy_matrix(&input_neurons, &res);
            //Pass the activation function, sigmoid
            sigmoid(&input_neurons);
            //Store the result into nodes matrix array
            free_matrix(&mlp->neurons[i+1]);
            init_matrix(&mlp->neurons[i+1], input_neurons.rows, input_neurons.columns);
            copy_matrix(&mlp->neurons[i+1], &input_neurons);
            //Free unnecessary memory
            free_matrix(&res);
        }

        //== PASS IN BACKWARD PROPAGATION (Might make this a separate function) ==
        for (int i = num_of_hidden_layers; i > 0; i--) {
            //Calculate error from the predicted output
            if (i == num_of_hidden_layers) {
                subtract_matrix(&mlp->neurons[i], outputs, &error);
                //printf("== Error ==\n");
                //print_matrix(&error);
            }
            //Calculate error for the hidden layers
            else {
                //printf("\n= Hidden Layer Error =\n");
                Matrix error_h;
                Matrix transpose_weights;
                transpose_matrix(&old_weights, &transpose_weights);

                //Multiply and print resulting errors
                dot_product(&error, &transpose_weights, &error_h);
                //print_matrix(&error_h);

                //Copy error_h to error for later use in backward pass
                copy_matrix(&error, &error_h);

                //Free unused matricies
                free_matrix(&error_h);
                free_matrix(&transpose_weights);
            }

            //In the backward pass we calculate the derivative of the cost function in respect to
            //the weights.
            //Derivative sigmoid (error * sigmoid(x) * (1 - sigmoid(x)) * output_prev_layer)
            //printf("== Sigmoid Derivative ==\n");
            Matrix sig_derivative;
            init_matrix(&sig_derivative, mlp->neurons[i].rows, mlp->neurons[i].columns);
            for (int j = 0; j < mlp->neurons[i].rows; j++) {
                for (int k = 0; k < mlp->neurons[i].columns; k++) {
                    double x = mlp->neurons[i].data[j][k];
                    sig_derivative.data[j][k] = error.data[j][k] * x * (1 - x);
                }
            }
            //print_matrix(&sig_derivative);

            //Multiply the previous layer nodes with the sigmoid derivative matrix
            //printf("== Delta Weights ==\n");
            Matrix delta_weights;
            Matrix transpose_nodes;
            transpose_matrix(&mlp->neurons[i-1], &transpose_nodes);
            //NOTE: Transpose the nodes here and multiply them other way around
            dot_product(&transpose_nodes, &sig_derivative, &delta_weights);

            //Scalar multiply the learning rate with the resulting matrix
            for (int j = 0; j < delta_weights.rows; j++) {
                for (int k = 0; k < delta_weights.columns; k++)
                    delta_weights.data[j][k] *= mlp->learning_rate;
            }
            //print_matrix(&delta_weights);

            //Store old weights before updating
            copy_matrix(&old_weights, &mlp->weights[i-1]);

            //Print the new weights
            //printf("= Resulting Weights =\n");
            //Apply the delta weights to the model weights by negating it
            for (int j = 0; j < mlp->weights[i-1].rows; j++) {
                for (int k = 0; k < mlp->weights[i-1].columns; k++)
                    mlp->weights[i-1].data[j][k] -= delta_weights.data[j][k];
            }
            //print_matrix(&mlp->weights[i-1]);

            //Free local scope memory
            free_matrix(&transpose_nodes);
            free_matrix(&delta_weights);
            free_matrix(&sig_derivative);
        }
        
        //print_mlp_nn(mlp, num_of_hidden_layers);

        //Free other unnecessary memory
        free_matrix(&error);
        free_matrix(&old_weights);
        free_matrix(&input_neurons);
        free_matrix(inputs);
        free_matrix(outputs);
        free(inputs);
        free(outputs);
    }
}

//Will deallocate the matrix arrays and set to NULL
void free_mat_array(Matrix** weights, int num_weights) {
   // Free the weights
   for (int i = 0; i < num_weights; i++) {
       free_matrix(&(*weights)[i]);
   }
   free(*weights);
   *weights = NULL;
}