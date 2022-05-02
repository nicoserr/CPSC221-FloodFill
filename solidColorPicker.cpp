#include "solidColorPicker.h"
#include <math.h>

/**
 * Constructs a new solidColorPicker.
 *
 * @param fillColor The color for this color picker.
 */
solidColorPicker::solidColorPicker(HSLAPixel fillColor)
{
    color = fillColor;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Simply returns the same color (used to construct the picker) all of the
 * time.
 */
HSLAPixel solidColorPicker::operator()(int x, int y)
{
    return color;
}
