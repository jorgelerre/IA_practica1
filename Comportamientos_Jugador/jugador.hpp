#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = 99;
      brujula = 0;
      girarDerecha = false;
      bienSituado  = false;
      tengoBikini = tengoZapatillas = false;
      ultimaAccion = actIDLE;
      pendienteGiroIzda = pendienteGiroDcha = false;
      sinDescubrirNorte = sinDescubrirEste = sinDescubrirSur = sinDescubrirOeste = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  // Declarar aquí las variables de estado
  int fil, col, brujula;
  bool girarDerecha, bienSituado;
  bool tengoBikini, tengoZapatillas;
  bool pendienteGiroIzda, pendienteGiroDcha;
  bool sinDescubrirNorte, sinDescubrirEste, sinDescubrirSur, sinDescubrirOeste;
  Action ultimaAccion;  

};

#endif
