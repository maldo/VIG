#include "objecte.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"
#include "material.h"

Objecte::Objecte(QString n, int mod, Point p, float scl, float ori):
        nom(n), model(mod), pos(p), scale(scl), orientation(ori)
{
    colorAmbient[0] = (float)(rand()%101)/100;
    colorAmbient[1] = (float)(rand()%101)/100;
    colorAmbient[2] = (float)(rand()%101)/100;
    colorAmbient[3] = (float)(rand()%101)/100;
    colorDiffuse[0] = (float)(rand()%101)/100;
    colorDiffuse[1] = (float)(rand()%101)/100;
    colorDiffuse[2] = (float)(rand()%101)/100;
    colorDiffuse[3] = (float)(rand()%101)/100;
    colorSpecular[0] = (float)(rand()%101)/100;
    colorSpecular[1] = (float)(rand()%101)/100;
    colorSpecular[2] = (float)(rand()%101)/100;
    colorSpecular[3] = (float)(rand()%101)/100;

    shininess = (float)(rand()%101)/100;

    escalado[0] = scl;
    escalado[1] = scl;
    escalado[2] = scl;
}

Objecte::Objecte(QString n, int mod, Point p, float scl, float ori, Box b):
        nom(n), model(mod), pos(p), scale(scl), orientation(ori), TBox(b)
{
    colorAmbient[0] = (float)(rand()%101)/100;
    colorAmbient[1] = (float)(rand()%101)/100;
    colorAmbient[2] = (float)(rand()%101)/100;
    colorAmbient[3] = (float)(rand()%101)/100;
    colorDiffuse[0] = (float)(rand()%101)/100;
    colorDiffuse[1] = (float)(rand()%101)/100;
    colorDiffuse[2] = (float)(rand()%101)/100;
    colorDiffuse[3] = (float)(rand()%101)/100;
    colorSpecular[0] = (float)(rand()%101)/100;
    colorSpecular[1] = (float)(rand()%101)/100;
    colorSpecular[2] = (float)(rand()%101)/100;
    colorSpecular[3] = (float)(rand()%101)/100;

    shininess = (float)(rand()%101)/100;

    escalado[0] = scl;
    escalado[1] = scl;
    escalado[2] = scl;
}

Objecte::Objecte()
{}

Objecte::~Objecte(void)
{}


void Objecte::Render(QVector<Model> &lmodels)
{
    // Cal pintar la geometria de l'objecte transformada de la forma adequada
    Model m = lmodels.at(this->model);
    m.updateBoundingBox();
    Box b = m.boundingBox();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(orientation, 0, 1, 0);

    glScalef(escalado[0],escalado[1],escalado[2]);

    glTranslatef(-(b.maxb.x+b.minb.x)/2.0, -b.minb.y, -(b.maxb.z+b.minb.z)/2.0);

    glMaterialfv(GL_FRONT,GL_AMBIENT,colorAmbient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,colorDiffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,colorSpecular);
    glMaterialf(GL_FRONT,GL_SHININESS, shininess);

    m.Render();

    glPopMatrix();
}

void Objecte::RenderBox(QVector<Model> &lmodels)
{
    transformarCaja(lmodels.at(this->model)).Render(255,0,255);
}

void Objecte::RenderSeleccio(QVector<Model> &lmodels, GLubyte i)
{
    // Cal pintar la geometria de l'objecte transformada de la forma adequada
    Model m = lmodels.at(this->model);
    m.updateBoundingBox();
    Box b = m.boundingBox();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(orientation, 0, 1, 0);

    glScalef(escalado[0],escalado[1],escalado[2]);

    glTranslatef(-(b.maxb.x+b.minb.x)/2.0, -b.minb.y, -(b.maxb.z+b.minb.z)/2.0);

    m.RenderSeleccio(i);

    glPopMatrix();
}

Box& Objecte::transformarCaja(const Model & m)
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

    glScalef(escalado[0],escalado[1],escalado[2]);

    glTranslatef(-(b.maxb.x+b.minb.x)/2.0, -b.minb.y, -(b.maxb.z+b.minb.z)/2.0);

    const vector<Vertex> &verts = m.vertices;
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

QString Objecte::getNom()
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

void Objecte::setAmbiente(float *ka)
{
    colorAmbient[0] = ka[0];
    colorAmbient[1] = ka[1];
    colorAmbient[2] = ka[2];
}

void Objecte::setDifusa(float *kd)
{
    colorDiffuse[0] = kd[0];
    colorDiffuse[1] = kd[1];
    colorDiffuse[2] = kd[2];
}

void Objecte::setEspecular(float *ks)
{
    colorSpecular[0] = ks[0];
    colorSpecular[1] = ks[1];
    colorSpecular[2] = ks[2];
}

void Objecte::setBrillo(float b)
{
     shininess = b;
}

float* Objecte::getCurrentMaterialAmbiente()
{
    return colorAmbient;
}

float* Objecte::getCurrentMaterialDifuso()
{
    return colorDiffuse;
}

float* Objecte::getCurrentMaterialEspecular()
{
    return colorSpecular;
}

float Objecte::getCurrentMaterialBrillo()
{
    return shininess;
}

void Objecte::setScaleX(float s)
{
    escalado[0] += s;
}

void Objecte::setScaleY(float s)
{
    escalado[1] += s;
}

void Objecte::setScaleZ(float s)
{
    escalado[2] += s;
}

QDataStream &operator<<(QDataStream &ds, const Objecte &o)
{

    ds << o.nom;
    ds << o.model;   // identificador del model
    ds << o.pos;    // posicio sobre el terra
    ds << o.scale;	   // mida en unitats
    ds << o.orientation;  //orientació respecte Y
    ds << o.TBox;
    ds << o.posOriginal;

    for (int i = 0; i < 4; i++)
    {
        ds << o.colorAmbient[i];
        ds << o.colorDiffuse[i];
        ds << o.colorSpecular[i];
    }

    for (int i = 0; i < 3; i++)
    {
        ds << o.escalado[i];
    }


    ds << o.shininess;

    return ds;
}

QDataStream &operator>>(QDataStream &ds, Objecte &o)
{

    ds >> o.nom;
    ds >> o.model;   // identificador del model
    ds >> o.pos;    // posicio sobre el terra
    ds >> o.scale;	   // mida en unitats
    ds >> o.orientation;  //orientació respecte Y
    ds >> o.TBox;
    ds >> o.posOriginal;

    for (int i = 0; i < 4; i++)
    {
        ds >> o.colorAmbient[i];
        ds >> o.colorDiffuse[i];
        ds >> o.colorSpecular[i];
    }

    for (int i = 0; i < 3; i++)
    {
        ds >> o.escalado[i];
    }

    ds >> o.shininess;

    return ds;
}
