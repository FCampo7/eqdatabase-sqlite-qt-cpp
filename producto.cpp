#include "producto.h"

Producto::Producto()
{
}

void Producto::AgregarProducto(QString p, QString s, QString pr, QString pv){
    producto=p;
    stock=s;
    precio=pr;
    preciov=pv;
}

QString Producto::VerProducto(){
    return producto;
}

QString Producto::VerStock(){
    return stock;
}

QString Producto::VerPrecio(){
    return precio;
}

QString Producto::VerPreciov(){
    return preciov;
}

