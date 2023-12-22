#include "image_classifier.hpp"
extern "C" {
   #define STB_IMAGE_IMPLEMENTATION
   #include "../stb_image/stb_image.h"
}

ImageClassifier::ImageClassifier() {
    //To initialize the neural network struct
    hidden_layer_nodes = new unsigned int[1];
    hidden_layer_nodes[0] = 200;
    neural_network = {
        .num_inputs = 784,
        .num_outputs = 2,
        .learning_rate = 0.05,
        .epoch = 10000,
        //Initialize neurons and weights to NULL
        .neurons = NULL, .weights = NULL
    };
    neural_network.num_hidden = hidden_layer_nodes;

    num_of_hidden_layers = 1; //sizeof(hidden_layer_nodes) / sizeof(hidden_layer_nodes[0]);
}

int ImageClassifier::train_from_dataset(const char* dataset) {
    Matrix input_nodes;
    Matrix output_nodes;

    printf("No Input: %i No Output: %i\n", neural_network.num_inputs, neural_network.num_outputs);
    printf("Learning Rate: %f Epoch: %i\n", neural_network.learning_rate, neural_network.epoch);
    printf("Hidden Layers:\n");
    for (int i = 0; i < num_of_hidden_layers; i++) {
        printf("= %d =\n", neural_network.num_hidden[i]);
    }

    if (!read_dataset(dataset, neural_network.num_inputs, neural_network.num_outputs, &input_nodes, &output_nodes)) {
        printf("Error reading the data set (check if file exists)\n");
        free_matrix(&input_nodes);
        free_matrix(&output_nodes);
        return -1;
    } else {
        Matrix* inputs = get_row_matrix(&input_nodes, 0);

        size_t num_weight_layers = initialize_rand_weights(&neural_network, num_of_hidden_layers);

        init_mlp_model(&neural_network, inputs, num_weight_layers);

        train_mlp_model(&neural_network, &input_nodes, &output_nodes, num_weight_layers);

        free_matrix(inputs);
        free(inputs);
        free_matrix(&input_nodes);
        free_matrix(&output_nodes);
    }

    return 0;
}

int ImageClassifier::train_from_dataset_load_weights(const char* dataset, const char* weightsFile) {
    Matrix input_nodes;
    Matrix output_nodes;

    printf("No Input: %i No Output: %i\n", neural_network.num_inputs, neural_network.num_outputs);
    printf("Learning Rate: %f Epoch: %i\n", neural_network.learning_rate, neural_network.epoch);
    printf("Hidden Layers:\n");
    for (int i = 0; i < num_of_hidden_layers; i++) {
        printf("= %d =\n", neural_network.num_hidden[i]);
    }

    if (!read_dataset(dataset, neural_network.num_inputs, neural_network.num_outputs, &input_nodes, &output_nodes)) {
        printf("Error reading the data set (check if file exists)\n");
        free_matrix(&input_nodes);
        free_matrix(&output_nodes);
        return -1;
    } else {
        Matrix* inputs = get_row_matrix(&input_nodes, 0);

        size_t num_weight_layers = load_mlp_weights(&neural_network, weightsFile, num_of_hidden_layers);

        init_mlp_model(&neural_network, inputs, num_weight_layers);

        train_mlp_model(&neural_network, &input_nodes, &output_nodes, num_weight_layers);

        free_matrix(inputs);
        free(inputs);
        free_matrix(&input_nodes);
        free_matrix(&output_nodes);
    }

    return 0;
}

int ImageClassifier::save_weights(const char* weightsFile) {
    if (neural_network.weights != NULL) {
        size_t num_weight_layers = num_of_hidden_layers + 1;
        save_mlp_weights(&neural_network, weightsFile, num_weight_layers);
        return 0;
    }
    return -1;
}

int ImageClassifier::load_weights(const char* weightsFile) {
    size_t num_weight_layers = load_mlp_weights(&neural_network, weightsFile, num_of_hidden_layers);
    return 0;
}

void ImageClassifier::forward_propagate_img(const char* imagePath) {
    Matrix inputs;
    flatten_img_data(imagePath, &inputs);
    size_t num_weight_layers = num_of_hidden_layers + 1;
    if (neural_network.neurons == NULL)
        init_mlp_model(&neural_network, &inputs, num_weight_layers);

    if (neural_network.neurons != NULL)
        forward_propagate(&neural_network, &inputs, num_weight_layers);

    free_matrix(&inputs);
}

//Flatten the image data from the selected colour channel
void ImageClassifier::flatten_img_data(const char* filePath, Matrix* result, ColourChannel channel) {
    //X, Y and channels
    int x,y,n;
    unsigned char* imgData = stbi_load(filePath, &x, &y, &n, 0);

    init_matrix(result, 1, x*y);

    if (imgData != nullptr && x > 0 && y > 0) {
        //RGB channels
        if (n == 3) {
            for (int i = 0; i < x * y; i++) {
                //Singular coloured component of the image data is only read
                unsigned char r = imgData[i * n + channel];
                double r_scaled = r / 255.0f;

                //Assign the values grayscaled values to the matrix
                result->data[0][i] = r_scaled; 
            }
        }
    }
    stbi_image_free(imgData);
}

ImageClassifier::~ImageClassifier() {
    printf("Dellaocating\n");
    size_t num_weight_layers = num_of_hidden_layers + 1;
    if (neural_network.weights != NULL)
        free_mat_array(&neural_network.weights, num_weight_layers);
    if (neural_network.neurons != NULL)
        free_mat_array(&neural_network.neurons, num_weight_layers + 1);
    
    delete[] hidden_layer_nodes;
}