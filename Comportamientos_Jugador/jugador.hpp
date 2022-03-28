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
      random1 = random2 = false;
      tengoBikini = tengoZapatillas = false;
      ultimaAccion = actIDLE;
      pendienteGiroIzda = pendienteGiroDcha = false;
      for(int i = 0; i < 4; i++)
      	for(int j = 0; j < 3; j++)
      		sinDescubrir[i][j] = false;
      muroDcha = muroIzda = false;
      contador_muroDcha = contador_muroIzda = 0;
      contadorObjeto = contadorDescubrir = 0;
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
  bool random1, random2;
  bool bienSituado;	//Determina si hemos pasado por una casilla de posicionamiento
  bool tengoBikini, tengoZapatillas;
  bool pendienteGiroIzda, pendienteGiroDcha;
  //Cuatro vectores (uno por orientación) de dos elementos. Indican si hay zona sin 
  //descubrir en esa dirección, uno a una distancia de 4 y el otro a 8
  bool sinDescubrir[4][2];
  //Indican si hay una zona no transitable a los lados del jugador
  //(ya que la vision no llega ahi)
  bool muroDcha, muroIzda;
  //Cuentan durante cuanto tiempo se tiene un muro a un lado. Util para hallar puertas.
  int contador_muroDcha, contador_muroIzda;
  //Muestra el tiempo que se lleva sin chocar con un borde en cierta dirección. Util para
  //determinar cuando girar al jugador para descubrir mapa evitando murallas o precipicios
  //grandes.
  int bloqueadoDireccion[4];
  //Contadores para saber si hay un objeto cerca o si se ha girado para descubrir mapa hace poco
  int contadorObjeto, contadorDescubrir;
  //Determina si el jugador está atrapado en una isla pequenia, rodeada de terreno intransitable.
  //Si esto se da, permitiremos el avance por zonas de bosque y agua, aun sin tener la 
  //indumentaria adecuada.
  bool noHaySalida;
  //Determina si hace falta recargar la batería, según cuanta quede y los ciclos restantes.
  bool recargarBateria;
  //Recordamos la acción anterior
  Action ultimaAccion;  


  //Funciones auxiliares
  
  //Devuelve true si el jugador puede avanzar por ese terreno.
  bool esTransitable(char tipoCasilla);
  
  //Gira al jugador desde sentido
  Action girarADireccion(int nuevoSentido);
  void registrarMapa(Sensores sensores, vector<vector<unsigned char>> & mapa);
};

#endif
