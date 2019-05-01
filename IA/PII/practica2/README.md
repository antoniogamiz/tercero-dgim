## Explicación comportamiento PII

### Nivel 1

Para el nivel 1 he usado prácticamente el mismo código descrito en el tutorial. Para la búsqueda en anchura
simplemente he sustituido la estructura de datos `stack` por una `queue`, en el método dado de la búsqueda 
en profundidad.

Para la búsqueda de coste justo he sustituido la estructura de datos `stack` por una `priority_queue` y he 
usado como peso la suma del coste de ciclos de pasar por cada una de las casillas de los ciclos junto con la
distancia ecuclídea hasta el objeitvo. El paso es calculado por la siguiente función: 

~~~C++
int ComportamientoJugador::calcularPeso(list<Action> &plan)
{
	estado current = actual;
	int peso = 0;
	int distance;
	for (list<Action>::iterator it = plan.begin(); it != plan.end(); ++it)
	{
		distance = sqrt((destino.fila - current.fila) ^ 2 + (destino.columna - current.columna) ^ 2);
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

Sumar la distancia euclídea entre el personaje y el objetivo penaliza encontrar la solución óptima pero reduce 
enormemente el tiempo de ejecución del algoritmo en mapas como el de las islas (en las que la mayor parte del 
mapa tiene el mismo terreno, agua, por lo que encuentra la solución más lento).

### Nivel 2

Para resolver este nivel he dividido el problema en varios apartados:

#### Hacia dónde estoy mirando

Cuando aparecemos por primera vez en el mapa, miramos hacia el norte, pero una vez empezamos a movernos aleatoriamente
perdemos esa información ya que aunque encontremos un punto de referencia este solo nos va a decir las coordenadas en 
las que estamos y no nuestra orientación. Para resolver esto he creado la siguiente función:

~~~C++
  int updateBrujula(int current, Action accion);
~~~

Que simplemente actualiza el valor de la brújula (`current`) según una acción (`accion`).

#### A dónde nos dirigimos

Una vez sabemos hacia donde miramos, necesitamos una forma de movernos aleatoriamente por el mapa sin chocar con obstáculos
ni caer a precipicios, hasta encontrar un punto de referencia. Para ello, he diseñado la siguiente función: 

~~~C++
Action ComportamientoJugador::randomMove(Sensores sensores)
{
	Action sigAccion = actIDLE;
	srand(time(NULL));
	float num = (1 + rand() % 100) / 100.0;
	if (num < 0.1)
	{
		sigAccion = actTURN_L;
	}
	else if (0.1 <= num and num < 0.2)
	{
		sigAccion = actTURN_R;
	}
	else
	{
		sigAccion = actFORWARD;
	}
	// Esta funcion es igual que HayObstaculoDelante pero usa
	// los sensores en lugar de mapaResultado (que desconocemos)
	if (HayObstaculoDelante2(sensores) and sigAccion == actFORWARD)
	{
		sigAccion = actTURN_R;
	}

	return sigAccion;
}
~~~

Simplemente genera un número entre 0 y 1 y devuelve una acción u otra basad en ese valor. Si encuentra un obstáculo en 
la acción resultado, gira a la derecha por defecto.

#### Dónde estamos

Una vez sabemos hacia dónde miramos y cómo movernos, tenemos que saber dónde estamos. Esto solo lo podemos lograr una vez 
encontremos un punto de referencia moviéndonos aleatoriamente por el mapa. Cuando esto ocurra, pondremos a `true` la 
variable booleana `pkencontrado` e inicializamos `actual` con los datos recibidos. Una vez conocida la posición que tenemos, 
debemos mantenerla actualizada con nuestros movimientos. Todo esto es hecho por el siguiente código:

~~~C++
		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;
		brujula = updateBrujula(brujula, ultimaAccion);
		actual.orientacion = brujula;
		actual = move(actual, ultimaAccion);
		if (sensores.mensajeF != -1 and !pkencontrado and sensores.mensajeC != sensores.destinoC)
		{
			actual.fila = sensores.mensajeF;
			actual.columna = sensores.mensajeC;
			pkencontrado = true;
			Pintar(actual); // ignorar por ahora
		}
~~~

Además, en cada ejecución, actualizamos la variable `destino` con la información de los sensores.

#### ¡A colorear!

Una vez sabemos todo lo anterior, al fin podemos empezar a colorear el mapa! Usando nuestra posición (`actual`) junto con 
los sensores `terreno` y `superficie` tenemos suficiente. Las funciones encargadas de esto son `updateView` y `updateViewAldeanos` (pongo solo una ya que la segunda es idéntica a la primera pero solo actualiza `mapaResultado` cuando 
hay un aldeano a la vista):

~~~C++
void ComportamientoJugador::updateView(const estado &pos, vector<unsigned char> &view)
{
	switch (pos.orientacion)
	{
	case 0:
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2 * i + 1; j++)
			{
				if (validIndex(pos.fila - i, pos.columna - i + j, mapaResultado.size()))
				{
					mapaResultado[pos.fila - i][pos.columna - i + j] = view[i * i + j];
				}
			}
		}
	...
	}
]
~~~

La única complicación de esta función es indizar correctamente `mapaResultado` y `view` debido a la forma en la que 
está representada la vista. He puesto solo el caso Norte porque los demás son prácticamente iguales.

Además, para aprovechar todo lo que vemos hasta que vemos del mapa mientras nos movemos aleatoriamente hasta encontrar un punto de referencia, he creado una variable adicional: `stack<pair<Action, vector<unsigned char>>> buf`. `buf` contiene 
las acciones realizadas junto con la vista en ese momento (no podemos guardar los estados porque no sabemos la posición!). 
Se guarda por parejas en una pila para falicitar su reconstrucción en la función `Pintar` que lo que haces es reconstruir el mapa visto con anterioridad:

~~~C++
void ComportamientoJugador::Pintar(const estado &st)
{
	pair<Action, vector<unsigned char>> current;
	estado mov = st;
	while (!buf.empty())
	{
		current = buf.top();
		updateView(mov, current.second);
		mov = undoMove(mov, current.first);
		buf.pop();
	}
}
~~~

#### Hora del azúcar

Ya podemos movernos sin morir y ver sin problemas, hora de conseguir comida. Este es el código encargado del movimiento una
vez encontrado un punto de referencia (si no se ha encontrado simplemente nos movemos como antes he explicado):

~~~C++
			if (!hayPlan || recalcular)
			{
				hayPlan = pathFinding(3, actual, destino, plan);
				recalcular = false;
			}

			if (hayPlan and plan.size() > 0 and !recalcular)
			{
				sigAccion = plan.front();
				plan.erase(plan.begin());

				if (HayObstaculoDelante2(sensores) and sigAccion == actFORWARD)
				{
					sigAccion = randomMove(sensores);
					recalcular = true;
				}
			}
			else
			{
				sigAccion = randomMove(sensores);
				recalcular = false;
			}

			if (actual.fila == sensores.destinoF and actual.columna == sensores.destinoC || plan.size() == 0)
				hayPlan = false;
~~~

Simplemente usamos la función `pathFinding` con el algoritmo de peso justo para calcular el camino hacia el objetivo. El segundo if se encarga de seguir el camino generado, y en caso de encontrar un obstáculo, se mueve aleatoriamente y pone `recalcular`  a `true` para rehacer el camino una vez evitado el obstáculo.

El último if es un pequeño ajuste para que no se bloquee cuando llegue a un objetivo y pueda seguir avanzando.
