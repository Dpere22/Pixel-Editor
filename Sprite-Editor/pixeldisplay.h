#ifndef PIXELDISPLAY_H
#define PIXELDISPLAY_H

/// Source code style checked by Jackson Heaps u1438591

#include <QFrame>
#include <QPainter>
#include <QPixmap>
#include "types.h"

/// @brief Visually displays a pixel frame
///
/// Receives input signals from Model and adjusts displayed
/// image accordingly.
///
/// @authors Goobers team
class PixelDisplay : public QFrame
{
    Q_OBJECT

protected:
    /// @brief Assigned frame index to display
    int frameIndex;

    /// @brief Most recently received frame data
    PixelFrame frame;

    /// @brief Stored mouse X position
    int mousePixelX;

    /// @brief Stored mouse Y position
    int mousePixelY;

    /// @brief Returns height of Frame Data
    int pixelsWidth();

    /// @brief Returns width of Frame Data
    int pixelsHeight();

    /// @brief Background image data
    QPixmap backgroundImage;

public:
    /// @brief Constructs with parent
    explicit PixelDisplay(QWidget *parent = nullptr);

    /// @brief Returns the frame index
    int getFrameIndex();

    /// @brief Method override to execute paint update
    void paintEvent(QPaintEvent *);

    /// @brief Method override to resize pixel display window
    void resizeEvent(QResizeEvent *);

    /// @brief Send signal to request most recent frame data
    void updateFrameIndex(int);

public slots:
    /// @brief Update current frame data
    void updateImage(int index, const PixelFrame &);

signals:
    /// @brief Request most recent frame data
    void imageUpdated(int);
};

#endif // PIXELDISPLAY_H
