#ifndef AGREGARP_H
#define AGREGARP_H

#include <QDialog>
#include "producto.h"

namespace Ui {
class AgregarP;
}

class AgregarP : public QDialog
{
    Q_OBJECT

public:
    explicit AgregarP(QWidget *parent = 0);
    void recibirP(Producto *pr);
    ~AgregarP();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AgregarP *ui;
    Producto *p;
};

#endif // AGREGARP_H
