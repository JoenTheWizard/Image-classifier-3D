# Image Classifier 3D
An image classifier from a fully connected neural network implemented in C++. The neural network is implemented in C (in the `mlp_nn` folder). The classification then renders an object (from a predefined vertex array object) depending on the maximum output neuron index done from a forward propagation pass done from a 28x28 image. As of now there are two classifications either a Cube or Pyramid.

## Usage
Initially, you can train the network from a dataset (with the valid number of input and outputs, 784 input 2 output) and pass in a valid 28x28 image for a forward pass with the following:
`./main <28x28-image> -t <dataset>`

The file `dataset/shapes.data` can be used as an example. It was produced from the `ImageClassifier::create_dataset_from_dir()` from the images located in the `datset/` folder with the classifications in separate folders. After training you can save a file to disk containing binary data of the weights from each layer using:
`./main <28x28-image> -t <dataset> -o <weights-file>`

The weights file can be reused for either a singular forward pass such as:
`./main <28x28-image> -l <weights-file>`

Or to further train the weights:
`./main <28x28-image> -l <weights-file> -t <dataset> -o <weights-file>`

**NOTE:** As of right now, there are no headers/extra metadata stored in the weights files so it doesn't check number of weight layers on the neural network when the file is loaded or saved. Currently planning to make the neural network configurable (aka change number of epochs, learning rate, no. of hidden layer etc). Also there is no check if the passed in image for the forward pass is a 28x28 image (might either make it return error or automatically resize image).

# Todo
- ~~Make the object classification (rendered to the OpenGL scene)~~
- Add a header in the weights file to check if the amount of neurons is corresponding to the loaded neural network
- Pass in arguments in CLI
- Extra error checking in the `ImageClassifier` class
- ~~Make base class for `Pyramid` and `Cube` (optional)~~
- Set some getters and setters in `ImageClassifier` class
- Set uniform buffer objects for the object shaders (optional)

# Sources
- [300 images of squares, circles, and triangles](https://www.kaggle.com/datasets/cactus3/basicshapes/)
- [Make Your Own Neural Network - Tariq Rashid](https://github.com/harshitkgupta/StudyMaterial/blob/master/Make%20Your%20Own%20Neural%20Network%20(Tariq%20Rashid)%20-%20%7BCHB%20Books%7D.pdf)
- [LearnOpenGL by Joey de Vries](https://learnopengl.com/)
