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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_obj_O_triggered();

    void on_action_R_triggered();

    void on_UpBtn_clicked();

    void on_DownBtn_clicked();

    void on_LeftBtn_clicked();

    void on_RightBtn_clicked();

    void on_FrontBtn_clicked();

    void on_BehindBtn_clicked();

    void on_DotBtn_clicked();

    void on_LineBtn_clicked();

    void on_FaceBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
