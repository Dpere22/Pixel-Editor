#include "pixeldisplay.h"
#include "qevent.h"

/// Source code style checked by Jackson Heaps u1438591

/// @brief Pixel highlight color
const QColor HIGHLIGHT_COLOR(250, 250, 250, 125);

/// @brief Alternating background colors
const QColor PIXEL_ONE(80, 80, 80);
const QColor PIXEL_TWO(100, 100, 100);

/// @brief Alternating background pixel scaling
const int PIXEL_DISPLAY_SIZE = 1000;
const int PIXEL_DISPLAY_SCALE = 10 * PIXEL_DISPLAY_SIZE;

/// @brief Generates a static pixel display map for drawing visual aid
const QPixmap GeneratePixelDisplayAlternatingPattern()
{
    QPixmap pattern(PIXEL_DISPLAY_SIZE, PIXEL_DISPLAY_SIZE);
    pattern.fill(Qt::transparent);
    QPainter painter(&pattern);
    for (int i = 0; i < PIXEL_DISPLAY_SIZE; i++) {
        for (int j = 0; j < PIXEL_DISPLAY_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                painter.fillRect(QRect(i, j, 1, 1), PIXEL_ONE);
            } else {
                painter.fillRect(QRect(i, j, 1, 1), PIXEL_TWO);
            }
        }
    }
    return pattern.scaled(PIXEL_DISPLAY_SCALE, PIXEL_DISPLAY_SCALE);
};

PixelDisplay::PixelDisplay(QWidget *parent)
    : QFrame(parent)
    , frameIndex(0)
    , mousePixelX(-1)
    , mousePixelY(-1)
    , backgroundImage(GeneratePixelDisplayAlternatingPattern())
{}

int PixelDisplay::pixelsWidth()
{
    if (frame.size() == 0) {
        return 1;
    }
    return frame.at(0).size();
}

int PixelDisplay::pixelsHeight()
{
    if (frame.size() == 0) {
        return 1;
    }
    return frame.size();
}

void PixelDisplay::resizeEvent(QResizeEvent *)
{
    if (frame.isEmpty()) {
        return;
    }

    int rheight = rect().height();
    int rwidth = rect().width();
    int pheight = pixelsHeight();
    int pwidth = pixelsWidth();
    int scale = pheight < pwidth ? rheight / pheight : rwidth / pwidth;

    // constrain dimensions to container
    if (pheight * scale > rheight) {
        scale = rheight / pheight;
    }

    if (pwidth * scale > rwidth) {
        scale = rwidth / pwidth;
    }

    resize(pwidth * scale, pheight * scale);
    setMinimumHeight(pheight * 1);
    setMinimumWidth(pwidth * 1);
}

void PixelDisplay::paintEvent(QPaintEvent *)
{
    if (frame.isEmpty()) {
        return;
    }

    QPixmap image(frame.at(0).size(), frame.size());
    image.fill(Qt::transparent);
    QPainter imagePainter(&image);
    int x = 0, y = 0;
    for (const QVector<QColor> &pixelRow : frame) {
        x = 0;
        for (const QColor &pixelColor : pixelRow) {
            if (pixelColor != Qt::transparent) {
                imagePainter.fillRect(QRect(x, y, 1, 1), pixelColor);
            }
            x++;
        }
        y++;
    }

    QPainter mainPainter(this);

    // background checkerboard
    mainPainter.drawPixmap(0, 0, backgroundImage);
    mainPainter.drawPixmap(0, 0, image.scaled(width(), height()));

    // mouse pixel highlight
    if (mousePixelX == -1 || mousePixelY == -1) {
        return;
    }
    QPixmap imageOverlay(frame.at(0).size(), frame.size());
    imageOverlay.fill(Qt::transparent);
    QPainter imageOverlayPainter(&imageOverlay);
    imageOverlayPainter.fillRect(QRect(mousePixelX, mousePixelY, 1, 1), HIGHLIGHT_COLOR);
    mainPainter.drawPixmap(0, 0, imageOverlay.scaled(width(), height()));

    QResizeEvent resizer{size(), size()};

    resizeEvent(&resizer);
}

void PixelDisplay::updateFrameIndex(int index)
{
    frameIndex = index;
    emit imageUpdated(index);
}

void PixelDisplay::updateImage(int index, const PixelFrame &newFrame)
{
    if (index != frameIndex) {
        return;
    }
    frame = newFrame;
    update();
}
