#ifndef BLUELABEL_H
#define BLUELABEL_H

#include <QtGui/QLabel>


class BlueLabel : public QLabel
{
    Q_OBJECT

public:
    BlueLabel(QWidget *parent = 0);

signals:

public slots:
    void concatenate(QString x);

};

#endif
