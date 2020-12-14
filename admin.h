#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QtSql/QtSql>

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = 0);
    ~Admin();

private slots:
    void on_EliminarUsuario_clicked();

    void on_Registrar_clicked();

private:
    QSqlDatabase db;
    QSqlTableModel *model;
    Ui::Admin *ui;
};

#endif // ADMIN_H
