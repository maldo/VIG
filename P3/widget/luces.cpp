#include "luces.h"
#include "ui_luces.h"
#include "glwidget.h"

Luces::Luces(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Luces)
{
    ui->setupUi(this);
    id = 0;
}

Luces::~Luces()
{
    delete ui;
}

void Luces::setThis(GLWidget *s)
{
    gw = s;

    first = false;
}

void Luces::getR(int r)
{
    if (!first) return;

    colorfoco[0] = (float)r/100;
    imprimeColor(colorfoco[0],colorfoco[1],colorfoco[2]);
    gw->setFocoColor(id,colorfoco);
}

void Luces::getG(int g)
{
    if (!first) return;

    colorfoco[1] = (float)g/100;
    imprimeColor(colorfoco[0],colorfoco[1],colorfoco[2]);
    gw->setFocoColor(id,colorfoco);
}

void Luces::getB(int b)
{
    if (!first) return;

    colorfoco[2] = (float)b/100;
    imprimeColor(colorfoco[0],colorfoco[1],colorfoco[2]);
    gw->setFocoColor(id,colorfoco);
}

void Luces::getAltura(int a)
{
    altura = a;
    //imprimeAltura(altura);
    gw->setFocoAltura(id,altura);
}

void Luces::getId(int i)
{
    first = false;

    id = i;

    float * c = gw->getFocoColor(id);
    bool b = gw->getFocoActivado(id);

    for (unsigned int i = 0; i < 3 ; i++)
    {
        cout << c[i] << endl;
    }

    emit setR(c[0]*100);
    emit setG(c[1]*100);
    emit setB(c[2]*100);

    emit setActivado(b);

    imprimeColor(c[0], c[1], c[2]);

    first = true;
}

void Luces::getActivacion(bool b)
{
    activado = b;
    gw->setFocoActivado(id,activado);
}

void Luces::imprimeColor(float r, float g, float b)
{
    char buffer[50];

    sprintf(buffer,"(%.2g, %.2g, %.2g)",r,g,b);

    QString x = QString(buffer);

    emit setRGB(x);
}
