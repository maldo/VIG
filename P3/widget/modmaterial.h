#ifndef MODMATERIAL_H
#define MODMATERIAL_H

#include <QWidget>

class GLWidget;

namespace Ui {
    class ModMaterial;
}

class ModMaterial : public QWidget
{
    Q_OBJECT

public:
    explicit ModMaterial(QWidget *parent = 0);
    ~ModMaterial();

    float kd[3];
    float ks[3];
    float ka[3];
    float brillo;
    GLWidget *gw;
    void setThis(GLWidget*);
    bool firstAmbiente;
    bool firstDifusa;
    bool firstEspecular;

    void currentka(float*);
    void currentks(float*);
    void currentkd(float*);
    void currentbrillo(float);

signals:
    void setSliderkdR(int);
    void setSliderkdG(int);
    void setSliderkdB(int);

    void setSliderkaR(int);
    void setSliderkaG(int);
    void setSliderkaB(int);

    void setSliderksR(int);
    void setSliderksG(int);
    void setSliderksB(int);

    void setSliderBrillo(int);

    void setRGBDifuso(QString);
    void setRGBAmbiente(QString);
    void setRGBEspecular(QString);
    void setBrillo(QString);

public slots:
    void getkdR(int);
    void getkdG(int);
    void getkdB(int);

    void getksR(int);
    void getksG(int);
    void getksB(int);

    void getkaR(int);
    void getkaG(int);
    void getkaB(int);

    void getBrillo(int);

private:
    Ui::ModMaterial *ui;
    void imprimeAmbiente(float, float, float);
    void imprimeDifusa(float, float, float);
    void imprimeEspecular(float, float, float);
    void imprimeBrillo(float);
};

#endif // MODMATERIAL_H
