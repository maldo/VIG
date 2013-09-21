#include "modmaterial.h"
#include "ui_modmaterial.h"
#include <cstdio>
#include <iostream>
#include <glwidget.h>

using namespace std;

ModMaterial::ModMaterial(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ModMaterial)
{
    ui->setupUi(this);

}

ModMaterial::~ModMaterial()
{
    delete ui;
}

void ModMaterial::setThis(GLWidget *s)
{
    gw = s;

    firstAmbiente = false;
    firstDifusa = false;
   firstEspecular = false;
}

/*-----------------Ambiente----------------*/
void ModMaterial::getkaR(int r)
{
    if (!firstAmbiente) return;

    ka[0] = (float)r/100;
    cout<<ka[0]<<endl;
    imprimeAmbiente(ka[0],ka[1],ka[2]);
    gw->setAmbiente(ka);
}

void ModMaterial::getkaG(int g)
{
     if (!firstAmbiente) return;

    ka[1] = (float)g/100;
    imprimeAmbiente(ka[0],ka[1],ka[2]);
    gw->setAmbiente(ka);
}

void ModMaterial::getkaB(int b)
{
     if (!firstAmbiente) return;

    ka[2] = (float)b/100;
    imprimeAmbiente(ka[0],ka[1],ka[2]);
    gw->setAmbiente(ka);
}

/*-----------------Difusa----------------*/
void ModMaterial::getkdR(int r)
{
     if (!firstDifusa) return;

    kd[0] = (float)r/100;
    imprimeDifusa(kd[0],kd[1],kd[2]);
    gw->setDifusa(kd);
}

void ModMaterial::getkdG(int g)
{
    if (!firstDifusa) return;

    kd[1] = (float)g/100;
    imprimeDifusa(kd[0],kd[1],kd[2]);
    gw->setDifusa(kd);
}

void ModMaterial::getkdB(int b)
{
    if (!firstDifusa) return;

    kd[2] = (float)b/100;
    imprimeDifusa(kd[0],kd[1],kd[2]);
    gw->setDifusa(kd);
}

/*-----------------Especular----------------*/
void ModMaterial::getksR(int r)
{
    if (!firstEspecular) return;

    ks[0] =( float)r/100;
    imprimeEspecular(ks[0],ks[1],ks[2]);
    gw->setEspecular(ks);
}

void ModMaterial::getksG(int g)
{
     if (!firstEspecular) return;

    ks[1] = (float)g/100;
    imprimeEspecular(ks[0],ks[1],ks[2]);
    gw->setEspecular(ks);
}

void ModMaterial::getksB(int b)
{
     if (!firstEspecular) return;

    ks[2] = (float)b/100;
    imprimeEspecular(ks[0],ks[1],ks[2]);
    gw->setEspecular(ks);
}


/*-----------------Brillo----------------*/
void ModMaterial::getBrillo(int b)
{
    brillo = (float)b/100;
    imprimeBrillo(brillo);
    gw->setBrillo(b);
}


/*-----------------Actualizar Labels----------------*/
void ModMaterial::imprimeAmbiente(float r, float g, float b)
{
    char buffer[50];

    sprintf(buffer,"(%.2g, %.2g, %.2g)",r,g,b);

    QString x = QString(buffer);

    emit setRGBAmbiente(x);
}

void ModMaterial::imprimeDifusa(float r, float g, float b)
{
    char buffer[50];

    sprintf(buffer,"(%.2g, %.2g, %.2g)",r,g,b);

    QString x = QString(buffer);

    emit setRGBDifuso(x);
}

void ModMaterial::imprimeEspecular(float r, float g, float b)
{
    char buffer[50];

    sprintf(buffer,"(%.2g, %.2g, %.2g)",r,g,b);

    QString x = QString(buffer);

    emit setRGBEspecular(x);
}

void ModMaterial::imprimeBrillo(float r)
{
    char buffer[10];

    sprintf(buffer,"(%.2g)",r);

    QString x = QString(buffer);

    emit setBrillo(x);
}

void ModMaterial::currentbrillo(float b)
{
    emit setSliderBrillo((int)(b*100));
}

void ModMaterial::currentka(float *ka)
{
    this->ka[0] = ka[0];
    this->ka[1] = ka[1];
    this->ka[2] = ka[2];

    emit setSliderkaR((int)(ka[0]*100));
    emit setSliderkaG((int)(ka[1]*100));
    emit setSliderkaB((int)(ka[2]*100));

    firstAmbiente = true;

    imprimeAmbiente(ka[0],ka[1],ka[2]);
}

void ModMaterial::currentkd(float *kd)
{
    this->kd[0] = kd[0];
    this->kd[1] = kd[1];
    this->kd[2] = kd[2];

    emit setSliderkdR((int)(kd[0]*100));
    emit setSliderkdG((int)(kd[1]*100));
    emit setSliderkdB((int)(kd[2]*100));

    firstDifusa = true;

    imprimeDifusa(kd[0],kd[1],kd[2]);
}

void ModMaterial::currentks(float *ks)
{
    this->ks[0] = ks[0];
    this->ks[1] = ks[1];
    this->ks[2] = ks[2];

    emit setSliderksR((int)(ks[0]*100));
    emit setSliderksG((int)(ks[1]*100));
    emit setSliderksB((int)(ks[2]*100));

    firstEspecular = true;

    imprimeEspecular(ks[0],ks[1],ks[2]);
}
