#include "scene.h"
#include <math.h>
#include <algorithm>


MaterialLib Scene::matlib;

Scene::Scene():lightScene(GL_LIGHT0), lightObs(GL_LIGHT1)
{
    for (GLenum i = GL_LIGHT2; i < GL_LIGHT7; i++)
    {
            freeLights.append(i);
    }
}

void Scene::Init(vector<string> &files)
{
    construirBase();

    for (unsigned int i = 0; i < files.size(); i++)
    {
        string filename = files[i];

        InitModel(filename.data());
    }

    InitLuzBase();
    InitLuz3persona();
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
    for (int i=0; i< lobjectes.size(); i++)
    {
        //cout << lobjectes[i].getNom().toAscii().constData()<< endl;
        Box & bobj = lobjectes[i].getBox();

        //cout <<  bobj.minb <<  bobj.maxb << endl;

        caja.update(bobj.minb);
        caja.update(bobj.maxb);
    }

    centro =  (caja.minb + caja.maxb)/2.f;
    radio = (caja.maxb-caja.minb).length()/2.f;
    //cout << "Esfera: " << centro << ";radio:" << radio << endl;
}

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

void Scene::Render(bool b)
{
    for (int i=0; i< lobjectes.size(); i++)
    {
        lobjectes[i].Render(lmodels);
    }

    glDisable(GL_LIGHTING);

    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        lobjectes[indice].RenderBox(lmodels);
    }

    if (b) glEnable(GL_LIGHTING);
}

void Scene::RenderMoviment(float x, float z)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
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
        else
        {
            pos.x -= x;
            pos.z -= z;

            for (QSet<int>::iterator z = j ; z !=indices.begin(); z--)
            {
                lobjectes[indice].setPos(pos);
                lobjectes[indice].Render(lmodels);
            }
            indices.clear();
            return;
        }
    }
}

void Scene::RenderSeleccio()
{
    for(GLubyte i=0; i < lobjectes.size();i++)
    {
        lobjectes[i].RenderSeleccio(lmodels, i);
    }
}

void Scene::RenderLuces(light::Referencies referencia)
{
    foreach (light* l, lights)
    {
        if (l->activada && l->referencia == referencia)
        {
            //cout << "LUZ " << l->id <<endl;
            l->Render();
        }
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

void Scene::InitModel(const char* filename)
{
    Model m(filename);
    m.readObj(filename,Scene::matlib);
    AddModel(m);
}

void Scene::InitObjecte(const char* filename)
{
    int w = ExisteModel(filename);

    bool existe = (w!=lmodels.size());

    //cout << existe << endl;

    if (!existe)
    {
        cout <<"escondido"<<endl;
        // No existe el modelo
        Model m(filename);
        m.readObj(filename,Scene::matlib);
        AddModel(m);
    }

    //Existe el modelo que esta en lmodels[w]

    //cout << "TENEMOS EL MODELO" << endl;

    Box b = lmodels[w].boundingBox();

    float sx = b.maxb.x - b.minb.x;
    float sz = b.maxb.z - b.minb.z;

    float baseGrande = max(sx,sz);

    Objecte o(filename,w,0,1/baseGrande,1);
    //cout << "CREACION OBJETO" << endl;

    Box mbox = o.transformarCaja(lmodels[w]);

    o.setBox(mbox);

    //AddObjecte(o);
    //o.setOrigen();
    //indices.insert(lobjectes.size() - 1);

    int i;
    bool x = false;

    //cout << "tamaño de objetos " << lobjectes.size()<<endl;

    for (i=1; i < lobjectes.size(); i++)
    {
            x = o.colisionaCon(lobjectes[i]);
            if (x) break;
    }

    if (x)
    {
        Point p = lobjectes[i].getPosition();

        p.y = lobjectes[i].getBox().maxb.y;

        o.setPos(p);
    }

    AddObjecte(o);
    o.setOrigen();
    indices.insert(lobjectes.size() - 1);

}

int Scene::ExisteModel(const char* filename)
{
    int i = 0;
    while(i < lmodels.size())
    {
        int z = strcmp(lmodels[i].getNom().toAscii().constData(),filename);
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
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        Point pos = o.getPosition();

        float x = pos.x + i + i;

        if (colisionaConBase(x,lobjectes[0]))
        {
            o.setPos(Point(pos.x+i,pos.y,pos.z));
        }
        else
        { 
            for (QSet<int>::iterator z = j ; z !=indices.begin(); z--)
            {
                o.setPos(Point(pos.x-i,pos.y,pos.z));
            }
            indices.clear();
            return;
        }
    }
}

void Scene::moveY(float i)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        Point pos = o.getPosition();

        //cout << pos.y+i<< endl;

        if (pos.y+i < 0.0)
        {
            o.setPos(Point(pos.x,0,pos.z));
        }
        else
        {
             o.setPos(Point(pos.x,pos.y+i,pos.z));
        }
    }
}

