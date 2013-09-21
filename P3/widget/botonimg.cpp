#include "botonimg.h"

BotonImg::BotonImg(QWidget *parent) :
        QPushButton(parent)
{
    QObject::connect(this, SIGNAL(clicked()),this,SLOT(imgClick()));
}

void BotonImg::setImg(string file)
{
    this->imagen = file;

    this->setIcon(QIcon(QString(imagen.data())));

    this->setIconSize(QSize(64,64));
}

void BotonImg::imgClick()
{
    emit cargar(this->imagen);
}
