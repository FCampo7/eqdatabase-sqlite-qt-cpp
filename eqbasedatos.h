#ifndef EQBASEDATOS_H
#define EQBASEDATOS_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QMessageBox>
#include <vector>
#include "agregarp.h"
#include "producto.h"

using namespace std;

namespace Ui {
class EQBaseDatos;
}

class EQBaseDatos : public QMainWindow
{
    Q_OBJECT

public:
    explicit EQBaseDatos(QWidget *parent = 0);
    void AlinearTabla();
    void CalcularTotales();
    void ActualizarTabla();
    void CargarTabla();
    void TablaBuscador(vector<int> ids);
    ~EQBaseDatos();

private slots:
    void on_tableWidget_cellChanged(int row, int column);

	void on_tableWidget_itemSelectionChanged();

	void activar_admin();

	void on_Buscador_textEdited(const QString &arg1);

	void on_Opcion_currentIndexChanged(int index);

	void on_AgregarFila_triggered();

	void on_actionNuevo_Producto_triggered();

	void on_EliminarProducto_triggered();

	void on_Buscar_triggered();

	void on_AdministrarUsuarios_triggered();

	void on_LogOut_triggered();

	void on_Salir_triggered();

signals:
    void logout();

private:
    Ui::EQBaseDatos *ui;
	int flag;
	bool flagB;
    QSqlDatabase db;
    Producto p;
};

#endif // EQBASEDATOS_H
