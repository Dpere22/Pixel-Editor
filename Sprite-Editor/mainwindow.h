#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/// Source code style checked by Gabe Eralie u1344808

#include <QColorDialog>
#include <QMainWindow>
#include "model.h"
#include "startupquestions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/// @brief Container for ui elements
///
/// @authors Goobers team
class MainWindow : public QMainWindow
{
    Q_OBJECT

    /// @brief Reference to model
    Model *model;

public:
    /// @brief reference to model in constructor for signal connection
    MainWindow(Model &model, QWidget *parent = nullptr);

    /// @brief destructor
    ~MainWindow();

private:
    /// @brief application window UI
    Ui::MainWindow *ui;

public slots:
    /// @brief Updates visual current frame selection
    void updateCurrentFrameSelector(int);

    /// @brief Updates frame selector scroll area
    void updateFrameSelectors(int);

    /// @brief Opens save file dialog and receives name
    void saveFile();

    /// @brief Opens load file dialog and receives file
    void loadFile();

    /// @brief Opens color pick dialog and receives color
    void pickColor();

    /// @brief Opens load file dialog and receives a filename.
    void exportFile();

    /// @brief Checks with the user to see if they want to save the file before an action.
    void checkingSave();

    /// @brief Resets the window.
    void resetWindow();

signals:

    /// @brief Notifies image file save
    void fileSaved(const QString &);

    /// @brief Notifies image file load
    void fileLoaded(const QString &);

    /// @brief Notifies color picked
    void colorPicked(const QColor &);

    /// @brief Notifies png file save;
    void fileExported(const QString &);

    /// @brief Notifies to start eh process of making a new file
    void makeNewFile();

    /// @brief toggles wantsToSave in the model.
    void toggleIntent();
};

#endif // MAINWINDOW_H
