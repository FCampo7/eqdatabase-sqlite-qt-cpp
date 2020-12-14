#include "eqbasedatos.h"
#include "ui_eqbasedatos.h"
#include <QtSql/QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <vector>
#include <iterator>
#include "agregarp.h"
#include "producto.h"
#include "admin.h"

using namespace std;

EQBaseDatos::EQBaseDatos(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EQBaseDatos)
{
    ui->setupUi(this);
	ui->AdministrarUsuarios->setVisible(false);//Escondemos el boton de administrador.
	ui->Buscador->hide();
	ui->label->hide();
	ui->Opcion->hide();
	ui->EliminarProducto->setEnabled(false);//Deshabilita el boton eliminar cuando no hay ninguna fila seleccionada.

    flag=0;//Esta bandera es para impedir que se ejecute la funcion editar celda. (Es una bandera global) (Al estar habilitada todo el tiempo crashea el programa)
	flagB=false;

    ui->label_2->setStyleSheet("font-weight: bold;"); //Pone en negrita el texto de ese label.
    ui->label_3->setStyleSheet("font-weight: bold;"); //Idem.
    ui->label_4->setStyleSheet("font-weight: bold;"); //Idem.

    db = QSqlDatabase::addDatabase("QSQLITE"); //Agregamos base de datos
	db.setDatabaseName("EQProductos.dat"); //Seteamos el nombre (o direccion) de la base de datos.

    if(!db.open()){ //Explicado en userlogin.cpp
        QMessageBox::warning(0,"Error","No conecto la base de datos.");
    }

    QSqlQuery query("CREATE TABLE Productos (ID INTEGER PRIMARY KEY AUTOINCREMENT, Producto TEXT, Stock INTEGER, Precio DOUBLE, Pv DOUBLE)");

    query.exec("SELECT * FROM Productos");

    //Damos Forma a la tabla.
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableWidget->setHorizontalHeaderLabels(QString("Id;Producto;Stock;Precio Compra;Precio Venta").split(";"));
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setAlternatingRowColors(true);
    for (int c = 1; c < ui->tableWidget->horizontalHeader()->count(); ++c){
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    //Fin dar forma a la tabla.

    CargarTabla();

    CalcularTotales();

    flag=1;
}

void EQBaseDatos::AlinearTabla(){ //Centramos el contenido de las celdas.
    for(int i=0; i<ui->tableWidget->rowCount(); i++){
        for(int c=0; c<5; c++){
            ui->tableWidget->item(i, c)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void EQBaseDatos::CargarTabla(){//Cargamos la tabla con el contenido de la base de datos.
    QSqlQuery query;
    query.exec("SELECT * FROM Productos");
    int i=0;

    while(query.next()){
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(query.value(3).toString().replace(".", ",")));//Reemplazamos el punto "." por la coma ",".
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(query.value(4).toString().replace(".", ",")));//Idem.
        i++;
    }
    AlinearTabla();//Centramos el contenido de las celdas.
}

void EQBaseDatos::activar_admin(){
	ui->AdministrarUsuarios->setVisible(true);//Mostramos el boton de administrador.
}

void EQBaseDatos::CalcularTotales() //Calcular los totales
{
    QString totalgan, totalpv, totalp;
    QSqlQuery query2("SELECT '$' || ROUND(SUM((Stock*Pv)-(Stock*Precio)), 2) FROM Productos");
    query2.next();
	totalgan="Total Ganancias: \n"+query2.value(0).toString();
    query2.exec("SELECT '$' || ROUND(SUM(Stock*Pv), 2) FROM Productos");
    query2.next();
	totalpv="Total Precio Venta: \n"+query2.value(0).toString();
    query2.exec("SELECT '$' || ROUND(SUM(Stock*Precio), 2) FROM Productos");
    query2.next();
	totalp="Total Precio Compra: \n"+query2.value(0).toString();
    query2.finish();
    //Seteamos el texto de estos labels con los totales de precio de compra, venta y ganancias.
    ui->label_4->setText(totalp);
    ui->label_3->setText(totalpv);
    ui->label_2->setText(totalgan);
}

void EQBaseDatos::TablaBuscador(vector<int> ids){//Actualizamos la tabla de acuerdo a los resultados de la busqueda.
    flag=0;

    QSqlQuery qry("SELECT * FROM Productos");
    vector<int>::iterator it;
    int i=0;

    while(qry.next()){
        it=ids.begin();
        while(it!=ids.end()){
            if(*it==qry.value(0).toInt()){
                ui->tableWidget->insertRow(i);
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(qry.value(0).toString()));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(qry.value(1).toString()));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(qry.value(2).toString()));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(qry.value(3).toString().replace(".", ",")));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(qry.value(4).toString().replace(".", ",")));
                i++;
            }
            it++;
        }
    }

    AlinearTabla();

    flag=1;
}

