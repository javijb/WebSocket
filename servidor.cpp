#include "servidor.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXSocketTLSOptions.h"
#include <QDebug>
#include "user.h"


bool Servidor::exists(const JSON& json, const std::string& key)
{
    return json.find(key) != json.end();
}

Servidor::Servidor(int puerto)
{

    this->m_puerto = puerto;

}

int Servidor::startServidor()
{

    //Crea un servidor
    ix::WebSocketServer server(this->m_puerto, "0.0.0.0");

    ix::SocketTLSOptions webSocketTLS;
    webSocketTLS.keyFile = "/etc/ssl/private/serverTLS/serverTLS.key";
    webSocketTLS.certFile = "/etc/ssl/private/serverTLS/serverTLS.crt";
    webSocketTLS.caFile = "/root/ca/myCA.pem";
    std::cout << webSocketTLS.getErrorMsg() << std::endl;
    qDebug() << webSocketTLS.isValid();
    server.setTLSOptions(webSocketTLS);

    server.setOnConnectionCallback(
                [this](std::shared_ptr<ix::WebSocket> webSocket,
                          std::shared_ptr<ix::ConnectionState> connectionState)
    {

        webSocket->setOnMessageCallback(
            [webSocket, connectionState, this](const ix::WebSocketMessagePtr msg)
            {

                if (msg->type == ix::WebSocketMessageType::Open)
                {

                    std::cout << "New connection" << std::endl;

                }
                else if (msg->type == ix::WebSocketMessageType::Close)
                {
                    std::cout << "Bye bye connection" << std::endl;
                }
                else if (msg->type == ix::WebSocketMessageType::Message)
                {
                    if(!msg->binary)
                    {
                        std::cout << msg->str << std::endl;
                    }

                    /// 1) Decodificar JSON
                    JSON receivedObject = JSON::parse(msg->str, nullptr, false);

                    /// 2) Trabajar con el mensaje JSON
                    JSON respuesta = this->nuevoMensajeJSON(webSocket, receivedObject);

                    /// 3) Enviar respuesta al cliente
                    std::string respuestaCliente = respuesta.dump();
                    webSocket->send(respuestaCliente);

                }
                else if (msg->type == ix::WebSocketMessageType::Error)
                {
                    std::cout << "Error: " << msg->errorInfo.reason << std::endl;
                }

            }
        );

    }
    );

    auto res = server.listen();

    if(!res.first)
    {
        return 1;
    }

    server.start();
    server.wait();
    server.stop();

    return 0;

}

void userNuevo(ix::WebSocket *webSocket, JSON received){

    std::string nombre_user;
    std::string apellidos_user;
    std::string email_user;
    std::string telefono_user;
    std::string password_user;
    std::string fecha_user;
    std::string genero_user;
    std::string nacio_user;
    std::string prov_user;
    std::string dir_user;

    received["nombre"].get_to(nombre_user);
    received["apellidos"].get_to(apellidos_user);
    received["email"].get_to(email_user);
    received["telefono"].get_to(telefono_user);
    received["password"].get_to(password_user);
    received["fecha_nac"].get_to(fecha_user);
    received["genero"].get_to(genero_user);
    received["nacionalidad"].get_to(nacio_user);
    received["provincia"].get_to(prov_user);
    received["direccion"].get_to(dir_user);

    qDebug() << QString::fromStdString(received["password"]);
    qDebug() << QString::fromStdString(password_user);


    User u;
    u.setNombre(QString::fromStdString(nombre_user));
    u.setApellidos(QString::fromStdString(apellidos_user));
    u.setEmail(QString::fromStdString(email_user));
    u.setTelefono(QString::fromStdString(telefono_user));
    u.setPassword(QString::fromStdString(password_user));
    u.setFecha_nac(QString::fromStdString(fecha_user));
    u.setGen(QString::fromStdString(genero_user));
    u.setNac(QString::fromStdString(nacio_user));
    u.setProvincia(QString::fromStdString(prov_user));
    u.setDir(QString::fromStdString(dir_user));

    u.save();

    JSON jsonMessage = {
           {"action", "UsuarioCreado"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump();
    webSocket->send(messageToSend);
}

void login(ix::WebSocket *webSocket, JSON received){

    bool log;
    int id_user;
    std::string email;
    std::string password;

    received["email"].get_to(email);
    received["password"].get_to(password);

    ///consulta para buscar el usuario solicitado en la BBDD
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE email = :email");
    query.bindValue(":email",  email.c_str());
    query.exec();


    JSON respuesta;
    respuesta["type"] = "login";

    if (query.size() > 0){

        QSqlQuery iquery;
        iquery.prepare("SELECT ENCRYPT(:password, :email)");
        iquery.bindValue(":password",  password.c_str());
        if (query.next()){
            id_user = query.value("idusuario").toInt();
            respuesta["idusuario"] = query.value("idusuario").toString().toStdString();
            qDebug() << "Existe el usuario, idusuario = " << id_user;
            iquery.bindValue(":idusuario",  query.value("idusuario"));
            iquery.exec();
        }

        std::string encrypt;

        if (iquery.first()){
            encrypt = iquery.value(0).toString().toStdString();
        }


        if (encrypt.compare(query.value("password").toString().toStdString()) == 0){
            respuesta["operationSuccess"] = "true";
            log = true;
        } else {
            respuesta["operationSuccess"] = "false";
            respuesta["errorMessage"] = "La contraseña es incorrecta.";
        }

    } else {
        qDebug() << "No se ha encontrado el usuario. Registrate por favor.";
        respuesta["operationSuccess"] = "false";
        respuesta["errorMessage"] = "No se ha encontrado el usuario. Registrate por favor.";
    }
}

JSON Servidor::nuevoMensajeJSON(std::shared_ptr<ix::WebSocket> webSocket, const JSON &mensaje)
{

    JSON resultado;

    /// 1) Saber si es un JSON válido
    if(mensaje.is_discarded())
    {
        std::cout << "Error, no es un JSON válido" << std::endl;
    }
    else
    {

        /// 2) Saber si existe action en el JSON
        if(exists(mensaje, "action"))
        {

                if(mensaje["action"] == "crearUsuario")
                {

                    userNuevo(webSocket.get(), mensaje);
                }

                if(mensaje["action"] == "cargarUsuario")
                {

                    std::cout << "Cargar usuario" << std::endl;
                    resultado["id"] = mensaje["id"];

                    /*User usuario = User::cargar(mensaje["nombre"], mensaje["apellidos"],  mensaje["email"], mensaje["tlf"], mensaje["pass"], mensaje["fecha"], mensaje["gen"], mensaje["nacionalidad"], mensaje["prov"], mensaje["dir"]);
                    JSON usuarioJSON = usuario.toJSON();
                    resultado["resultado"][0] = usuarioJSON;

                    return resultado;*/

                }
            }
            return resultado;

        }

}

