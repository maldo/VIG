#include "imagenes.h"
#include "ui_imagenes.h"
#include "botonimg.h"
#include "glwidget.h"

Imagenes::Imagenes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Imagenes)
{
    ui->setupUi(this);

    layout = new QGridLayout(this);
}

Imagenes::~Imagenes()
{
    delete ui;
}

void Imagenes::setListaImg(vector<string> & l)
{
    int row = 0;
    int column = 0;

    for (unsigned int i = 0; i < l.size(); i++)
    {
        BotonImg *bi = new BotonImg(this);

        QObject::connect(bi,SIGNAL(cargar(string)),this,SLOT(cargarImg(string)));

        bi->setImg(l[i]);

        if (i==3)row++;
        column = i%3;

        layout->addWidget(bi,row,column);
    }
}

void Imagenes::setThis(GLWidget * gw)
{
    this->gw = gw;
}

void Imagenes::cargarImg(string f)
{
    gw->LoadObjectFromImage(f);
}
