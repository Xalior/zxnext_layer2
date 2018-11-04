/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Implementation of layer2_draw_pixel() in zxnext_layer2.h.
 ******************************************************************************/

#include <arch/zxn.h>
#include <z80.h>
#include <intrinsic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "zxnext_layer2.h"
#include "layer2_defs.h"
#include "layer2_common.h"


/**
 *
 *  Get pixel at X, Y
 *
 *
 *   TODO:
 *      Handle more than default screen
 *      Handle that screen may not be at MMU8
 *
 *
 * @param x
 * @param y
 * @param screen
 * @return
 */

uint8_t layer2_get_pixel(uint8_t x,
                       uint8_t y,
                       layer2_screen_t *screen)
{

    uint8_t color;
    if (y > 191)
    {
        return 0;
    }

    intrinsic_di();
    if (y < 32)
    {
        // top1
        z80_bpoke(0x50, 16);
    }
    else if (y < 64)
    {
        // top2
        z80_bpoke(0x50, 17);
        y -= 32;
    }
    else if (y < 76)
    {
        // middle1
        z80_bpoke(0x50, 18);
        y -= 64;
    }
    else if (y < 128)
    {
        // middle2
        z80_bpoke(0x50, 19);
        y -= 76;
    }
    else if (y < 160)
    {
        // bottom1
        z80_bpoke(0x50, 20);
        y -= 128;
    }
    if (y > 160)
    {
        // bottom2
        z80_bpoke(0x50, 21);
        y -= 160;
    }

    color = z80_bpeek(x + (y << 8));

    z80_bpoke(0x50, 255);
    intrinsic_ei();

    return color;
}
