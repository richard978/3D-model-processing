#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    activeRotate = 0;
    activeTrans = 0;
    activeTranScale = 0;
    velx=vely=velz=0;
    mid_x=mid_y=mid_z=0;
    scale_x=scale_y=scale_z=1;
    mode = GL_POLYGON;
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::chooseMode(int op)
{
    switch(op){
    case 1:
        activeRotate = 1;
        activeTrans = 0;
        activeTranScale = 0;
        break;
    case 2:
        activeRotate = 0;
        activeTrans = 0;
        activeTranScale = 1;
        break;
    case 3:
        activeRotate = 0;
        activeTrans = 1;
        activeTranScale = 0;
        break;
    }
    update();
}

float OGLWidget::getDist(float posX, float posY)
{
    float tailx = -0.7;
    float taily = 0.35;
    float dis1 = (posX-mid_x)*(posX-mid_x)+(posY-mid_y)*(posY-mid_y);
    float dis2 = (posX-tailx)*(posX-tailx)+(posY-taily)*(posY-taily);
    return sqrt(qMin(dis1,dis2));
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
    if(mid_x==mid_y==mid_z==0){
        mid_x = (max_x+min_x)/2;
        mid_y = (max_y+min_y)/2;
        mid_z = (max_z+min_z)/2;
        float tmp = qMax(fabs(max_x-min_x),fabs(max_y-min_y));
        length = 0.5*qMax(tmp, (float)fabs(max_z-min_z));
        length *= 1.2;
        mid_x /= length;
        mid_y /= length;
        mid_z /= length;
    }
    mid_x = mid_y = mid_z = 0;
}

void OGLWidget::reset()
{
    v_arr = ori_v_arr;
    vn_arr = ori_vn_arr;
    f_arr = ori_f_arr;
    t_arr = ori_t_arr;
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
    vn_arr.clear();t_arr.clear();v_arr.clear();f_arr.clear();
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
                QMessageBox::warning(NULL, tr("警告"), tr("文件内容不支持，仅限牛模型"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                return 1;
                break;
            }
            else{
                QList<QString> tmpL = line.split(' ',QString::SkipEmptyParts);
                attriFace tmpAf;
                tmpAf.v1 = tmpL[1].toInt();
                tmpAf.v2 = tmpL[2].toInt();
                tmpAf.v3 = tmpL[3].toInt();
                f_arr.push_back(tmpAf);
            }
        }
    }
    ori_v_arr = v_arr;
    ori_vn_arr = vn_arr;
    ori_f_arr = f_arr;
    ori_t_arr = t_arr;

    getMedian();
    for(int i=0;i<v_arr.length()+1;i++)
        assigned.push_back(0);
    return 0;
}

void OGLWidget::readTex()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("BMP Files(*.bmp);;JPG Files(*.JPG);;JEPG Files(*.JEPG);;PNG Files(*.PNG)"));
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

void OGLWidget::saveModel()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Model"), "", tr("Object Files(*.obj)"));
    QFile saveObj(filename);
    if(!saveObj.open(QIODevice::ReadWrite|QIODevice::Text))
        return;
    QTextStream in(&saveObj);
    for(int i=0;i<v_arr.length();i++){
        in<<"v "<<QString("%1").arg(v_arr[i].x)<<" "<<QString("%1").arg(v_arr[i].y)<<" "<<QString("%1").arg(v_arr[i].z)<<"\n";
    }
    for(int i=0;i<f_arr.length();i++){
        in<<"f "<<QString("%1").arg(f_arr[i].v1)<<" "<<QString("%1").arg(f_arr[i].v2)<<" "<<QString("%1").arg(f_arr[i].v3)<<"\n";
    }
}

void OGLWidget::transform(float movX, float movY)
{
    float dis = INT_MAX;
    for(int i=0;i<v_arr.length();i++){
        if(assigned[i] == 1){
            float temp = getDist(v_arr[i].x/length,v_arr[i].y/length);
            if(dis>temp)
                dis = temp;
        }
    }
    for(int i=0;i<v_arr.length();i++){
        if(assigned[i] == 1){
            float temp = getDist(v_arr[i].x/length,v_arr[i].y/length);
            v_arr[i].x+=(temp-dis)*500*movX;
            v_arr[i].y+=(temp-dis)*500*movY;
        }
    }
    update();
}

