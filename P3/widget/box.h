#ifndef _BOX_H_
#define _BOX_H_

#include "point.h"
#include <QDataStream>

#ifdef min 
#undef min
#endif

#ifdef max 
#undef max
#endif

// Box - permet representar una capsa aliniada amb els eixos
// (ex. capsa englobant)
class Box
{
 private:
 // void Render(double r, double g, double b);

 public:
  Box(const Point& minimum=Point(), const Point& maximum=Point());

  // update - expandeix la capsa per a que inclogui el punt donat
  void update(const Point& p);
  // init - inicialitza la capsa amb el punt donat
  void init(const Point& p);
	
  // atributs
  Point minb, maxb;	// extrems mínim i màxim de la capsa

  void Render();

  bool intersectaCon(Box & i);

  void Render(double r, double g, double b);

  friend QDataStream &operator<<(QDataStream &, const Box &);
  friend QDataStream &operator>>(QDataStream &, Box &);
};

QDataStream &operator<<(QDataStream &, const Box &);
QDataStream &operator>>(QDataStream &, Box &);



#endif
