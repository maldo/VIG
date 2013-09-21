#include "glwidget.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <algorithm>
#include <fstream>


static inline double rad2deg(double rad)
{
    return (rad * 180.0) / M_PI;
}

static inline double deg2rad(double deg)
{
    return (deg * M_PI) / 180.0;
}

GLWidget::GLWidget(QWidget *parent) :
        QGLWidget(parent)
{
    allowMovement = false;
    DoingView = S;    
}

void GLWidget::initializeGL()
{
    glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);//zbuffer
    glEnable(GL_CULL_FACE);//partes ocultas
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    allowLlum = true;
    allowPE = false;
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    emit buttonLlum(true);
    emit buttonBackFaceCulling(true);
    emit buttonZbuffer(true);

    /**
      Leer la carpeta, sacar un vector string con las rutas a las imagenes
      cambiar los jpg por la extension del modelo y ponerlos en la scene cargados en lmodels

      */

    vector<string> img;

    vector<string> objs;

    string ruta = "../imatges";

    carga(ruta, img, objs);

    imgs.setThis(this);

    //cout << img.size() << endl;

    imgs.setListaImg(img);

    scene.Init(objs);

    cameraInit();
}

int GLWidget::carga(string dir, vector<string> &img, vector<string> &objs)
{
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(dir.c_str())) == NULL)
    {
        //cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        string pic = dir;

        pic.append("/");

        string o = "../models/";

        string name = string(dirp->d_name);

        int i = name.find(".");

        string nom = name.substr(0,i+1);
        string ext = name.substr(i+1,name.size());

        if (!ext.compare("jpg"))
        {
            img.push_back(pic.append(name));

            name = nom.append("obj");

            objs.push_back(o.append(name));
        }
    }

    closedir(dp);
    return 0;
}

void GLWidget::cameraInit()
{
    double radio;
    Point centro;

    scene.calcularEsfera(radio,centro);

    VRP = centro;

    dist = 2*radio;
    anglecam = rad2deg(2 * asin(radio/(2*radio)));
    //cout << "anglecam: " << anglecam << endl;
    //anglecam = 60;
    anterior = radio;
    posterior = 3*radio;

    angleX=-15;
    angleY=30;
    angleZ=0;

    panX=0;
    panY=0;
}

void GLWidget::setModelView()
{
    float m[4][4];

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -dist);
    glRotatef(-angleZ,0,0,1);
    glRotatef(-angleX,1,0,0);
    glRotatef(-angleY,0,1,0);
    glTranslatef(-VRP.x, -VRP.y, -VRP.z);


    glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);
    Point s(m[0][0], m[1][0], m[2][0]);
    Point u(m[0][1], m[1][1], m[2][1]);

    // Fer pan sobre el pla
    s = Point(s.x * panX,s.y * panX,s.z * panX);
    u = Point(u.x * panY, u.y * panY, u.z * panY);
    glTranslatef(-s.x, -s.y, -s.z);
    glTranslatef(-u.x, -u.y, -u.z);

}

void GLWidget::setProjection()
{
    QSize size = this->size();
    double radio = dist/2;
    GLfloat ra = (GLfloat) size.width() / (GLfloat) size.height();
    fov = anglecam;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (ra < 1)
    {
        GLfloat a = fov / 2;
        a = rad2deg(atan(tan(deg2rad(a))/ra));
        fov = 2.0 * a;
    }

    gluPerspective(fov, ra, dist - radio, radio + dist);
}

void GLWidget::resizeGL (int width, int height)
{
    glViewport (0, 0, width, height);
}

void GLWidget::paintGL(void)
{
     QSize s = this->size();
    if (allowPE)
    {
        planElevation();
    }
    else
    {
        // Esborrem els buffers

        glViewport(0, 0, s.width(), s.height());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setProjection();

        if (allowLlum)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            scene.RenderLuces(light::OBSERVADOR);
        }

        setModelView();

        if (allowLlum)
        {
            scene.RenderLuces(light::ESCENA);
        }

        // dibuixar eixos aplicacio
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
        glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
        glEnd();

        if (allowLlum)
        {
            glEnable(GL_LIGHTING);
        }

        scene.Render(allowLlum);
    }
}

