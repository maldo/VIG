#include "coloraobutton.h"

ColoraoButton::ColoraoButton(QWidget *parent) :
    QPushButton(parent)
{
    QObject::connect(this, SIGNAL(clicked()),this,SLOT(coloraoclick()));
}

void ColoraoButton::SetColorcillo(QString x)
{
    color = x;
}

QString ColoraoButton::colorcillo()
{
    return color;
}

void ColoraoButton::coloraoclick()
{
    QString x = colorcillo();
    emit(simon_dice_color(x));
}
