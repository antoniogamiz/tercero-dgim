
#include <iostream>
#include <iomanip>
#include <chrono>  // incluye now, time\_point, duration
#include <future>
#include <vector>
#include <cmath>

using namespace std ;
using namespace std::chrono;

const long m  = 1024l*1024l*1024l,
           n  = 4  ;

const long chunk_size = ( m % 2 == 0 ) ? m/n : (m/n) + 1;

// -----------------------------------------------------------------------------
// evalua la función $f$ a integrar ($f(x)=4/(1+x^2)$)
double f( double x )
{
  return 4.0/(1.0+x*x) ;
}
// -----------------------------------------------------------------------------
// calcula la integral de forma secuencial, devuelve resultado:
double calcular_integral_secuencial(  )
{
   double suma = 0.0 ;                        // inicializar suma
   for( long i = 0 ; i < m ; i++ )            // para cada $i$ entre $0$ y $m-1$:
      suma += f( (i+double(0.5)) /m );         //   $~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                            // devolver valor promedio de $f$
}

// -----------------------------------------------------------------------------
// cálculo de la función repartiendo la carga consecutivamente en lugar de por bloques
double funcion_hebra_scatter( long i )
{
  double suma = 0.0;
  for( long j = i ; j < m; j += n )
    if( j < m ) suma += f( (j + double(0.5) )/m );
  return suma;
}

// -----------------------------------------------------------------------------
// calculo de la integral de forma concurrente (scatter)
double calcular_integral_concurrente_scatter( )
{
  future<double> futuros[n] ;

  // poner en marcha todas las hebras y obtener los futuros
  for( int i = 0 ; i < n ; i++ )
    futuros[i] = async( launch::async, funcion_hebra_scatter, i ) ;

  long double pi = 0.0;
  for( int i = 0 ; i < n ; i++ )
     pi += futuros[i].get();

  pi/=m;

  return pi;
}

// -----------------------------------------------------------------------------

int main()
{

  time_point<steady_clock> inicio_sec  = steady_clock::now() ;
  const double             result_sec  = calcular_integral_secuencial(  );
  time_point<steady_clock> fin_sec     = steady_clock::now() ;
  double x = sin(0.4567);

  time_point<steady_clock> inicio_conc_sca  = steady_clock::now() ;
  const double             result_conc_sca  = calcular_integral_concurrente_scatter(  );
  time_point<steady_clock> fin_conc_sca     = steady_clock::now() ;

  duration<float,milli>    tiempo_sec  = fin_sec  - inicio_sec ,
                           tiempo_conc_sca = fin_conc_sca - inicio_conc_sca;
  const float              porc        = 100.0*tiempo_conc_sca.count()/tiempo_sec.count() ;


  constexpr double pi = 3.14159265358979323846l ;

  cout << "Número de muestras (m)            : " << m << endl
       << "Número de hebras (n)              : " << n << endl
       << setprecision(18)
       << "Valor de PI                       : " << pi << endl
       << "Resultado secuencial              : " << result_sec  << endl
       << "Resultado concurrente (scatter)   : " << result_conc_sca << endl
       << setprecision(5)
       << "Tiempo secuencial                 : " << tiempo_sec.count()  << " milisegundos. " << endl
       << "Tiempo concurrente (scatter)      : " << tiempo_conc_sca.count() << " milisegundos. " << endl
       << setprecision(4)
       << "Porcentaje t.conc_sca/t.sec.      : " << porc << "%" << endl;
}
