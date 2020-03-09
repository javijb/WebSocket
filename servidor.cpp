#include "servidor.h"
#include "IXWebSocket-master/ixwebsocket/IXSocketTLSOptions.h"
#include <QDebug>
#include "user.h"
#include <QSqlRecord>

/*! \file */

/*! \mainpage Documentación del eMarket de Adidas
 * Adidas eMarket es una tienda online en la que se venden productos de la marca.
 *
 * Esta aplicación aún se encuentra en fase de desarrollo. Versión: 0.5
 *
 * */


/**
 * @brief La función permite si en el caso de que servidor exista, devolver un mensaje json con el parámetro key.
 * @param json
 * @param key
 * @return
 */
bool Servidor::exists(const JSON& json, const std::string& key)
{
    return json.find(key) != json.end();
}
/**
 * @brief Asigna un puerto al servidor
 * @param puerto
 */

Servidor::Servidor(int puerto)
{

    this->m_puerto = puerto;

}
/**
 * @brief Creamos el servidor webSocket indicando los certificados, entonces lo iniciamos e indicamos los pasos 1, 2 y 3 comentados.
 * @return
 */

int Servidor::startServidor()
{

    //Crea un servidor
    ix::WebSocketServer server(this->m_puerto, "0.0.0.0");
    /*ix::SocketTLSOptions tlsOptions;
    tlsOptions.tls=true;
    tlsOptions.certFile = "./cert/localhost.crt";
    tlsOptions.keyFile = "./cert/localhost.key";
    tlsOptions.caFile = "NONE";

    if (tlsOptions.isValid()) {
        std::cerr << "SSL valid" << std::endl;
    }*/

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

bool g_log;
int g_id;

/**
 * @brief Creamos la función userNuevo, que nos permite recibir el mensaje Json del html, llamar a la función save() del user.cpp y almacenar los datos seleccionados en la base de datos de la aplicación.
 * @param webSocket
 * @param received
 */

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

    //qDebug() << QString::fromStdString(received["password"]);
    //qDebug() << QString::fromStdString(password_user);

    //Usamos los setters del user.cpp y convertimos las variables a QString.
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
    //Mensaje para confirmar que ha funcionado la creación del usuario.
    JSON jsonMessage = {
           {"action", "UsuarioCreado"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump();
    webSocket->send(messageToSend);
}

void usercheck(ix::WebSocket *webSocket){
    JSON r;
    r["type"] = "usercheck";
    if (g_log == true) {
        r["operation"] = "true";
        r["id_user"] = g_id;
    } else {
        r["operation"] = "false";
    }

    std::string messageToSend = r.dump();
    webSocket->send(messageToSend);
}


/**
 * @brief Creamos la función login, que nos permite coger un usuario ya creado (email y contraseña) y poder loggearse, en el caso de tener los datos incorrectos, no te permite loggearte.
 * @param webSocket
 * @param received
 */
void login(ix::WebSocket *webSocket, JSON received){

    bool log;
    JSON respuesta;
    ///consulta para buscar el usuario solicitado en la BBDD
    QSqlQuery query;
    query.prepare("SELECT nombre, apellidos, email, password FROM public.user WHERE email = :email and password = crypt(:password , password)");
    query.bindValue(":email",  QString::fromStdString(received["email"]));
    query.bindValue(":password",  QString::fromStdString(received["password"]));

    if (query.exec() == true) {
            query.next();
            respuesta["action"] = "Login";

            if (query.value("password") != ""){

                //QString password = query.value("password").toString();


                    log = true;
                    qDebug() << log;
                    respuesta["operation"] = log;
                    respuesta["nombreus"] = query.value("nombre").toString().toStdString();
                    respuesta["apellidous"] = query.value("apellidos").toString().toStdString();
                    qDebug() << query.value("nombre").toString();


            } else {

                qDebug() << "La contraseña es incorrecta.";

            }


    }

     else {
        qDebug() << "No se ha encontrado el usuario. Registrate por favor.";
    }


    std::string messageToSend = respuesta.dump();
    webSocket->send(messageToSend);
}
/**
 * @brief Intento de compra de un solo producto + confirmación de la compra.
 * @param webSocket
 * @param received
 */

/**void compraProducto(ix::WebSocket *webSocket, JSON received){
    JSON respuesta;
    bool compra;
    QSqlQuery query;
    query.prepare("SELECT * FROM productos where nombre = :nombre");
    query.bindValue(":nombre",  QString::fromStdString(received["nombre"]));
    query.exec();

    qDebug() << query.value("nombre").toString();

    if (query.exec() == true) {
            query.next();
            respuesta["action"] = "compraRealizada";
            respuesta["nombreprod"] = query.value("nombre").toString().toStdString();
            respuesta["descripcionprod"] = query.value("descripcion").toString().toStdString();
            respuesta["precioprod"] = query.value("precio").toString().toStdString();
            compra = true;

    }else {
            qDebug() << "Ha habido un fallo en la compra.";
            compra = false;
    }

    JSON jsonMessage = {
        {"action", "compraRealizada"},
        {"operation", compra},
    };

    std::string messageToSend = jsonMessage.dump();
    webSocket->send(messageToSend);


}*/

/**
 * @brief Intento de imprimir catálogo desde la bbdd pero no ha salido bien.
 * @param webSocket
 * @param received
 */

/**void catalogo(ix::WebSocket *webSocket, JSON received){

    JSON respuesta;
    QSqlQuery query;
    query.prepare("SELECT nombre, descripcion, precio FROM productos");
    query.exec();

    respuesta["type"] = "articulos";

    JSON elementos;

    while (query.next()){
        elementos["nom"] = query.value("nombre").toString().toStdString();
        elementos["des"] = query.value("descripcion").toString().toStdString();
        elementos["pre"] = query.value("precio").toString().toStdString();

        respuesta["artic"].push_back(elementos);
    }

    if (query.size() !=1) respuesta["operation"] = true;
    else respuesta["operation"] = false;

    std::string messageToSend = respuesta.dump();
    webSocket->send(messageToSend);

}*/

/**
 * @brief Función para salir del usuario.
 * @param webSocket
 * @param received
 */

void logout(ix::WebSocket *webSocket, JSON received){
    JSON jsonMessage = {
        {"action", "logout"},
        {"operation", true},
    };

    std::string messageToSend = jsonMessage.dump();
    webSocket->send(messageToSend);

    g_log = false;
}

void borrarUser(ix::WebSocket *webSocket, JSON received) {

}

/*void compraCarrito(ix::WebSocket *webSocket, JSON received){

    bool compraRealizada;

    QSqlQuery query;
    query.prepare("Select nombre, precio from productos where nombre = :nombre and precio = :precio");
    query.bindValue(":nombre",  QString::fromStdString(received["producto1"]));
    query.bindValue(":precio",  QString::fromStdString(received["precio1"]));
    query.bindValue(":nombre",  QString::fromStdString(received["producto2"]));
    query.bindValue(":precio",  QString::fromStdString(received["precio2"]));
    query.bindValue(":nombre",  QString::fromStdString(received["producto3"]));
    query.bindValue(":precio",  QString::fromStdString(received["precio3"]));

    if (query.exec() == true)
    {
        query.next();
            JSON respuesta;
            respuesta["type"] = "compraRealizada";
            qDebug() << "Tu compra se ha realizado con éxito.";
            compraRealizada = true;
    } else {

        qDebug() << "Se ha producido un error en su compra.";
        compraRealizada = false;

    }

    JSON jsonMessage = {
           {"action", "compraRealizada"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump();
    webSocket->send(messageToSend);

}*/


/**
 * @brief Llama las funciones cada vez que el servidor recibe un mensaje.
 * @param webSocket
 * @param mensaje
 * @return
 */
JSON Servidor::nuevoMensajeJSON(std::shared_ptr<ix::WebSocket> webSocket, const JSON &mensaje)
{

    JSON resultado;

    /// 1) JSON válido
    if(mensaje.is_discarded())
    {
        std::cout << "Error, no es un JSON válido" << std::endl;
    }
    else
    {

        /// 2) Saber si existe action
        if(exists(mensaje, "action"))
        {

                if(mensaje["action"] == "crearUsuario")
                {

                    userNuevo(webSocket.get(), mensaje);
                }

                if(mensaje["action"] == "cargarUsuario")
                {

                    login(webSocket.get(), mensaje);
                    //catalogo(webSocket.get(), mensaje);

                }

                if(mensaje["action"] == "compraCarrito")
                {

                    //compraCarrito(webSocket.get(), mensaje);

                }
                if(mensaje["action"] == "compraProducto"){
                   // compraProducto(webSocket.get(), mensaje);
                }

                if(mensaje["action"] == "logout")
                {
                    logout(webSocket.get(), mensaje);
                }
            }
            return resultado;

        }

}

