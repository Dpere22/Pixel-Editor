#include "colorpicker.h"

colorPicker::colorPicker()
    : QColorDialog()
{
    colorPicker::color = getColor(Qt::white, this);
    emit colorSelected(color);
}
