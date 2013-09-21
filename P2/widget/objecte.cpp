#include "objecte.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"

Objecte::Objecte(std::string n, int mod, Point p, float scl, float ori):
  nom(n), model(mod), pos(p), scale(scl), orientation(ori)
{}

Objecte::Objecte(std::string n, int mod, Point p, float scl, float ori, Box b):
  nom(n), model(mod), pos(p), scale(scl), orientation(ori), TBox(b)
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

void Objecte::RenderBox(std::vector<Model> &lmodels)
{
    transformarCaja(lmodels[this->model]).Render(255,0,255);
}

void Objecte::RenderSeleccio(std::vector<Model> &lmodels, GLubyte i)
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

    m.RenderSeleccio(i);

    glPopMatrix();
}

Box& Objecte::transformarCaja(Model & m)
{
    Box b = m.boundingBox();
    Box *r=new Box(); // Retornem un Box, aix� que ens cal al heap.
    Box &result = *r;

    /*float sx = b.maxb.x - b.minb.x;
    float sy = b.maxb.y - b.minb.y;
    float sz = b.maxb.z - b.minb.z;*/

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(orientation, 0, 1, 0);

    //cout << "s: " << sx << "," << sy << "," << sz << endl;
    //cout << "sl: " << scale << endl;

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

void Objecte::setOrientation(float ori)
{
    orientation = ori;
}

void Objecte::setBox(Box b)
{
    TBox = b;
}

Box& Objecte::getBox()
{
    return TBox;
}

void Objecte::setOrigen()
{
    posOriginal = pos;
}

void Objecte::setBack()
{
    pos = posOriginal;
}

bool Objecte::colisionaCon(Objecte & i)
{
    Point max = TBox.maxb + pos;
    Point min = TBox.minb + pos;

    Point imax = i.getBox().maxb + i.getPosition();
    Point imin = i.getBox().minb + i.getPosition();

    bool xxx = (min.x < imax.x && max.x > imin.x)
               && (min.y < imax.y && max.y > imin.y)
               && (min.z < imax.z && max.z > imin.z);

    return xxx;
}

bool Objecte::colisionaConBase(Objecte & i)
{
    Box & base = i.getBox();

    return (base.minb.x < pos.x && pos.x < base.maxb.x) && (base.minb.z < pos.z && pos.z < base.maxb.z);
}
