#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include <QMessageBox>

/// Source code style checked by Gabe Eralie u1344808

const int FRAMESELECTOR_FIXED_HEIGHT = 90;

MainWindow::MainWindow(Model &modelInput, QWidget *parent)
    : QMainWindow(parent)
    , model(&modelInput)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pixeldisplay, &PixelDisplay::imageUpdated, model, &Model::sendFrameUpdate);

    connect(ui->pixelgrid, &PixelGrid::pixelLeftClicked, model, &Model::storePixel);
    connect(ui->pixelgrid, &PixelGrid::pixelRightClicked, model, &Model::removePixel);
    connect(ui->pixelgrid, &PixelDisplay::imageUpdated, model, &Model::sendFrameUpdate);
    connect(ui->pixelgrid, &PixelGrid::drawActionCompleted, model, &Model::sendToUndoStack);

    connect(ui->undobutton, &QPushButton::clicked, model, &Model::undo);
    connect(ui->redobutton, &QPushButton::clicked, model, &Model::redo);
    connect(ui->clearButton, &QPushButton::clicked, model, &Model::clearFrame);
    connect(ui->colorbutton, &QPushButton::clicked, this, &MainWindow::pickColor);
    connect(ui->saveButton, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->loadButton, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui->exportButton, &QAction::triggered, this, &MainWindow::exportFile);
    connect(ui->newButton, &QAction::triggered, model, &Model::openNew);

    connect(this, &MainWindow::fileSaved, model, &Model::saveToJson);
    connect(this, &MainWindow::fileLoaded, model, &Model::loadFromJson);
    connect(this, &MainWindow::colorPicked, model, &Model::changeColor);
    connect(this, &MainWindow::fileExported, model, &Model::exportToPNG);
    connect(this, &MainWindow::makeNewFile, model, &Model::openNew);
    connect(this, &MainWindow::toggleIntent, model, &Model::doesntWantSave);

    connect(model, &Model::frameUpdated, ui->pixelgrid, &PixelDisplay::updateImage);
    connect(model, &Model::frameUpdated, ui->pixeldisplay, &PixelDisplay::updateImage);
    connect(model, &Model::checkSave, this, &MainWindow::checkingSave);
    connect(model, &Model::resetSpriteWindow, this, &MainWindow::resetWindow);

    connect(model, &Model::frameAmountUpdated, this, &MainWindow::updateFrameSelectors);
    connect(model, &Model::currentFrameUpdated, this, &MainWindow::updateCurrentFrameSelector);

    connect(ui->addframebutton, &QPushButton::clicked, model, &Model::addFrame);
    connect(ui->deleteframebutton, &QPushButton::clicked, model, &Model::deleteFrame);
    connect(ui->copyframebutton, &QPushButton::clicked, model, &Model::copyFrame);

    connect(ui->slidetoggleplay, &QPushButton::clicked, model, &Model::toggleTimer);
    connect(ui->slidespeedslider, &QSlider::valueChanged, model, &Model::setAnimationInterval);

    connect(model, &Model::timerToggled, ui->slidetoggleplay, &QPushButton::setChecked);
    connect(model, &Model::animationFrameSent, ui->pixeldisplay, &PixelDisplay::updateFrameIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCurrentFrameSelector(int index)
{
    auto fBoxContent = ui->frameboxcontent;

    // framebox childrens
    auto fBoxChs = fBoxContent->findChildren<QPushButton *>(QString(), Qt::FindDirectChildrenOnly);
    int size = fBoxChs.size();

    // set whether each active frame box is the current selected box
    // simulates a radio button group
    for (int i = 0; i < size; i++) {
        fBoxChs.at(i)->setChecked(index == i);
    }

    ui->pixeldisplay->updateFrameIndex(index);
    ui->pixelgrid->updateFrameIndex(index);
}

void MainWindow::updateFrameSelectors(int numFrames)
{
    auto fBoxContent = ui->frameboxcontent;

    // framebox childrens
    auto fBoxChs = fBoxContent->findChildren<QPushButton *>(QString(), Qt::FindDirectChildrenOnly);
    int size = fBoxChs.size();

    auto fBoxLayout = fBoxContent->layout();

    // really make sure the frames are kaputsky (this means dead)
    while (fBoxLayout->takeAt(0) != 0) {
        for (int i = 0; i < size; i++) {
            delete fBoxChs[i];
        }
    }

    // create a new frame box for each active frame
    for (int i = 0; i < numFrames; i++) {
        auto boxButton = new QPushButton();
        auto boxLayout = new QHBoxLayout(boxButton);
        auto pxDisplay = new PixelDisplay();

        fBoxLayout->addWidget(boxButton);
        boxLayout->addWidget(pxDisplay);

        connect(boxButton, &QPushButton::clicked, model, [=] { model->updateCurrentFrame(i); });
        boxButton->setCheckable(true);

        connect(pxDisplay, &PixelDisplay::imageUpdated, model, &Model::sendFrameUpdate);
        connect(model, &Model::frameUpdated, pxDisplay, &PixelDisplay::updateImage);
        pxDisplay->updateFrameIndex(i);
        boxButton->setFixedHeight(FRAMESELECTOR_FIXED_HEIGHT);
    }

    updateCurrentFrameSelector(model->getCurrentFrameIndex());
}

void MainWindow::pickColor()
{
    QColor color = QColorDialog::getColor();
    emit colorPicked(color);
}

void MainWindow::resetWindow(){
    ui->setupUi(this);
}

void MainWindow::saveFile()
{
    // fileName is an exact path
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.ssp", tr("*.ssp"));
    emit fileSaved(fileName);
}

void MainWindow::exportFile()
{
    // fileName is an exact path
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "untitled.png", tr("*.png"));
    emit fileExported(fileName);
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), tr("*.ssp"));
    emit fileLoaded(fileName);
}

void MainWindow::checkingSave()
{
    int userInput = QMessageBox::warning(this,
                                         tr("My Application"),
                                         tr("The document has been modified.\n"
                                            "Do you want to save your changes?"),
                                         QMessageBox::Save | QMessageBox::Discard
                                             | QMessageBox::Cancel,
                                         QMessageBox::Save);

    if(userInput != QMessageBox::StandardButton::Cancel)
    {
        if (userInput == QMessageBox::StandardButton::Save)
        {
            saveFile();
        } else {
            emit toggleIntent();
        }
        emit makeNewFile();
    }
}
