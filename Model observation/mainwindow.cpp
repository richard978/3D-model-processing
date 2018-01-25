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

void MainWindow::on_action_obj_O_triggered()
{
    ui->openGLWidget->readModel();
    ui->openGLWidget->update();
}

void MainWindow::on_action_R_triggered()
{
    ui->openGLWidget->readTex();
    ui->openGLWidget->update();
}

void MainWindow::on_UpBtn_clicked()
{
    ui->openGLWidget->pos[0] = !ui->openGLWidget->pos[0];
    ui->openGLWidget->update();
}

void MainWindow::on_DownBtn_clicked()
{
    ui->openGLWidget->pos[1] = !ui->openGLWidget->pos[1];
    ui->openGLWidget->update();
}

void MainWindow::on_LeftBtn_clicked()
{
    ui->openGLWidget->pos[2] = !ui->openGLWidget->pos[2];
    ui->openGLWidget->update();
}

void MainWindow::on_RightBtn_clicked()
{
    ui->openGLWidget->pos[3] = !ui->openGLWidget->pos[3];
    ui->openGLWidget->update();
}

void MainWindow::on_FrontBtn_clicked()
{
    ui->openGLWidget->pos[4] = !ui->openGLWidget->pos[4];
    ui->openGLWidget->update();
}

void MainWindow::on_BehindBtn_clicked()
{
    ui->openGLWidget->pos[5] = !ui->openGLWidget->pos[5];
    ui->openGLWidget->update();
}

void MainWindow::on_DotBtn_clicked()
{
    ui->openGLWidget->setMode(1);
}

void MainWindow::on_LineBtn_clicked()
{
    ui->openGLWidget->setMode(2);
}

void MainWindow::on_FaceBtn_clicked()
{
    ui->openGLWidget->setMode(3);
}
