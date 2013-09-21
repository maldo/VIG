#include <qgl.h>

#include "light.h"

light::light(GLenum identificador)
{
        id = identificador;

        posicion[0] = 0;
        posicion[1] = 0;
        posicion[2] = 0;
        posicion[3] = 1;

        ambiente[0] = 1;
        ambiente[1] = 1;
        ambiente[2] = 1;
        ambiente[3] = 1;

        difusa[0] = 1;
        difusa[1] = 1;
        difusa[2] = 1;
        difusa[3] = 1;

        especular[0] = 1;
        especular[1] = 1;
        especular[2] = 1;
        especular[3] = 1;

        activada = false;
}

void light::Configura()
{
    glDisable(id);

    glLightfv(id, GL_AMBIENT, ambiente);
    glLightfv(id, GL_DIFFUSE, difusa);
    glLightfv(id, GL_SPECULAR, especular);

    if (activada)
    {
        glEnable(id);
    }
    else
    {
        glDisable(id);
    }
}

void light::Render()
{
    if (activada)
    {
        glLightfv(id, GL_POSITION, posicion);
    }
}

QDataStream &operator<<(QDataStream & ds, const light &l)
{

    /*GLfloat posicion[4];
    GLfloat ambiente[4];
    GLfloat difusa[4];
    GLfloat especular[4];

    typedef enum {
        OBSERVADOR = 0,
        ESCENA = 1,
    } Referencies;

    Referencies referencia;

    int id;

    bool activada;*/

    ds << l.id;
    ds << l.activada;
    //ds << l.referencia;

    for (int i = 0; i < 4; i++)
    {
        ds << l.posicion[i];
        ds << l.ambiente[i];
        ds << l.difusa[i];
        ds << l.especular[i];
    }

    return ds;
}


QDataStream &operator>>(QDataStream &ds, light &l)
{
    ds >> l.id;
    ds >> l.activada;
    //ds >> l.referencia;

    for (int i = 0; i < 4; i++)
    {
        ds >> l.posicion[i];
        ds >> l.ambiente[i];
        ds >> l.difusa[i];
        ds >> l.especular[i];
    }

    return ds;
}
