#ifndef _OBJECTE_H_
#define _OBJECTE_H_
#include <QtOpenGL/qgl.h>
#include <vector>
#include <stdio.h>
#include "point.h"
#include "model.h"
#include "box.h"
#include "material.h"


using namespace std;

class Objecte
{
 protected:
  QString nom;
  int model;   // identificador del model
  Point pos;    // posicio sobre el terra
  float scale;	   // mida en unitats
  float orientation;  //orientació respecte Y
  Box TBox;
  Point posOriginal;
  float colorAmbient[4];
  float colorDiffuse[4];
  float colorSpecular[4];
  float shininess;
  float escalado[3];

 public:
  Objecte(QString nom, int idmod, Point p, float scl, float ori);
  Objecte(QString nom, int idmod, Point p, float scl, float ori, Box b);
  Objecte();
  ~Objecte(void);

  void Render(QVector<Model> &);
  QString getNom();
  int getModelId();
  Point getPosition();
  float getScale();
  float getOrientation();
  void setOrientation(float ori);
  Box & getBox();
  void setPos(Point p);
  void setBox(Box b);
  void setOrigen();
  void setBack();
  bool colisionaCon(Objecte & i);
  void RenderBox(QVector<Model> &);
  bool colisionaConBase(Objecte & i);
  void RenderSeleccio(QVector<Model> &,GLubyte);

  Box& transformarCaja(const Model &m);

  void setScaleX(float);
  void setScaleY(float);
  void setScaleZ(float);

  void setAmbiente(float[]);
  void setDifusa(float[]);
  void setEspecular(float[]);
  void setBrillo(float);

  float* getCurrentMaterialAmbiente();
  float* getCurrentMaterialDifuso();
  float* getCurrentMaterialEspecular();
  float getCurrentMaterialBrillo();

  friend QDataStream &operator<<(QDataStream &, const Objecte &s);
  friend QDataStream &operator>>(QDataStream &, Objecte &);
};

QDataStream &operator<<(QDataStream &, const Objecte &);
QDataStream &operator>>(QDataStream &, Objecte &);

#endif
