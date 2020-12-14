#include "userlogin.h"
#include "ui_userlogin.h"
#include <QtSql/QtSql>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QString>

UserLogIn::UserLogIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserLogIn)
{
    ui->setupUi(this);
    ui->UserEdit->setFocus();
    db = QSqlDatabase::addDatabase("QSQLITE", "db2"); //db2 Es el nombre de la conexion a la base de datos
    db.setDatabaseName("User.sqlite");

    if(!db.open()){//Si no se puede abrir la base de datos muestra un dialogo de error.
        QMessageBox::warning(this, "Error", "No se pudo abrir la base de datos.");
    }

    QSqlQuery qry("CREATE TABLE Users (ID INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT, apellido TEXT, user TEXT, pass TEXT, admin TEXT)", db);
    qry.exec();

    qry.exec("SELECT * FROM Users");

    //Crear el usuario administrador en caso de que no exista uno.
    bool existeAdmin=false;
    while(qry.next()){//Recorremos para ver si existe algun usuario administrador.
        if(qry.value(5).toString()=="Si"){
            existeAdmin=true;
            break;
        }
    }
    if(qry.isNull("user")||!existeAdmin){//Si no hay usuarios o no existe ningun usuario administrador crea uno.
        qry.prepare("INSERT INTO Users (nombre, apellido, user, pass, admin) VALUES (?,?,?,?,?)");
        qry.addBindValue("Francisco Luis");
        qry.addBindValue("Campo");
        qry.addBindValue("fcampo7");
        qry.addBindValue(QString(QCryptographicHash::hash("falshpass",QCryptographicHash::Md5).toHex()));
        qry.addBindValue("Si");
        qry.exec();
    }
    //Fin Crear usuario.
}

UserLogIn::~UserLogIn()
{
    delete ui;
}

void UserLogIn::on_LogIn_clicked()
{
    QString user, pass, userC, passC;
    QSqlQuery qry("SELECT * FROM Users", db);
    bool flag=false;//Bandera para aceptar o rechazar login.

    user=ui->UserEdit->text();
    pass=QString(QCryptographicHash::hash(ui->PassEdit->text().toUtf8(),QCryptographicHash::Md5).toHex());//Obtenemos la pass y la hasheamos para compararla con la base de datos.

    //Vemos si el usuario y la pass coinciden.
    while(qry.next()){
        userC=qry.value(3).toString();
        passC=qry.value(4).toString();
        if((user==userC)&&(pass==passC)){
            if(qry.value(5).toString()=="Si"){
                emit administrador();//Si el usuario es administrador muestra el boton de administrador.
            }
            flag=true;
            break;
        }
    }
    if(flag){
        ui->UserEdit->clear();
        ui->PassEdit->clear();
        emit loggin();
        this->close();
        ui->UserEdit->setFocus();
        ui->Control->setText("");
    }
    else{
        ui->Control->setText("Usuario y/o Contraseña Incorrectos");
		ui->UserEdit->setFocus();
		ui->PassEdit->clear();
    }
}
