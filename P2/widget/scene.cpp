#include "scene.h"
#include <math.h>


MaterialLib Scene::matlib;

Scene::Scene()
{}

void Scene::Init()
{
    indice = -1;
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
    Box box = oref.transformarCaja(b);
    oref.setBox(box);
    AddObjecte(oref);
}

void Scene::calcularEsfera(double &radio, Point &centro)
{
    Box caja(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
    for (unsigned int i=0; i< lobjectes.size(); i++)
    {
        cout << lobjectes[i].getNom()<< endl;
        Box & bobj = lobjectes[i].getBox();

        cout <<  bobj.minb <<  bobj.maxb << endl;

        caja.update(bobj.minb);
        caja.update(bobj.maxb);
    }

    centro =  (caja.minb + caja.maxb)/2.f;
    radio = (caja.maxb-caja.minb).length()/2.f;
    //cout << "Esfera: " << centro << ";radio:" << radio << endl;
}

//Inici Codi P1 exam
void Scene::calcularEsferaCurrent(double &radio, Point &centro)
{
    Box caja(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
    Box bobj;

    Objecte& o = lobjectes[indice];

    bobj=o.getBox();

    caja.update(bobj.minb);
    caja.update(bobj.maxb);

    centro =  (caja.minb + caja.maxb)/2.f;
    radio = (caja.maxb-caja.minb).length()/2.f;
    //cout << "Esfera: " << centro << ";radio:" << radio << endl;
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
    for (unsigned int i=0; i< lobjectes.size(); i++)
    {
        lobjectes[i].Render(lmodels);

        if (i == indice)
        {
            lobjectes[i].RenderBox(lmodels);
        }
    }
}

void Scene::RenderMoviment(float x, float z)
{
    Point pos;
    glColor3f(1,0,0);

    pos = lobjectes[indice].getPosition();

    pos.x+=x;
    pos.z+=z;

    if (colisionaConBase(pos,lobjectes[0]))
    {
        lobjectes[indice].setPos(pos);
        lobjectes[indice].Render(lmodels);
        glFlush();
    }
}

void Scene::RenderSeleccio()
{
    for(GLubyte i=0; i < lobjectes.size();i++)
    {
        lobjectes[i].RenderSeleccio(lmodels, i);
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

    Box mbox = o.transformarCaja(lmodels[w]);

    o.setBox(mbox);

    AddObjecte(o);
    o.setOrigen();
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

    pos.x = pos.x + i+ i;

    if (colisionaConBase(pos,lobjectes[0]))
    {
        o.setPos(Point(pos.x+i,pos.y,pos.z));
    }
}

void Scene::moveZ(float i)
{
    Objecte& o = lobjectes[indice];
    Point pos = o.getPosition();

    pos.z = pos.z + i+ i;

    if (colisionaConBase(pos,lobjectes[0]))
    {
        o.setPos(Point(pos.x,pos.y,pos.z+i));
    }
}

void Scene::girarD()
{
    Objecte& o = lobjectes[indice];
    float ori = o.getOrientation();
    o.setOrientation(ori+1);
}

void Scene::girarI()
{
    Objecte& o = lobjectes[indice];
    float ori = o.getOrientation();
    o.setOrientation(ori-1);
}

void Scene::setObjectBack()
{
    Objecte& o = lobjectes[indice];
    o.setBack();
}

void Scene::refreshPosOriginal()
{
    Objecte& o = lobjectes[indice];
    indice = -1;
    o.setOrigen();
}

int Scene::numObjectes()
{
    return lobjectes.size();
}

void Scene::DesferCopia()
{
   lobjectes.pop_back();
}

bool Scene::colisiona()
{
    Objecte& o = lobjectes[indice];

    cout << "Objecto seleccionado indice"<<indice<< " pos "<<  o.getPosition()<<endl;

   for (unsigned int i=1; i< lobjectes.size(); i++)
   {
       if (i != indice)
       {
           bool x = o.colisionaCon(lobjectes[i]);
           if (x)
           {
               cout << "COLISION" <<endl;
               return x;
           }
       }
       else
       {
           // El objecto colisiona consigo mismo....
       }
   }
    return false;
}

bool Scene::colisionaConBase(Point p, Objecte & o)
{
    Box & base = o.getBox();

    return (base.minb.x < p.x && p.x < base.maxb.x) && (base.minb.z < p.z && p.z < base.maxb.z);
}

void Scene::setIndice(int i)
{
    indice = i;
}

bool Scene::validaPixel(int i)
{
    if (i == 0) return false;
    if (i >= lobjectes.size()) return false;
    return true;
}
