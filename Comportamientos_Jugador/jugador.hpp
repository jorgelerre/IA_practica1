#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <vector>
#define NORTE 0
#define ESTE  1
#define SUR   2
#define OESTE 3


using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      tam_mapa = size;
      fil = col = tam_mapa;
      brujula = NORTE;
      repeticionesUltimaAccion = 0;
      bienSituado = false;
      girarDerecha = girarDerecha2 = false;
      tengoBikini = tengoZapatillas = false;
      ultimaAccion = actIDLE;
      pendienteGiroIzda = pendienteGiroDcha = false;
      for(int i = 0; i < 4; i++)
      	for(int j = 0; j < 3; j++)
      		sinDescubrir[i][j] = false;
      muroDcha = muroIzda = false;
      contador_muroDcha = contador_muroIzda = 0;
      contadorObjeto = 0;
      for(int i = 0; i < 4; i++)
      	bloqueadoDireccion[i] = 0;
      
      mapaPreLocalizacion.resize(tam_mapa*2+1);
      for(int i = 0; i < tam_mapa*2+1; i++)
        mapaPreLocalizacion[i].resize(tam_mapa*2+1, '?');
      
      noHaySalida = false;
      recargarBateria = false;
      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  // Declarar aquí las variables de estado
  int fil, col, brujula, tam_mapa;
  vector<vector<unsigned char>> mapaPreLocalizacion;
  int repeticionesUltimaAccion;
  bool girarDerecha, girarDerecha2, bienSituado;
  bool tengoBikini, tengoZapatillas;
  bool pendienteGiroIzda, pendienteGiroDcha;
  bool sinDescubrir[4][3];
  bool muroDcha, muroIzda;
  int contador_muroDcha, contador_muroIzda;
  int bloqueadoDireccion[4];
  int contadorObjeto;
  bool noHaySalida, recargarBateria;
  Action ultimaAccion;  


  //Funciones auxiliares
  bool esTransitable(char tipoCasilla);
  Action girarADireccion(int sentidoActual, int nuevoSentido);
};

#endif