void Scene::moveZ(float i)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        Point pos = o.getPosition();

        float z = pos.z + i + i;

        if (colisionaConBase(z,lobjectes[0]))
        {
            o.setPos(Point(pos.x,pos.y,pos.z+i));
        }
        else
        {
            /*Al tener varios objetos en movimiento cuando un objeto llegue al final
             *dejamos de moverlo pero tambien dejamos de mover al resto
             */
            for (QSet<int>::iterator z =j; z!=indices.begin(); z--)
            {
                o.setPos(Point(pos.x,pos.y,pos.z-i));
            }
            indices.clear();
            return;
        }
    }
}


void Scene::scaleX(float s)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setScaleX(s);
    }
}


void Scene::scaleY(float s)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setScaleY(s);
    }
}


void Scene::scaleZ(float s)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setScaleZ(s);
    }
}


void Scene::montarU()
{
    if (indices.size() != 2) return;

    /*for (unsigned int i = 0; i < lobjectes.size(); i++)
    {
        cout << i << " "<< lobjectes[i].getNom().toAscii().constData()<<endl;
    }

    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        cout << *j<< endl;
    }*/

    QSet<int>::iterator up = indices.begin();
    Objecte& u = lobjectes[*up];
    up++;
    Objecte& d = lobjectes[*up];

    Point base = d.getPosition();
    base.y = base.y + d.getBox().maxb.y;

    u.setPos(base);
}

void Scene::montarSU()
{
    if (indices.size() != 2) return;

    /*for (unsigned int i = 0; i < lobjectes.size(); i++)
    {
        cout << i << " "<< lobjectes[i].getNom().toAscii().constData()<<endl;
    }

    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        cout << *j<< endl;
    }*/

    QSet<int>::iterator up = indices.begin();
    Objecte& u = lobjectes[*up];
    up++;
    Objecte& d = lobjectes[*up];

    Point base = u.getPosition();
    base.y = base.y + u.getBox().maxb.y;

    d.setPos(base);
}

void Scene::girarD()
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        float ori = o.getOrientation();
        o.setOrientation(ori+1);
    }
}

void Scene::girarI()
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        float ori = o.getOrientation();
        o.setOrientation(ori-1);
    }
}

void Scene::setObjectBack()
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setBack();
    }
}

void Scene::refreshPosOriginal()
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setOrigen();
    }
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
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];

        //cout << "Objecto seleccionado indice"<<indice<< " pos "<<  o.getPosition()<<endl;

        for (int i=1; i< lobjectes.size(); i++)
        {
            if ((int)i != indice)
            {
                bool x = o.colisionaCon(lobjectes[i]);
                if (x)
                {
                    //if (o.getBox())
                    //cout << "COLISION" <<endl;
                    return x;
                }
            }
            else
            {
                // El objecto colisiona consigo mismo....
            }
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
    indices.insert(i);
}

bool Scene::validaPixel(int i)
{
    if (i == 0) return false;
    if (i >= (int) lobjectes.size()) return false;
    return true;
}

void Scene::borrarObjectes()
{
     //cout << "Tamaño de los indices " << indices.size()<<endl;

     QSet<int>::iterator j = indices.end();

     while (j != indices.begin())
     {
         j--;
         int indice = *j;
         cout<<indice<<endl;
         Objecte& o = lobjectes[indice];

         cout << o.getNom().toAscii().constData()<<endl;

         lobjectes.erase(lobjectes.begin()+indice);

         /*ocurrencias[x] = ocurrencias[x] - 1;

         if (ocurrencias[x]==0)
         {
             //borramos el modelo

             lmodels.erase(lmodels.begin()+x);
             ocurrencias.erase(ocurrencias.begin()+x);
         }*/
     }

    indices.clear();
}

void Scene::borrarIndices()
{
    indices.clear();
}

bool Scene::isSelected()
{
    return !indices.empty();
}

void Scene::setAmbiente(float *ka)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setAmbiente(ka);
    }
}

void Scene::setDifusa(float *kd)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setDifusa(kd);
    }
}

void Scene::setEspecular(float *ks)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setEspecular(ks);
    }
}

void Scene::setBrillo(float b)
{
    for (QSet<int>::iterator j = indices.begin(); j != indices.end(); j++)
    {
        int indice = *j;
        Objecte& o = lobjectes[indice];
        o.setBrillo(b);
    }
}

