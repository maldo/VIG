#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTimer>
#include "point.h"
#include "scene.h"
#include "modmaterial.h"
#include "luces.h"
#include "imagenes.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);

    void setAmbiente(float[]);
    void setDifusa(float[]);
    void setEspecular(float[]);
    void setBrillo(float);
    void setFocoColor(int, float[]);
    float* getFocoColor(int id);
    void setFocoAltura(int,float);
    void setFocoActivado(int, bool);
    bool getFocoActivado(int);

signals:
     void buttonLoad(bool);
     void buttonLlum(bool);
     void buttonZbuffer(bool);
     void buttonBackFaceCulling(bool);
     void buttonMaterial(bool);

protected:
    void initializeGL();

    void paintGL();
    void resizeGL(int width, int height);

    QTimer timer;

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);
    void cameraInit();
    void setModelView();
    void setProjection();
    void seleccionar(int, int);
    void borrar();
    void planElevation();

    int carga(string, vector<string> &, vector<string> &);

    double dist, anterior, posterior, radi, angleX, angleY, angleZ, anglecam, ra;
    float panX, panY;
    Point VRP;
    Scene scene;
    ModMaterial mm;
    Luces l;
    Imagenes imgs;
    GLfloat fov;
    bool seleccionado;

    typedef  enum {NONE, ROTATE, ZOOM, PAN, MOVING} InteractiveAction;
    InteractiveAction DoingInteractive;
    typedef enum {S, W} ModeView;
    ModeView DoingView;
    bool allowMovement;
    bool allowLlum;
    bool allowzbuffer;
    bool allowBFC;
    bool allowPE;

    int xClick, yClick;

 public slots:
    void Load();
    void Save();
     void LoadObject();
     void LoadObjectFromImage(string);
     void ResetCamera();
     void SetSolid();
     void SetWireframe();
     void PlanElevation(bool);
     void Lighting(bool);
     void BackFaceCulling(bool);
     void Zbuffer(bool);
     void ChangeMaterial(bool);
     void ChangeLuces(bool);
     void LoadImagenes();

};

#endif
