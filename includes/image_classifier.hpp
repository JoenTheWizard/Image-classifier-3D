#ifndef IMG_CLASSIFIER 
#define IMG_CLASSIFIER

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "../mlp_nn/mlp_nn.h"

enum ColourChannel { RED = 0, GREEN = 1, BLUE = 2 };

//This will be used for the Image classification utilizing the MLP neural network
class ImageClassifier {
public:
    //Will initialize the neural network with default parameters
    ImageClassifier();
    ~ImageClassifier();
    MLP_NN neural_network;
    //Read the specified dataset and train
    int train_from_dataset(const char* dataset);
    int train_from_dataset_load_weights(const char* dataset, const char* weightsFile);
    //Load and save weights
    int save_weights(const char* weightsFile);
    int load_weights(const char* weightsFile);
    //Forward propagate
    void forward_propagate_img(const char* imagePath);
    //Will flatten the grayscaled image by reading the specified colour channel values and return Matrix
    static void flatten_img_data(const char* filePath, Matrix* result, ColourChannel channel = RED);
    //Create data set from directories that contain the images
    static void create_dataset_from_dir(const char* datasetPath, const std::vector<std::string>& directories);
    static void append_img_to_dataset(const char* datasetPath, const char* imagePath, const std::vector<std::string>& directories, int outputIndex);
    //Get maximum output value from the neurons (Returns the index of the column, will only read the first row as the output layer is expected to only have one row)
    size_t classify_max_column_index();
private:
    //We need to know the size of the network (total_layers = layer_neurons + layer_weights)
    //Num of weight layers = num of hidden layers + 1
    size_t num_of_hidden_layers;
    unsigned int* hidden_layer_nodes;
};

#endif