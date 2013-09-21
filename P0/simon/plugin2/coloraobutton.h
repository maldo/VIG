#ifndef COLORAOBUTTON_H
#define COLORAOBUTTON_H

#include <QtGui/QPushButton>

class ColoraoButton : public QPushButton
{
    Q_OBJECT

public:
    ColoraoButton(QWidget *parent = 0);
       Q_PROPERTY(QString NombreColorao READ colorcillo WRITE SetColorcillo);


       void SetColorcillo(QString);
       QString colorcillo();

signals:
    void simon_dice_color(QString c);

private slots:
    void coloraoclick();

private:
    QString color;

};

#endif
