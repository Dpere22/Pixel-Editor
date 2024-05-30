#ifndef STARTUPQUESTIONS_H
#define STARTUPQUESTIONS_H

/// Source code style checked by William Yavornitzky u1381262
#include "model.h"

#include <QDialog>
///
/// @brief Class for the dialog window that comes up before the main application
///
/// Intended to either load a project from the users computer
/// or to create a new project with the user defined dimensions
///
/// This class must point to the model which is given as a parameter fed in by main
///
namespace Ui {
class StartupQuestions;
}

class StartupQuestions : public QDialog
{
    Q_OBJECT

public:
    explicit StartupQuestions(Model &model, QWidget *parent = nullptr);
    ~StartupQuestions();
    Model* model;
    const int MAX_WIDTH;
    const int MAX_HEIGHT;
    const int MIN_WIDTH;
    const int MIN_HEIGHT;
public slots:
    /// @brief call this slot to check if the the user has entered a valid dimension, if they have application will open
    void checkValidDimensions();
    /// @brief call this slot to open a filepicker and load a file from that filepicker which will open the application
    void loadFile();
signals:
    //this signal is used to quit this startup window and go to the application window
    void goToApplication();
    //this signal is used to let the model know there is a file to be loaded
    void fileLoaded(const QString &);
private:
    Ui::StartupQuestions *ui;
};

#endif // STARTUPQUESTIONS_H
