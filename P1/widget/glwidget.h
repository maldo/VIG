#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include "point.h"
#include "scene.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);

protected:
    void initializeGL();

    void paintGL();
    void resizeGL(int width, int height);

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);
    void cameraInit();
    void setModelView();
    void setProjection();

    double dist, anterior, posterior, radi, angleX, angleY, angleZ, anglecam, ra;
    float panX, panY;
    Point VRP;
    Scene scene;

    typedef  enum {NONE, ROTATE, ZOOM, PAN} InteractiveAction;
    InteractiveAction DoingInteractive;
    typedef enum {S, W} ModeView;
    ModeView DoingView;
    bool allowMovement;

    int xClick, yClick;

 public slots:
     void LoadObject();
     void ResetCamera();
     void SetSolid();
     void SetWireframe();
     void PlanElevation();

};

#endif
