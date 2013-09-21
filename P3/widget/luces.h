#ifndef LUCES_H
#define LUCES_H

#include <QWidget>

class GLWidget;

namespace Ui {
    class Luces;
}

class Luces : public QWidget
{
    Q_OBJECT

public:
    explicit Luces(QWidget *parent = 0);
    ~Luces();

    int id;
    float colorfoco[4];
    float altura;
    bool activado;
    GLWidget *gw;
    bool first;

    void setThis(GLWidget*);

signals:

    void setRGB(QString);

    void setR(int);
    void setG(int);
    void setB(int);

    void setActivado(bool);


public slots:

    void getR(int);
    void getG(int);
    void getB(int);

    void getAltura(int);

    void getId(int);

    void getActivacion(bool);

private:
    Ui::Luces *ui;
    void imprimeColor(float, float, float);
    void setFocoActivo(int, bool);

};

#endif // LUCES_H
