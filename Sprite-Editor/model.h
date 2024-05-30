#ifndef MODEL_H
#define MODEL_H

/// Source code style checked by William Yavornitzky u1381262

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include "types.h"

/// @brief Stores and operates on application data
///
/// Receives input signals from View/Controller and adjusts data
/// accordingly. Sends signals back to the View/Controller once
/// data has been modified.
///
/// @authors Goobers team
class Model : public QObject
{
    Q_OBJECT

    /// @brief Timer class for animation signals
    QTimer animationTimer;

    /// @brief Current animation frame being sent
    int currentAnimationFrame;

    /// @brief Current animation frame speed
    int currentAnimationSpeed;

    /// @brief current frame being edited
    int currentFrame;

    /// @brief current color being used to draw
    QColor currentColor;

    /// @brief contains pixel frame data
    QVector<PixelFrame> frames;

    /// @brief contains states of pixel frames after subsequent operations
    QVector<PixelFrame> undoStack;

    /// @brief stores "redo" states after undo button used
    QVector<PixelFrame> redoStack;

    /// @brief stores whether or not the current thing has been saved
    bool hasBeenSaved;

    /// @brief stores the filename currently being used to save the the file
    QString currFileName;

    /// @brief keeps track of whether or not the file is currently saving.
    bool isSaving;

    /// @brief Used data loss. Keeps track of whether or not the user wants to save the thing
    bool wantsToSave;

public:
    /// @brief default constructor
    explicit Model(QObject *parent = nullptr);
    /// @brief height of pixel image frames (in pixels)
    int pixelsHeight;

    /// @brief width of pixel image frames (in pixels)
    int pixelsWidth;


    /// @brief Returns current index of active frame
    int getCurrentFrameIndex();

private:
    /// @brief determines location of pixel based on input point within
    /// container rectangle. updates pixel at location to be provided
    /// color.
    void updatePixel(const QPoint &, const QRect &, const QColor &);

public slots:

    /// @brief Sets animation interval for slideshow display
    void setAnimationInterval(int);

    /// @brief Toggles timer for animation slideshow display
    void toggleTimer();

    /// @brief adds a frame and changes current frame to index of new frame.
    void addFrame();

    /// @brief Deletes the currently selected frame.
    void deleteFrame();

    /// @brief Copies the currently selected frame.
    void copyFrame();

    /// @brief undoes previous action.
    void undo();

    /// @brief reverses undo action.
    void redo();

    /// @brief Changes current color.
    void changeColor(const QColor &);

    /// @brief sends a frame state to the undo stack.
    void sendToUndoStack(const PixelFrame &);

    /// @brief Gets frame at index and emits signal to update that frame
    void sendFrameUpdate(int);

    /// @brief Sets current frame to index of frame received
    void updateCurrentFrame(int);

    /// @brief updates corresponding pixel to be current color
    void storePixel(const QPoint &, const QRect &);

    /// @brief updates corresponding pixel to be transparent
    void removePixel(const QPoint &, const QRect &);

    /// @brief changes drawing size
    void resizePixels(int height, int width);

    /// @brief Saves current image frame data to file
    void saveToJson(const QString &);

    /// @brief Loads current image frame data from file
    void loadFromJson(const QString &);

    /// @brief Exports the current frame data to a png.
    void exportToPNG(const QString &filename);

    /// @brief Resets the canvas if the file is saved. If not, signal to ask if the user wants to save the file.
    void openNew();

    /// @brief toggles wantsToSave
    void doesntWantSave();

    /// @brief Finishes setup (this refers to when the application starts)
    void finishSetup(int width, int height);

    void clearFrame();

signals:
    /// @brief Notifies animation frame change
    void animationFrameSent(int);

    /// @brief Notifies timer toggled
    void timerToggled(bool);

    /// @brief Notifies current selected frame being changed
    void currentFrameUpdated(int);

    /// @brief Notifies all display widgets new frame added
    void frameAmountUpdated(int);

    /// @brief notifies all display widgets of change in pixel frame at index
    void frameUpdated(int, const PixelFrame &);

    /// @brief Notifies of need for a save check
    void checkSave();

    /// @brief Notifies need to reset the window.
    void resetSpriteWindow();
};

#endif // MODEL_H
