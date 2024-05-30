#include "pixelgrid.h"

/// Source code style checked by William Yavornitzky u1381262

PixelGrid::PixelGrid(QWidget *parent)
    : PixelDisplay(parent)
{
    setMouseTracking(true);
}

void PixelGrid::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        emit drawActionCompleted(frame);
    }
}

void PixelGrid::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().x() >= width() || event->pos().x() <= 0) {
        return;
    } else if (event->buttons().testFlag(Qt::LeftButton)) {
        emit pixelLeftClicked(event->pos(), rect());
    } else if (event->buttons().testFlag(Qt::RightButton)) {
        emit pixelRightClicked(event->pos(), rect());
    } else {
        return;
    }
    emit imageUpdated(frameIndex);
}

void PixelGrid::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::NoButton)) {
        int pixelX = event->pos().x() * pixelsWidth() / rect().width();
        int pixelY = event->pos().y() * pixelsHeight() / rect().height();
        if (pixelX < 0 || pixelY < 0 || pixelX >= pixelsWidth() || pixelY >= pixelsHeight()) {
            // Nothing
        } else {
            mousePixelX = pixelX;
            mousePixelY = pixelY;
        }
        update();
    } else {
        mousePixelX = -1;
        mousePixelY = -1;
        mousePressEvent(event);
    }
}
