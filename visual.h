#ifndef VISUAL_H
#define VISUAL_H

#include "neuron.h"

void init_visualization();
void render_scene(const Neuron* neuron, float user_x, int has_user_input);
void destroy_visualization();

#endif