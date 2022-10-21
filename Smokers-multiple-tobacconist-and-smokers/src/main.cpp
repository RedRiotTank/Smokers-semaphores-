/* 
 * File:   main.cpp
 * Author: albertoplaza
 *
 * Created on 5 de octubre de 2021, 10:00
 */

#include <iostream>
#include <cstdlib>
#include <chrono> 
#include "scd.h"
#include <mutex>
#include <thread>
using namespace std;
using namespace scd;

// ---------------- VAR GLOBALES MODIFICABLES --------------------

const int numEstanqueros = 4;
const int numTipoFumadores = 3;


const int NumFumadoresXtipo[numTipoFumadores] = {3, 1, 2};

Semaphore
mostr_vacio = 1,        //incrementando este número basta para añadir más espacio en la mesa del estanquero
        ingr_disp[numTipoFumadores] = {0, 0, 0}; //hay que añadir un 0 si se añade un tipo más de fumador.

const int 
        minFumar = 20,
        maxFumar = 100,
        minProducir = 20,
        maxProducir = 100;

// ---------------- CÓDIGO --------------------

void fumar(int tipoFumador, int numFumador) {
    cout << "Fumador tipo " << tipoFumador << " nº " << numFumador << " comienza a fumar" << endl;
    this_thread::sleep_for(chrono::milliseconds(aleatorio<minFumar, maxFumar>()));
    cout << "Fumador tipo " << tipoFumador << " nº " << numFumador << " termina de fumar" << endl;
}

int producir() {
    this_thread::sleep_for(chrono::milliseconds(aleatorio<minProducir, maxProducir>()));
    int produccion = aleatorio < 0, numTipoFumadores - 1 > ();
    return produccion;
}

void HebraEstanquero(int numEstanquero) {
    int ingrediente;
    
    while (true) {
        //simular la producción
        ingrediente = producir();
        mostr_vacio.sem_wait();
        
        cout << "Estanquero "<< numEstanquero <<" Pone ingrediente: " << ingrediente << endl;
        ingr_disp[ingrediente].sem_signal();
    }
}

void HebraFumador(int tipoFumador, int numFumador) {


    while (true) {
        ingr_disp[tipoFumador].sem_wait();
        cout << "Fumador tipo " << tipoFumador << " nº " << numFumador << " Retira ingrediente: " << tipoFumador << endl;
        mostr_vacio.sem_signal();
        fumar(tipoFumador, numFumador);
    }
}

int main(int argc, char** argv) {
    int totalFumadores = 0;
    int numFumador = 0;

    for (int i = 0; i < numTipoFumadores; i++)
        totalFumadores += NumFumadoresXtipo[i];

    thread
    estanquero[numEstanqueros],
            fumador[totalFumadores];
    
    for(int numEstanquero=0; numEstanquero<numEstanqueros; numEstanquero++)
        estanquero[numEstanquero] = thread(HebraEstanquero,numEstanquero);

    for (int tipoFumador = 0; tipoFumador < numTipoFumadores; tipoFumador++) {
        for (int numTipoFumador = 0; numTipoFumador < NumFumadoresXtipo[tipoFumador]; numTipoFumador++) {
            fumador[numFumador] = thread(HebraFumador, tipoFumador, numTipoFumador);
            numFumador++;
        }
    }
    for(int numEstanquero=0; numEstanquero<numEstanqueros; numEstanquero++)
        estanquero[numEstanquero].join();
    
    return 0;
}