void Scene::InitLuzBase()
{
    Box b = lobjectes[0].getBox();

    lightScene.referencia = light::ESCENA;
    lightScene.id = GL_LIGHT0;
    lightScene.posicion[0] = (b.maxb.x+b.minb.x)/2.0;
    lightScene.posicion[1] = 10;
    lightScene.posicion[2] = (b.maxb.z+b.minb.z)/2.0;
    lightScene.posicion[3] = 1;

    lightScene.ambiente[0] = 0.2;
    lightScene.ambiente[1] = 0.2;
    lightScene.ambiente[2] = 0.2;
    lightScene.difusa[0] = 0.6;
    lightScene.difusa[1] = 0.6;
    lightScene.difusa[2] = 0.6;
    lightScene.especular[0] = 0.6;
    lightScene.especular[1] = 0.6;
    lightScene.especular[2] = 0.6;
    lightScene.activada = true;

    lightScene.Configura();

    lights.append(&lightScene);
}

void Scene::InitLuz3persona()
{
    lightObs.id = GL_LIGHT1;

    lightObs.referencia = light::OBSERVADOR;
    lightObs.posicion[0] = 0.0f;
    lightObs.posicion[1] = 0.0f;
    lightObs.posicion[2] = 0.0f;
    lightObs.posicion[3] = 1;
    lightObs.activada = true;

    lightObs.Configura();

    lights.append(&lightObs);
}

float* Scene::getFocoColor(int id)
{
    foreach (light* l, lights)
    {
        if (l->id == GL_LIGHT0+id)
        {
            return l->difusa;
        }
    }

    return 0;
}

bool Scene::getFocoActivado(int id)
{
    foreach (light* l, lights)
    {
        if (l->id == GL_LIGHT0+id)
        {
            return l->activada;
        }
    }
    return false;
}

void Scene::setFocoColor(int id, float *color)
{
    foreach (light* l, lights)
    {
        if (l->id == GL_LIGHT0+id)
        {
            l->difusa[0] = color[0];
            l->difusa[1] = color[1];
            l->difusa[2] = color[2];

            l->especular[0] = color[0];
            l->especular[1] = color[1];
            l->especular[2] = color[2];

            l->ambiente[0] = color[0] * 0.05f;
            l->ambiente[1] = color[1] * 0.05f;
            l->ambiente[2] = color[2] * 0.05f;

            l->Configura();
        }
    }
}

void Scene::setFocoAltura(int id,float a)
{
    foreach (light* l, lights)
    {
         if (l->id == GL_LIGHT0+id)
        {
            l->posicion[2] = -a;
        }
    }
}

void Scene::setFocoActivado(int id, bool b)
{
    foreach (light* l, lights)
    {
        if (l->id == GL_LIGHT0+id)
        {
            l->activada = b;

            l->Configura();
        }
    }
}

float* Scene::getCurrentMaterialAmbiente()
{
    QSet<int>::iterator j = indices.begin();

    return lobjectes[*j].getCurrentMaterialAmbiente();
}

float* Scene::getCurrentMaterialDifuso()
{
    QSet<int>::iterator j = indices.begin();

    return lobjectes[*j].getCurrentMaterialDifuso();
}

float* Scene::getCurrentMaterialEspecular()
{
    QSet<int>::iterator j = indices.begin();

    return lobjectes[*j].getCurrentMaterialEspecular();
}

float Scene::getCurrentMaterialBrillo()
{
    QSet<int>::iterator j = indices.begin();

    return lobjectes[*j].getCurrentMaterialBrillo();
}

QDataStream &operator<<(QDataStream & ds, const Scene &s)
{

    /*QVector<Model> lmodels;
    QVector<Objecte> lobjectes;
    QSet<int> indices;
    int estatRenderSeleccio;
    light lightScene;
    light lightObs;
    QLinkedList<GLenum> freeLights;
    QVector<light*> lights;*/

    ds << s.lmodels;
    ds << s.lobjectes;
    ds << s.indices;
    ds << s.estatRenderSeleccio;
    ds << s.lightScene;
    ds << s.lightObs;
    ds << s.freeLights;

    /*for (unsigned int i = 0; s.lights.size(); i++)
    {
        ds << s.lights[i];
    }*/

    return ds;
}


QDataStream &operator>>(QDataStream &ds, Scene &s)
{
    ds >> s.lmodels;
    ds >> s.lobjectes;
    ds >> s.indices;
    ds >> s.estatRenderSeleccio;
    ds >> s.lightScene;
    ds >> s.lightObs;
    ds >> s.freeLights;

    /*for (unsigned int i = 0; s.lights.size(); i++)
    {
        ds >> s.lights[i];
    }*/

    return ds;
}
