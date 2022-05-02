#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    color = gridColor;
    spacing = gridSpacing;
    white = HSLAPixel();
}

HSLAPixel gridColorPicker::operator()(int x, int y)
{
    if (x%spacing == 0 || y%spacing == 0) {
        return color;
    } else
        return white;
}
