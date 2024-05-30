#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QColor>
#include <QColorDialog>

class colorPicker : public QColorDialog
{
    QColor color;

public:
    explicit colorPicker();
};

#endif // COLORPICKER_H
