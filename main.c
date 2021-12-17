#include "gfx/gfx.h"
#include "vec2/vec2.h"
#include "planet/planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FPS 10

int main()
{
    srand(time(NULL));
    struct gfx_context_t *ctxt =
        gfx_create("Planetary system", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ctxt)
    {
        fprintf(stderr, "Graphics initialization failed!\n");
        return EXIT_FAILURE;
    }

    time_t last_update_time;
    time(&last_update_time);
    system_t system = create_system(0);

    while (true)
    {
        
        gfx_present(ctxt);
        time_t current_time;
        time(&current_time);
        double ellasped_time_milliseconds = 1000 * (current_time - last_update_time);
        last_update_time = current_time;
        update_system(&system, ellasped_time_milliseconds);
        gfx_clear(ctxt, COLOR_BLACK);
        if (gfx_keypressed() == SDLK_ESCAPE)
        {
            break;
        }
        show_system(ctxt, &system);
        usleep(1000000 / FPS);
    }

    free_system(&system);
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
