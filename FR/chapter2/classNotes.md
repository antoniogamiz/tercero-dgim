Modelos de interaccion
*  Cliente servidor: (mayormente usada)
    *  desventajas-limitaciones: 
        *  fuerte aclopamiento "espacial" => para poder solicitar un recurso, tengo que **saber** donde **está** el recurso.
        *  Acoplamiento temporal: el cliente tiene que interaccionar con el server siempre que quiera obtener un recurso.
    * P2P (peer to peer):
        * un tipo de P2P: Publish/Subscribe: en lugar de tener un relacion de dame esto toma esto, aqui se "publica" informacion, y quien quiere la consulta. Existe un Data Space donde unas entidades llamadas Publicadores publican datos, y otras llamadas Subscriptores la recogen. Esto evita el acoplamiento espacial, ya que no necesitas saber donde estan los recursos. El Data Space es algo abstracto, por ejemplo una cache distribuida y replicada entre los peers, donde por ejemplo nos subscrimos a la posicion de un avion, el avion puede publicar su informacion en el DS y el controlador aereo puede consultar esa informacion en el data space sin necesita de hablar con el publicador.
