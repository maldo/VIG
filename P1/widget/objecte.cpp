#include "objecte.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"

Objecte::Objecte(std::string n, int mod, Point p, float scl, float ori):
  nom(n), model(mod), pos(p), scale(scl), orientation(ori)
{}

Objecte::~Objecte(void)
{}

void Objecte::Render(std::vector<Model> &lmodels)
{
    // Cal pintar la geometria de l'objecte transformada de la forma adequada
    Model m = lmodels[this->model];
    m.updateBoundingBox();
    Box b = m.boundingBox();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(orientation, 0, 1, 0);

    glScalef(scale,scale,scale);

    glTranslatef(-(b.maxb.x+b.minb.x)/2.0, -b.minb.y, -(b.maxb.z+b.minb.z)/2.0);

    m.Render();

    glPopMatrix();
}


Box& Objecte::transformarCaja(Model & m)
{
    Box b = m.boundingBox();
    Box *r=new Box(); // Retornem un Box, aix� que ens cal al heap.
    Box &result = *r;

    float sx = b.maxb.x - b.minb.x;
    float sy = b.maxb.y - b.minb.y;
    float sz = b.maxb.z - b.minb.z;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(orientation, 0, 1, 0);

    cout << "s: " << sx << "," << sy << "," << sz << endl;
    cout << "sl: " << scale << endl;

    glScalef(scale,scale,scale);

    glTranslatef(-(b.maxb.x+b.minb.x)/2.0, -b.minb.y, -(b.maxb.z+b.minb.z)/2.0);

    vector<Vertex> &verts = m.vertices;
    Point punt(verts[0].coord.x, verts[0].coord.y, verts[0].coord.z);
    Scene::transformPoint(punt);
    result.init(punt);

    for (unsigned int i=1; i<verts.size(); ++i)
    {
        Point punt(verts[i].coord.x, verts[i].coord.y, verts[i].coord.z);
        Scene::transformPoint(punt);
        result.update(punt);
    }

    glPopMatrix();

    return result;

}

std::string Objecte::getNom()
{
  return nom;
}

int Objecte::getModelId()
{
  return model;
}

Point Objecte::getPosition()
{
   return pos;
}

void Objecte::setPos(Point p)
{
    pos = p;
}

float Objecte::getScale()
{
   return scale;
}

float Objecte::getOrientation()
{
   return orientation;
}

