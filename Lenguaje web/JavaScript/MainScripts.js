
//Conexión WebSocket con servidor
let socket = new WebSocket("ws://localhost:9990");

socket.onopen = function(e) {
    alert("[open] Connection established");
};

socket.onmessage = function(event){
    console.log(event.data);
    var mensaje = JSON.parse(event.data);
    if (mensaje.action == "Login")
    {
        if (mensaje.operation == true) {
            document.getElementById("log").style.display="none";
            document.getElementById("prod").style.display="block";   
            document.getElementById("paneluser").style.display="block"; 
            document.getElementById("nombreuser").appendChild(document.createTextNode(mensaje.nombreus)); 
            document.getElementById("apellidosuser").appendChild(document.createTextNode(mensaje.apellidous));
            for (var i = 0; i < Object.keys(mensaje.artic).length; i++) {
             console.log(mensaje.artic);   
            }
            
            for (var i = 0; i < Object.keys(mensaje.artic.nom).length; i++) {
        
        var div = document.createElement("div");
        div.id="Articulo";
        var nombres = document.createElement("nombre");
        nombre.style.display = "block";
        var nombre = document.createTextNode("Nombre: " + mensaje.art.nom[i]);
        nombres.appendChild(nombre);
        
        var desc = document.createElement("descripción");
        desc.style.display = "block";
        var des = document.createTextNode("Descripción: " + mensaje.art.des[i]);
        desc.appendChild(des);
        
        var prec = document.createElement("precio");
        prec.style.display = "block";
        var pre = document.createTextNode("Precio: " + mensaje.art.pre[i]);
        prec.appendChild(pre);
        
        var boton = document.createElement("input");
        boton.type = "button";
        boton.value = "X";
        boton.style.backgroundColor = "blue";
        boton.position = "relative";
        boton.style.float = "right";
        boton.style.marginRight = "50px";
        boton.onClick = function() {};
        
        div.appendChild(nombres);
        div.appendChild(desc);
        div.appendChild(prec);
        
        var capa = document.getElementById("prod");
        
        capa.appendChild(div);
    } 
        } else alert("Usuario incorrecto.")
}
}

socket.onclose = function(event) {
    if (event.wasClean) {
        //alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
    } else {
        alert('[close] Connection died');
    }
};

socket.onerror = function(error) {
    alert(`[error] ${error.mensaje}`);
};

/*function catalogo(mensaje){
    for (var i = 0; i < Object.keys(mensaje.art.nom).length; i++) {
        
        var div = document.createElement("div");
        div.id="Articulo";
        var nombres = document.createElement("nombre");
        nombre.style.display = "block";
        var nombre = document.createTextNode("Nombre: " + mensaje.art.nom[i]);
        nombres.appendChild(nombre);
        
        var desc = document.createElement("descripción");
        desc.style.display = "block";
        var des = document.createTextNode("Descripción: " + mensaje.art.des[i]);
        desc.appendChild(des);
        
        var prec = document.createElement("precio");
        prec.style.display = "block";
        var pre = document.createTextNode("Precio: " + mensaje.art.pre[i]);
        prec.appendChild(pre);
        
        var boton = document.createElement("input");
        boton.type = "button";
        boton.value = "X";
        boton.style.backgroundColor = "blue";
        boton.position = "relative";
        boton.style.float = "right";
        boton.style.marginRight = "50px";
        boton.onClick = function() {};
        
        div.appendChild(nombres);
        div.appendChild(desc);
        div.appendChild(prec);
        
        var capa = document.getElementById("prod");
        
        capa.appendChild(div);
    } 
}*/


function logout()
{
    document.getElementById("log").style.display="block";
    document.getElementById("prod").style.display="none";   
    document.getElementById("paneluser").style.display="none";
    var Json = {action: "logout"};
    socket.send(JSON.stringify(Json));
}

/*function changeLoginStatus(mensaje) {
    var exito = String(mensaje.operation).localeCompare("true") == 0;
    
    if (exito) {
        if (logged)
}*/

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


//Crear usuario
function crearUsuario()
{
    
    
    // 1) Coger los datos del registro
    var nombre = document.getElementById("nombre").value;
    var apellidos = document.getElementById("apellidos").value;
    var email = document.getElementById("email").value;
    var tlf = document.getElementById("tlf").value;
    var pass = document.getElementById("pass").value;
    var fecha = document.getElementById("fecha").value;
    var gen = document.getElementById("gen").value;
    var nacionalidad = document.getElementById("nacionalidad").value;
    var prov = document.getElementById("prov").value;
    var dir = document.getElementById("dir").value;
    
    
    // 2) Crear JSON que se envia al servidor
    var idMensaje = dameId();
    //insert
    var Json = {action:"crearUsuario", id: idMensaje, nombre: nombre, apellidos: apellidos , email: email, telefono: tlf, password: pass, fecha_nac: fecha, genero: gen, nacionalidad: nacionalidad, provincia: prov, direccion: dir};
    socket.send(JSON.stringify(Json));
    
    // 3) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    
    mensaje.funcionEjecutar = function(resultado)
    {
        
        resultado.forEach(usuarioNuevo);
        
        function usuarioNuevo(item, index)
        {
            usuario = new Usuario(item.id, item.nombre, item.apellidos, item.email, item.telefono, item.password, item.fecha_nac, item.genero, item.nacionalidad, item.provincia, item.direccion);
            
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
function cargarUsuario()
{
    
    // 1) Crear JSON que se envia al servidor
    var email = document.getElementById("email").value;
    var password = document.getElementById("password").value;
    var idMensaje = dameId();
    var Json = {action:"cargarUsuario", id:idMensaje, email:email, password:password};
    socket.send(JSON.stringify(Json));
    
    /// 2) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    
    mensaje.funcionEjecutar = function(resultado)
    {
        
        resultado.forEach(usuarios);
        
        function usuarios(item, index)
        {
            usuario = new Usuario(item.id, item.nombre, item.apellidos, item.email, item.tlf, item.password, item.fecha_nac, item.genero, item.nacionalidad, item.provincia, item.direccion);
            
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

function cargarProductos()
{
    var prod1 = "Chaqueta Adidas Sportive Black"
    var prod2 = "Chaqueta Radkin"
    var prod3 = "Chaqueta Firebird"
    var prod4 = "Chaqueta Core S10"
    
    var Json = {action:"cargarProductos", prod1:prod1, prod2:prod2, prod3:prod3, prod4:prod4};
    socket.send(JSON.stringify(Json));
}

function compraCarrito()
{
    // 1) Crear JSON que se envia al servidor
    var producto1 = document.getElementById("nom1").value;
    var precio1 = document.getElementById("p1").value;
    var producto2 = document.getElementById("nom2").value;
    var precio2 = document.getElementById("p2").value;
    var producto3 = document.getElementById("p3").value;
    var precio3 = document.getElementById("p3").value;
    var idMensaje = dameId();
    var Json = {action:"compraCarrito", id:idMensaje, prod1:producto1, pre1:precio1, prod2:producto2, pre2:precio2, prod3:producto3, pre3:precio3 };
    socket.send(JSON.stringify(Json));
    
    /// 2) Crear el mensaje de respuesta que debe esperar el cliente y añadirlo a la lista de mensajes en espera
    mensaje = new mensajeEspera(idMensaje);
    
    mensaje.funcionEjecutar = function(resultado)
    {
        
        resultado.forEach(compras);
        
        function compras(item, index)
        {
            compra = new Compra(item.id, item.producto1, item.precio1, item.producto2, item.precio2, item.producto3, item.precio3);
            
            if(compra.id == 0)
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
