#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      tam_mapa = size;
      fil = col = 99;
      brujula = 0;
      repeticionesUltimaAccion = 0;
      bienSituado = false;
      girarDerecha = girarDerecha2 = false;
      tengoBikini = tengoZapatillas = false;
      ultimaAccion = actIDLE;
      pendienteGiroIzda = pendienteGiroDcha = false;
      sinDescubrirNorte = sinDescubrirEste = sinDescubrirSur = sinDescubrirOeste = false;
      muroDcha = muroIzda = false;
      bloqueadoNorte = bloqueadoEste = bloqueadoSur = bloqueadoOeste = 0;
      
      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  // Declarar aquí las variables de estado
  int fil, col, brujula, tam_mapa;
  int repeticionesUltimaAccion;
  bool girarDerecha, girarDerecha2, bienSituado;
  bool tengoBikini, tengoZapatillas;
  bool pendienteGiroIzda, pendienteGiroDcha;
  bool sinDescubrirNorte, sinDescubrirEste, sinDescubrirSur, sinDescubrirOeste;
  bool muroDcha, muroIzda;
  int bloqueadoNorte, bloqueadoEste, bloqueadoSur, bloqueadoOeste;
  bool noHaySalida;
  Action ultimaAccion;  


  //Funciones auxiliares
  bool esTransitable(char tipoCasilla);
  Action girarADireccion(int sentidoActual, int nuevoSentido);
};

#endif
