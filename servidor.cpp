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
                    JSON respuesta = this->nuevoMensajeJSON(receivedObject);

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

JSON Servidor::nuevoMensajeJSON(const JSON &mensaje)
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

                    std::cout << "Crear usuario" << std::endl;
                    resultado["id"] = mensaje["id"];

                    std::string nombre = mensaje["nombre"];
                    std::string apellidos = mensaje["apellidos"];
                    std::string email = mensaje["email"];
                    std::string pass = mensaje["pass"];
                    std::string telefono = mensaje["tlf"];
                    std::string fecha_nac = mensaje["fecha"];
                    std::string genero = mensaje["gen"];
                    std::string nacionalidad = mensaje["nacionalidad"];
                    std::string provincia = mensaje["prov"];
                    std::string dir = mensaje["dir"];

                    User user(nombre, apellidos, email, telefono, pass, fecha_nac, genero, nacionalidad, provincia, dir);
                    user.save();

                    /*JSON usuarioJSON = user.toJSON();
                    resultado["resultado"][0] = usuarioJSON;

                    return resultado;*/

                }

                if(mensaje["action"] == "cargarUsuario")
                {

                    std::cout << "Cargar usuario" << std::endl;
                    resultado["id"] = mensaje["id"];

                    User usuario = User::load(mensaje["email"], mensaje["pass"]);
                    JSON usuarioJSON = usuario.toJSON();
                    resultado["resultado"][0] = usuarioJSON;

                    return resultado;

                }
            }
            return resultado;

        }

}

