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

#define PLANETES_CSV_PATH "./planetes.csv"
#define PLANETES_CSV_HEADERS true
#define PLANETES_CSV_SEPARATOR ";"
#define PLANETES_CSV_ROW_MAX_LENGTH 1024

static vec2 compute_velocity(system_t *system, planet_t planet) {
    return vec2_mul(
            sqrt((G * system->star.mass * (1 + planet.eccentricity)) / (planet.semi_major_axis * (1 - planet.eccentricity))),
            vec2_normalize(vec2_create(-planet.pos.y, -planet.pos.x))
            );
}

static vec2 compute_acceleration(system_t *system, planet_t planet, int32_t planet_index) {
    vec2 f_res = vec2_create_zero();
    for (int32_t i = -1; i < system->nb_planets; i++) {
        if (i != planet_index) {
            planet_t planet_b = i == -1 ? system->star : system->planets[i];
            vec2 rab = vec2_sub(planet_b.pos, planet.pos);
            vec2 fab = vec2_mul(G * ((planet.mass * planet_b.mass) / pow(vec2_norm(rab), 3)), rab);
            f_res = vec2_add(f_res, fab);
        }
    }
    return vec2_mul(1 / planet.mass, f_res);
}

static vec2 compute_initial_position(system_t *system, planet_t planet, int32_t planet_index, double delta_t) {
    vec2 velocity = compute_velocity(system, planet);
    vec2 acceleration = compute_acceleration(system, planet, planet_index);
    return vec2_add(vec2_add(planet.pos, vec2_mul(delta_t, velocity)), vec2_mul(pow(delta_t, 2) / 2, acceleration));
}

static vec2 compute_next_position(system_t *system, planet_t planet, int32_t planet_index, double delta_t) {
    vec2 acceleration = compute_acceleration(system, planet, planet_index);
    return vec2_add(vec2_sub(vec2_mul(2, planet.pos), planet.prec_pos), vec2_mul(pow(delta_t, 2), acceleration));
}

planet_t create_planet(double mass, int color, double radius, double semi_major_axis, double eccentricity, bool is_star) {
    planet_t planet = { //attribution de tous les paramètres
        .mass = mass,
        .color = color,
        .radius = radius,
        .semi_major_axis = semi_major_axis,
        .eccentricity = eccentricity,
    };
    if (is_star) { //Si c'est une étoile on le place au milieu donc vecteur 0
        planet.prec_pos = planet.pos = vec2_create_zero();
    } else { //Sinon on le place par rapport à sa distance au soleil(étoile) et son excentricité
        planet.prec_pos = planet.pos = vec2_create(semi_major_axis * (1 - eccentricity), 0);
    } //La planète est aligné horizontalement au soleil
    return planet;
}

static void create_planetes_from_csv(system_t *system) {
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
    system->planets = malloc(sizeof(planet_t) * nb_planetes);
    system->nb_planets = nb_planetes;
    // Reset stream
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
                system->star = create_planet(mass, color, radius, semi_major_axis, eccentricity, true);
            } else {
                system->planets[line_index - 1] = create_planet(mass, color, radius, semi_major_axis, eccentricity, false);
            }
        }
        line_index++;
    }
    fclose(stream);
}

system_t create_system(double delta_t) {
    system_t system;
    create_planetes_from_csv(&system);
    for (int32_t i = 0; i < system.nb_planets; i++) {
        system.planets[i].pos = compute_initial_position(&system, system.planets[i], i, delta_t);
    }
    return system;
}

void show_system(struct gfx_context_t *ctxt, system_t *system) {
    double maximum_radius = 0;
    double margin = 0.25;
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
    for (int32_t i = 0; i < system->nb_planets; i++) {
        vec2 current_position = system->planets[i].pos;
        system->planets[i].pos = compute_next_position(system, system->planets[i], i, delta_t);
        system->planets[i].prec_pos = current_position;
    }
}

void free_system(system_t *system) {
    free(system->planets);
}