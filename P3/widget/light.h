#ifndef LIGHT_H
#define LIGHT_H

#include <qgl.h>
#include <QString>

struct light
{
    light(GLenum identificador);

    GLfloat posicion[4];
    GLfloat ambiente[4];
    GLfloat difusa[4];
    GLfloat especular[4];

    typedef enum {
        OBSERVADOR = 0,
        ESCENA = 1,
    } Referencies;

    Referencies referencia;

    int id;

    bool activada;

public:
    void Configura();
    void Render();


    friend QDataStream &operator<<(QDataStream &, const  light &l);
    friend QDataStream &operator>>(QDataStream &, light  &);
  };

  QDataStream &operator<<(QDataStream &, const light &l);
  QDataStream &operator>>(QDataStream &, light  &);

#endif // LIGHT_H
