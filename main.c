#include "gfx/gfx.h"
#include "vec2/vec2.h"
#include "planet/planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DELTA_T 15000 // Not working when dynamic

// TODO Refactoring
// TODO Add comments
// TODO Do report

int main()
{
    struct gfx_context_t *ctxt =
        gfx_create("Planetary system", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ctxt)
    {
        fprintf(stderr, "Graphics initialization failed!\n");
        return EXIT_FAILURE;
    }

    system_t system = create_system(DELTA_T);

    while (true)
    {
        
        gfx_present(ctxt);
        update_system(&system, DELTA_T);
        gfx_clear(ctxt, COLOR_BLACK);
        if (gfx_keypressed() == SDLK_ESCAPE)
        {
            break;
        }
        show_system(ctxt, &system);
    }

    free_system(&system);
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
