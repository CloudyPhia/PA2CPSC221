#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    color = gridColor;
    spacing = gridSpacing;
}

HSLAPixel gridColorPicker::operator()(int x, int y)
{
    if (x % spacing == 0 || y % spacing == 0) {
        return color;
    } else {
        HSLAPixel retCol;
        retCol.h = 0; retCol.s = 0; retCol.l = 1;
        return retCol;
    }
}
