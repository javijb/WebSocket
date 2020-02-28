#ifndef USER_H
#define USER_H

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include "json.hpp"

using JSON = nlohmann::json;

class User
{

public:
    JSON toJSON();
    bool save();  /*! <Funcion guardar conecta bbdd*/
    void cargar(int id);
    static void remove(int id);
    int getId();


    QString getNombre();
    QString getApellidos();
    QString getEmail();
    QString getTelefono();
    QString getPassword();
    QString getFecha_nac();
    QString getGen();
    QString getNac();
    QString getProvincia();
    QString getDir();

    void setNombre(QString nombre);
    void setApellidos(QString apellidos);
    void setEmail(QString email);
    void setTelefono(QString telefono);
    void setPassword(QString password);
    void setFecha_nac(QString fecha_nac);
    void setGen(QString genero);
    void setNac(QString nacionalidad);
    void setProvincia(QString provincia);
    void setDir(QString direccion);


private:
    int m_id{0};
    QString m_nombre;
    QString m_apellidos;
    QString m_email;
    QString m_telefono;
    QString m_password;
    QString m_fecha_nac;
    QString m_gen;
    QString m_nac;
    QString m_provincia;
    QString m_dir;

};

#endif // USER_H
