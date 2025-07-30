#include "opacity.h"

unsigned int read_unsigned_from_file(const char *path);
double getOpacity();

unsigned int read_unsigned_from_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    unsigned int value = 0;
    if (fscanf(f, "%u", &value) != 1) {
        fprintf(stderr, "Erreur lecture valeur dans %s\n", path);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    fclose(f);
    return value;
}

double getOpacity() {
    unsigned int max_brightness = read_unsigned_from_file(FILE_MAX_BRIGHTNESS);
    unsigned int actual_brightness = read_unsigned_from_file(FILE_BRIGHTNESS);

    // Calcul de l'opacité avec 2 chiffres après la virgule
    double brightness = ceil(((double)actual_brightness / (double)max_brightness) * 100.) / 100.;
    double opacity = 1. - brightness;

    // printf("Brightness = %.2f\n", brightness);
    // printf("Opacity = %.2f\n", opacity);

    return opacity;
}
