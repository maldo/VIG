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
    cout << "anglecam: " << anglecam << endl;
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
        GLfloat fov = anglecam;

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
        allowMovement = false;
        cout<<"permitir mov "<< allowMovement<<endl;
  }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    double radio;
    Point centro;

    switch (e->key())
    {
    case Qt::Key_F:
        if (DoingView == W)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            updateGL();
        }
        break;
    case Qt::Key_S:
        if (DoingView == S)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            updateGL();
        }
        break;
    case Qt::Key_X:
        if (allowMovement)
        {//esto ha introducido un fallo raro... al cerrar la aplicacion...
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
        if (allowMovement){

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
        cout << "anglecam: " << anglecam << endl;
        //anglecam = 60;
        anterior = radio;
        posterior = 3*radio;

        updateGL();

        break;
        //FI

    default: e->ignore();
    }

   /* if (Qt::Key_F&e->key())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        updateGL();
    }
    else if (Qt::Key_S&e->key())
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        updateGL();
    }
    else if (allowMovement)
    {
        if ((e->key()&Qt::Key_X) && (e->modifiers()&Qt::ShiftModifier))
        {
            scene.move(-1,0,0);
            updateGL();
        }
        else if (e->key()&Qt::Key_X)
        {
            scene.move(1,0,0);
            updateGL();
        }
        else if ((e->key()&Qt::Key_Z) && (e->modifiers()&Qt::ShiftModifier))
        {
            scene.move(0,0,-1);
            updateGL();
        }
        else if (e->key()&Qt::Key_Z)
        {
            scene.move(0,0,1);
            updateGL();
        }
    }*/

 }

void GLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
  // Aqui cal que es calculi i s'apliqui la rotaci, el zoom o el pan
  // com s'escaigui...

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
  xClick = xPos;
  yClick = yPos;

  updateGL();

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

void GLWidget::PlanElevation()
{
    QSize s = this->size();

    glViewport(0, 0, s.width()/2, s.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, dist/2, dist+dist/2);
    gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glViewport(s.width()/2, 0, s.width()/2, s.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0, 1.0, 50.0);
    gluLookAt(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //sglCallList(object);



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
}
