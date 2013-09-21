#ifndef _OBJECTE_H_
#define _OBJECTE_H_
#include <QtOpenGL/qgl.h>
#include <vector>
#include <stdio.h>
#include "point.h"
#include "model.h"
#include "box.h"


using namespace std;

class Objecte
{
 protected:
  std::string nom;
  int model;   // identificador del model
  Point pos;    // posicio sobre el terra
  float scale;	   // mida en unitats
  float orientation;  //orientació respecte Y
  Box TBox;
  Point posOriginal;

 public:
  Objecte(std::string nom, int idmod, Point p, float scl, float ori);
  Objecte(std::string nom, int idmod, Point p, float scl, float ori, Box b);
  ~Objecte(void);

  void Render(std::vector<Model> &);
  std::string getNom();
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
  void RenderBox(std::vector<Model> &);
  bool colisionaConBase(Objecte & i);
  void RenderSeleccio(std::vector<Model> &,GLubyte);

  Box& transformarCaja(Model &m);

};

#endif
