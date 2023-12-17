#ifndef IMG_CLASSIFIER 
#define IMG_CLASSIFIER

#include <iostream>
#include <fstream>
#include <filesystem>
#include "../mlp_nn/mlp_nn.h"

enum ColourChannel { RED = 0, GREEN = 1, BLUE = 2 };

//This will be used for the Image classification utilizing the MLP neural network
class ImageClassifier {
public:
    ImageClassifier();
    MLP_NN neural_network;
    //Will flatten the grayscaled image by reading the specified colour channel values
    static void flatten_img_data(const char* filePath, Matrix* result, ColourChannel channel);
};

#endif