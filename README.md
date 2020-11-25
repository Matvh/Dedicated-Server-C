# Dedicated-Server-C

<h2>Descripción</h2>
Este proyecto trata de crear un servidor dedicado el cuál recive archivos (imagenes y texto). Los guarda, los procesa y los clientes pueden consultar la información.<br>
<br>
Se utilizaron los siguientes conceptos:<br>
        1)Sockets<br>
        2)Forks<br>
        3)Threads<br>
        4)Mutex y Semáforos<br>
        5)Pipes<br>
        6)Colas de Mensajes<br>
        7)Memoria Dinámica<br>
        8)Signals<br>
        

<h2>Entorno y Funcionalidades</h2>

<img width="913" alt="Entorno" src="https://user-images.githubusercontent.com/24244834/100281936-8e36b480-2f49-11eb-9921-3bff65523fed.png">

*El servidor es "Lionel". Este es un servidor dedicado tanto para clientes "McGruder" y "McTavish"<br>
*El cliente "McGruder" le envía imagenes/archivos de texto a "Lionel" cada X tiempo para ser procesados<br>
*"Paquita" es un fork de "Lionel" y esta se encarga generar las estadísticas a partir de la información que recive de los clientes "McGruder"<br>
*El cliente "McTavish" consulta las estadísticas que contiene "Lionel"
