#include "admin.h"
#include "ui_admin.h"
#include <QtSql/QtSql>
#include <QMessageBox>

Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    ui->nombre->setFocus();//Foco en el lineEdit nombre.

    db = QSqlDatabase::database("db2");//Selecciono la base de datos db2 (Explicado en userlogin.cpp).

    //Creo y edito el objeto model que es la tabla donde se mostraran los usuarios.
    model = new QSqlTableModel(0, db);
    model->setTable("Users");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
    model->setHeaderData(2, Qt::Horizontal, tr("Apellido"));
    model->setHeaderData(3, Qt::Horizontal, tr("Usuario"));
    model->setHeaderData(4, Qt::Horizontal, tr("Contraseña"));
    model->setHeaderData(5, Qt::Horizontal, tr("Administrador"));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    //Fin model.

}

Admin::~Admin()
{
    delete ui;
    delete model;
}

void Admin::on_EliminarUsuario_clicked()//Explicado en la parte Eliminar de eqbasedatos.cpp.
{
    switch(QMessageBox::warning(this, "Advertencia", "¿Desea eliminar los usuarios seleccionados?", "&Si", "&No")){
        case 0:{
            int row;

            QSqlQuery query("SELECT * FROM Users", db);//query a db2.
            QItemSelectionModel *selecciones = ui->tableView->selectionModel();
            QModelIndexList seleccionado = selecciones->selectedRows();
            qSort(seleccionado);

            for(int i=(seleccionado.size()-1); i>=0; i--){
                row=seleccionado[i].row();
                query.exec("DELETE FROM Users WHERE ID="+model->record(row).value(0).toString());
                model->removeRow(row);
            }
            query.exec("UPDATE sqlite_sequence SET seq = 0 WHERE  rowid = 1");
            query.exec();

            model->select();

            break;
        }
        case 1:{
            break;
        }
    }
}

void Admin::on_Registrar_clicked()//Agregar usuarios. (Similar agregar producto).
{
    QSqlQuery qry("SELECT * FROM Users", db);//qry a db2.
    bool flag=false;
    while(qry.next()){//Control para ver si ya existe un usuario con ese nombre.
        if (ui->user->text()==qry.value(3).toString()){
            flag=true;
            ui->user->setFocus();
            ui->Control->setText("Ya existe un usuario con ese nombre de usuario.");
            break;
        }
    }
    if(!flag){
        ui->Control->setText("Registro exitoso.");
        qry.prepare("INSERT INTO Users (nombre, apellido, user, pass, admin) VALUES(?,?,?,?,?)");
        qry.addBindValue(ui->nombre->text());
        qry.addBindValue(ui->apellido->text());
        qry.addBindValue(ui->user->text());
        qry.addBindValue(QString(QCryptographicHash::hash(ui->pass->text().toUtf8(),QCryptographicHash::Md5).toHex()));
        qry.addBindValue(ui->admin->currentText());
        qry.exec();
        //Limpiamos todos los campos de registro.
        ui->nombre->clear();
        ui->apellido->clear();
        ui->user->clear();
        ui->pass->clear();
        ui->admin->setCurrentIndex(0);
        ui->nombre->setFocus();
        model->select();
    }
}
