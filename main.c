#include "gfx/gfx.h"
#include "vec2/vec2.h"
#include "planet/planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

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

    // TODO : create your system

    while (true)
    {
        gfx_present(ctxt);
        // TODO : draw the current state of your system
        // TODO : update your system
        gfx_clear(ctxt, COLOR_BLACK);
        if (gfx_keypressed() == SDLK_ESCAPE)
        {
            break;
        }
    }

    // TODO : Free your system
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
