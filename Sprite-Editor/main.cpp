#include "mainwindow.h"

/// Source code style checked by William Yavornitzky u1381262

#include <QApplication>

/// @brief Application for creating pixel sprites
///
/// @author Goobers team
/// @author Diego Perez u1301598
/// @author Gabe Eralie u1344808
/// @author Jackson Heaps u1438591
/// @author William Yavornitzky u1381262
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    StartupQuestions start(model);
    MainWindow w(model);
    //if not accepted user exited the startup dialog or an error occured
    if(start.exec() == QDialog::Accepted){
        w.show();
        return a.exec();
    }
    else return 0;
}
