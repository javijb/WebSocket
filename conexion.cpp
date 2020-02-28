#include "conexion.h"
#include <iostream>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QString>

/*! \file */

/**
 * @brief Constructor conexion
 */
Conexion::Conexion()
{

}

bool Conexion::open()
{

    /// 1) Cargar propiedades
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("AdidasWeb");
    m_db.setUserName("postgres");
    m_db.setPassword("");


    /// 2) Abrir conexión
    bool ok = m_db.open();

    return ok;

}

void Conexion::close()
{

    m_db.close();

}
