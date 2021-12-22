#ifndef _PLANET_H_
#define _PLANET_H_

#include "../vec2/vec2.h"
#include "../gfx/gfx.h"

typedef struct _planet
{
    double mass; //masse de la planète
    int32_t color; //couleur sur la simulation
    double radius; //Grandeur de la planète 
    double semi_major_axis; //Distance par rapport au soleil
    double eccentricity; //excentricité
    vec2 pos;      // x(t)
    vec2 prec_pos; // x(t - dt)
} planet_t;

typedef struct _system
{
    planet_t star;       // ex. The sun
    int32_t nb_planets; // The number of orbiting planets
    planet_t *planets;   // An array of orbiting planets
} system_t;

// Those function are not mandatory to implement,
// it's rather a hint of what you should have.
planet_t create_planet(double mass, int color, double radius, double semi_major_axis, double eccentricity, bool is_star);
system_t create_system(double delta_t);
void show_system(struct gfx_context_t *ctxt, system_t *system);
void update_system(system_t *system, double delta_t);
void free_system(system_t *system);
vec2 compute_next_position(system_t *system, planet_t planet, int32_t planet_index, double delta_t);
vec2 compute_initial_position(system_t *system, planet_t planet, int32_t planet_index, double delta_t);
vec2 compute_acceleration(system_t *system, planet_t planet, int32_t planet_index);
vec2 compute_velocity(system_t *system, planet_t planet);

#endif