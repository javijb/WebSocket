#include <iostream>
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocketServer.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocket.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXConnectionState.h"

#include "json.hpp"

#include "conexion.h"

int main()
{

    /// 1) Abrir conexión con la base de datos
    Conexion con;
    con.open();

    /// 2) Iniciar servidor
    Servidor servidor(9990);
    return servidor.startServidor();

}

using JSON = nlohmann::json;

static int g_idMensaje = 0;
static int id_usuario = 0;

int dameIdMensaje() {
    g_idMensaje++;
    return g_idMensaje;
}

int logInUser() {
    return id_usuario;
}

bool exists(const JSON& json, const std::string& key) {
    return json.find(key) != json.end();
}

JSON loginuser(JSON receivedObject) {
    JSON respuesta;
    id_usuario++;
    static bool estaDentro {false};

    respuesta["idServidor"] = dameIdMensaje();
    respuesta["idUsuario"] = receivedObject["id"];
    respuesta["HayError"] = 0;

    /// 1) Acceder a la base de datos para saber si el usuario es válido
    if (receivedObject["idUsuario"] == "01234567") {
        respuesta["HayError"] = 1;
        respuesta["mensajeError"] = "Usuario no válido";
    }
    else {
        /// 2) Está entrando o saliendo?
        if (estaDentro){
            respuesta["mensaje"] = "Estás saliendo";
        }
        else {
          respuesta["mensaje"] = "Estás entrando";
        } //end if
    } //end if
}

int main(int argc, char *argv[])
{

    ix::WebSocketServer server(9990, "0.0.0.0");

    server.setOnConnectionCallback(
        [&server](std::shared_ptr<ix::WebSocket> webSocket,
                  std::shared_ptr<ix::ConnectionState> connectionState)
        {
            webSocket->setOnMessageCallback(
                [webSocket, connectionState, &server](const ix::WebSocketMessagePtr msg)
                {
                    if (msg->type == ix::WebSocketMessageType::Open)
                    {

                        std::cout << "New connection" << std::endl;

                        /// Tipos de mensaje:
                        /// 1) Mensaje que no espera respuesta
                        /// 2) Mensaje que sea una respuesta a una petición previa
                        /// 3) Mensaje que espera respuesta


                        JSON jsonMessage = {
                          {"type", "1"},
                          {"message", "hola"}
                        };

                        //Mensaje del servidor al cliente:



                    }
                    else if (msg->type == ix::WebSocketMessageType::Close)
                    {
                        std::cout << "Bye bye connection" << std::endl;
                    }
                    else if (msg->type == ix::WebSocketMessageType::Message)
                    {
                        if (!msg->binary)
                        {
                            /// Text format
                            std::cout << "Received message: " << msg->str << std::endl;
                        }
                        webSocket->send(msg->str, msg->binary);

                        JSON receivedObject = JSON::parse(msg->str, nullptr, false);

                        if (receivedObject.is_discarded()) {
                            ///error
                            std::cout << "error" << std::endl;
                        }
                        else {
                            ///JSON válido
                            if (exists(receivedObject, "loginuser"))
                            {
                                std::string action = receivedObject["loginuser"];

                                if (action == "loginuser") {
                                    JSON respuesta = loginuser(receivedObject);
                                    webSocket->send(respuesta.dump());
                                } //end if
                            }
                            else {
                                std::cout << "error no hay action" << std::endl;
                            } //end if
                        } // end if
                    }
                }
            );
        }
    );



    auto res = server.listen();
    if (!res.first)
    {
        // Error handling
        return 1;
    }

    server.start();
    server.wait();
    server.stop();



}
