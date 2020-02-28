# RESUMEN DOCUMENTACION eMARKET ADIDAS
*************************************************


**Presentación de la aplicación:**

Es una aplicación diseñada para simular un el concepto de eMarket de Adidas (mercado online), en el cual un usuario puede registrarse en la página y a partir de allí comprar los artículos que se encuentran en la tienda.
Es una aplicación que se basa en el envió y recibo de información mediante mensajes JSON. Estos mensajes se envían desde la página html al servidor websocket y al revés. La aplicación también esta compuesta por una base de datos PHP.
La principal plataforma para la que estaría disponible esta aplicación sería para PC hasta que llegará su adaptación a Android e IOS.


**Herramientas utilizadas para el proyecto:**

- Lenguajes de programación: HTML, JavaScript, CSS y C++.

- Aplicaiones y herramientas: QT, PHP, librerías websocket y git.


**Funcionamiento de la aplicación**

IMPORTANTE (Es necesario tener implementadas las librerías de IXWebsocket.)

1-. El cliente deberá abrir su navegador y con ello, la aplicación. 

2-. Entonces podrá ver una interfaz de usuario con un menú, en el cual verá la opción de login/register, y deberá entrar allí.

3-. Ya en el formulario puedes o loggearte en el caso de que ya tengas un usuario, o registrarte. (Es imprescindible tener un usuario para realizar una compra en la tienda.)

4-. En el caso de no tener un usuario deberás rellenar el formulario de registro.

5-. Ya loggeado podrás comprar, acceder a tu carrito de compra y recibir descuentos.

*******************************************************************************************

**Problemas que han surgido:**

- "no matching constructor for initialing". En este caso el error fue no crear un constructor desde un principio para la variable y tener un error de syntaxis.

- Cambiar toda la clase "User.cpp" por un gran fallo en la estructura principal.

- Fallo que no me dejaba introducir los datos en el query, ya que tenía que especificar la ruta de la tabla con public.user (schema.tabla), ya que el nombre user ya esta generado en PHP y provoca conflictos.

- Fallo en la función de login a la hora de comparar contraseñas, faltaba añadir query.next para que me cogiese bien la consulta y me permitiese compararlas.

- Fallo sin solución: No me permitía cambiar de archivo html sin que se cerrase la conexión, entonces no me permitía que el usuario logeado se mantuviese al cambiar de página html. Estuve 3 días para solucionar el fallo y ya era tarde como para cambiar el html entero e intentar juntarlo todo en una misma página con diferentes div.

- Poder implemetar la función de Compra al completo, ya que tengo el carrito de compra predeterminado pero tendría que implemetar en el servidor la clase de compra para que me mandase un mensaje con el usuario, los productos comprados y el precio total.

- Diferentes fallos a la hora de recibir los mensajes JSON. 

- Complicaciones a la hora de implementar los test.

- Formulario bastante extenso, por lo que ha provocado bastantes complicaciones.


**Opciones que me hubiera gustado implementar en el programa**

- Me hubiera gustado implementar una función que me permitiese enviar un mensaje de confirmación al registrarse en la página.

- También creo que podría haber funcionado muy bien una función de logout.

- Pensé en que la compra no fuera predeterminada, pero no tenía muy claro como poder personalizar el carro de compra y poder introducirlo todo en el servidor.

- Me hubiera gustado poder pulir un poco más el diseño de la interfaz, y así hacerla un poco más dinámica.

*********************************************************************************************

**COPYRIGHT**

    eMarket Adidas. Version 0.5.
    Copyright (C) 2020  Javier Jaume Belda y Javier Miró.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.