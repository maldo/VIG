#include "scene.h"
#include <math.h>


MaterialLib Scene::matlib;

Scene::Scene()
{}

void Scene::Init()
{
  construirBase ();
}

void Scene::construirBase()
{
  Model b("Base");

  // Construim el poligon base
  Vertex v1(Point(-5,0,-5));
  Vertex v2(Point(-5,0,5));
  Vertex v3(Point(5,0,5));
  Vertex v4(Point(5,0,-5));
  b.vertices.push_back(v1);
  b.vertices.push_back(v2);
  b.vertices.push_back(v3);
  b.vertices.push_back(v4);

  Face face(0,1,2,3);
  face.normal = Vector(0,1,0);
  face.material = 0;
  b.faces.push_back(face);
  b.updateBoundingBox();

  AddModel(b);
  Objecte oref("Base", 0, Point(0,0,0), 1, 0);
  AddObjecte(oref);
  Box & box = oref.transformarCaja(b);
  cout << "Punto: " << box.minb << "  max: " << box.maxb << endl;
}

void Scene::calcularEsfera(double &radio, Point &centro)
{
    Box caja(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
    Box bobj;
    // actualitzem la capsa amb tots els objectes de l'escena
    std::vector<Objecte>::iterator ito;
    for (ito=lobjectes.begin(); ito!=lobjectes.end(); ito++)
    {
        bobj=(*ito).transformarCaja(lmodels[(*ito).getModelId()]);

        caja.update(bobj.minb);
        caja.update(bobj.maxb);
    }

    centro =  (caja.minb + caja.maxb)/2.f;
    radio = (caja.maxb-caja.minb).length()/2.f;
    cout << "Esfera: " << centro << ";radio:" << radio << endl;
}

//Inici Codi P1 exam
void Scene::calcularEsferaCurrent(double &radio, Point &centro)
{
    Box caja(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
    Box bobj;

    Objecte& o = lobjectes[indice];

    bobj=o.transformarCaja(lmodels[o.getModelId()]);

    caja.update(bobj.minb);
    caja.update(bobj.maxb);

    centro =  (caja.minb + caja.maxb)/2.f;
    radio = (caja.maxb-caja.minb).length()/2.f;
    cout << "Esfera: " << centro << ";radio:" << radio << endl;
}
//FI


void Scene::transformPoint(Point &p)
{
        GLfloat m[16];
        GLfloat p1[16];

        for (unsigned int i=4; i<16; ++i)
        {
                p1[i]=0.0f;
        }

        p1[0] = p.x;
        p1[1] = p.y;
        p1[2] = p.z;
        p1[3] = 1;

        glPushMatrix();
        glMultMatrixf(p1);
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        p.x = m[0];
        p.y = m[1];
        p.z = m[2];
        glPopMatrix();
}

void Scene::Render()
{
    // Cal pintar tots els objectes de l'escena que es troben al vector
    for (unsigned int i=0; i< lobjectes.size(); i++)
    {
        lobjectes[i].Render(lmodels);
    }
}

void Scene::AddModel(Model &o)
{
  lmodels.push_back(o);
}

void Scene::AddObjecte(Objecte &oref)
{
  lobjectes.push_back(oref);
}

void Scene::InitObjecte(const char* filename)
{
    unsigned int w = ExisteModel(filename);

    if (w == lmodels.size())
    {
        // No existe el modelo

        Model m(filename);
        m.readObj(filename,Scene::matlib);
        AddModel(m);

    }

    //Existe el modelo que esta en lmodels[w]


    Box b = lmodels[w].boundingBox();

    float sx = b.maxb.x - b.minb.x;
    float sz = b.maxb.z - b.minb.z;

    float baseGrande = max(sx,sz);

    Objecte o(filename,w,0,1/baseGrande,1);

    //Box mbox = o.transformarCaja(lmodels[w]);

    /*float enz = (mbox.maxb.z - mbox.minb.z)/2.0f;
    float enx = (mbox.maxb.x - mbox.minb.x)/2.0f;

    o.setPos(Point(-enx/2,mbox.minb.y,-enz/2));*/

    AddObjecte(o);
    indice = lobjectes.size() - 1;

}

int Scene::ExisteModel(const char* filename)
{
    unsigned int i = 0;
    while(i < lmodels.size())
    {
       int z = strcmp(lmodels[i].getNom().c_str(),filename);
       if (z==0)
       {
           return i;
       }
       i++;
    }
    return i;
}

void Scene::moveX(float i)
{
  Objecte& o = lobjectes[indice];
  Point pos = o.getPosition();
  o.setPos(Point(pos.x+i,pos.y,pos.z));
}

void Scene::moveZ(float i)
{
    Objecte& o = lobjectes[indice];
    Point pos = o.getPosition();
    o.setPos(Point(pos.x,pos.y,pos.z+i));
}