void OGLWidget::setLight()
{
    GLfloat lightAmbient[4] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat lightPosition[4] = { 0.3, 0.6, 0.6, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void OGLWidget::setMode(int op)
{
    switch(op){
    case 1:
        mode = GL_POINTS;
        break;
    case 2:
        mode = GL_LINE_STRIP;
        break;
    case 3:
        mode = GL_POLYGON;
        break;
    }
    update();
}

void OGLWidget::paint3D(GLenum mode)
{
    glTranslatef(-mid_x, -mid_y, -mid_z);
    if(activeRotate){
        glScalef(scale_x, scale_y, scale_z);
        glRotatef(vely, 1, 0, 0);
        glRotatef(velx, 0, -1, 0);
    }
    for(int i=0;i<f_arr.length();i++){
        glBegin(mode);
        glVertex3f(v_arr[f_arr[i].v1-1].x/length,v_arr[f_arr[i].v1-1].y/length,v_arr[f_arr[i].v1-1].z/length);
        glVertex3f(v_arr[f_arr[i].v2-1].x/length,v_arr[f_arr[i].v2-1].y/length,v_arr[f_arr[i].v2-1].z/length);
        glVertex3f(v_arr[f_arr[i].v3-1].x/length,v_arr[f_arr[i].v3-1].y/length,v_arr[f_arr[i].v3-1].z/length);
        glEnd();
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
    paint3D(mode);
    if(activeTranScale){
        glLineStipple(1, 0x0F0F);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);
        glVertex2d(tranScaleX1,tranScaleY1);
        glVertex2d(tranScaleX3,tranScaleY1);
        glVertex2d(tranScaleX3,tranScaleY3);
        glVertex2d(tranScaleX1,tranScaleY3);
        glEnd();
    }
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
    if(activeTrans){
        if(event->button() == Qt::LeftButton){
            int wid = width()/2;
            int hei = height()/2;
            tranX1 = static_cast<float>(event->x()-wid)/wid;
            tranY1 = static_cast<float>((2*hei-event->y())-hei)/hei;
        }
    }
    if(activeTranScale){
        if(event->button() == Qt::LeftButton){
            int wid = width()/2;
            int hei = height()/2;
            tranScaleX1 = static_cast<float>(event->x()-wid)/wid;
            tranScaleY1 = static_cast<float>((2*hei-event->y())-hei)/hei;
            for(int i=0;i<v_arr.length();i++)
                if(assigned[i] == 1)
                    assigned[i] = 0;
        }
    }
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
   if(activeTranScale){
        if(event->button() == Qt::LeftButton){
            int wid = width()/2;
            int hei = height()/2;
            tranScaleX2 = static_cast<float>(event->x()-wid)/wid;
            tranScaleY2 = static_cast<float>((2*hei-event->y())-hei)/hei;
            for(int i=0;i<v_arr.length();i++)
                if(v_arr[i].x/length>qMin(tranScaleX1,tranScaleX2)&&v_arr[i].x/length<qMax(tranScaleX1,tranScaleX2)
                        &&v_arr[i].y/length>qMin(tranScaleY1,tranScaleY2)&&v_arr[i].y/length<qMax(tranScaleY1,tranScaleY2))
                    assigned[i] = 1;
        }
    }
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(activeRotate){
        int wid = width()/2;
        int hei = height()/2;
        rotateX2 = static_cast<float>(event->x()-wid)/wid;
        rotateY2 = static_cast<float>((2*hei-event->y())-hei)/hei;
        velx+=100*(rotateX2-rotateX1);
        vely+=100*(rotateY2-rotateY1);
        update();
        rotateX1 = rotateX2;
        rotateY1 = rotateY2;
    }
    if(activeTrans){
        int wid = width()/2;
        int hei = height()/2;
        tranX2 = static_cast<float>(event->x()-wid)/wid;
        tranY2 = static_cast<float>((2*hei-event->y())-hei)/hei;
        transform((tranX2-tranX1), (tranY2-tranY1));
        tranX1 = tranX2;
        tranY1 = tranY2;
    }
    if(activeTranScale){
        int wid = width()/2;
        int hei = height()/2;
        tranScaleX3 = static_cast<float>(event->x()-wid)/wid;
        tranScaleY3 = static_cast<float>((2*hei-event->y())-hei)/hei;
        update();
    }
}

void OGLWidget::wheelEvent(QWheelEvent *event)
{
    if(activeRotate){
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
