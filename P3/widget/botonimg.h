#ifndef BOTONIMG_H
#define BOTONIMG_H

#include <QtGui/QPushButton>

using namespace std;

class BotonImg : public QPushButton
{
    Q_OBJECT

public:
    BotonImg(QWidget *parent = 0);

    void setImg(string);
signals:
    void cargar(string);

public slots:
    void imgClick();

private:
    string imagen;

};

#endif // BOTONIMG_H
