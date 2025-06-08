#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <time.h>

// Constantes pour la fenêtre
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_POINTS 100

// Structure pour représenter un point dans le plan
typedef struct {
    double x, y;
    double label;
} Point;

// Génération du dataset pour la fonction AND
void generateDataset(Point *points, int num_points) {
    srand(time(NULL)); // Initialisation du générateur aléatoire
    for (int i = 0; i < num_points; i++) {
        points[i].x = ((double)rand() / RAND_MAX) * 2 - 1; // [-1, 1]
        points[i].y = ((double)rand() / RAND_MAX) * 2 - 1; // [-1, 1]
        // Label 1 si dans le premier quadrant, 0 sinon
        points[i].label = (points[i].x > 0 && points[i].y > 0) ? 1.0 : 0.0;
    }
}

// Calcul du taux d'erreur (log-vraisemblance négative)
double Loss(double prediction, double sortie_attendue) {
    double epsilon = 1e-15; // Prévenir log(0)
    prediction = fmax(epsilon, fmin(1 - epsilon, prediction));
    return -(sortie_attendue * log(prediction) + (1 - sortie_attendue) * log(1 - prediction));
}

// Fonction d'activation (sigmoïde)
double activation(double y) {
    return 1.0 / (1.0 + exp(-y));
}

// Dessiner la ligne de décision
void draw_decision(SDL_Renderer *renderer, double w[2], double b, double scale_x, double scale_y, double offset_x, double offset_y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Ligne blanche
    double x1 = -1.0, x2 = 1.0;
    double y1 = -(w[0] * x1 + b) / w[1];
    double y2 = -(w[0] * x2 + b) / w[1];
    int px1 = (int)((x1 + 1) * scale_x + offset_x);
    int py1 = (int)((-y1 + 1) * scale_y + offset_y);
    int px2 = (int)((x2 + 1) * scale_x + offset_x);
    int py2 = (int)((-y2 + 1) * scale_y + offset_y);
    SDL_RenderDrawLine(renderer, px1, py1, px2, py2);
}

// Dessiner un point avec couleur et taille
void draw_point(SDL_Renderer *renderer, double x, double y, double label, double scale_x, double scale_y, double offset_x, double offset_y) {
    int px = (int)((x + 1) * scale_x + offset_x);
    int py = (int)((-y + 1) * scale_y + offset_y); // Inverser y pour les coordonnées écran
    // Bleu pour classe 0, Jaune pour classe 1
    SDL_SetRenderDrawColor(renderer, label > 0.5 ? 255 : 0, label > 0.5 ? 255 : 0, label > 0.5 ? 0 : 255, 255);
    SDL_Rect rect = {px - 6, py - 6, 12, 12}; // Carré de 12x12 pixels
    SDL_RenderFillRect(renderer, &rect);
}

// Dessiner une légende avec bordures blanches
void draw_legend(SDL_Renderer *renderer) {
    // Rectangle bleu pour classe 0
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect_class0 = {20, 20, 20, 20};
    SDL_RenderFillRect(renderer, &rect_class0);
    // Bordure blanche pour classe 0
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect_class0);
    // Rectangle jaune pour classe 1
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect_class1 = {20, 50, 20, 20};
    SDL_RenderFillRect(renderer, &rect_class1);
    // Bordure blanche pour classe 1
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect_class1);
}

int main() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Impossible d'initialiser SDL : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Entraînement du Perceptron", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur lors de la création du Renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Génération du dataset
    Point points[NUM_POINTS];
    generateDataset(points, NUM_POINTS);

    // Initialisation des poids et du biais
    double w[2] = {0.5, -1.0};
    double b = 1.0;
    double learning_rate = 0.1;
    int max_epochs = 1000;

    // Facteurs de mise à l'échelle pour la visualisation
    double scale_x = WINDOW_WIDTH / 2.0;
    double scale_y = WINDOW_HEIGHT / 2.0;
    double offset_x = WINDOW_WIDTH / 2.0;
    double offset_y = WINDOW_HEIGHT / 2.0;

    // Afficher la légende dans la console avec texte blanc
    printf("\033[1;37m"); // Texte blanc (ANSI)
    printf("Légende :\n");
    printf("- Carré bleu avec bordure blanche : Classe 0 (x <= 0 ou y <= 0)\n");
    printf("- Carré jaune avec bordure blanche : Classe 1 (x > 0 et y > 0)\n");
    printf("- Ligne blanche : Frontière de décision\n\n");
    printf("\033[0m"); // Réinitialiser la couleur

    // Boucle d'entraînement
    int epoch = 0;
    int running = 1;
    SDL_Event event;
    while (running && epoch < max_epochs) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        // Dessiner la légende
        draw_legend(renderer);

        // Dessiner les points
        for (int i = 0; i < NUM_POINTS; i++) {
            draw_point(renderer, points[i].x, points[i].y, points[i].label, scale_x, scale_y, offset_x, offset_y);
        }

        // Dessiner la ligne de décision
        draw_decision(renderer, w, b, scale_x, scale_y, offset_x, offset_y);

        // Afficher le rendu
        SDL_RenderPresent(renderer);

        // Entraînement du perceptron
        for (int i = 0; i < NUM_POINTS; i++) {
            double input[2] = {points[i].x, points[i].y};
            double y = input[0] * w[0] + input[1] * w[1] + b;
            double prediction = activation(y);
            double error = Loss(prediction, points[i].label);

            // Mise à jour des poids et du biais
            w[0] -= learning_rate * (prediction - points[i].label) * input[0];
            w[1] -= learning_rate * (prediction - points[i].label) * input[1];
            b -= learning_rate * (prediction - points[i].label);
        }

        // Affichage des poids et du biais en blanc
        printf("\033[1;37mÉpoque %d : w1 = %f, w2 = %f, b = %f\n\033[0m", epoch, w[0], w[1], b);

        // Pause pour visualiser l'entraînement
        SDL_Delay(100);
        epoch++;
    }

    // Test final du perceptron
    printf("\033[1;37m\nPoids finaux : w1 = %f, w2 = %f, b = %f\n", w[0], w[1], b);
    printf("\nRésultats des prédictions :\n");
    for (int i = 0; i < NUM_POINTS; i++) {
        double input[2] = {points[i].x, points[i].y};
        double y = input[0] * w[0] + input[1] * w[1] + b;
        double prediction = activation(y);
        printf("Point %d : (x=%.3f, y=%.3f) => Prédiction : %.3f, Attendu : %.0f (Classe %s)\n",
               i + 1, input[0], input[1], prediction, points[i].label,
               points[i].label > 0.5 ? "1 (Jaune)" : "0 (Bleu)");
    }
    printf("\033[0m"); // Réinitialiser la couleur

    // Nettoyage et fermeture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
