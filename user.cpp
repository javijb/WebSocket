#include "user.h"
#include <QVariant>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include "servidor.h"
//Getters
int User::getId(){ return m_id; }
QString User::getNombre(){ return m_nombre; }
QString User::getApellidos(){ return m_apellidos; }
QString User::getEmail(){ return m_email; }
QString User::getTelefono(){ return m_telefono; }
QString User::getPassword(){ return m_password; }
QString User::getFecha_nac(){ return m_fecha_nac; }
QString User::getGen(){ return m_gen; }
QString User::getNac(){ return m_nac; }
QString User::getProvincia(){ return m_provincia; }
QString User::getDir(){ return m_dir; }

//Setters
void User::setNombre(QString nombre){
    m_nombre = nombre;
}

void User::setApellidos(QString apellidos){
    m_apellidos = apellidos;
}

void User::setEmail(QString email){
    m_email = email;
}

void User::setTelefono(QString telefono){
    m_telefono = telefono;
}

void User::setPassword(QString password){
    m_password = password;
}

void User::setFecha_nac(QString fecha_nac){
    m_fecha_nac = fecha_nac;
}

void User::setGen(QString genero){
    m_gen = genero;
}

void User::setNac(QString nacionalidad){
    m_nac = nacionalidad;
}

void User::setProvincia(QString provincia){
    m_provincia = provincia;
}

void User::setDir(QString direccion){
    m_dir = direccion;
}
/**
 * @brief Creamos la función save y la query, que nos permite hacer la consulta y insertar los datos correspodientes en la tabla.
 * @return
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


