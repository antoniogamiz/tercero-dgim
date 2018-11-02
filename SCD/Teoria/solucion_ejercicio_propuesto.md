Asumimos que hay una cola con 3 metodos (no ze mu bien cualeh)

procedure adquirir(tiempo : integer)

begin
if (ocupado)
    { 
    ColaOrd.Insertar(tiempo)
    cola.wait()
    while (ColaOrde.Cabeza != tiempo)
    {
        cola.signal(); // probamos con el siguiente
        cola.wait()
    }

    ColaOrd.extraer(); // sacamos ee tiempo de la cola 
    ocupado = true;

procedure librerar()

begin 
    ocupado=false
    cola.signal()
end