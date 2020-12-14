#include "eqbasedatos.h"
#include "userlogin.h"
#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Seteamos paleta de colores.
    qApp->setStyle(QStyleFactory::create("fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

	palette.setColor(QPalette::Highlight, QColor(36,130,50).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(palette);
    //Fin paleta de colores.

    EQBaseDatos w;
    UserLogIn l;

    l.show();

    QObject::connect(&l, SIGNAL(loggin()), &w, SLOT(show()));//Si el usuario y la pass coinciden, se muestra la ventana de la tabla de productos.
    QObject::connect(&l, SIGNAL(administrador()), &w, SLOT(activar_admin()));//Si el usuario es administrador se muestra el boton de aministrar de la ventana de la tabla de productos.
    QObject::connect(&w, SIGNAL(logout()), &l, SLOT(show()));//Si se deslogea se cierra la ventana de la tabla de productos y se muestra la ventana de login.

    return a.exec();
}
