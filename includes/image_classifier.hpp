#ifndef IMG_CLASSIFIER 
#define IMG_CLASSIFIER

#include <iostream>
extern "C" {
   #define STB_IMAGE_IMPLEMENTATION
   #include "../stb_image/stb_image.h"
}

enum ColourChannel { RED = 0, GREEN = 1, BLUE = 2};

//Will flatten the grayscaled image by reading the specified colour channel values
//Need to still work on this
void flatten_img_data(const char* filePath, ColourChannel channel) {
    //X, Y and channels
    int x,y,n;
    unsigned char* imgData = stbi_load(filePath, &x, &y, &n, 0);

    if (imgData != nullptr && x > 0 && y > 0) {
        //RGB channels
        if (n == 3) {
            for (int i = 0; i < x * y; i++) {
                //Red component of the image data is only read
                unsigned char r = imgData[i * n + channel];
                float r_scaled = r / 255.0f;

                //Print data
                std::cout << r_scaled;
                std::cout << ((i % x == x - 1) ? '\n' : ' ');
            }
        }
    }
}

#endif