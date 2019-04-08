### Lección 1 - ssh

#### Pequeña explicación sobre como funciona ssh

---

#### Instalación de SSH en Ubuntu

- Crear máquinas virtuales CentOS y Ubuntu con la configuración estándar.
- Hay que volver a configurar la tarjeta de red (adaptador 2) como Host only (igual que en la práctica anterior).
- Instalar servidor ssh (en CentOS viene por defecto):
  - `apt-cache search sshd`
  - `sudo apt-get install openssh-server`
- Con `systemctl status ssh` podemos ver el estado del servicio y con `systemctl list-unit-files` podemos ver todos los servicios que hay.

#### Conexión mediante SSH a Ubuntu

- Necesitamos la IP de la máquina de Ubuntu. Para conseguirla, necesitamos configurar la red primero (como en la práctica 1). Una vez configurada, si ejecutamos `ifconfig -a` deberíamos ver nuestra IP.
- Para conectarnos usamos `ssh <usuario>@ip` desde la máquina anfitriona (nuestro ordenador).

#### Contenido carpeta .ssh (se encuentra en el cliente)

- `~/.ssh/id_rsa`: tu clave privada
- `~/.ssh/id_rsa.pub`: tu clave pública
- `~/.ssh/authorized_keys`: contiene una lista de claves públicas autorizadas para servidores. Cuando el cliente se conecta al servidor, esta autentica al cliente comprobando la firma de su clave pública guardada en este archivo.
- `~/.ssh/known_hosts`: contiene claves DSA públicas de los servidores SSH accedidos por el usuario. Este archivo es muy importante para asegurar que el cliente SSH se está conectando con el servidor SSH correcto.

#### Contenido carpeta /etc/ssh (en el servidor)

- `/etc/ssh/sshd_config`: archivo de configuración para el daemon de ssh. Campos que nos interesan:
  - `Port`: puerto en el que va a escuchar el daemon.
  - `PermitRootLogin`: para ver permitir acceso root o no.

#### Modificando la configuración del cliente

- Ejecutamos `vi /etc/ssh/sshd_config` y cambiamos el campo `Port 22` a `Port 22322`.
- Reiniciamos el servicio con `systemctl restart ssh`.
- Al hacer `ssh <usuario>ip` debería aparecernos conection refused.
- Nos conectamos desde el cliente con `ssh -p 22322 <usuario>ip`.
- Cambiamos el campo `PermitRootLogin prohibit-password` a `PermitRootLogin yes` para permitir el login al usuario root (normalmente esto se deshabilita).
- Para conectarnos sin usar la contraseña usamos: `ssh-copy-id <usuario>@ip`.
- Si copiamos el archivo `authorize_keys` en `/root` (que es el directorio home del usuario root) podremos acceder al usuario root mediante ssh.
- Cambiamos el campo `PermitRootLogin` a `PermitRootLogin no` para deshabilitar el login al usuario root (el profesor lo pedirá así).

#### Conectarnos mediante SSH sin contraseña

Nos vamos a la máquina desde la que nos queramos conectar, por ejemplo Ubuntu, y generamos nuestras llaves con `ssh-keygen` (le damos todo a enter) y luego ejecutamos `ssh-copy-id <usuariocentos>@ipcentos` y ya estaría. Ahora al hacer `ssh <usuariocentos>@ipcentos` se conectará directamente sin pedir contraseña.

#### Configuración del firewall

##### Ubuntu

`ufw` es el encargado de gestionar el firewall en Ubuntu. Con `ufw status` podemos ver si está habilitado a deshabilitado y con `ufw enable` y `ufw disable` lo activamos o desactivamos. Esos cambios solo se guardan localmente por lo que cuando reiniciemos la máquina se perderán. Para hacerlos de forma fija usamos `systemctl enable ufw` y `systemctl status ufw`.

Si ahora nos intentamos conectar a la máquina usando ssh, veremos que no nos dejará. Tenemos que activar `OpenSSH` en el firewall: `ufw allow OpenSSH`.

Si borramos las reglas anteriores (con `ufw deny OpenSSH` o `ufw delete 1`) y hacemos `ufw allow 22/tcp` también nos dejará conectarnos.

##### CentOS

Si por comodidad, en lugar de usar ips, puedes crear un alias de la máquina usando `sudo hostname <alias>`.

