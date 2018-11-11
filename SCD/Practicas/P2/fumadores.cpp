#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include "HoareMonitor.h"

using namespace HM;
using namespace std;

static const int n_fum = 3;

template< int min, int max > int aleatorio()
{
    static default_random_engine generador( (random_device())() );
    static uniform_int_distribution<int> distribucion_uniforme( min, max);
    return distribucion_uniforme( generador );
}

void fumar( int n_fum )
{
    chrono::milliseconds t( aleatorio<20,200>() );
    cout << "Fumador ( " << n_fum << " ) empieza a fumar durante " << t.count() << " ms." << endl << flush;  
    this_thread::sleep_for( t );
    cout << "Fumador ( " << n_fum << " ) termina de fumar." << endl << flush;
}

class MF : public HoareMonitor {
    private:
        int ingrediente_mostrador;
        CondVar fumadores[ n_fum ], estanquero;
        bool vacio, lleno;
    public:
        MF();
        void PonerIngrediente( int ingrediente );
        void ObtenerIngrediente( int ingrediente );
        void EsperarRecogida();
};

MF::MF()
{
    ingrediente_mostrador = -1;
    for( int i = 0; i < n_fum; i++ ) fumadores[i] = newCondVar();
    estanquero = newCondVar();
}

void MF::PonerIngrediente( int ingrediente )
{
    ingrediente_mostrador = ingrediente;
    fumadores[ ingrediente ].signal();
}

void MF::ObtenerIngrediente( int ingrediente )
{
    if( ingrediente_mostrador != ingrediente ) fumadores[ingrediente].wait();
    ingrediente_mostrador = -1;
    estanquero.signal();
}

void MF::EsperarRecogida()
{
    if( ingrediente_mostrador != -1 ) estanquero.wait();
}

void funcion_hebra_estanquero( MRef<MF> monitor )
{
    int ingrediente;
    while( true )
    {
        ingrediente = aleatorio<0, n_fum - 1>();
        cout << "Estanquero produce ingrediente: " << ingrediente << "." << endl << flush;
        monitor->PonerIngrediente( ingrediente );
        monitor->EsperarRecogida();
    }
}

void funcion_hebra_fumador( MRef<MF> monitor, int f )
{
    while( true )
    {
        monitor->ObtenerIngrediente( f );
        fumar( f );
    }
}

int main()
{
    cout << "<-------------Estanco------------->" << endl;
    auto monitor = Create<MF>();
    thread fumadores[n_fum], 
           estanquero( funcion_hebra_estanquero, monitor );

    for( int i = 0; i < n_fum; i++ ) fumadores[i] = thread( funcion_hebra_fumador, monitor, i );
    for( int i = 0; i < n_fum; i++ ) fumadores[i].join();
}