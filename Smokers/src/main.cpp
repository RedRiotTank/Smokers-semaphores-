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

const int fumadores = 3;

Semaphore
    mostr_vacio = 1,
    ingr_disp[fumadores] = {0,0,0};     //si se añade fumador, hay que añadir un 0.
   
//tiempo (ms)
const int 
    minTimeFumar = 1000,
    maxTimeFumar = 2000,
    minTimeProd = 50,
    maxTimeProd = 200;


// ---------------- CÓDIGO --------------------

void fumar( int num_fum )
{
cout << "Fumador número " << num_fum << ": comienza a fumar." << endl;
this_thread::sleep_for( chrono::milliseconds( aleatorio<minTimeFumar,maxTimeFumar>() ));
cout << "Fumador número " << num_fum << ": termina de fumar." << endl;
}

int producir(){
    this_thread::sleep_for(chrono::milliseconds(aleatorio<minTimeProd, maxTimeProd>()));
    int produccion = aleatorio<0, fumadores - 1>();
    return produccion;
}


void HebraEstanquero() {
    int i;
    while (true){
        //simular la producción
        i = producir();
        mostr_vacio.sem_wait();
        cout << "Puesto ingrediente: " << i << endl;
        ingr_disp[i].sem_signal();
    }
}

void HebraFumador(int i){
    while (true){
        ingr_disp[i].sem_wait();
        cout << "Retirado ingrediente: " << i << endl;
        mostr_vacio.sem_signal();
        fumar(i);
    }
}


int main(int argc, char** argv) {
    
    thread estanquero (HebraEstanquero), fumador[fumadores];
    
    for(int i=0; i < fumadores; i++){
        fumador[i] = thread(HebraFumador,i);
    }
    
    estanquero.join();
    return 0;
}

