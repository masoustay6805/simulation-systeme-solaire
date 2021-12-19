#include "planet.h"
#include "../vec2/vec2.h"
#include <stdlib.h>
#include "../gfx/gfx.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define G 6.67e-11

#define SUN_M 1.989e30
#define SUN_X 0
#define SUN_Y 0

#define MERCURY_M 3.3022e23
#define MERCURY_X 0.2
#define MERCURY_Y 0    

#define VENUS_M 4.8685e24
#define VENUS_X 0.4
#define VENUS_Y 0

#define EARTH_M 3.5838e25
#define EARTH_X 0.6
#define EARTH_Y 0

#define MARS_M 6.4185e23
#define MARS_X 0.8
#define MARS_Y 0

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define PLANETES_CSV_PATH "./planetes.csv"
#define PLANETES_CSV_HEADERS true
#define PLANETES_CSV_SEPARATOR ";"
#define PLANETES_CSV_ROW_MAX_LENGTH 1024

planet_t create_planet(double mass, int color, double radius, double semi_major_axis, double eccentricity, bool is_star) {
    planet_t planet = {
        .mass = mass,
        .color = color,
        .radius = radius,
        .semi_major_axis = semi_major_axis,
        .eccentricity = eccentricity,
    };
    if (is_star) {
        planet.prec_pos = planet.pos = vec2_create_zero();
    } else {
        planet.prec_pos = planet.pos = vec2_create(semi_major_axis * (1 - eccentricity), 0);
    }
    return planet;
}

system_t create_system(double delta_t) {
    FILE *stream = fopen(PLANETES_CSV_PATH, "r");
    if (stream == NULL) {
        printf("%s not found !", PLANETES_CSV_PATH);
        exit(EXIT_FAILURE);
    }
    char line[PLANETES_CSV_ROW_MAX_LENGTH];
    // Count csv lines
    int32_t csv_nb_lines = 0;
    while (fgets(line, PLANETES_CSV_ROW_MAX_LENGTH, stream)) {
        csv_nb_lines++;
    }
    // Create system
    int32_t nb_planetes = PLANETES_CSV_HEADERS ? csv_nb_lines - 2 : csv_nb_lines - 1;
    system_t system = {
            .planets = malloc(sizeof(planet_t) * nb_planetes),
            .nb_planets = nb_planetes
    };
    rewind(stream);
    // Read csv line per line
    int32_t line_index = PLANETES_CSV_HEADERS ? -1 : 0;
    while (fgets(line, PLANETES_CSV_ROW_MAX_LENGTH, stream) && line_index - 1 < nb_planetes) {
        // If line isn't headers
        if (line_index > -1) {
            double mass = 0, radius = 0, semi_major_axis = 0, eccentricity = 0;
            int color = 0, field_index = 0;
            // Read each fields
            for (char *token = strtok(line, PLANETES_CSV_SEPARATOR); token != NULL; token = strtok(NULL, PLANETES_CSV_SEPARATOR), field_index++) {
                switch (field_index) {
                    case 1:
                        mass = atof(token);
                        break;
                    case 2:
                        semi_major_axis = atof(token);
                        break;
                    case 3:
                        eccentricity = atof(token);
                        break;
                    case 4:
                        radius = atof(token);
                        break;
                    case 5:
                        color = (int32_t)strtol(token, NULL, 0);
                        break;
                }
            }
            // Create planet (or star)
            if (line_index == 0) {
                system.star = create_planet(mass, color, radius, semi_major_axis, eccentricity, true);
            } else {
                system.planets[line_index - 1] = create_planet(mass, color, radius, semi_major_axis, eccentricity, false);
            }
        }
        line_index++;
    }
    fclose(stream);
    return system;
}

void show_system(struct gfx_context_t *ctxt, system_t *system) {
    double maximum_radius = 0;
    double margin = 0.1;
    for (int32_t i = 0; i < system->nb_planets; i++) {
        if (system->planets[i].semi_major_axis > maximum_radius) {
            maximum_radius = system->planets[i].semi_major_axis;
        }
    }
    maximum_radius *= (1 + margin);
    for (int32_t i = -1; i < system->nb_planets; i++) {
            planet_t planet = i == -1 ? system->star : system->planets[i];
            coordinates coords = vec2_to_coordinates(vec2_mul(1.0 / maximum_radius, planet.pos), SCREEN_WIDTH, SCREEN_HEIGHT);
            draw_full_circle(ctxt, coords.column, coords.row, planet.radius, planet.color);
    }
}

void update_system(system_t *system, double delta_t) {

}

void free_system(system_t *system) {
    free(system->planets);
}