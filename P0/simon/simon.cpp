#include "simon.h"
#include "ui_simon.h"

simon::simon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::simon)
{
    ui->setupUi(this);
}

simon::~simon()
{
    delete ui;
}

void simon::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
