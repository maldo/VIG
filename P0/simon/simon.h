#ifndef SIMON_H
#define SIMON_H

#include <QWidget>

namespace Ui {
    class simon;
}

class simon : public QWidget {
    Q_OBJECT
public:
    simon(QWidget *parent = 0);
    ~simon();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::simon *ui;
};

#endif // SIMON_H
