#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "point.h"
#include "QDataStream"

// Vertex - representa un vertex d'un objecte 3D
class Vertex
{
public:
    Vertex(const Point& coordenades);
    Vertex();

    // atributs
    Point coord;		// coordenades del vertex

    friend QDataStream &operator<<(QDataStream &, const Vertex &);
    friend QDataStream &operator>>(QDataStream &, Vertex &);
};

QDataStream &operator<<(QDataStream &, const Vertex &);
QDataStream &operator>>(QDataStream &, Vertex &);


#endif
