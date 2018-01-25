#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    activeRotate = 1;
    activeScale = 1;
    keyRotate = 0;
    scale_x=scale_y=scale_z=1;
    vel=velx=vely=velz=0;
    mode1 = GL_TRIANGLES;
    mode2 = GL_POLYGON;
}

OGLWidget::~OGLWidget()
{

}

int OGLWidget::readModel()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Open Model"), "", tr("Object Files(*.obj)"));
    QFile obj(filename);
    if(!obj.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 1;
    }
    vn_arr.clear();v_arr.clear();t_arr.clear();f_arr.clear();
    while(!obj.atEnd()){
        QString line = obj.readLine();
        if(line[0] == 'v'){
            if(line[1] == 'n'){
                QList<QString> tmpL = line.split(' ',QString::SkipEmptyParts);
                Normal tmpN;
                tmpN.x = tmpL[1].toFloat();
                tmpN.y = tmpL[2].toFloat();
                tmpN.z = tmpL[3].toFloat();
                vn_arr.push_back(tmpN);
            }
            else if(line[1] == 't'){
                QList<QString> tmpL = line.split(' ',QString::SkipEmptyParts);
                Texture tmpT;
                tmpT.u = tmpL[1].toFloat();
                tmpT.v = tmpL[2].toFloat();
                tmpT.w = tmpL[3].toFloat();
                t_arr.push_back(tmpT);
            }
            else{
                QList<QString> tmpL = line.split(' ',QString::SkipEmptyParts);
                Points tmpP;
                tmpP.x = tmpL[1].toFloat();
                tmpP.y = tmpL[2].toFloat();
                tmpP.z = tmpL[3].toFloat();
                v_arr.push_back(tmpP);
            }
        }
        else if(line[0] == 'f'){
            if(line.contains("/")){
                QList<QString> tmpL = line.split(' ',QString::SkipEmptyParts);
                QList<attriFace> faceT;
                for(int i=1;i<tmpL.length()-1;i++){
                    QList<QString> tmpLL = tmpL[i].split('/',QString::SkipEmptyParts);
                    attriFace tmpAf;
                    tmpAf.v = tmpLL[0].toInt();
                    tmpAf.t = tmpLL[1].toInt();
                    tmpAf.vn = tmpLL[2].toInt();
                    faceT.push_back(tmpAf);
                }
                f_arr.push_back(faceT);
            }
            else{
                QMessageBox message(QMessageBox::Warning, tr("警告"), tr("文件内容不支持，仅限茶壶模型。"), QMessageBox::Yes | QMessageBox::No, NULL);
                if(message.exec()==QMessageBox::Yes)
                    return 1;
                else
                    return 0;
                break;
            }
        }
    }
    getMedian();
    return 0;
}

void OGLWidget::readTex()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images(*.bmp *.JPG *.JEPG *.PNG)"));
    QImage buf;
    if(!buf.load(filename)){
        return;
    }
    tex = QGLWidget::convertToGLFormat(buf);
}

void OGLWidget::setTex()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void OGLWidget::setLight()
{
    GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat lightPosition1[4] = { 0.0, 2.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);

    GLfloat lightPosition2[4] = { 0.0, -2.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);

    GLfloat lightPosition3[4] = { -2.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);

    GLfloat lightPosition4[4] = { 2.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4);

    GLfloat lightPosition5[4] = { 0.0, 0.0, -2.0, 1.0 };
    glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT5, GL_POSITION, lightPosition5);

    GLfloat lightPosition6[4] = { 0.0, 0.0, 2.0, 1.0 };
    glLightfv(GL_LIGHT6, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT6, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT6, GL_POSITION, lightPosition6);

    if(pos[0])
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    if(pos[1])
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);
    if(pos[2])
        glEnable(GL_LIGHT3);
    else
        glDisable(GL_LIGHT3);
    if(pos[3])
        glEnable(GL_LIGHT4);
    else
        glDisable(GL_LIGHT4);
    if(pos[4])
        glEnable(GL_LIGHT5);
    else
        glDisable(GL_LIGHT5);
    if(pos[5])
        glEnable(GL_LIGHT6);
    else
        glDisable(GL_LIGHT6);

    glEnable(GL_LIGHTING);
}

void OGLWidget::getMedian()
{
    float max_x=INT_MIN, max_y=INT_MIN, max_z=INT_MIN;
    for(int i=0;i<v_arr.length();i++){
        if(v_arr[i].x>max_x)
            max_x=v_arr[i].x;
        if(v_arr[i].y>max_y)
            max_y=v_arr[i].y;
        if(v_arr[i].z>max_z)
            max_z=v_arr[i].z;
    }
    float min_x=INT_MAX, min_y=INT_MAX, min_z=INT_MAX;
    for(int i=0;i<v_arr.length();i++){
        if(v_arr[i].x<min_x)
            min_x=v_arr[i].x;
        if(v_arr[i].y<min_y)
            min_y=v_arr[i].y;
        if(v_arr[i].z<min_z)
            min_z=v_arr[i].z;
    }
    mid_x = (max_x+min_x)/2;
    mid_y = (max_y+min_y)/2;
    mid_z = (max_z+min_z)/2;
}

