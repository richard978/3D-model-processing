#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMessageBox>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    bool activeRotate;
    bool activeScale;
    int keyRotate;
    int pos[6] = {1,1,1,1,1,1};
    float velx, vely, velz;
    float vel;
    float mid_x, mid_y, mid_z;
    float rotateX1, rotateY1, rotateX2, rotateY2;
    float scale_x, scale_y, scale_z;
    QImage tex;

    int readModel();
    void readTex();
    void setTex();
    void setLight();
    void getMedian();
    void setMode(int op);

    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);

private:
    GLuint texture;
    GLenum mode1, mode2;

    struct Points{
        float x, y, z;
    };
    QList<Points> v_arr;

    struct Normal{
        float x, y, z;
    };
    QList<Normal> vn_arr;

    struct Texture{
        float u, v, w;
    };
    QList<Texture> t_arr;

    struct attriFace{
        int v, t, vn;
    };
    QList<QList<attriFace> > f_arr;

    void paint3D(GLenum mode1, GLenum mode2);
};

#endif // OGLWIDGET_H
