#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>

namespace Ui {
    class Principal;
}

class Principal : public QWidget {
    Q_OBJECT
public:
    Principal(QWidget *parent = 0);
    ~Principal();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Principal *ui;
};

#endif // PRINCIPAL_H
