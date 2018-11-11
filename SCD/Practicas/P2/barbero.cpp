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

static const int n_clientes = 3;

template< int min, int max > int aleatorio()
{
    static default_random_engine generador( (random_device())() );
    static uniform_int_distribution<int> distribucion_uniforme( min, max);
    return distribucion_uniforme( generador );
}

void espera()
{
    chrono::milliseconds t( aleatorio<20,200>() );
    this_thread::sleep_for( t );
}

void CortarPeloACliente() { espera(); }
void EsperarFueraBarberia( int c ) 
{
    cout << "Cliente ( " << c << " ) espera fuera de la barberia." << endl << flush;
    espera();
}

class MB : public HoareMonitor {
    private:
        CondVar clientes, 
                barbero,
                silla;
        bool libre;
    public:
        MB();
        void CortarPelo( int i );
        void SiguienteCliente();
        void FinCliente();
};

MB::MB()
{
    clientes = newCondVar();
    barbero = newCondVar();
    silla = newCondVar();
    libre = true;
}

void MB::CortarPelo( int i )
{
    cout << "Cliente ( " << i << " ) entra a la barbería." << endl << flush;

    if( !clientes.empty() || !libre ) clientes.wait();

    libre = false;
    barbero.signal();
    cout << "Cliente ( " << i << " ) se sienta." << endl << flush;
    silla.wait();
    cout << "Cliente ( " << i << " ) se levanta." << endl << flush; 
}

void MB::SiguienteCliente()
{
    if( clientes.empty() )
    {
        cout << "Barbero duerme..." << endl << flush;
        barbero.wait();
    } else 
    {
        clientes.signal();
    }
}

void MB::FinCliente()
{
    cout << "Barbero termina cliente..." << endl << flush;
    libre = true;
    silla.signal();
}

void funcion_hebra_barbero( MRef<MB> monitor )
{
    while( true )
    {
        monitor->SiguienteCliente();
        CortarPeloACliente();
        monitor->FinCliente();
    }
}

void funcion_hebra_cliente( MRef<MB> monitor, int c )
{
    while( true )
    {
        monitor->CortarPelo( c );
        EsperarFueraBarberia( c );
    }
}

int main()
{
    cout << "<-------------Barberia------------->" << endl;
    auto monitor = Create<MB>();
    thread clientes[n_clientes], 
           estanquero( funcion_hebra_barbero, monitor );

    for( int i = 0; i < n_clientes; i++ ) clientes[i] = thread( funcion_hebra_cliente, monitor, i );
    for( int i = 0; i < n_clientes; i++ ) clientes[i].join();
}