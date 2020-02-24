#ifndef USER_H
#define USER_H

#include <string>
#include <list>
#include <QSqlQuery>
#include <QSqlRecord>
#include "json.hpp"

using JSON = nlohmann::json;

class User
{

public:
    User(std::string nombre, std::string apellidos, std::string email, std::string telefono, std::string password, std::string fecha_nac, std::string gen, std::string nac, std::string provincia,std::string dir);
    JSON toJSON();
    void save();
    static User load(std::string email, std::string password);
    static void remove(int id);
    int getId();
    std::string getNombre();
    std::string getApellidos();
    std::string getEmail();
    std::string getTelefono();
    std::string getPassword();
    std::string getFecha_nac();
    std::string getGen();
    std::string getNac();
    std::string getProvincia();
    std::string getDir();

private:
    int m_id{0};
    std::string m_nombre{""};
    std::string m_apellidos{""};
    std::string m_email{""};
    std::string m_telefono{""};
    std::string m_password{""};
    std::string m_fecha_nac{""};
    std::string m_gen{""};
    std::string m_nac{""};
    std::string m_provincia{""};
    std::string m_dir{""};

};

#endif // USER_H