void EQBaseDatos::ActualizarTabla(){ //Actualizar tabla.
    flag=0;

    QSqlQuery query("SELECT * FROM Productos");
    query.last();
    int last=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(last);
    ui->tableWidget->setItem(last, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->tableWidget->setItem(last, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->tableWidget->setItem(last, 2, new QTableWidgetItem(query.value(2).toString()));
    CalcularTotales();
    ui->tableWidget->setItem(last, 3, new QTableWidgetItem(query.value(3).toString().replace(".", ",")));
    ui->tableWidget->setItem(last, 4, new QTableWidgetItem(query.value(4).toString().replace(".", ",")));

    AlinearTabla();

    flag=1;
}

EQBaseDatos::~EQBaseDatos()
{
    delete ui;
}

void EQBaseDatos::on_tableWidget_cellChanged(int row, int column) //Editar Celda
{
    if(flag==1){
        QSqlQuery query("SELECT * FROM Productos");
        QString aux;
        query.prepare("UPDATE Productos SET Producto=:prod, Stock=:stock, Precio=:pre, Pv=:pv WHERE ID = :id");
        query.bindValue(":prod", ui->tableWidget->item(row, 1)->text());
        query.bindValue(":stock", ui->tableWidget->item(row, 2)->text());
        aux=ui->tableWidget->item(row, 3)->text().remove(".");
        query.bindValue(":pre", aux.replace(",", "."));
        aux=ui->tableWidget->item(row, 4)->text().remove(".");
        query.bindValue(":pv", aux.replace(",", "."));
        query.bindValue(":id", ui->tableWidget->item(row, 0)->text());
        query.exec();

        CalcularTotales();
    }
}

void EQBaseDatos::on_tableWidget_itemSelectionChanged() //Habilita el boton eliminar
{
	ui->EliminarProducto->setEnabled(true);
}

void EQBaseDatos::on_Buscador_textEdited(const QString &arg1)
{
    flag=0;
    vector<int> id;
    QSqlQuery qry("SELECT * FROM Productos");
    switch(ui->Opcion->currentIndex()){//Segun opcion (Producto = 0, ID=1).
        case 0:{
            while(qry.next()){
                if(qry.value(1).toString().toLower().contains(arg1.toLower())){
                    id.push_back(qry.value(0).toInt());//Guardo en el vector id los ids en los que el producto contiene arg1.
                }
            }
            break;
        }
        case 1:{
            while(qry.next()){
                if(qry.value(0).toString().contains(arg1))//Lo mismo pero con el id del producto.
                    id.push_back(qry.value(0).toInt());
            }
            break;
        }
    }
    qry.exec();

    //Modificar Tabla.
    if(arg1==""){
        ui->tableWidget->clearContents();
        for(int i=ui->tableWidget->rowCount(); i>=0; i--){
            ui->tableWidget->removeRow(i);
        }
        CargarTabla();
    }
    else{
        ui->tableWidget->clearContents();
        for(int i=ui->tableWidget->rowCount(); i>=0; i--){
            ui->tableWidget->removeRow(i);
        }
        TablaBuscador(id);
    }
    //Fin Modificar Tabla.
    flag=1;
}

void EQBaseDatos::on_Opcion_currentIndexChanged(int index)
{
	switch(index){
		case 0:
			ui->Buscador->setInputMask("");
		break;
		case 1:
			ui->Buscador->setInputMask("000000");
		break;
	}
	ui->Buscador->setFocus();
}

void EQBaseDatos::on_AgregarFila_triggered()
{
	QSqlQuery query("SELECT * FROM Productos");
	query.prepare("INSERT INTO Productos (Producto,Stock,Precio,Pv) VALUES (?,?,?,?)");
	query.addBindValue("");
	query.addBindValue("0");
	query.addBindValue("0,00");
	query.addBindValue("0,00");
	query.exec();
	ActualizarTabla();
}

void EQBaseDatos::on_actionNuevo_Producto_triggered()
{
	QSqlQuery query("SELECT * FROM Productos");
	AgregarP ap; //Creamos la Ventana para agregar productos.
	ap.recibirP(&p); //Pasamos por referencia un objeto producto (ver producto.cpp)
	if(ap.exec()){ //Ejecutamos la ventana, insertamos el producto en la base de datos y actualizamos la tabla.
		query.prepare("INSERT INTO Productos (Producto,Stock,Precio,Pv) VALUES (?,?,?,?)");
		query.addBindValue(p.VerProducto());
		query.addBindValue(p.VerStock());
		query.addBindValue(p.VerPrecio().replace(",", "."));
		query.addBindValue(p.VerPreciov().replace(",", "."));
		query.exec();

		ActualizarTabla();
	}
}

void EQBaseDatos::on_EliminarProducto_triggered()
{
	flag=0;

	switch(QMessageBox::warning(this, "Advertencia", "¿Desea eliminar las filas seleccionadas?", "&Si", "&No")){//Mostramos advertencia y segun corresponda eliminamos o no los productos seleccionados.
		case 0:{
			int row;

			QSqlQuery query("SELECT * FROM Productos");
			QItemSelectionModel *selecciones = ui->tableWidget->selectionModel();
			QModelIndexList seleccionado = selecciones->selectedRows();
			qSort(seleccionado);//Ordenamos las filas seleccionadas.

			for(int i=(seleccionado.size()-1); i>=0; i--){
				row=seleccionado[i].row();//Obtenemos la fila del item en la posicion i.
				query.exec("DELETE FROM Productos WHERE ID="+ui->tableWidget->item(row, 0)->text());//Eliminamos el producto de la base de datos por su id.
				ui->tableWidget->removeRow(row);//Removemos la fila seleccionada.
			}
			query.exec("UPDATE sqlite_sequence SET seq = 0 WHERE  rowid = 1");//Seteamos los id en 0.
			query.exec();

			CalcularTotales();

			ui->EliminarProducto->setEnabled(false);//Deshabilitamos el boton eliminar.

			break;
		}
		case 1:{
			break;
		}
	}

	flag=1;
}

void EQBaseDatos::on_Buscar_triggered()
{
	if(flagB==true){
		flagB=false;
	}
	else{
		flagB=true;
	}
	if(flagB){
		ui->Buscador->show();
		ui->label->show();
		ui->Opcion->show();
		ui->Buscador->setFocus();
	}
	else {
		ui->Buscador->hide();
		ui->label->hide();
		ui->Opcion->hide();
	}
}

void EQBaseDatos::on_AdministrarUsuarios_triggered()
{
	Admin a;
	a.exec();
}

void EQBaseDatos::on_LogOut_triggered()
{
	emit logout();
	ui->AdministrarUsuarios->setVisible(false);
	this->close();
}

void EQBaseDatos::on_Salir_triggered()
{
	switch(QMessageBox::warning(this, "Salir", "¿Cerrar el programa?", "&Aceptar", "&Cancelar")){
		case 0:
			qApp->quit();
		break;
		case 1:
		break;
	}
}