Para poder hacerte root en CentOS necesitas modificar el archivo `/etc/sudoers` mediante el comando `usermod -G wheel <usuario>`.

En CentOS para configurar el firewall se usa el comando `firewall-cmd`. Para ver el estado del servicio usamos `systemctl status firewalld`. Los archivos de configuración se encuentran en `/etc/firewalld`.

- `firewall-cmd --remove service=ssh --permanent`
- `firewall-cmd --add-service=ssh --permanent`
- `firewall-cmd --add-port=22/tcp --permanent`
- En lugar de `--permanent` podemos usar `--runtime-to-permanent`.

### Lección 2 - Backups y Sistemas de Control de Versiones

#### dd

Sistema de copias a nivel de dispositivos.

Esta utilidad lo copia todo en raw por lo que nos saltamos el buffer del sistema de ficheros (algunas cosas podrían perderse).

- `dd if=/dev/sd1 of=./sda1.raw bs=1024k`
- `dd if=/dev/zero of=./zeros.dat bs=1k count=5`

#### cpio

Sistema de copias a nivel de sistemas de ficheros.

Es un formato estándar, se usa para meter estructuras de directorios complejas en un solo archivo.

- `find /etc/ -iname '\*.conf' | cpio -ov > ./etc/Conf.cpio`
- `cpio -iduv < ./etc/Conf.cpio`
- `cpio -iduv '/etc/fuse.conf' < ./etc/Conf.cpio`

#### tar

Sistemas de copias a nivel de archivos

- `tar czf /tmp/etc.tgz`
- `tar xzf /tmp/etc.tgz`

#### rsync

Para mantener ficheros sincronizados entre dos máquinas.

- `rsync -a ./dev antonio@ip ip:/home/antonio`
- borramos algo
- ejecutamos otra vez el comando anterior y vemos que reaparece.

### Lección 3: LAMP Stack (Linux, Apache, MySQL, PHP)

Nos vamos a Ubuntu y ejecutamos `tasksel`, buscamos la opción `LAMP server`, la seleccionamos con espacio y pulsamos enter.

Con `systemctl status apache2` podemos ver que Apache está correctamente instalado. En `/etc/apache2` está la configuración. El archivo más importante es `apache2.conf`. Podemos comprobar que funciona correctamente lanzando un navegador y escribiendo `http://tuipdeubuntu`.

Si no funciona puede ser porque el firewall esté activo: desactívalo o añade el puerto 80 con `ufw allow 80/tcp`. También podemos usar `lynx localhost` o `curl http://localhost` (con -v podemos ver las cabeceras de la petición).

Ahora tenemos que comprobar que funciona MySQL ejecutando `mysql`. Con php igual: `php -a` (en `/etc/php/7.0` está la configuración)

Añadimos a `var/www/html/index.html` en el body:

```
<?php
phpinfo();
?>
```

Y renombramos el archivo con: `mv index.html index.php`.

Ahora nos cambiamos a CentOS y vamos a hacer lo mismo:

- `yum install httpd`
- `systemctl enable httpd`
- `systemctl start httpd`
- `firewall-cmd --add-service=http`
- `yum install php`
- `apachectl restart`

Creamos `index.php` en `/var/www/html` conteniendo:

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<?php
phpinfo();
?>
</body>
</html>
```

Instalamos y configuramos MariaDB:

- `yum install mariadb-server`
- `systemctl enable mariadb`
- `systemctl start mariadb`

Es recomnedable ejecutar `mysql_secure_installation`.

Creamos un usuario:

- `mysql -uroot -pantonio`
- `create database ise2019;`
- `grant all privileges on ise2019.\* to 'isedev'@'localhost' identified by 'isedev';`
- `flush privileges;`
- `exit`
- Para comprobar que funciona: `mysql -uisedev -pisedev ise2019`.
- `yum install php-mysql` y reiniciamos apache `apachectl restart`.

Y sustituimos el body de `/var/www/index.php`, quedándonos así:

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<?php
<?php
$link = mysql_connect('localhost', 'mysql_user', 'mysql_password');
if (!$link) {
    die('Could not connect: ' . mysql_error());
}
echo 'Connected successfully';
mysql_close($link);
?>
?>
</body>
</html>
```
