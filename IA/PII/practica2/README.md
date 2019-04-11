## Explicación comportamiento PII

### Nivel 1

Para el nivel 1 he usado prácticamente el mismo código descrito en el tutorial. Para la búsqueda en anchura
simplemente he sustituido la estructura de datos `stack` por una `queue`, en el método dado de la búsqueda 
en profundidad.

Para la búsqueda de coste justo he sustituido la estructura de datos `stack` por una `priority_queue` y he 
usado como peso la suma del coste de ciclos de pasar por cada una de las casillas de los ciclos. El paso es 
calculado por la siguiente función: 

~~~C++
int ComportamientoJugador::calcularPeso(list<Action> &plan)
{
	estado current = actual;
	int peso = 0;
	int distance;
	for (list<Action>::iterator it = plan.begin(); it != plan.end(); ++it)
	{
		distance = sqrt((destino.fila - current.fila) ^ 2 + (destino.columna - current.columna) ^ 2);
		// prettier-ignore
		switch (*it)
		{
		case actTURN_R:
			current.orientacion = (current.orientacion + 1) % 4;
			peso += 1;
			break;
		case actTURN_L:
			current.orientacion = (current.orientacion + 3) % 4;
			peso += 1;
			break;
		case actFORWARD:
			peso += getPeso(current);
			switch (current.orientacion)
			{
			case 0:
				current.fila--;
				break;
			case 1:
				current.columna++;
				break;
			case 2:
				current.fila++;
				break;
			case 3:
				current.columna--;
				break;
			}
			break;
		}
	}
	return peso + distance;
}
~~~
