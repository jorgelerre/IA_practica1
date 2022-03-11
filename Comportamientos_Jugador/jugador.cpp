#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;


Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Este" << endl; break;
		case 2: cout << "Sur " << endl; break;
		case 3: cout << "Oeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	// Determinar el efecto de la ultima accion enviada
	
	//Si se produce un reset, damos a todas las variables de estado su valor inicial
	if(sensores.reset){
		fil = col = -1;
      	brujula = 0;
      	girarDerecha = false;
      	if(sensores.nivel != 0){
      		bienSituado  = false;
      	}
      	tengoBikini = tengoZapatillas = false;
      	ultimaAccion = actIDLE;
      	pendienteGiroIzda = pendienteGiroDcha = false;
      	sinDescubrirNorte = sinDescubrirEste = sinDescubrirSur = sinDescubrirOeste = false;
	}
	
	//Actualizamos el valor aleatorio
	girarDerecha = (rand() % 2 == 0);
	
	
	switch (ultimaAccion){
		case actFORWARD:
			switch (brujula){
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			break;
		case actTURN_L:
			brujula = (brujula+3)%4;
			break;
		case actTURN_R:
			brujula = (brujula+1)%4;
			break;	
	}
	
	if(sensores.terreno[0] == 'G' && !bienSituado){
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
		bienSituado = true;
	}
	if(sensores.terreno[0] == 'K')
		tengoBikini = true;
	
	if(sensores.terreno[0] == 'D')
		tengoZapatillas = true;
	
	//Actualización del mapa (vision)
	if(bienSituado){
		mapaResultado[fil][col] = sensores.terreno[0];
		int coef = 1;
		if(brujula == 0 || brujula == 3)
			coef = -1;
			
		if(brujula == 1 || brujula == 3){
			mapaResultado[fil-1*coef][col+1*coef] = sensores.terreno[1];
			mapaResultado[fil]       [col+1*coef] = sensores.terreno[2];
			mapaResultado[fil+1*coef][col+1*coef] = sensores.terreno[3];
			mapaResultado[fil-2*coef][col+2*coef] = sensores.terreno[4];
			mapaResultado[fil-1*coef][col+2*coef] = sensores.terreno[5];
			mapaResultado[fil]       [col+2*coef] = sensores.terreno[6];
			mapaResultado[fil+1*coef][col+2*coef] = sensores.terreno[7];
			mapaResultado[fil+2*coef][col+2*coef] = sensores.terreno[8];
			mapaResultado[fil-3*coef][col+3*coef] = sensores.terreno[9];
			mapaResultado[fil-2*coef][col+3*coef] = sensores.terreno[10];
			mapaResultado[fil-1*coef][col+3*coef] = sensores.terreno[11];
			mapaResultado[fil]       [col+3*coef] = sensores.terreno[12];
			mapaResultado[fil+1*coef][col+3*coef] = sensores.terreno[13];
			mapaResultado[fil+2*coef][col+3*coef] = sensores.terreno[14];
			mapaResultado[fil+3*coef][col+3*coef] = sensores.terreno[15];
		}
		else{
			mapaResultado[fil+1*coef][col+1*coef] = sensores.terreno[1];
			mapaResultado[fil+1*coef][col]        = sensores.terreno[2];
			mapaResultado[fil+1*coef][col-1*coef] = sensores.terreno[3];
			mapaResultado[fil+2*coef][col+2*coef] = sensores.terreno[4];
			mapaResultado[fil+2*coef][col+1*coef] = sensores.terreno[5];
			mapaResultado[fil+2*coef][col]        = sensores.terreno[6];
			mapaResultado[fil+2*coef][col-1*coef] = sensores.terreno[7];
			mapaResultado[fil+2*coef][col-2*coef] = sensores.terreno[8];
			mapaResultado[fil+3*coef][col+3*coef] = sensores.terreno[9];
			mapaResultado[fil+3*coef][col+2*coef] = sensores.terreno[10];
			mapaResultado[fil+3*coef][col+1*coef] = sensores.terreno[11];
			mapaResultado[fil+3*coef][col]        = sensores.terreno[12];
			mapaResultado[fil+3*coef][col-1*coef] = sensores.terreno[13];
			mapaResultado[fil+3*coef][col-2*coef] = sensores.terreno[14];
			mapaResultado[fil+3*coef][col-3*coef] = sensores.terreno[15];
		}
	}
	
	//Comprobación de sectores cercanos sin explorar
	if(bienSituado){
		if(fil>=5 && (mapaResultado[fil-5][col] == '?'))// || mapaResultado[sensores.posF][sensores.posC-8] == '?'))
			sinDescubrirNorte = true;
		else
			sinDescubrirNorte = false;
			
		if(col<tam_mapa-5 && (mapaResultado[fil][col+5] == '?'))// || mapaResultado[sensores.posF][sensores.posC+8] == '?'))
			sinDescubrirEste = true;
		else
			sinDescubrirEste = false;
		
		if(fil<tam_mapa-5 && (mapaResultado[fil+5][col] == '?'))// || mapaResultado[sensores.posF+8][sensores.posC] == '?'))
			sinDescubrirSur = true;
		else
			sinDescubrirSur = false;
		
		if(col>=5 && (mapaResultado[fil][col-5] == '?' ))//|| mapaResultado[sensores.posF][sensores.posC-8] == '?'))
			sinDescubrirOeste = true;
		else
			sinDescubrirOeste = false;
	}
	
	
	
	//Definir la nueva acción
	//Si comienza en agua o bosque, avanza hasta salir de ahí
	if(((sensores.terreno[0]=='A' && !tengoBikini) || (sensores.terreno[0]=='B' && !tengoZapatillas)) && sensores.terreno[2] != 'P'){
		accion = actFORWARD;
	}
	//Si en el anterior movimiento decidimos hacer un giro a posteriori, se ejecuta ahora
	else if(pendienteGiroDcha){
		accion = actTURN_R;
		pendienteGiroDcha = false;
	}
	else if (pendienteGiroIzda){
		accion = actTURN_L;
		pendienteGiroIzda = false;	
	}
	
	//Si solo al norte hay una zona sin descubrir, ve hacia el norte el 50% de las veces
	else if (bienSituado && !sinDescubrirOeste && sinDescubrirNorte && !sinDescubrirEste && !sinDescubrirSur && girarDerecha){
		if(brujula == 3)		//Si ahora va hacia el oeste
			accion = actTURN_R;			//Gira a la derecha
		else if(brujula == 1)	//Si va al este
			accion = actTURN_L;			//Gira a la izquierda
		else if(brujula == 2){	//Si va en direccion contraria
			accion = actTURN_L;			//Da media vuelta
			pendienteGiroIzda = true;
		}	
	}
	//Si solo al este hay una zona sin descubrir, ve hacia el este el 50% de las veces
	else if (bienSituado && sinDescubrirEste && !sinDescubrirNorte && !sinDescubrirSur && !sinDescubrirOeste && girarDerecha){
		if(brujula == 0)		//Si ahora va hacia el norte
			accion = actTURN_R;			//Gira a la derecha
		else if(brujula == 2)	//Si va al sur
			accion = actTURN_L;			//Gira a la izquierda
		else if(brujula == 3){	//Si va al oeste
			accion = actTURN_L;			//Da media vuelta
			pendienteGiroIzda = true;
		}
	}
	//Si solo al sur hay una zona sin descubrir, ve hacia el sur el 50% de las veces
	else if (bienSituado && !sinDescubrirOeste && !sinDescubrirNorte && !sinDescubrirEste && sinDescubrirSur && girarDerecha){
		if(brujula == 1)		//Si ahora va hacia el este
			accion = actTURN_R;			//Gira a la derecha
		else if(brujula == 3)	//Si va al oeste
			accion = actTURN_L;			//Gira a la izquierda
		else if(brujula == 0){	//Si va en direccion contraria
			accion = actTURN_L;			//Da media vuelta
			pendienteGiroIzda = true;
		}
	}
	//Si solo al oeste hay una zona sin descubrir, ve hacia al oeste el 50% de las veces
	else if (bienSituado && sinDescubrirOeste && !sinDescubrirNorte && !sinDescubrirEste && !sinDescubrirSur && girarDerecha){
		if(brujula == 2)		//Si ahora va hacia el sur
			accion = actTURN_R;			//Gira a la derecha
		else if(brujula == 0)	//Si va al norte
			accion = actTURN_L;			//Gira a la izquierda
		else if(brujula == 1){	//Si va en direccion contraria
			accion = actTURN_L;			//Da media vuelta
			pendienteGiroIzda = true;
		}
	}
	
	if (accion == actIDLE){
		if((sensores.terreno[2]=='T' || sensores.terreno[2]=='S' || sensores.terreno[2]=='G'
		 ||(sensores.terreno[2]=='A' && tengoBikini) || (sensores.terreno[2]=='B' && tengoZapatillas)
		 || sensores.terreno[2] == 'K' || sensores.terreno[2] == 'D')
		&& (sensores.superficie[2]=='_')){
			accion = actFORWARD;
			
			//Si se tiene un objeto a la izquierda, se girará al siguiente movimiento a la izquierda
			if (
			((sensores.terreno[1] == 'K' || sensores.terreno[4] == 'K' || sensores.terreno[9] == 'K') && !tengoBikini)     || 
			((sensores.terreno[1] == 'D' || sensores.terreno[4] == 'D' || sensores.terreno[9] == 'D') && !tengoZapatillas) ||
			((sensores.terreno[1] == 'G' || sensores.terreno[4] == 'G' || sensores.terreno[9] == 'G') && !bienSituado)){
				pendienteGiroIzda = true;
			}
			//Si se tiene un objeto a la izquierda, se girará al siguiente movimiento
			else if (
			((sensores.terreno[3] == 'K' || sensores.terreno[8] == 'K' || sensores.terreno[15] == 'K') && !tengoBikini)     || 
			((sensores.terreno[3] == 'D' || sensores.terreno[8] == 'D' || sensores.terreno[15] == 'D') && !tengoZapatillas) ||
			((sensores.terreno[3] == 'G' || sensores.terreno[8] == 'G' || sensores.terreno[15] == 'G') && !bienSituado))
			{
				pendienteGiroDcha = true;
			}
		}
		//Mejora para que el personaje no se bloquee en pasajes estrechos girando todo el rato
		else if((ultimaAccion == actTURN_L || ultimaAccion == actTURN_R) && (sensores.terreno[2] == 'M' || sensores.terreno[2] == 'P')){
			accion = ultimaAccion;
		} 
		
		else if(!girarDerecha){
			accion = actTURN_L;
		} else {
			accion = actTURN_R;
		}
	}
	
	//Recordar última acción
	if(ultimaAccion == accion)
		repeticionesUltimaAccion++;
	else
		repeticionesUltimaAccion = 1;
		
	ultimaAccion = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
