#ifndef PSQLINTERFACE_H
#define PSQLINTERFACE_H

#include <QtSql>
#include <QString>

class PSQLInterface : public QObject
{
    Q_OBJECT

    public:
    PSQLInterface();
    ~PSQLInterface();
    QSqlQueryModel* issue_query(QString query);
    bool isOpen();
    QString get_login(){return login;}

    public slots:
    void connect_to_bd(QString login, QString pswd);

    private:
    QSqlDatabase db;
    QString login;
};

#endif 