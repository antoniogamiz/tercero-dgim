p3 monitoring and profiling

monitorizar el sistema en produccion

Profiling: mas usados en sistemas en desarrollo, para medir el uso de memoria, tiempo de ejecucion, etc.

lo vamos a haer ocn ssh en ubuntu pero es igual en centos

dmesg => buffer circular, con tamaño fijo. Se configura recompilando el kernel de linux. Se usa para depurar hardware.

se ejeuta ocn dmesg, siañadimos -h nos da mas inof

dmesg -Hw, para segiurlo en teimpo real

pinchamos un pen drive
lo expulsamos para que lo pueda reconocer virtual box
en settings ports y click sobrre usb lo añadimos con el icono de añadir,
como no hay cambios en la terminal paace ser que no lo coge asi que volvemos a saarlo y metelo. Parece que sigue sin cogelo

en el mundo linux el sistema de ficheros /proc es muy impotante. Se usa para reflejar el estado del kernel de linux en todo momento.Esto es asi por dos razones: el sistema de ficheros es muy antiguo y estandar por lo que necesita drivrs ni nada. Ademas un sistema de ficheos es un sistema de lamaecnamiento jeaquico. Po rlo que viene muy bien paa ese tipo de infmacion.
Si entramos a poc y hacemos ls vemos que hay carpetas con numerrros y cada una es de un prrocesa en ejecucion, ysi entramos a una de esas podemos ver la informacion sobre ese proceso.

cd algunnumero, en mem esta la memoria que usa,que no se puede ver, en stat estan las estadisiticas de uso de la cpu, en fd estan los accesos al sistema de ficheros, en cmdline podemos ver quien lo ha ejecutado.

el comando top usa proc para funcionarr

en cpuinfo podemos ver la info de la cpu, se usa bastante
en inteuupts sale el numero de interupciones hardware
meminof nos da inof sobre rel uso de la memoria
en vesion podemos ver la vesion del ekenel

podemos cambia ralguns parametros del kernel, en poc sys net ipv4 estan los parametros de red. El archivo ip_forward se usa mucho para convertir el ordenador en un oruter o algo asi. Podemos moficar esto de dos formas, recompilando el kenel, o modiicandolo hay haciendo echo 1 > ip_forward, ese ambio hay que haerlo cada vez que se iniie la maquina.

el dierotrio proc ces volatoil, por lo que cualquier cambio dse perrderar el reinicciar

htop es parecido a top.

los ampos de top son impotantes

aiba esta la hoa, el uptime, los usuaios onvetados, y la load aveage: las ters ciffas que hay es un estanda que se llama CCPU load, la media de carga de 1min, 5min, 15min
si tenemos una caga de 1 significa que la cpu esta al cien por cien, si esta al 1.5, signiica que esta al 15?% de su capaidad, eso se calcula con los pocesos que estan esperando a ejeucion.

los erocmendado es no pasarrr de ero ocn 75-. sin contar los picos, si esto se mantiene durante un rato ocmo 5 minutos la pu falla.

Lo anteiror e S para una cpu de 1 ocer. Si tenemos dos coers la arga al cien por cien sera 2, luego tendermos que dividi rpo rl enumero de coers para obterner el valor real.

las tares son los procesos o hebras, los estados son omo estan las tareas,

el zombioe es peligoso, son poesos que han sido maados para moi peo todavia no han mueto.

poentae de pcu si esta medido en total, poentaje del odigo de usuariro, porentaje del systema,el siguiente es nie p, que son los poeoss que se las ha maodiiado la pioidad, la ero es la mas alta. los negativo tienesn mas pioidad todavia. El id es el pocentaje del tiempo de espera, que es la iddle, que es la que se ejeuta en la espera, wa, es waiting espera, por algun ercurso o algo, hi es hardware inteuption, tiempo dedicado a resolverlas, si son sotwaer interrutions, st son iclos robados pero no es muy imporante.

dos ultimas lineas:
si la damos a la h sale una ayuda, asi podemos ve que si le damos a la e ambiamos al esala de la memoria
es
la ultima olumna de la uarta signiica cuanta memoria es usada para bugger o cahe, el parametro avail Memoriy es el maX tamaño de ram que puede onsumi runa app sin llega a usar swap. free mas used mas la ultima suma el total.

lo de las oclmunas lo ha dicho muy apido y no me ha dado teiempo a copiarl,

#############

vamos a ve rahora /var/log

