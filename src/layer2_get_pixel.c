#include <arch/zxn.h>
#include <z80.h>
#include <stdint.h>
#include <intrinsic.h>

uint8_t layer2_get_pixel(uint8_t x, uint8_t y)
{
    uint8_t page;
    uint8_t color;

    if (y > 191)
    {
        return 0;
    }

    // Disable any Interrupts that may be going on, in case they need ROM
    intrinsic_di();

    // Determine the layer-2 MMU page of the pixel (one of six pages).
    page = (ZXN_READ_REG(REG_LAYER_2_RAM_BANK) << 1) + (y / 32);

    // Page in the required layer-2 MMU page into MMU slot 0.
    ZXN_WRITE_MMU0(page);

    // For flexibility, the working area MMU slot could be passed in as a parameter and then
    // use ZXN_WRITE_REG() to page in the required layer-2 MMU page into the given MMU slot.
    // In this case, we could also restore the original page in that MMU slot at the end of
    // this function if the caller expects that.
    //ZXN_WRITE_REG(REG_MMU0 + mmu_slot, page);

    // Calculate the address of the pixel in its layer-2 MMU page and get the value.
    color = z80_bpeek(x + ((y % 32) << 8));

    // Return ROM to MMU0
    ZXN_WRITE_MMU0(255);

    // Reenable IM2
    intrinsic_ei();

    // Return the value of the pixel.
    return color;
}
