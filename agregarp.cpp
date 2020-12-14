#include "agregarp.h"
#include "ui_agregarp.h"
#include "producto.h"

AgregarP::AgregarP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgregarP)
{
    ui->setupUi(this);
}

AgregarP::~AgregarP()
{
    delete ui;
}

void AgregarP::recibirP(Producto *pr){//Recibimos producto por referencia.
    p=pr;
}

void AgregarP::on_buttonBox_accepted()//Cargamos obj producto.
{
    QString prod, stock, precio, pv;
    prod=ui->lineEdit->text();
    stock=ui->spinBox->text();
    precio=ui->doubleSpinBox->text();
    pv=ui->doubleSpinBox_2->text();
    p->AgregarProducto(prod, stock, precio, pv);
}
