#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "visual.h"

#define TRAINING_SIZE 500
#define EPOCHS 1000
#define LEARNING_RATE 0.000001f
#define TEST_SIZE (TRAINING_SIZE / 3)

void generate_data(float inputs[][INPUT_SIZE], float* targets) {
    for (int i = 0; i < TRAINING_SIZE; ++i) {
        float x = ((float)rand() / RAND_MAX) * 10;
        inputs[i][0] = x;
        targets[i] = 3 * x + 1 + ((float)rand() / RAND_MAX - 0.5f) * 4;
    }
}

void split_data(float inputs[][INPUT_SIZE], float* targets, float train_inputs[][INPUT_SIZE], float* train_targets, float test_inputs[][INPUT_SIZE], float* test_targets) {
    for (int i = 0; i < TRAINING_SIZE; ++i) {
        if (i < TRAINING_SIZE - TEST_SIZE) {
            train_inputs[i][0] = inputs[i][0];
            train_targets[i] = targets[i];
        }
        else {
            test_inputs[i - (TRAINING_SIZE - TEST_SIZE)][0] = inputs[i][0];
            test_targets[i - (TRAINING_SIZE - TEST_SIZE)] = targets[i];
        }
    }
}

int main() {
    srand((unsigned)time(NULL));

    Neuron neuron;
    initialize_neuron(&neuron);

    float inputs[TRAINING_SIZE][INPUT_SIZE];
    float targets[TRAINING_SIZE];
    float train_inputs[TRAINING_SIZE - TEST_SIZE][INPUT_SIZE];
    float train_targets[TRAINING_SIZE - TEST_SIZE];
    float test_inputs[TEST_SIZE][INPUT_SIZE];
    float test_targets[TEST_SIZE];

    generate_data(inputs, targets);
    split_data(inputs, targets, train_inputs, train_targets, test_inputs, test_targets);

    init_visualization();

    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        for (int i = 0; i < TRAINING_SIZE - TEST_SIZE; ++i) {
            train(&neuron, train_inputs[i], train_targets[i], LEARNING_RATE);
        }

        if (epoch % 5 == 0 || epoch == EPOCHS - 1) {
            float loss = evaluate(&neuron, test_inputs, test_targets, TEST_SIZE);
            printf("Epoch %d - MSE: %.4f\n", epoch, loss);
            render_scene(&neuron, 0, 0);
            SDL_Delay(100);
        }
    }

    float user_input;
    printf("\nEnter x (or Ctrl+C for exit):\n");
    while (scanf("%f", &user_input) == 1) {
        float y = predict(&neuron, &user_input);
        printf("True y: %.2f\n", 3 * user_input + 1);
        printf("Predicted y: %.2f\n", y);
        render_scene(&neuron, user_input, 1);
        printf("Enter new x:\n");
    }

    destroy_visualization();
    return 0;
}