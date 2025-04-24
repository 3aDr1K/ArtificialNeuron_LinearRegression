#include "visual.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define X_MIN 0.0f
#define X_MAX 10.0f
#define Y_MIN 0.0f
#define Y_MAX 40.0f

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;

void float_to_str(float value, char* buffer) {
    snprintf(buffer, 50, "%.2f", value);
}

static int to_screen_x(float x) {
    return (int)((x - X_MIN) / (X_MAX - X_MIN) * WINDOW_WIDTH);
}

static int to_screen_y(float y) {
    return (int)(WINDOW_HEIGHT - (y - Y_MIN) / (Y_MAX - Y_MIN) * WINDOW_HEIGHT);
}

void init_visualization() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Linear Regression Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 12);

    if (font == NULL) {
        printf("Font loading error: %s\n", TTF_GetError());
        exit(1);
    }
}

void draw_line(float a, float b, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for (float x = X_MIN; x <= X_MAX; x += 0.1f) {
        float y1 = a * x + b;
        float y2 = a * (x + 0.1f) + b;
        SDL_RenderDrawLine(renderer, to_screen_x(x), to_screen_y(y1), to_screen_x(x + 0.1f), to_screen_y(y2));
    }
}

void render_text(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);

    if (surface == NULL) {
        printf("Error rendering text: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_scene(const Neuron* neuron, float user_x, int has_user_input) {
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderClear(renderer);

    SDL_Color original_color = { 0, 0, 255 };
    draw_line(3.0f, 1.0f, original_color);

    SDL_Color predicted_color = { 0, 200, 0 };
    draw_line(neuron->weights[0], neuron->bias, predicted_color);

    char weight_text[100];
    char bias_text[50];
    char equation_text[200];
    float_to_str(neuron->weights[0], weight_text);
    float_to_str(neuron->bias, bias_text);
    snprintf(equation_text, sizeof(equation_text), "Predicted model y = %sx + %s", weight_text, bias_text);

    render_text("Original line (y = 3x + 1)", 20, 20, original_color);
    render_text(equation_text, 20, 50, predicted_color);

    if (has_user_input) {
        float real_y = 3 * user_x + 1;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect real_point = { to_screen_x(user_x) - 3, to_screen_y(real_y) - 3, 6, 6 };
        SDL_RenderFillRect(renderer, &real_point);
    }

    if (has_user_input) {
        float predicted_y = predict(neuron, &user_x);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect user_point = { to_screen_x(user_x) - 3, to_screen_y(predicted_y) - 3, 6, 6 };
        SDL_RenderFillRect(renderer, &user_point);
    }

    SDL_RenderPresent(renderer);
}

void destroy_visualization() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}