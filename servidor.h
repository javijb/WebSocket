#ifndef SERVIDOR_H
#define SERVIDOR_H

#include<iostream>
#include "IXWebSocket-master/ixwebsocket/IXWebSocketServer.h"
#include "IXWebSocket-master/ixwebsocket/IXWebSocket.h"
#include "IXWebSocket-master/ixwebsocket/IXConnectionState.h"
#include "json.hpp"


using JSON = nlohmann::json;

class Servidor
{

public:
    Servidor(int);
    int startServidor();
    bool g_log;
    int g_id;

private:
    int m_puerto{9990};
    static bool exists(const JSON&, const std::string&);
    JSON nuevoMensajeJSON(std::shared_ptr<ix::WebSocket>, const JSON&);

};

#endif // SERVIDOR_H
