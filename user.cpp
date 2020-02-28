#include "user.h"
#include <QVariant>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include "servidor.h"

/*! \file */


//Getters
int User::getId(){ return m_id; }
/**
 * @brief Obtener nombre del usuario
 * @return nombre
 */
QString User::getNombre(){ return m_nombre; }

/**
 * @brief Obtener apellidos del usuario
 * @return apellidos
 */
QString User::getApellidos(){ return m_apellidos; }

/**
 * @brief Obtener email del usuario
 * @return email
 */
QString User::getEmail(){ return m_email; }

/**
 * @brief Obtener telefono del usuario
 * @return telefono
 */
QString User::getTelefono(){ return m_telefono; }

/**
 * @brief Obtener contraseña del usuario
 * @return contraseña
 */
QString User::getPassword(){ return m_password; }

/**
 * @brief Obtener fecha de nacimiento del usuario
 * @return fecha de nacimiento
 */
QString User::getFecha_nac(){ return m_fecha_nac; }

/**
 * @brief Obtener genero del usuario
 * @return genero
 */
QString User::getGen(){ return m_gen; }

/**
 * @brief Obtener nacionalidad del usuario
 * @return nacionalidad
 */
QString User::getNac(){ return m_nac; }

/**
 * @brief Obtener provincia del usuario
 * @return provincia
 */
QString User::getProvincia(){ return m_provincia; }

/**
 * @brief Obtener direccion del usuario
 * @return direccion
 */
QString User::getDir(){ return m_dir; }

//Setters
/**
 * @brief Aplica el nombre a un usuario
 * @param nombre
 */

void User::setNombre(QString nombre){
    m_nombre = nombre;
}

/**
 * @brief Aplica el apellidos a un usuario
 * @param apellidos
 */

void User::setApellidos(QString apellidos){
    m_apellidos = apellidos;
}

/**
 * @brief Aplica el email a un usuario
 * @param email
 */

void User::setEmail(QString email){
    m_email = email;
}

/**
 * @brief Aplica el telefono a un usuario
 * @param telefono
 */

void User::setTelefono(QString telefono){
    m_telefono = telefono;
}

/**
 * @brief Aplica la contraseña a un usuario
 * @param password
 */

void User::setPassword(QString password){
    m_password = password;
}

/**
 * @brief Aplica la fecha de nacimiento a un usuario
 * @param fecha_nac
 */

void User::setFecha_nac(QString fecha_nac){
    m_fecha_nac = fecha_nac;
}

/**
 * @brief Aplica la genero a un usuario
 * @param genero
 */

void User::setGen(QString genero){
    m_gen = genero;
}

/**
 * @brief Aplica la nacionalidad a un usuario
 * @param nacionalidad
 */

void User::setNac(QString nacionalidad){
    m_nac = nacionalidad;
}

/**
 * @brief Aplica la provincia a un usuario
 * @param provincia
 */

void User::setProvincia(QString provincia){
    m_provincia = provincia;
}

/**
 * @brief Aplica la direccion a un usuario
 * @param direccion
 */

void User::setDir(QString direccion){
    m_dir = direccion;
}


/**
 * @brief Creamos la función save y la query, que nos permite hacer la consulta y insertar los datos correspodientes en la tabla.
 * @return Nos devuelve el resultado de la query.
 */
bool User::save()
{

    QSqlQuery query;
    query.prepare("INSERT INTO public.user (nombre, apellidos, email, telefono, password, fecha_nac, genero, nacionalidad, provincia, direccion) VALUES (:nombre, :apellidos, :email, :telefono, :password, :fecha_nac, :genero, :nacionalidad, :provincia, :direccion);");

    query.bindValue(":nombre", m_nombre);
    query.bindValue(":apellidos", m_apellidos);
    query.bindValue(":email", m_email);
    query.bindValue(":telefono", m_telefono);
    query.bindValue(":password", m_password);
    query.bindValue(":fecha_nac", m_fecha_nac);
    query.bindValue(":genero", m_gen);
    query.bindValue(":nacionalidad", m_nac);
    query.bindValue(":provincia", m_provincia);
    query.bindValue(":direccion", m_dir);

    bool result {query.exec()};

    qDebug() << query.lastQuery();

    qDebug() << result;
    qDebug() << query.lastError();


    QString error (query.lastError().text());

    if(error == " ")
    {
        query.prepare("select * from user order by idusuario desc limit 1;");
        query.exec();
        QSqlRecord rec = query.record();
        while (query.next())
        {
            int id = query.value("idusuario").toInt();
            this->m_id = id;
        }
    }else
    {
        qDebug() << error;
    }

}
/**
 * @brief Cargamos el usuario y la contraseña para que la función de login se pueda utilizar.
 * @param id
 */
void User::cargar(int id)
{

    QSqlQuery query;   

    query.prepare("SELECT * from public.user where email = :email LIMIT 1");
    query.bindValue(":email", id);
    bool result {query.exec()};
    qDebug() << query.size();


    if (result)
    {
        query.next();
        m_email = query.value("email").toString();
        m_password = query.value("password").toString();

    }

}
/**
 * @brief Permite borrar un usuario registrado.
 * @param id
 */
void User::remove(int id)
{

    QSqlQuery query;
    query.prepare("DELETE FROM usuario where idusuario = :id");

    query.bindValue(":id", id);
    query.exec();

}


