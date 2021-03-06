/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Implementation of layer2_blit_off_screen_row() and
 * layer2_blit_off_screen_sub_row() in zxnext_layer2.h.
 ******************************************************************************/

#include <arch/zxn.h>
#include <z80.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "zxnext_layer2.h"
#include "layer2_common.h"

void layer2_blit_off_screen_sub_row(uint8_t dest_x,
                                    uint8_t dest_y,
                                    layer2_screen_t *source,
                                    uint8_t source_x,
                                    uint8_t source_y,
                                    uint16_t width)
{
    uint8_t *dest_addr;
    uint8_t *source_addr;

    if ((dest_y > 191) || (source_y > 191) || (width == 0) ||
        (source == NULL) || (source->screen_type != OFF_SCREEN))
    {
        return;
    }

    if (dest_x + width - 1 > 255)
    {
        width = 255 - dest_x + 1;
    }

    if (source_x + width - 1 > 255)
    {
        width = 255 - source_x + 1;
    }

    if (dest_y < 64)
    {
        // top
        layer2_configure(true, true, false, LAYER2_SCREEN_TOP);
    }
    else if (dest_y < 128)
    {
        // middle
        layer2_configure(true, true, false, LAYER2_SCREEN_MIDDLE);
        dest_y -= 64;
    }
    else
    {
        // bottom
        layer2_configure(true, true, false, LAYER2_SCREEN_BOTTOM);
        dest_y -= 128;
    }

    source->tmp = z80_bpeek(__SYSVAR_BANKM);

    if (source_y < 64)
    {
        // top
        switch_ram_bank(source->top_bank);
    }
    else if (source_y < 128)
    {
        // middle
        switch_ram_bank(source->middle_bank);
        source_y -= 64;
    }
    else
    {
        // bottom
        switch_ram_bank(source->bottom_bank);
        source_y -= 128;
    }

    dest_addr = (uint8_t *) (dest_x + (dest_y << 8));
    source_addr = (uint8_t *) (0xC000 + source_x + (source_y << 8));

    memcpy(dest_addr, source_addr, width);

    layer2_configure(true, false, false, 0);
    switch_ram_bank(source->tmp);
}