var es el diertorio de ocntenido variable, log es para los mensajitos de informacion.
cd va log
muchos son binarios y no podemos leerlos, algunos como syslog nop. ada uno suele tene rsu popio formato definido por los creadores.
who -a, last, mia rque hacen, son accesos de ausuario o alog asi.
un poblema con el archivo de log es que es muy gande, por lo que hay sotware que lo gestiona.
hay un pograma que todas las noches ervisa los arhivos de logs que tiene que ambia y empieza a omprimirlos, y por ejemplo cuando tiene 2389 comprimidos empieza a borar. o te los puedes lleva ra otos sitios.
el pgroama que hae eso lse lalma logRotate
la coniguacion esta en /etC/logotate.ocnfg,
domde reralmente esta<ña<pcmfog<es<em</e>>>>> /etcC/logrotate.d

en apache2 por ejemplo podemos ver algunas cosas, en postrotate y prerotate son mportantes,

en var lib logotate deberia apareer la base interna de logortate, lo ejecutamos apra que apaezca algo con
logotate -f -v -v -d /etC/logrotate.d/apahe12

vaya pus despues de eso no hay aparecido nad atampoco.
obvia TODO ESO QUE NO SIRVE PA NA HICOXS

podemos definir rotationes, por daily o weekly o con un numero o tamaño con el parametro size 5M para 5 megas, si definimos tiempo y tamaaño se cumpliara primero el tamañoen la configuracion. Lo mas normal es tiempo. para analiazarlos

////////// cosas de corn que no sirven pa na
tenemos el ontab qeu es system wide, se aplia todo el sistema estamos en /et/ontab tiene un path paa busar omandos, las lineas de on tiene el ormato minutos horas day of the month, mes, deay of hwthe week, el y el 7 oeresponden al domingo, el usuario y el comando que le corresponde.l

dentro de ls -l cr estan cron daily, houly, nothly, con que coloquemos un sopt en on hourly se ejeutara toodas las horas
ver el formato de etC crontrabo

on ontrab -e
editamos un fichero de cron nuestro propio, que nos afeta solo a nosotros
1 1 \* \* \* 'echo "oedioie"' day of the mont mon dow

en swad hay una guia para los monitories del sistema, al final de la presentacion esta el ejericio que tenemos que entregar.

hay que demostrar que se estan monitoizando, para eso vamos a esibi runa guia donde queramos y la subimos eal esapacio de trabajos ocmpraritods en swad, hay que instibir pasao a paso todo lo que haecmos y ocn capturas de pnantalla, para demotras que lo estamos monitoizando, lanzamos ssh lo tirrramos y lo volvemos a levanta.
hay que entegarlo antes del examen.

para instala zabix nos vamos a su ayuda.
instalamos zabbix 3.4
zabbix se compone de varios elementos, en la ayuda zzbiz poesses estan desitos.
tenemos que saber usar setver get y agent.
server es un proeos que corer en una maquina y se dedica unicamente a reibir logs, de por si no genera nada, en sever corern los agentes,el serrver esta en ubntu , y en centos instalamos solo los agentes. ademas vamos a instala run agente tambien en ubunut para monitoizar nuesta popia maquina- DeADEmas mavamos a instalar UI para verlo en el neavagado

ademas tenemos que instala subyaentemente una base de datos, elegimos mysql que ya esta instalado.

zabbix funciona ocn dos ambitos: pull y push
pull significa que el servidor se dedia a erabar la informacion de todos los agentes.
si no es eiciente puede tardar mucho en haer un muesteo, po eso existe el Poxy, que es un sevicio intermedio que se dedia a muesterarr a agentes y se los pasa al sevido,
push lo otro,
incoveniente de que pel serve se puede satuar, ventaja de que puede ocmunicar eventos asincronos, los agentes pueden ocmunicar mas rapido ylsoo eventos asincocrnos mas erpiado.

eordaemos que en centos tenemos un irewall asi que hay ue busa recual es el puerto del agente y pemitilo. En centos el agente de zabbix tnneesita mas permisos que los que entos da, paa soluionalo usamos la eerenia del profeb bug 134nuevenuevenuveohco.

muy impotante usa l a instalacion po paquetes de la pagina, usamos para ecntros la 7 nos slatamos la pate del servido.
uidado ocn la vesion de ubuntu!! que etenmos la 16.ero4

tenemos que ocnoecs los items de zabbix de system.pu-.\* y proc.mem