void GLWidget::planElevation()
{
    QSize s = this->size();

    GLfloat ra = (GLfloat) (s.width()/2) / (GLfloat) s.height();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glViewport(0, 0, s.width()/2, s.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-7, 7, -7/ra, 7/ra, -dist/2, dist+dist/2);

    glRotatef(180,0,1,0);
    glRotatef(-90,1,0,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
    glEnd();

    if (allowLlum)
    {
        glEnable(GL_LIGHTING);
    }

    scene.Render(allowLlum);

    glViewport(s.width()/2, 0, s.width()/2, s.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-7.0, 7.0, -7/ra, 7/ra, -dist/2, dist+dist/2);

    glRotatef(180,0,1,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
    glEnd();

    if (allowLlum)
    {
        glEnable(GL_LIGHTING);
    }

    scene.Render(allowLlum);

}

void GLWidget::PlanElevation(bool xxx)
{
    allowPE = xxx;

    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    xClick = e->x();
    yClick = e->y();

    if (e->button()&Qt::LeftButton &&
        ! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
    {
        if (!allowPE) DoingInteractive = ROTATE;
    }
    else if(e->modifiers()&Qt::ShiftModifier && e->modifiers()&Qt::ControlModifier && e->button()&Qt::LeftButton)
    {
        DoingInteractive = MOVING;
    }
    else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
    {
        if (!allowPE) DoingInteractive = ZOOM;
    }
    else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ControlModifier)
    {
        if (!allowPE)  DoingInteractive = PAN;
    }
    else if (e->button()&Qt::RightButton)
    {
        if (scene.colisiona())
        {
            allowMovement = true; //no haria falta pero para mayor claridad
            // cout<<"colisiona"<< allowMovement<<endl;
        }
        else
        {
            allowMovement = false;
            scene.refreshPosOriginal();
            scene.borrarIndices();
            emit buttonLoad(true);
            updateGL();
        }
        //cout<<"permitir mov "<< allowMovement<<endl;
    }
    else if (e->button()&Qt::MidButton)
    {
        seleccionar(xClick, yClick);
        updateGL();
    }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_F:

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        updateGL();

        break;
    case Qt::Key_S:

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        updateGL();

        break;
    case Qt::Key_X:

        if (allowMovement)
        {
            if (e->modifiers()&Qt::ShiftModifier)
            {
                scene.moveX(5.00/25);
            }
            else
            {
                scene.moveX(-5.00/25);
            }
            updateGL();
        }
        break;
    case Qt::Key_Z:

        if (allowMovement)
        {
            if (e->modifiers()&Qt::ShiftModifier)
            {
                scene.moveZ(5.00/25);
            }
            else
            {
                scene.moveZ(-5.00/25);
            }
            updateGL();
        }
        break;
    case Qt::Key_Y:

        if (allowMovement)
        {
            scene.moveY(-5.00/25);
        }

        updateGL();
        break;
    case Qt::Key_J:
        scene.montarSU();
        updateGL();
        break;
    case Qt::Key_U:

        scene.montarU();
        updateGL();
        break;
    case Qt::Key_D:

        scene.girarD();
        updateGL();
        break;
     case Qt::Key_E:

        scene.girarI();
        updateGL();
        break;
     case Qt::Key_Escape:

        if (allowMovement)
        {
            scene.setObjectBack();
            allowMovement = scene.colisiona();
            if (!allowMovement)
            {
                scene.refreshPosOriginal();
                emit buttonLoad(true);
            }
            updateGL();
        }
        break;
    case Qt::Key_Delete:

        borrar();
        updateGL();

        break;
    case Qt::Key_K:

        if (e->modifiers()&Qt::ShiftModifier)
        {
            scene.scaleX(0.1);
        }
        else
        {
            scene.scaleX(-0.1);
        }
        updateGL();

        break;
    case Qt::Key_L:

        if (e->modifiers()&Qt::ShiftModifier)
        {
            scene.scaleY(0.1);
        }
        else
        {
            scene.scaleY(-0.1);
        }
        updateGL();

        break;
    case Qt::Key_O:

        if (e->modifiers()&Qt::ShiftModifier)
        {
            scene.scaleZ(0.1);
        }
        else
        {
            scene.scaleZ(-0.1);
        }
        updateGL();

        break;

    default: e->ignore();
    }
}

void GLWidget::mouseReleaseEvent( QMouseEvent *)
{
    DoingInteractive = NONE;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int xPos = e->x();
    int yPos = e->y();
    int xDist = xPos - xClick;
    int yDist = yPos - yClick;

    if (DoingInteractive == ROTATE)
    {
        angleY -= xDist;
        angleX += yDist;
    }
    else if (DoingInteractive == ZOOM)
    {
        anglecam += yDist / 2; //Menys velocitat
        if (anglecam < 0)
        {
            anglecam = 0;
        }
        else if (anglecam > 180)
        {
            anglecam = 180;
        }

    }
    else if (DoingInteractive==PAN)
    {
        panX -= xDist / 2;
        panY += yDist / 2;

    }
    else if (DoingInteractive==MOVING)
    {
        Point posNX,posNZ,posNF;

        if (allowMovement)
        {
            float m[4][4];
            glGetFloatv(GL_MODELVIEW_MATRIX,&m[0][0]);
            Point Xobs(m[0][0],0,m[2][0]);
            Point Zobs((m[1][0]*m[2][2])-(m[2][0]*m[1][2]),0,(m[0][0]*m[1][2])-(m[1][0]*m[0][2]));

            posNX = (Xobs * (e->x() - xClick));
            posNZ = (Zobs * -(e->y() - yClick));
            posNF += posNX-posNZ;

            scene.RenderMoviment(posNF.x/20,posNF.z/20);
            updateGL();
        }
    }
    xClick = xPos;
    yClick = yPos;

    updateGL();

}

/*
 donat (x,y)
 1. pintar tots els objectes amb colors diferents
 glflush
 gldisable(GL_lighting)
 2. detectar quin color te el pixel(x,y)
 3. color pixel -> id de lobjecte
 4. pintar normalment

 pintar los colores en plan (1,0,0) luego (2,0,0) glcolors3ub
 glreadpixels(1,1, llegir GL_red)
*/

void GLWidget::seleccionar(int x, int y)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    GLubyte pixel[1];

    scene.RenderSeleccio();
    glFlush();
    glReadPixels(x, size().height() - y, 1 , 1, GL_RED, GL_UNSIGNED_BYTE, pixel);

    allowMovement = scene.validaPixel(pixel[0]);

    if (allowMovement)
    {
        scene.setIndice(pixel[0]);
    }

    if (allowLlum)
    {
        glEnable(GL_LIGHTING);
    }

    scene.Render(allowLlum);
}

