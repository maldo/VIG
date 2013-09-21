#ifndef _Scene_H_
#define _Scene_H_
#include "objecte.h"
#include "model.h"
#include <QtOpenGL/qgl.h>


class Scene
{
 private:
	
  // Tindrem un vector amb els models geomètrics dels objectes geomètrics
  // i un altre amb instàncies seves (instàncies o referències a objectes).
  std::vector<Model> lmodels;
  std::vector<Objecte> lobjectes;
  int indice;
  int estatRenderSeleccio;
  
 public:
  static MaterialLib matlib;	  // col·lecció de materials
  static void transformPoint(Point &p);

  Scene();

  void Init();
  void construirBase();
  void Render();
  void RenderSeleccio();
  void AddModel(Model &);
  void AddObjecte(Objecte &);
  void InitObjecte(const char* filename);
  int ExisteModel(const char* filename);
  void calcularEsfera(double &radio, Point &centro);
  void moveX(float i);
  void moveZ(float i);
  void calcularEsferaCurrent(double &radio, Point &centro);
  void girarD();
  void girarI();
  void setObjectBack();
  void refreshPosOriginal();
  bool colisiona();
  bool colisionaConBase(Point, Objecte &);
  int numObjectes();
  void CopiaObjecte(int);
  void DesferCopia();
  void RenderMoviment(float, float);
  void setIndice(int);
  bool validaPixel(int);

};

#endif

