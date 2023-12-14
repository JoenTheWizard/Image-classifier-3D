#include "mlp_nn.h"

int
main(int argc, char* argv[]) {
    //Initialize NN
    unsigned int hidden_layer_nodes[] = {200};
    MLP_NN nn = {
        .num_inputs = 784,
        .num_outputs = 2,
        .num_hidden = hidden_layer_nodes,
        .learning_rate = 0.05,
        .epoch = 10000,
        //Initialize neurons and weights to NULL
        .neurons = NULL, .weights = NULL
    };

    //Initialize the input and output nodes
    Matrix input_nodes;
    Matrix output_nodes;

    //Read the data set
    if (read_dataset("../dataset/shapes.data", nn.num_inputs, nn.num_outputs, &input_nodes, &output_nodes)) {
        //We're going to train from the first row on our dataset (we can randomize it later)
        int row_index = 0;
        Matrix* inputs = get_row_matrix(&input_nodes, row_index);

        //Initialize the random weights for our model (will set nn.weights property)
        //Note: initialize_rand_weights() mallocs for nn.weights (needs to be freed)
        size_t num_of_hidden_layers = sizeof(hidden_layer_nodes) / sizeof(hidden_layer_nodes[0]);
        size_t sz = initialize_rand_weights(&nn, num_of_hidden_layers);

        //With all the weights set, we can initialize the full MLP model by a single forward propagate and
        //setting the MLP_NN neurons
        //Note: init_mlp_model() mallocs for nn.neurons (needs to be freed). Also assumes weights are set
        init_mlp_model(&nn, inputs, sz);

        //Print the MLP model after initializing the weights and calculating the initial neurons
        //Note: This function assumes that the weights and neurons are set
        //print_mlp_nn(&nn, sz);

        //Train the model, will update the weights
        train_mlp_model(&nn, &input_nodes, &output_nodes, sz);

        //Forward propagate to test the inputs on the newly trained model
        forward_propagate(&nn, inputs, sz);

        //FORWARD PROPAGATE TEST AFTER TRAIN
        Matrix* inputs1 = get_row_matrix(&input_nodes, 1);
        forward_propagate(&nn, inputs1, sz);
        free_matrix(inputs1);
        free(inputs1);

        Matrix* inputs2 = get_row_matrix(&input_nodes, 122);
        forward_propagate(&nn, inputs2, sz);
        free_matrix(inputs2);
        free(inputs2);

        Matrix input_test, output_test;
        read_dataset("../dataset/test.data", nn.num_inputs, nn.num_outputs, &input_test, &output_test);
        forward_propagate(&nn, &input_test, sz);
        free_matrix(&input_test);
        free_matrix(&output_test);

        //Save all the weights into a file to be able to reuse the trained weights
        save_mlp_weights(&nn, "weights.data", sz);

        //Free memory
        free_matrix(inputs);
        //free_matrix(outputs);
        free(inputs);
        //free(outputs);
        free_mat_array(&nn.weights, sz);
        free_mat_array(&nn.neurons, sz + 1);
    }

    free_matrix(&input_nodes);
    free_matrix(&output_nodes);
    
    return 0;
}