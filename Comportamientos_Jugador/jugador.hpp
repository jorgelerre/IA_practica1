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
  
  //Variables de posicionamiento
  int fil, col, brujula, tam_mapa;
  //Número de veces consecutivas que se ha realizado la última acción
  int repeticionesUltimaAccion;
  //Valores aleatorios generados cada iteración
  bool random1, random2;
  bool bienSituado;	//Determina si hemos pasado por una casilla de posicionamiento
  //Mapa auxiliar donde se registra el mapa antes de tomar una casilla de posicionamiento
  //(cuando bienSituado es falso)
  vector<vector<unsigned char>> mapaPreLocalizacion;
  //Variables que registran los objetos que tiene el jugador
  bool tengoBikini, tengoZapatillas;
  //Variables usadas para realizar combinaciones de movimientos. Si una se activa, en el
  //siguiente turno el jugador la realizará.
  bool pendienteGiroIzda, pendienteGiroDcha;
  //Cuatro vectores (uno por orientación) de dos elementos. Indican si hay zona sin 
  //descubrir en esa dirección, uno a una distancia de 4 (justo una unidad más allá del 
  //alcance de la visión del jugador) y el otro a 8 (algo más lejos).
  bool sinDescubrir[4][2];
  //Indican si hay una zona no transitable a los lados izquierdo y derecho del jugador
  //(ya que la vision no llega ahi). Se registra en el turno anterior.
  bool muroDcha, muroIzda;
  //Cuentan durante cuanto tiempo se tiene un muro a un lado. Util para hallar puertas.
  int contador_muroDcha, contador_muroIzda;
  //Muestra un contador que cada turno se decrementa en 1. Cuando el personaje choca contra 
  //una zona no transitable en cierta dirección, el contador bloqueadoDireccion[brujula] se
  //resetea a un valor positivo. Cuando el valor de la variable es mayor que 0, se intentan 
  //evitar giros a esa dirección. Util para determinar cuando girar al jugador para descubrir
  //mapa evitando murallas o precipicios grandes.
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
  //La variable noHaySalida puede cambiar qué casillas son consideradas transitables.
  bool esTransitable(char tipoCasilla);
  //Gira al jugador hacia cierta orientación.
  Action girarADireccion(int nuevoSentido);
  //Se encarga de registrar la vision del jugador en cierto mapa.
  void registrarMapa(Sensores sensores, vector<vector<unsigned char>> & mapa);
};

#endif
