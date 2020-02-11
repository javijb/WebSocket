//Conexión WebSocket con servidor
let socket = new WebSocket("");

socket.onopen = function(event)
{
    
    /// 1) Pedir las últimas búsquedas
    console.log("Conexión con servidor.");
    
    /// 2) Crear un id y el JSON que se envía al servidor
    var idMensaje = dameId();
    
    /// 3) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    mensajesEsperandoRespuesta.push(mensaje);
};

socket.onclose = function(event)
{
    console.log("Conexión cerrada")
};

socket.onerror = function(event)
{
    console.log("Error");
};

socket.onmessage = function(event)
{
    
    console.log(event.data);
    
    /// 1) Parsear el JSON
    var mensaje = JSON.parse(event.data);
    
    /// 2) Recorrer los mensajes en espera y si existe ejecutamos la funcion correspondiente
    if(mensaje != null)
    {
        
        mensajesEsperandoRespuesta.forEach(mostrarMensajes);
    
        function mostrarMensajes(item, index)
        {
            
            if(mensaje.id == item.id)
            {
                item.funcionEjecutar(mensaje.resultado); 
            }
            
        }
        
    }
    
    
};


class Usuario {
    
    constructor(id, nombre, apellidos, email, tlf, pass, fecha, gen, nacionalidad, prov, dir) {
        this._id = id;
        this._nombre = nombre;
        this._apellidos = apellidos;
        this._email = email;
        this._tlf = tlf;
        this._pass = pass;
        this._fecha = fecha;
        this._gen = gen;
        this._nacionalidad = nacionalidad;
        this._prov = prov;
        this._dir = dir;
    }
    
    get id()
    {
        return this._id;
    }
    
}

var usuario;

function register()
{   
    document.getElementsByClassName("login")[0].style.display = "none"; 
    document.getElementsByClassName("register")[0].style.display = "block"; 
}

//Crear usuario
function crearUsuario()
{
    
    
    // 1) Coger los datos del registro
    var nombre = document.getElementById("nombre").value;
    var apellidos = document.getElementById("apellidos").value;
    var email = document.getElementById("email2").value;
    var tlf = document.getElementById("tlf").value;
    var pass = document.getElementById("password").value;
    var fecha = document.getElementById("fecha").value;
    var gen = document.getElementById("gen").value;
    var nacionalidad = document.getElementById("nacionalidad").value;
    var prov = document.getElementById("prov").value;
    var dir = document.getElementById("dir").value;
    
    
    // 2) Crear JSON que se envia al servidor
    var idMensaje = dameId();
    var obj = {action:"crearUsuario", id:idMensaje, nombre:nombre, apellidos:apellidos , email:email, tlf:tlf, pass:pass, fecha:fecha, gen:gen, nacionalidad:nacionalidad, prov:prov, dir:dir};
    socket.send(JSON.stringify(obj));
    
    // 3) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    
    mensaje.funcionEjecutar = function(resultado)
    {
        
        resultado.forEach(usuarioNuevo);
        
        function usuarioNuevo(item, index)
        {
            usuario = new Usuario(item.id, item.nombre, item.apellidos, item.email, item.tlf, item.pass, item.fecha, item.gen, item.nacionalidad, item.prov, item.dir);
            
            if(usuario.id == 0)
            {
                document.getElementById("errorSignIn").style.display = "flex";
            }
            else
            {
                document.getElementsByClassName("register")[0].style.display = "none";
            }
        }
        
    }
    
    mensajesEsperandoRespuesta.push(mensaje);
    
}

//Cargar usuario
function cargarUsuario(idUsuario)
{
    
    // 1) Crear JSON que se envia al servidor
    var email = document.getElementById("email").value;
    var password = document.getElementById("pass").value;
    var idMensaje = dameId();
    var obj = {action:"cargarUsuario", id:idMensaje, email:email, pass:password};
    socket.send(JSON.stringify(obj));
    
    /// 2) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    
    mensaje.funcionEjecutar = function(resultado)
    {
        
        resultado.forEach(usuarios);
        
        function usuarios(item, index)
        {
            usuario = new Usuario(item.id, item.nombre, item.apellidos, item.email, item.tlf, item.pass, item.fecha, item.gen, item.nacionalidad, item.prov, item.dir);
            
            if(usuario.id == 0)
            {
                document.getElementById("errorLogin").style.display = "flex";
            }
            else
            {
                document.getElementsByClassName("login")[0].style.display = "none";
            }
        }
        
    }
    mensajesEsperandoRespuesta.push(mensaje); 
    
}

//Mensajes JSON
function mensajeEspera(id)
{
    this.id = id;
}

var mensajesEsperandoRespuesta = [];

var g_ultimoId = 0;

function dameId()
{
    g_ultimoId++;
    return g_ultimoId;
}
