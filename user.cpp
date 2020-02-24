#include "user.h"
#include <QVariant>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include "servidor.h"

User::User(std::string nombre, std::string apellidos, std::string email, std::string telefono, std::string password, std::string fecha_nac, std::string gen, std::string nac, std::string provincia,std::string dir)
{
    this->m_nombre = nombre;
    this->m_apellidos = apellidos;
    this->m_email = email;
    this->m_telefono = telefono;
    this->m_password = password;
    this->m_fecha_nac = fecha_nac;
    this->m_gen = gen;
    this->m_nac = nac;
    this->m_provincia = provincia;
    this->m_dir = dir;

}

void User::save()
{

    QSqlQuery query;
    query.prepare("INSERT INTO user (nombre, apellidos, email, telefono, password, fecha_nac, genero, nacionalidad, provincia, direccion) VALUES (:nombre, :apellidos, :email, :telefono, :pass, :fecha_nac, :genero, :nacionalidad, :provincia, :direccion);");

    QVariant nombre = QString::fromStdString(m_nombre);
    QVariant apellidos = QString::fromStdString(m_apellidos);
    QVariant email = QString::fromStdString(m_email);
    QVariant telefono = QString::fromStdString(m_telefono);
    QVariant pass = QString::fromStdString(m_password);
    QVariant fecha_nac = QString::fromStdString(m_fecha_nac);
    QVariant genero = QString::fromStdString(m_gen);
    QVariant nacionalidad = QString::fromStdString(m_nac);
    QVariant provincia = QString::fromStdString(m_provincia);
    QVariant direccion = QString::fromStdString(m_dir);


    query.bindValue(":nombre", nombre);
    query.bindValue(":apellidos", apellidos);
    query.bindValue(":email", email);
    query.bindValue(":telefono", telefono);
    query.bindValue(":password", pass);
    query.bindValue(":fecha_nac", fecha_nac);
    query.bindValue(":genero", genero);
    query.bindValue(":nacionalidad", nacionalidad);
    query.bindValue(":provincia", provincia);
    query.bindValue(":direccion", direccion);

    query.exec();

    QString error (query.lastError().text());

    if(error == " ")
    {
        query.prepare("select * from usuario order by idusuario desc limit 1;");
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

User User::load(std::string email, std::string password)
{

    User User("", "", "", "", "", "", "", "", "", "");

    QSqlQuery query;
    query.prepare("SELECT (password = :password AS pwd_match  FROM usuario where email = :email;");

    QVariant eml = QString::fromStdString(email);
    QVariant pas = QString::fromStdString(password);
    query.bindValue(":email", eml);
    query.bindValue(":password", pas);
    query.exec();

    bool passCorrect = false;
    QSqlRecord rec = query.record();
    while (query.next())
    {
        passCorrect = QVariant(query.value("pwd_match")).toBool();
    } // end while

    if(passCorrect)
    {

        query.prepare("SELECT * from usuario where email = :email");
        query.bindValue(":email", eml);
        query.exec();

        QSqlRecord rec = query.record();
        while (query.next())
        {
            std::string nombre = query.value("nombre").toString().toStdString();
            std::string apellidos = query.value("apellidos").toString().toStdString();
            std::string email = query.value("email").toString().toStdString();
            std::string telefono = query.value("telefono").toString().toStdString();
            std::string pass = query.value("password").toString().toStdString();
            std::string fecha_nac = query.value("fecha_nac").toString().toStdString();
            std::string gen = query.value("genero").toString().toStdString();
            std::string nac = query.value("nacionalidad").toString().toStdString();
            std::string provincia = query.value("provincia").toString().toStdString();
            std::string dir = query.value("direccion").toString().toStdString();

            User usuar(std::string nombre, std::string apellidos, std::string email, std::string telefono, std::string pass, std::string fecha_nac, std::string gen, std::string nac, std::string provincia, std::string dir);
            usuar.m_id = query.value("idusuario").toInt();
            return usuar;
        }

    } // end if


    return User;

}

void User::remove(int id)
{

    QSqlQuery query;
    query.prepare("DELETE FROM usuario where idusuario = :id");

    query.bindValue(":id", id);
    query.exec();

}

JSON User::toJSON()
{

    JSON usuario;
    usuario["id"] = m_id;
    usuario["nombre"] = m_nombre;
    usuario["apellidos"] = m_apellidos;
    usuario["email"] = m_email;
    usuario["tlf"] = m_telefono;
    usuario["pass"] = m_password;
    usuario["fecha_nac"] = m_fecha_nac;
    usuario["gen"] = m_gen;
    usuario["nacionalidad"] = m_nac;
    usuario["provincia"] = m_provincia;
    usuario["direccion"] = m_dir;


    return usuario;

}

int User::getId(){ return m_id; }
std::string User::getNombre(){ return m_nombre; }
std::string User::getApellidos(){ return m_apellidos; }
std::string User::getEmail(){ return m_email; }
std::string User::getTelefono(){ return m_telefono; }
std::string User::getPassword(){ return m_password; }
std::string User::getFecha_nac(){ return m_fecha_nac; }
std::string User::getGen(){ return m_gen; }
std::string User::getNac(){ return m_nac; }
std::string User::getProvincia(){ return m_provincia; }
std::string User::getDir(){ return m_dir; }
