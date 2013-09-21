#include "glwidget.h"
#define _USE_MATH_DEFINES
#include <math.h>


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
  glEnable(GL_DEPTH_TEST);

  scene.Init();

  cameraInit();
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

void GLWidget::paintGL( void )
{
  // Esborrem els buffers
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  setProjection();
  setModelView();

  // dibuixar eixos aplicacio
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
  glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
  glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
  glEnd();

  scene.Render();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    xClick = e->x();
    yClick = e->y();

    if (e->button()&Qt::LeftButton &&
        ! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
    {
        DoingInteractive = ROTATE;
    }
    else if(e->modifiers()&Qt::ShiftModifier && e->modifiers()&Qt::ControlModifier && e->button()&Qt::LeftButton)
    {
        DoingInteractive = MOVING;
    }
    else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
    {
        DoingInteractive = ZOOM;
    }
    else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ControlModifier)
    {
        DoingInteractive = PAN;
    }
    else if (e->button()&Qt::RightButton)
    {
        if (scene.colisiona())
        {
            allowMovement = true; //no haria falta pero para mayor claridad
            cout<<"colisiona"<< allowMovement<<endl;
        }
        else
        {
            allowMovement = false;
            scene.refreshPosOriginal();
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
    double radio;
    Point centro;

    //cout<<"barbaridad"<<endl;

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
        //Inici Codi P1 exam
    case Qt::Key_C:

        scene.calcularEsferaCurrent(radio,centro);

        VRP = centro;

        dist = 2*radio;
        anglecam = rad2deg(2 * asin(radio/(2*radio)));
        //cout << "anglecam: " << anglecam << endl;
        //anglecam = 60;
        anterior = radio;
        posterior = 3*radio;

        updateGL();

        break;
        //FI
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
          cout << posNX << endl;
          posNZ = (Zobs * -(e->y() - yClick));
          cout << posNZ << endl;
          posNF+= posNX-posNZ;

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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    GLubyte pixel[1];

    scene.RenderSeleccio();
    glFlush();
    glReadPixels(x, size().height() - y, 1 , 1, GL_RED, GL_UNSIGNED_BYTE, pixel);

    allowMovement = scene.validaPixel(pixel[0]);

    if (allowMovement)
    {
         scene.setIndice(pixel[0]);
    }

    scene.Render();
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

void GLWidget::PlanElevation(bool xxx)
{
    cout << xxx << endl;

    QSize s = this->size();

    if (xxx)
    {
        /*glViewport(0, 0, s.width()/2, s.height());

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( fov, (GLfloat)(s.width()/2)/(GLfloat)(s.height()), anterior,
                        posterior );
       glOrtho(0,s.width()/2,0,s.height(),anterior,posterior);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        scene.Render();
        glFlush();



        glViewport(s.width()/2, 0, s.width()/2, s.height());

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( fov, (GLfloat)(s.width())/(GLfloat)(s.height()/2), anterior,
                        posterior );
        glOrtho(s.width()/2,s.width(),0,s.height(),anterior,posterior);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        scene.Render();
        glFlush();*/


       glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glViewport(0, 0, s.width()/2, s.height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-3.0, 3.0, -3.0, 3.0, 1.0, 50.0);
        gluLookAt(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
        glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
        glEnd();

        scene.Render();

       glViewport(s.width()/2, 0, s.width()/2, s.height());
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();
       glOrtho(-5.0, 5.0, -5.0, 5.0, -dist/2, dist+dist/2);

       gluLookAt(0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();

       glDisable(GL_LIGHTING);
       glBegin(GL_LINES);
       glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
       glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
       glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
       glEnd();

       scene.Render();
    }
    else
    {
        glViewport(0, 0, s.width(), s.height());
        //setProjection();
        //setModelView();
    }

updateGL();

}

void GLWidget::LoadObject()
{
    QString model = QFileDialog::getOpenFileName(this, tr("Open File"), "../data", tr("Objectes (*.obj)"));
    if (model != "")
    {
        scene.InitObjecte(model.toAscii().constData());
        allowMovement = true;
    }

    emit buttonLoad(false);
}
