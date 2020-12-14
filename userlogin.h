#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QMainWindow>
#include <QtSql/QtSql>

namespace Ui {
class UserLogIn;
}

class UserLogIn : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserLogIn(QWidget *parent = 0);
    ~UserLogIn();

private slots:
    void on_LogIn_clicked();

signals:
    void loggin();
    void administrador();

private:
    Ui::UserLogIn *ui;
    QSqlDatabase db;
};

#endif // USERLOGIN_H
