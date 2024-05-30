#ifndef PIXELGRID_H
#define PIXELGRID_H

/// Source code style checked by William Yavornitzky u1381262

#include <QColor>
#include <QMouseEvent>
#include <QPaintEvent>
#include "pixeldisplay.h"

/// @brief Governs user interface for editing pixels on grid
///
/// Receives input from user to notify Model of a desired change.
/// Input from user is received via mouse input.
///
/// @authors Goobers team
class PixelGrid : public PixelDisplay
{
    Q_OBJECT

public:
    /// @brief Constructs with parent widget like parent class
    explicit PixelGrid(QWidget *parent = nullptr);

    /// @brief Override to receive mouse input from user on key release
    void mouseReleaseEvent(QMouseEvent *);

    /// @brief Override to receive mouse input from user on key press
    void mousePressEvent(QMouseEvent *);

    /// @brief Override to receive mouse input from user on key press + move
    void mouseMoveEvent(QMouseEvent *);

signals:
    /// @brief Notifies that a draw action has been completed
    void drawActionCompleted(const PixelFrame &);

    /// @brief Notifies that left mouse has been pressed
    void pixelLeftClicked(const QPoint &, const QRect &);

    /// @brief Notifies that Right mouse has been pressed
    void pixelRightClicked(const QPoint &, const QRect &);
};

#endif // PIXELGRID_H
