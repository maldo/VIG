#ifndef IMAGENES_H
#define IMAGENES_H

#include <QWidget>
#include <vector>
#include <QGridLayout>

class GLWidget;

using namespace std;

namespace Ui {
    class Imagenes;
}

class Imagenes : public QWidget
{
    Q_OBJECT

public:
    explicit Imagenes(QWidget *parent = 0);
    ~Imagenes();

    QGridLayout *layout;
    GLWidget *gw;

    void setThis(GLWidget*);

 void setListaImg(vector<string>&);

public slots:
    void cargarImg(string);

private:
    Ui::Imagenes *ui;
};

#endif // IMAGENES_H
