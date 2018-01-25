#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniBtn()
{
    ui->choseBtn->setStyleSheet("background-color: white");
    ui->choseBtn->setDown(false);
    ui->transBtn->setStyleSheet("background-color: white");
    ui->transBtn->setDown(false);
    ui->rotBtn->setStyleSheet("background-color: white");
    ui->rotBtn->setDown(false);
    ui->rstBtn->setStyleSheet("background-color: white");
    ui->rstBtn->setDown(false);
    ui->dotBtn->setStyleSheet("background-color: white");
    ui->dotBtn->setDown(false);
    ui->lineBtn->setStyleSheet("background-color: white");
    ui->lineBtn->setDown(false);
    ui->faceBtn->setStyleSheet("background-color: white");
    ui->faceBtn->setDown(false);
}

void MainWindow::on_choseBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->chooseMode(2);
    ui->choseBtn->setDown(true);
    ui->choseBtn->setStyleSheet("background-color: rgb(0,191,255)");
}

void MainWindow::on_transBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->chooseMode(3);
    ui->transBtn->setDown(true);
    ui->transBtn->setStyleSheet("background-color: rgb(0,191,255)");
}

void MainWindow::on_rotBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->chooseMode(1);
    ui->rotBtn->setDown(true);
    ui->rotBtn->setStyleSheet("background-color: rgb(0,191,255)");
}

void MainWindow::on_action_O_triggered()
{
    ui->openGLWidget->readModel();
    ui->openGLWidget->update();
}

void MainWindow::on_rstBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->reset();
    ui->openGLWidget->update();
}

void MainWindow::on_action_S_triggered()
{
    ui->openGLWidget->saveModel();
}

void MainWindow::on_dotBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->setMode(1);
    ui->dotBtn->setDown(true);
    ui->dotBtn->setStyleSheet("background-color: rgb(0,191,255)");
}

void MainWindow::on_lineBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->setMode(2);
    ui->lineBtn->setDown(true);
    ui->lineBtn->setStyleSheet("background-color: rgb(0,191,255)");
}

void MainWindow::on_faceBtn_clicked()
{
    iniBtn();
    ui->openGLWidget->setMode(3);
    ui->faceBtn->setDown(true);
    ui->faceBtn->setStyleSheet("background-color: rgb(0,191,255)");
}
