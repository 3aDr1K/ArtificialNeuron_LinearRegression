#include "neuron.h"
#include <stdlib.h>

void initialize_neuron(Neuron* neuron) {
    for (int i = 0; i < INPUT_SIZE; ++i)
        neuron->weights[i] = ((float)rand() / RAND_MAX) * 2 - 1;
    neuron->bias = ((float)rand() / RAND_MAX) * 2 - 1;
}

float predict(const Neuron* neuron, const float* inputs) {
    float sum = neuron->bias;
    for (int i = 0; i < INPUT_SIZE; ++i)
        sum += neuron->weights[i] * inputs[i];
    return sum;
}

void train(Neuron* neuron, const float* inputs, float target, float learning_rate) {
    float prediction = predict(neuron, inputs);
    float error = prediction - target;
    for (int i = 0; i < INPUT_SIZE; ++i)
        neuron->weights[i] -= learning_rate * error * inputs[i];
    neuron->bias -= learning_rate * error;
}

float evaluate(const Neuron* neuron, const float inputs[][INPUT_SIZE], const float* targets, int count) {
    float mse = 0.0f;
    for (int i = 0; i < count; ++i) {
        float pred = predict(neuron, inputs[i]);
        float err = pred - targets[i];
        mse += err * err;
    }
    return mse / count;
}