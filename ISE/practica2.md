### P2 Leccion 1: ssh + firewall

las imagenes las podemos borrar a partir de hoy
creamos una maquina ubuntu y centos con la configuracion estandar para TODO
lo primero que tenemos que hacer es instalar el servidor ssh, en centos viene por defecto
hay dos formas de hacerl, una es usando la utilidad tasksel, pero al profe no le gusta por que en la vida real vamos a usar los paquetes de ubuntu de toda la vida
las distribuciones de ubuntu y ubuntu son paquetes .deb y se gestionan con una utilidad que se llama apt. apt get y apt cache
hacemos apt-cache para buscar 

apt-cache search sshd

sudo apt-get install openssh-server 

lo unico que tenemos que tener en cuenta es que en ubuntu hay dos comandos, los dos anteriores, pero en centos se usa yump para los dos
fijate que en la salida pone rsa, dsa y cosas asi que son algoritmos de cifrado 
Al instalarlo ya se queda el ssh como servicio
Este comando sirve para que servicios hay

systemctl list-unit-files | grep ssh

con el comando:

systemctl status ssh

vemos como esta el servicio. Como esta funcionando deberia ser capar de conectarme

Hemos instlado ssh, que son las siglas de secure shell, es una utilidad que usa protocolo de terminal, es decir nos permite conectarnos desde una terminal a un ordenador remoto y ejecutar cosicas,
antiguamente se usaba una utilidad que se llama telnet, que ya no se instala por defecto (lo usaremos en la lecciono 3), una caracteristica de telnet era que al hacer telnet <ip> abria una conexion de terminal (se abria una terminal especial llamada rshell), la peculiaridad era que toda la comunicacion se hacia a traves de paquetes abiertos,de forma que no era seguro. No era seguro hasta el punto de que si en la terminal remote haciamos un cat /etc/password eso se transmitiria.

SSH usa cifrado para que la comunicacion sea segura, usa dos protocolos, simetrico y asimetrico (o tambien llmado de clave publica)
noso vamos a conectar desde mi ordenador anfitrion a mi mv, desde el momento que se abre el prompt, todo la comunicacion esta cifrada con una contraseña acordada por ambos anfitriones (por eso es simetrica).

como lo probamos? nos vamos al ordenador anfitrion, en macs el cliente es ssh, en ubuntu tambien 

ssh david@ipdetuordenador (puedes comprobar tu ip con ifconfig -a, la que aparece en enp0s8)

david es tu usuario en el ordenador


//// esto no entra
al profe le ha salido un mensaje de error que pone WARNING REMOTE HOST IDENTIFICATION HAS CHANGED 
ssh usa un algoritmo de clave asimetrico para el handshake, pero una vez establecido pasa a simetrico porque es mas eficiente

al principio todo el mundo usaba algoeritmos de clave simetrica, es decir, emisor y receptor compartian una misma llave, poniendose de acuerdo, o sea que cogemos un mensaje, lo ciframos con la llave, se lo mandamos al otro, que la descifraba tambien con la misma llave. Lo malo de esto es cuando tenemos una red con mas de una persona, cuando tenemos una red con varias personas tenemos varias alternativas, o todos tiene la misma llave (lo que se hacie en la segunda guerra mundial), el problema es que si se pierde la llave todo el sistema estaria en peligro, la alternative es que cada par de personas tengan una llave privada para ellos, pero creceria el numero de llaves por lo que seria complicado mantenerlos. Por estas razones se invento lo asimetrico

Las claves son un chorro de bits (256, 1024, etc) cuanto mas grande mejor, mas seguro.

Un algoritmo que sigue usando, simetrico, se llama des, tdes.

ASIMETRICO: algoritmo de llave publica. La diferencia con el anterior es que cad apersona tiene dos llaves, una llave que se llama privada, y otra llave que llamamos publica,no hay diferencia entre ellas. La publica se la damos a todo el mundo. Una caracteristica de este algoritmo es que cuando cifras algo usando tu llave privada, y se lo envias a alguien, se tiene que descifrar usando la llave publica, pero esto funciona en la dos direcciona, es decir, el emisor codifica con la publica del receptor. pero todo el mundo puede descifarlo, eso sirve para garantizar autencidad


firma digital

a partir de aqui es un lio tomar apuntes, es igual que en fr asi que no hace falta tomarlos.

////


en el directorio .ssh (que esta en el tu directorio home) donde se almacenan las claves publicas y privadas
como se inicia al handshajing de ssh? cuando lo instalamos 

