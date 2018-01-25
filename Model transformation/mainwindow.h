#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void iniBtn();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_choseBtn_clicked();

    void on_transBtn_clicked();

    void on_rotBtn_clicked();

    void on_action_O_triggered();

    void on_rstBtn_clicked();

    void on_action_S_triggered();

    void on_dotBtn_clicked();

    void on_lineBtn_clicked();

    void on_faceBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
