#ifndef NEURON_H
#define NEURON_H

#define INPUT_SIZE 1

typedef struct {
    float weights[INPUT_SIZE];
    float bias;
} Neuron;

void initialize_neuron(Neuron* neuron);
float predict(const Neuron* neuron, const float* inputs);
void train(Neuron* neuron, const float* inputs, float target, float learning_rate);
float evaluate(const Neuron* neuron, const float inputs[][INPUT_SIZE], const float* targets, int count);

#endif