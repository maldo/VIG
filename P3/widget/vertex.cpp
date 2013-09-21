#include "vertex.h"

Vertex::Vertex()
{}
Vertex::Vertex(const Point& coordenades)
: coord(coordenades)
{}

QDataStream &operator<<(QDataStream &ds, const Vertex &v)
{
    ds << v.coord;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, Vertex &v)
{
    ds >> v.coord;
    return ds;
}

