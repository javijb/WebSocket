#include <iostream>
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocketServer.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocket.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXConnectionState.h"
#include "json.hpp"
#include "servidor.h"
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
