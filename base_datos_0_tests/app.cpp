#include "app.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>

App::App(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, this, SLOT(run()));
}


void App::run()
{
    qDebug() << "running...";

    m_context.run(); // run
    if (m_context.shouldExit())
    {
        QCoreApplication::quit();
    } // end if

}

Db::~Db()
{
    if (m_db.isOpen())
    {
        m_db.close();
    } // end if
}

user::user(){

}

void user::cargar(Db db, int id){
    QSqlQuery query(db.m_db);
    query.prepare("SELECT * from user where iduser = :iduser LIMIT 1");
    query.bindValue(":iduser", id);
    bool r {query.exec()};
    qDebug() << query.size();

    if (r) {
        query.next();
        m_iduser = id;
        m_user = query.value("nombre").toString();
        m_password = query.value("password").toString();
    } //end if
}

bool Db::insert()
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO public.user (nombre, password) VALUES (:nombre, :pass)");
    query.bindValue(":nombre", "javier");
    query.bindValue(":pass", "hola1");
    bool r = query.exec();
    qDebug() << query.lastError().text();
    return r;
}

Db::Db()
{
    if (!QSqlDatabase::contains( "MyDb"))
    {
        m_db = QSqlDatabase::addDatabase("QPSQL", "MyDb");
    }
    else
    {
        m_db = QSqlDatabase::database("MyDb");
    } // end if
    m_db.setHostName("localhost");
    m_db.setPort(5432);
    m_db.setUserName("postgres");
    m_db.setPassword("");

}

void Db::close()
{
    m_db.close();
}

bool Db::init()
{
    bool res = 0;

    m_db.close();
    m_db.setDatabaseName("template1");

    bool ok = m_db.open();

    if (ok)
    {

        qDebug() << "Borrando...";
        QSqlQuery query0("DROP DATABASE IF EXISTS prueba_doctest", m_db);
        if (query0.lastError().type() == QSqlError::NoError)
        {
            qDebug() << "Creando...";
            QSqlQuery query1("CREATE DATABASE prueba_doctest", m_db);
            if (query1.lastError().type() == QSqlError::NoError)
            {
                m_db.close();
                m_db.setDatabaseName("prueba_doctest");
                m_db.open();

                /// Crea estructura en la base de datos
                QString sql {"CREATE TABLE public.user ( \
                    iduser     SERIAL, \
                    nombre    varchar(40), \
                    password  varchar(20), \
                    PRIMARY KEY(iduser) \
                )"};

                qDebug() << "Iniciando...";
                QSqlQuery query2(sql, m_db);
                if (query2.lastError().type() == QSqlError::NoError)
                {
                    res = true;
                } // end if
            }
            else
            {
                qDebug() << "BB:" << m_db.lastError().text();
            } // end if

        }
        else
        {
            qDebug() << "AA:" << m_db.lastError().text();
        } // end if

    } // end if

    return res;
}

QSqlError Db::lastError()
{
    return m_db.lastError();
}

TEST_CASE("base datos #0")
{
    Db db;
    int hello = db.init();

    if (hello)
    {
        qDebug() << "DB UP AND RUNNING";

        SUBCASE("Insertar #0")
        {
            CHECK( db.insert() == true );
        }
    }
    else
    {
        qDebug() << db.lastError().text();

    } // end if

}

TEST_CASE("base datos #1")
{
    Db db;
    int hello = db.init();
    user us;

    if (hello)
    {
        qDebug() << "DB UP AND RUNNING";
        db.insert();

        SUBCASE("UserLoad #0")
        {
            CHECK( us.m_iduser == 0 );
        }
    }
    else
    {
        qDebug() << db.lastError().text();

    } // end if

}



