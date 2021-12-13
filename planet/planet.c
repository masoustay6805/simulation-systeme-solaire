#include "planet.h"
#include "../vec2/vec2.h"
#include <stdlib.h>
#include <inttypes.h>

#define G 6.67e-11

#define SUN_M 1.989e30
#define SUN_X 0
#define SUN_Y 0

#define MERCURY_M 3.3022e23
#define MERCURY_X 0.1
#define MERCURY_Y 0    

#define VENUS_M 4.8685e24
#define VENUS_X 0.2
#define VENUS_Y 0

#define EARTH_M 3.5838e25
#define EARTH_X 0.3
#define EARTH_Y 0

#define MARS_M 6.4185e23
#define MARS_X 0.4
#define MARS_Y 0

#define NB_PLANETS 4

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

planet_t create_planet(double mass, vec2 pos) {
    planet_t planet = {
      .mass = mass,
      .pos = pos
    };
    return planet;
}

system_t create_system(double delta_t) {
    system_t system = {
            .planets = malloc(sizeof(planet_t) * NB_PLANETS),
            .nb_planets = NB_PLANETS
    };
    system.star = create_planet(SUN_M, vec2_create(SUN_X, SUN_Y));
    system.planets[0] = create_planet(MERCURY_M, vec2_create(MERCURY_X, MERCURY_Y));
    system.planets[1] = create_planet(VENUS_M, vec2_create(VENUS_X, VENUS_Y));
    system.planets[2] = create_planet(EARTH_M, vec2_create(EARTH_X, EARTH_Y));
    system.planets[3] = create_planet(MARS_M, vec2_create(MARS_X, MARS_Y));
    return system;
}

void show_system(struct gfx_context_t *ctxt, system_t *system) {
    for (int32_t i = -1; i<NB_PLANETS; i++) {
        planet_t planet = i == -1 ? system->star : system->planets[i];
        if(i==-1){
            printf("\nposition mercure = %lf", planet.pos.x);
            printf("\nposition mercure row= %lf", planet.pos.y);
        }
        coordinates coords = vec2_to_coordinates(planet.pos, SCREEN_WIDTH, SCREEN_HEIGHT);
        if(i==-1){
            printf("\ncoordonnées mercure = %d", coords.column);
            printf("\ncoordonnées mercure row= %d", coords.row);
        }
        draw_full_circle(ctxt, coords.column, coords.row, 10, COLOR_YELLOW);
    }
}

void update_system(system_t *system, double delta_t) {

}

void free_system(system_t *system) {
    free(system->planets);
}