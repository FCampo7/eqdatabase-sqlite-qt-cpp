#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <QString>

class Producto
{
private:
    QString producto;
    QString stock;
    QString precio;
    QString preciov;
public:
    Producto();
    void AgregarProducto(QString p, QString s, QString pr, QString pv);
    QString VerProducto();
    QString VerStock();
    QString VerPrecio();
    QString VerPreciov();
};

#endif // PRODUCTO_H
