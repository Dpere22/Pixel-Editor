
/// Source code style checked by William Yavornitzky u1381262
#include "startupquestions.h"
#include "ui_startupquestions.h"
#include <QFileDialog>
#include "model.h"

StartupQuestions::StartupQuestions(Model &modelIntake, QWidget *parent)
    : QDialog(parent)
    , MAX_WIDTH(128)
    , MAX_HEIGHT(128)
    , MIN_WIDTH(32)
    , MIN_HEIGHT(32)
    , ui(new Ui::StartupQuestions)
{
    ui->setupUi(this);
    model = &modelIntake;
    connect(ui->createButton, &QAbstractButton::clicked,this, &StartupQuestions::checkValidDimensions);
    connect(this, &StartupQuestions::goToApplication, this, &QDialog::accept);
    connect(ui->loadButton, &QAbstractButton::clicked, this, &StartupQuestions::loadFile);
    connect(this, &StartupQuestions::fileLoaded, model, &Model::loadFromJson);
}

StartupQuestions::~StartupQuestions()
{
    delete ui;
}
void StartupQuestions::checkValidDimensions(){
    int width = ui->widthEntry->text().toInt();
    int height = ui->heightEntry->text().toInt();
    if(width < MIN_WIDTH || width > MAX_WIDTH || height < MIN_HEIGHT || height > MAX_HEIGHT){
        QString ErrorMSG = "Width and Height must be in the correct range!";
        ui->ErrorLabel->setText(ErrorMSG);
    }
    else{
        model -> finishSetup(width, height);
        emit goToApplication();
    }
}
void StartupQuestions::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), tr("*.ssp"));
    emit fileLoaded(fileName);
    emit goToApplication();
}