void OGLWidget::setMode(int op)
{
    switch(op){
    case 1:
        mode1 = GL_POINTS;
        mode2 = GL_POINTS;
        break;
    case 2:
        mode1 = GL_LINE_STRIP;
        mode2 = GL_LINE_STRIP;
        break;
    case 3:
        mode1 = GL_TRIANGLES;
        mode2 = GL_POLYGON;
        break;
    }
    update();
}

void OGLWidget::paint3D(GLenum mode1, GLenum mode2)
{
    glTranslatef(-mid_x/50, -mid_y/50, -mid_z/50);
    if(activeRotate){
        glTranslatef(mid_x/50, mid_y/50, mid_z/50);
        if(keyRotate!=0){
            switch(keyRotate){
            case 1:
                glRotatef(vel, 1, 0, 0);
                break;
            case 2:
                glRotatef(vel, -1, 0, 0);
                break;
            case 3:
                glRotatef(vel, 0, -1, 0);
                break;
            case 4:
                glRotatef(vel, 0, 1, 0);
                break;
            default:
                break;
            }
        }
        else{
            glRotatef(vely, 1, 0, 0);
            glRotatef(velx, 0, -1, 0);
            glRotatef(velz, 0, 0, 1);
        }
        glTranslatef(-mid_x/50, -mid_y/50, -mid_z/50);
    }
    if(activeScale){
        glTranslatef(mid_x/(50/(1-scale_x)), mid_y/(50/(1-scale_y)), mid_z/(50/(1-scale_z)));
        glScalef(scale_x, scale_y, scale_z);
    }
    for(int i=0;i<f_arr.length();i++){
        if(f_arr[i].length()==3){
            glBegin(mode1);
            for(int j=0;j<3;j++){
                glNormal3f(vn_arr[f_arr[i][j].vn-1].x,vn_arr[f_arr[i][j].vn-1].y,vn_arr[f_arr[i][j].vn-1].z);
                glTexCoord2f(t_arr[f_arr[i][j].t-1].u,t_arr[f_arr[i][j].t-1].v);
                glVertex3f(v_arr[f_arr[i][j].v-1].x/50,v_arr[f_arr[i][j].v-1].y/50,v_arr[f_arr[i][j].v-1].z/50);
            }
            glEnd();
        }
        else{
            glBegin(mode2);
            for(int j=0;j<4;j++){
                glNormal3f(vn_arr[f_arr[i][j].vn-1].x,vn_arr[f_arr[i][j].vn-1].y,vn_arr[f_arr[i][j].vn-1].z);
                glTexCoord2f(t_arr[f_arr[i][j].t-1].u,t_arr[f_arr[i][j].t-1].v);
                glVertex3f(v_arr[f_arr[i][j].v-1].x/50,v_arr[f_arr[i][j].v-1].y/50,v_arr[f_arr[i][j].v-1].z/50);
            }
            glEnd();
        }
    }
}

void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1,1,1);
    setLight();
    setTex();
    paint3D(mode1, mode2);
}

void OGLWidget::resizeGL(int w, int h)
{
    if(0 == h)
        h = 1;
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(activeRotate){
        if(event->button() == Qt::LeftButton){
            int wid = width()/2;
            int hei = height()/2;
            rotateX1 = static_cast<float>(event->x()-wid)/wid;
            rotateY1 = static_cast<float>((2*hei-event->y())-hei)/hei;
        }
    }
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(activeRotate){
        keyRotate = 0;
        int wid = width()/2;
        int hei = height()/2;
        rotateX2 = static_cast<float>(event->x()-wid)/wid;
        rotateY2 = static_cast<float>((2*hei-event->y())-hei)/hei;
        velx+=100*(rotateX2-rotateX1);
        vely+=100*(rotateY2-rotateY1);
        if(velx>360) velx=0;
        if(vely>360) vely=0;
        update();
        rotateX1 = static_cast<float>(event->x()-wid)/wid;
        rotateY1 = static_cast<float>((2*hei-event->y())-hei)/hei;
    }
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void OGLWidget::wheelEvent(QWheelEvent *event)
{
    if(activeScale){
        if(event->delta()>0){
            scale_x+=0.1;
            scale_y+=0.1;
            scale_z+=0.1;
        }
        else{
            scale_x-=0.1;
            scale_y-=0.1;
            scale_z-=0.1;
        }
        update();
    }
}

void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(activeRotate){
        vel+=3;
        if(vel>360) vel=0;
        switch(event->key()){
        case Qt::Key_Up:
            keyRotate = 1;
            break;
        case Qt::Key_Down:
            keyRotate = 2;
            break;
        case Qt::Key_Right:
            keyRotate = 3;
            break;
        case Qt::Key_Left:
            keyRotate = 4;
            break;
        }
        update();
    }
}

