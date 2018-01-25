#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
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
#include <QTextStream>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    bool activeRotate;
    bool activeTrans;
    bool activeTranScale;
    QList<int> assigned;
    float velx, vely, velz;
    float mid_x, mid_y, mid_z;
    float rotateX1, rotateY1, rotateX2, rotateY2;
    float tranX1,tranX2,tranX3,tranY1,tranY2,tranY3;
    float tranScaleX1,tranScaleY1,tranScaleX2,tranScaleY2,tranScaleX3,tranScaleY3;
    float scale_x, scale_y, scale_z;
    float length;
    QImage tex;

    int readModel();
    void saveModel();
    void readTex();
    void setTex();
    void getMedian();
    float getDist(float posX, float posY);
    void transform(float movX, float movY);
    void chooseMode(int op);
    void reset();
    void setLight();
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

private:
    GLenum mode;
    GLuint texture;

    struct Points{
        float x, y, z;
    };
    QList<Points> v_arr;
    QList<Points> ori_v_arr;

    struct Normal{
        float x, y, z;
    };
    QList<Normal> vn_arr;
    QList<Normal> ori_vn_arr;

    struct Texture{
        float u, v, w;
    };
    QList<Texture> t_arr;
    QList<Texture> ori_t_arr;

    struct attriFace{
        int v1, v2, v3;
    };
    QList<attriFace> f_arr;
    QList<attriFace> ori_f_arr;

    void paint3D(GLenum mode);
};

#endif // OGLWIDGET_H
