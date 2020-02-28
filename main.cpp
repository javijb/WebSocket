#include <iostream>
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocketServer.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXWebSocket.h"
#include "/home/usuario/IXWebSocket-master/ixwebsocket/IXConnectionState.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTranslator>
#include "json.hpp"
#include "servidor.h"
#include "conexion.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTranslator myappTranslator;
    myappTranslator.load("myapp_es_ES", "../../miservidorwebsocket");
    a.installTranslator(&myappTranslator);
    /// 1) Abrir conexión con la base de datos
    Conexion con;
    con.open();
    qDebug() << QObject::tr("New connection");

    /// 2) Iniciar servidor
    Servidor servidor(9990);
    return servidor.startServidor();

}