void GLWidget::borrar()
{
    scene.borrarObjectes();
}

void GLWidget::ResetCamera()
{
    cameraInit();
    updateGL();
}

void GLWidget::SetSolid()
{
    DoingView = S;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    updateGL();
}

void GLWidget::SetWireframe()
{
    DoingView = W;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    updateGL();
}

void GLWidget::Save()
{
    QString s = QFileDialog::getSaveFileName(this, tr("Guardar como..."), "../untitled.vig",
                                             tr("VIG (*.vig)"));

    QFile file(s);

    file.open(QIODevice::WriteOnly);

    QDataStream out(&file);

    out << scene;

    file.close();
}

void GLWidget::Load()
{
    QString cosa = QFileDialog::getOpenFileName(this, tr("Open File"), "../", tr("VIG (*.vig)"));

    if (cosa !="")
    {
        QFile file(cosa);

        file.open(QIODevice::ReadOnly);

        QDataStream in(&file);

        in >> scene;

        cout << scene.numObjectes()<<endl;

        updateGL();
    }
}


void GLWidget::LoadObject()
{
    QString folder = "../models";

    QString model = QFileDialog::getOpenFileName(this, tr("Open File"), folder, tr("Objectes (*.obj)"));
    if (model != "")
    {
        QStringList l = model.split("/");

        folder.append("/");

        QString filename = folder.append(l[l.size()-1]);

        scene.InitObjecte(filename.toAscii().constData());
        allowMovement = true;
    }

    emit buttonLoad(false);
    updateGL();
}

void GLWidget::LoadObjectFromImage(string i)
{
    QString img = QString(i.data());

    QString folder = "../models/";

    QStringList l = img.split("/");

    QString name = l[l.size()-1];

    QStringList n = name.split(".");

    QString noext = n[0];

    name = noext.append(".obj");

    QString filename = folder.append(name);

    //cout<<filename.toAscii().constData() << endl;

    scene.InitObjecte(filename.toAscii().constData());
    allowMovement = true;

    imgs.close();

    updateGL();
}

void GLWidget::LoadImagenes()
{
    imgs.show();
}


void GLWidget::Lighting(bool xxx)
{   
    if (xxx)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
    allowLlum = xxx;
    updateGL();
}

void GLWidget::BackFaceCulling(bool xxx)
{
    if (xxx)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    updateGL();
}

void GLWidget::Zbuffer(bool xxx)
{
    if (xxx)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    updateGL();
}

void GLWidget::ChangeMaterial(bool xxx)
{
    if (xxx)
    {
        if(scene.isSelected())
        {
             mm.show();
             mm.setThis(this);

             mm.currentka(scene.getCurrentMaterialAmbiente());
             mm.currentkd(scene.getCurrentMaterialDifuso());
             mm.currentks(scene.getCurrentMaterialEspecular());
             mm.currentbrillo(scene.getCurrentMaterialBrillo());
        }
        else
        {
            emit buttonMaterial(false);
        }
    }
    else
    {
        mm.close();
    }
}

void GLWidget::setAmbiente(float *ka)
{
    scene.setAmbiente(ka);
    updateGL();
}

void GLWidget::setDifusa(float *kd)
{
   scene.setDifusa(kd);
   updateGL();
}

void GLWidget::setEspecular(float *ks)
{
   scene.setEspecular(ks);
   updateGL();
}

void GLWidget::setBrillo(float b)
{
    scene.setBrillo(b);
    updateGL();
}

void GLWidget::ChangeLuces(bool xxx)
{
    if (xxx)
    {
        l.show();
        l.setThis(this);
        l.getId(0);
    }
    else
    {
        l.close();
    }
}

void GLWidget::setFocoColor(int id, float *color)
{
    scene.setFocoColor(id,color);

    updateGL();
}

float* GLWidget::getFocoColor(int id)
{
    return scene.getFocoColor(id);
}

void GLWidget::setFocoAltura(int id,float a)
{
    scene.setFocoAltura(id,a);
    updateGL();
}

bool GLWidget::getFocoActivado(int id)
{
    return scene.getFocoActivado(id);
}

void GLWidget::setFocoActivado(int id, bool b)
{
    scene.setFocoActivado(id,b);
    updateGL();
}