ssh david@2192.168.56.11 
si te crees que esa es tu llave publica, ya la tenemos, a partir de ahora podemos enviar info y aunque haya alguien en el medio que se quede con la llave publica no le vale para naada al no tener la privada del servidor. el cliente en secreto le propone una clave simetrica, que es mas eficiente y a partir de ahi se transmiten las cosas cifradas. cd .ssh, less known_hosts tiene las llaves publicas de toodos los sitios en los uqe me he conectado. 
7ç
nos vamos a etc por estar alli los ficheros de configuracion, poniendonos en modo root. cd ssh/
tenemos varios archvios de configuracion, nos vamos a sshd_config, para modificar el ssh del servidor, su daemon. si lo abrimos tenemos su puerto, si se permite o no root, vamos a cambiar el puerto por ver como funciona y desactivar el acceso remoto del root, que normalmente siempre se hace, sin permitir el acceso a root directamente. se cambia el puerto para que si alguien hace sniffer de puertos no le salga directamente. 
poenemos por ejemplo el puerto 22322, para que ssh coja los cambios tenemos que reiniciar el servicio systemctl restart ssh. si esta bien configurado, no matamos el servidor y lo volvemos a arrancar perderiamos la conexion ssh, pero eso no pasa, ssh engendra un nuevo hijo para atender las peticiones sin matar a los anteriores. al hacer systemctl status ssh me aparece el puerto 22322, y al hacer ssh david@ip ya no se conecta, hay que especificar el puerto con -p 22322 antes de poner david@ip, antes no habia uqe poner nada porque lo tomaba por defecto.

ubuntu por defecto no da contraseña al root, se la damos con passwd (estamos en modo root), pero no nos deja conectarnos porque en el archvio /etc/ssh pone prohibit--passwor para el usuario. toda la administracion moderna de servidores se basa en el servicio de automatizacion, mas rapida y control de calidad porque todas las acciones que hagamos con scripts automaticos es codigo, qeu se puede gestionar, mejorar,... para conseguir una comunicacion sin contraseña usaremos llave publica y llave privada. si le damos a ssh tabulador , al ejecutar keygen , ssh-keygen me dice que vamos a generar un par llave public y llave privada (lo hacemos en modo normal), en david, y nos pregunta si queremos contrasseña para la llave privada, para no tener que darla todo el rato esta ssh-agent, la primera vez qe nos conectamos si la pide pero futuros usos la tiene en memoria y ya no te la pide mas. en sistemas totalmente automaticos como ansible la llave privada esta protegida con un certificado, se descifra automaticamnete, pero no es lo normal. metemos contraseña para ver un caso real. 

nos da una representacion grafica de la entropia de la llave que hemos generado, cuanto mas aleatoria la secuencia de bits mejor. cd, cd .ssh y tenemos id_rsa y id_rsa.pub, una es la llave privada y otra la publica. less id_rsa.pub. cuando las generamos tambien podemos poner un comentario ssh-keygen -c 'correoelectronico propio', si no coge el nombre de usuario en la maquina. si hacemos lo anterior con la privada vemos uqe sale que esta encrypted.

ya tenemos llave publica y privada, como nos conectamos usandola??

damos la llave publica y la usamos para autenticarnos, desde la maquina cliente le psaamos nuestra llave publica de manera que en el futuro cuando me vaya a conectar ciframos cualquier cadena, se la devolvemos cifrada con la llave privada, la descifra con la llave publica nuestra y se asegura asi que somos nosotros, es como un handshaking: el servidor me manda frase, se la devolvemos cifrada con la llave privada, el la descifra usando mi llave publica y ya obtiene la frase original pues. las compara, si son igual guay si no mal. como le hacemos llegar a un servidor nuestra llave publica, normalemnte copiando y pegando, por correo, en un pendrive, de cualquier forma porque es publica. 

nosotros lo hacemos entre dos maquinas linux. ssh-copy-id -p 22322david@ip, lo vamos a pasar por ftp. le damos a enter, me pide la contraseña, es la primera vez que me conecto, la ponemos. si volvemos a poner el puerto 22 mejor y nos ahorramos escribir. al hacer ls en el servidor ha aparecido un fichero nuevo, authorized keys. copiamos el comando que nos dice en el mac y nos pide contraseña, que es la que pide solo una vez, al hacer ssh-add, y hacemos ssh david@ip. nosotros	no hace falta uqe cifremos la clave privda, cuando pida passphrase la dejamos en blanco. ssh-agent lanza el proceso y ssh-add añade la llave.

fijarnos en los ficheros que se generan eso si preguntara, para que se usa la llave publica, la privada,... 

gestion de los firewalls en ubuntu--- se gestionan con ufw, unified firewalls, su ayuda es bastante intuitiva. ufw status, como root, por defecto esta desactivada, en centos por defecto activado. 
 


