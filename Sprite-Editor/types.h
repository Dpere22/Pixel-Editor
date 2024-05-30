#ifndef TYPES_H
#define TYPES_H

/// Source code style checked by William Yavornitzky u1381262

#include <QColor>

/// @brief Contextualized definition for simplified specification
///
/// Represents row of pixels: QVector<QColor>
///
/// Represents rows: QVector<QVector<QColor>>
///
/// @authors Goobers team
typedef QVector<QVector<QColor>> PixelFrame;

#endif // TYPES_H
