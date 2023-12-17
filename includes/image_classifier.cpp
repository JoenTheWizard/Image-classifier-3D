#include "image_classifier.hpp"
extern "C" {
   #define STB_IMAGE_IMPLEMENTATION
   #include "../stb_image/stb_image.h"
}

ImageClassifier::ImageClassifier() {
    
}

//Flatten the image data from the selected colour channel
void ImageClassifier::flatten_img_data(const char* filePath, Matrix* result, ColourChannel channel) {
    //X, Y and channels
    int x,y,n;
    unsigned char* imgData = stbi_load(filePath, &x, &y, &n, 0);

    init_matrix(result, y, x);

    if (imgData != nullptr && x > 0 && y > 0) {
        //RGB channels
        if (n == 3) {
            for (int i = 0; i < x * y; i++) {
                //Singular coloured component of the image data is only read
                unsigned char r = imgData[i * n + channel];
                double r_scaled = r / 255.0f;

                //Assign the values grayscaled values to the matrix
                result->data[i / y][i % y] = r_scaled; 
            }
        }
    }
}