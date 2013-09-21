#ifndef _Scene_H_
#define _Scene_H_
#include "objecte.h"
#include "model.h"
#include <QtOpenGL/qgl.h>
#include <QSet>
#include <QLinkedList>
#include <light.h>
#include <QDataStream>

class Scene
{
 private:
	
  QVector<Model> lmodels;
  QVector<Objecte> lobjectes;
  QSet<int> indices;
  int estatRenderSeleccio;
  light lightScene;
  light lightObs;
  QLinkedList<GLenum> freeLights;
  QVector<light*> lights;

  
 public:
  static MaterialLib matlib;	  // col·lecció de materials
  static void transformPoint(Point &p);

  Scene();

  void Init(vector<string> &);
  void construirBase();
  void Render(bool);
  void RenderSeleccio();
  void RenderLuces(light::Referencies referencia);
  void AddModel(Model &);
  void AddObjecte(Objecte &);
  void InitObjecte(const char* filename);
  void InitModel(const char* filename);
  int ExisteModel(const char* filename);
  void calcularEsfera(double &radio, Point &centro);
  void moveX(float i);
  void moveZ(float i);
  void moveY(float i);
  void montarU();
  void montarSU();
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
  void borrarObjectes();
  void borrarIndices();
  bool isSelected();
  void setAmbiente(float[]);
  void setDifusa(float[]);
  void setEspecular(float[]);
  void setBrillo(float);
  void InitLuzBase();
  void InitLuz3persona();
  void setFocoColor(int, float[]);
  float* getFocoColor(int);
  bool getFocoActivado(int);
  void setFocoAltura(int,float);
  void setFocoActivado(int, bool);

  void scaleX(float);
  void scaleY(float);
  void scaleZ(float);

  float* getCurrentMaterialAmbiente();
  float* getCurrentMaterialDifuso();
  float* getCurrentMaterialEspecular();
  float getCurrentMaterialBrillo();

  friend QDataStream &operator<<(QDataStream &, const Scene &s);
  friend QDataStream &operator>>(QDataStream &, Scene &);
};

QDataStream &operator<<(QDataStream &, const Scene &);
QDataStream &operator>>(QDataStream &, Scene &);

#endif

