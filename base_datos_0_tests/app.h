#ifndef APP_H
#define APP_H

#include "doctest.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class Db
{

public:
    QSqlDatabase m_db;
    Db();
    ~Db();
    bool init();
    void close();
    QSqlError lastError();
    bool insert();
};

class user {

public:

    user();
    int m_iduser = 0;
    QString m_user;
    QString m_password;


    //bbdd
    bool guardar();
    void cargar(Db db, int id);
};

class App : public QObject
{
    Q_OBJECT

    doctest::Context m_context;

public:
    App(QObject *parent = nullptr);

public slots:
    void run();
};

#endif // APP_H
