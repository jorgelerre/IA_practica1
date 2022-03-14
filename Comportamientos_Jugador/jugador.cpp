#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

bool ComportamientoJugador::esTransitable(char tipoCasilla){
	if(tipoCasilla == 'M' || tipoCasilla == 'P')
		return false;
	if(!noHaySalida){
		if((tipoCasilla=='B' && !tengoZapatillas) || (tipoCasilla=='A' && !tengoBikini))
			return false;
	}
	return true;
}

Action ComportamientoJugador::girarADireccion(int sentidoActual, int nuevoSentido){
	Action accion;
	int aux = sentidoActual - nuevoSentido;
	if(aux < 0)
		aux += 4;
		
	switch(aux){
		case 0:	//No hace falta girar
			accion = actIDLE;
			break;
		case 1:	//Gira a la izquierda
			accion = actTURN_L;
			break;
		case 2:	//Da media vuelta
			accion = actTURN_L;			
			pendienteGiroIzda = true;
			break;
		case 3: //Gira a la derecha
			accion = actTURN_R;
			break;
	}
	
	return accion;
}

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
	cout << "Fila: " << fil << "\tColumna: " << col << endl;
	cout << "Orientacion: " << brujula << endl;
	cout << "Detectado muro a la derecha? : " << muroDcha << endl;
	cout << "Detectado muro a la izquierda? : " << muroIzda << endl;

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
		sinDescubrirNorte[0] = sinDescubrirEste[0]  = sinDescubrirSur[0]  = sinDescubrirOeste[0]  = false;
	}
	
	//Actualizamos los valores aleatorios
	girarDerecha  = (rand() % 2 == 0);
	girarDerecha2 = (rand() % 2 == 0);
	
	if(sensores.nivel <= 1){
		brujula = sensores.sentido;
	}
	if(sensores.nivel == 0){
		bienSituado  = true;
		fil = sensores.posF;
		col = sensores.posC;
	}
	
	
	switch (ultimaAccion){
		case actFORWARD:
			if(!sensores.colision && sensores.nivel > 0){
				switch (brujula){
					case 0: fil--; break;
					case 1: col++; break;
					case 2: fil++; break;
					case 3: col--; break;
				}
			}
			break;
		case actTURN_L:
			if(sensores.nivel > 1)
				brujula = (brujula+3)%4;
			break;
		case actTURN_R:
			if(sensores.nivel > 1)
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
	//Actualizacion de si se necesita recargar bateria
	recargarBateria = (sensores.bateria < 3000 && sensores.vida > 1500) || (sensores.bateria < 1500 && sensores.vida > 700) || (sensores.bateria < 500 && sensores.vida > 100);
	
	//Comprobación de sectores cercanos sin explorar
	if(bienSituado){
		for(int i = 0; i < 3; i++){
			int coef = 4*(i+1);
			if(fil>=coef && mapaResultado[fil-coef][col] == '?')
				sinDescubrirNorte[i]  = true;
			else
				sinDescubrirNorte[i]  = false;	
			if(col<tam_mapa-coef && mapaResultado[fil][col+coef] == '?' )
				sinDescubrirEste[i]  = true;
			else
				sinDescubrirEste[i]  = false;
			
			if(fil<tam_mapa-coef && mapaResultado[fil+coef][col] == '?')
				sinDescubrirSur[i]  = true;
			else
				sinDescubrirSur[i]  = false;
			
			if(col>=coef && mapaResultado[fil][col-coef] == '?' )
				sinDescubrirOeste[i]  = true;
			else
				sinDescubrirOeste[i]  = false;
		}
	}
	
	//Comprobación de muros/obstaculos en frente del jugador
	bloqueadoNorte--;
	bloqueadoEste--;
	bloqueadoSur--;
	bloqueadoOeste--;
	
	if(!esTransitable(sensores.terreno[1]) &&
	   (!esTransitable(sensores.terreno[2]) || !esTransitable(sensores.terreno[6])) &&
	   !esTransitable(sensores.terreno[3]) 
	  ){
	  	switch(brujula){
	  		case 0: 
		  		if(bloqueadoNorte > -20)
		  			bloqueadoNorte = tam_mapa/2;
		  		else
		  			bloqueadoNorte = 20;
		  		break;
			case 1: 
				if(bloqueadoEste > -20)
		  			bloqueadoEste = tam_mapa/2;
		  		else
		  			bloqueadoEste = 20;
		  		break;
			case 2:
				if(bloqueadoSur > -20)
			  		bloqueadoSur = tam_mapa/2;
			  	else
			  		bloqueadoSur = 20;
			  	break;
			case 3:
				if(bloqueadoOeste > -20)
			  		bloqueadoOeste = tam_mapa/2;
			  	else
			  		bloqueadoOeste = 20;
			  	break;
	  	}
	  }
	  
	  noHaySalida = (bloqueadoNorte > 0) && (bloqueadoEste > 0) && (bloqueadoSur > 0) && (bloqueadoOeste > 0);
	cout << "Sin descubrir Norte: \t0->" << sinDescubrirNorte[0] << "\t1-> " << sinDescubrirNorte[1]<< "\t2->" << sinDescubrirNorte[2]<< endl;
	cout << "Sin descubrir Este:  \t0->"  << sinDescubrirEste[0] << "\t1-> " << sinDescubrirEste[1]<< "\t2->" << sinDescubrirEste[2]  << endl;
	cout << "Sin descubrir Sur:   \t0->"   << sinDescubrirSur[0] << "\t1-> " << sinDescubrirSur[1]<< "\t2->" << sinDescubrirSur[2]   << endl;
	cout << "Sin descubrir Oeste: \t0->" << sinDescubrirOeste[0] << "\t1-> " << sinDescubrirOeste[1]<< "\t2->" << sinDescubrirOeste[2] << endl;
	cout << "Bloqueado Norte: " << bloqueadoNorte << endl;
	cout << "Bloqueado Este: "  << bloqueadoEste  << endl;
	cout << "Bloqueado Sur: "   << bloqueadoSur   << endl;
	cout << "Bloqueado Oeste: " << bloqueadoOeste << endl;
	
	
	//Definir la nueva acción
	cout << "Regla tomada: ";
	
	//Si el jugador está en agua/bosque sin tener bikini/zapatillas, avanza hasta salir de ahí
	if(((sensores.terreno[0]=='A' && !tengoBikini) || (sensores.terreno[0]=='B' && !tengoZapatillas)) 
	 && (sensores.terreno[2] != 'P' && sensores.terreno[2] != 'M')){
		accion = actFORWARD;
		cout << "Huyendo de agua/bosque" << endl;
	}
	//Si en el anterior movimiento decidimos hacer un giro a posteriori, se ejecuta ahora
	else if(pendienteGiroDcha){
		accion = actTURN_R;
		pendienteGiroDcha = false;
		cout << "Giro Pendiente Derecha" << endl;
	}
	else if (pendienteGiroIzda){
		accion = actTURN_L;
		pendienteGiroIzda = false;	
		cout << "Giro Pendiente Izquierda" << endl;
	}
	//Si se necesita recargar bateria y estas en un punto de recarga, quedate en el punto de recarga
	else if(sensores.terreno[0] == 'X' && recargarBateria){
		accion = actIDLE;
		cout << "Recargando..." << endl;
	}
	else{
		//Si el personaje está frente a un callejón sin salida, hacemos que gire
		if (!esTransitable(sensores.terreno[1])
			&& !esTransitable(sensores.terreno[3])
			&& !esTransitable(sensores.terreno[6])
			){
			cout << "Detectado callejon 136" << endl;
			if(ultimaAccion == actTURN_R && !muroIzda){
				accion = actTURN_L;
				cout << "Giro con continuidad" << endl;
			}
			else if (ultimaAccion == actTURN_L && !muroDcha){
				accion = actTURN_R;
				cout << "Giro con continuidad" << endl;
			}
			else if(!muroDcha && muroIzda){
				cout << "Giro a la derecha (muros)" << endl;
				accion = actTURN_R;
			}
			else if(muroDcha && !muroIzda){
			cout << "Giro a la izquierda (muros)" << endl;
				accion = actTURN_L;
			}
			else if (girarDerecha){
				accion = actTURN_R;
			}
			else{
				accion = actTURN_L;
			}
		}
		
		//Dirigete a donde haya zona sin descubrir
		for(int i = 0; i < 2 && accion == actIDLE; i++){
		//Si solo al norte hay una zona sin descubrir, ve hacia el norte el 50% de las veces
			if (bienSituado && !sinDescubrirOeste[i]  && sinDescubrirNorte[i]  && !sinDescubrirEste[i]  && !sinDescubrirSur[i]  && girarDerecha && bloqueadoNorte <= 0){
				accion = girarADireccion(sensores.sentido, 0);
				cout << "Yendo al Norte para descubrir mapa" << endl;
			}
			//Si solo al este hay una zona sin descubrir, ve hacia el este el 50% de las veces
			else if (bienSituado && sinDescubrirEste[i]  && !sinDescubrirNorte[i]  && !sinDescubrirSur[i]  && !sinDescubrirOeste[i]  && girarDerecha && bloqueadoEste <= 0){
				accion = girarADireccion(sensores.sentido, 1);
				cout << "Yendo al Este para descubrir mapa" << endl;
			}
			//Si solo al sur hay una zona sin descubrir, ve hacia el sur el 50% de las veces
			else if (bienSituado && !sinDescubrirOeste[i]  && !sinDescubrirNorte[i]  && !sinDescubrirEste[i]  && sinDescubrirSur[i]  && girarDerecha && bloqueadoSur <= 0){
				accion = girarADireccion(sensores.sentido, 2);
				cout << "Yendo al Sur para descubrir mapa" << endl;
			}
			//Si solo al oeste hay una zona sin descubrir, ve hacia al oeste el 50% de las veces
			else if (bienSituado && sinDescubrirOeste[i]  && !sinDescubrirNorte[i]  && !sinDescubrirEste[i]  && !sinDescubrirSur[i]  && girarDerecha && bloqueadoOeste <= 0){
				accion = girarADireccion(sensores.sentido, 3);
				cout << "Yendo al Oeste para descubrir mapa" << endl;
			}
			//Si hay al norte y al sur zona sin descubrir, ve hacia alguna de ellas al azar
			else if (bienSituado && !sinDescubrirOeste[i]  && sinDescubrirNorte[i]  && !sinDescubrirEste[i]  && sinDescubrirSur[i]  && girarDerecha && bloqueadoNorte <= 0 && bloqueadoSur <= 0){
				if(girarDerecha2){
					accion = girarADireccion(sensores.sentido, 0);
					cout << "Yendo al Norte para descubrir mapa (v. dos posibilidades)." << endl;
				}
				else{
					accion = girarADireccion(sensores.sentido, 2);
					cout << "Yendo al Sur para descubrir mapa (v. dos posibilidades)." << endl;
				}
			}
			//Si hay al este y al oeste zona sin descubrir, ve hacia alguna de ellas al azar
			else if (bienSituado && sinDescubrirOeste[i]  && !sinDescubrirNorte[i]  && sinDescubrirEste[i]  && !sinDescubrirSur[i]  && girarDerecha && bloqueadoEste <= 0 && bloqueadoOeste <= 0){
				if(girarDerecha2){
					accion = girarADireccion(sensores.sentido, 1);
					cout << "Yendo al Este para descubrir mapa (v. dos posibilidades)." << endl;
				}
				else{
					accion = girarADireccion(sensores.sentido, 3);
					cout << "Yendo al Oeste para descubrir mapa (v. dos posibilidades)." << endl;
				}
			}
			//girarDerecha  = (rand() % 2 == 0);
		}
		if (accion == actIDLE){
			if(esTransitable(sensores.terreno[2]) && (sensores.superficie[2]=='_')){
				accion = actFORWARD;
				cout << "Hacia delante" << endl;
				//Si se tiene un objeto a la izquierda, se girará al siguiente movimiento a la izquierda
				if(
				((sensores.terreno[1] == 'K' || sensores.terreno[4] == 'K' || sensores.terreno[9] == 'K') && !tengoBikini)     || 
				((sensores.terreno[1] == 'D' || sensores.terreno[4] == 'D' || sensores.terreno[9] == 'D') && !tengoZapatillas) ||
				((sensores.terreno[1] == 'G' || sensores.terreno[4] == 'G' || sensores.terreno[9] == 'G') && !bienSituado)
				){
					pendienteGiroIzda = true;
					cout << "Objeto visto a la izquierda" << endl;
				}
				//Si se tiene un objeto a la derecha, se girará al siguiente movimiento a la derecha
				else if (
				((sensores.terreno[3] == 'K' || sensores.terreno[8] == 'K' || sensores.terreno[15] == 'K') && !tengoBikini)     || 
				((sensores.terreno[3] == 'D' || sensores.terreno[8] == 'D' || sensores.terreno[15] == 'D') && !tengoZapatillas) ||
				((sensores.terreno[3] == 'G' || sensores.terreno[8] == 'G' || sensores.terreno[15] == 'G') && !bienSituado)
				)
				{
					pendienteGiroDcha = true;
					cout << "Objeto visto a la derecha" << endl;
				}
				//Si hay una zona de recarga a la izquierda, ve hacia ella
				else if ((sensores.terreno[1] == 'X' || sensores.terreno[4] == 'X' || sensores.terreno[7] == 'X') && recargarBateria){
						pendienteGiroIzda = true;
						cout << "Zona de recarga vista a la izquierda" << endl;
				}
				//Si hay una zona de recarga a la derecha, ve hacia ella
				else if ((sensores.terreno[3] == 'X' || sensores.terreno[8] == 'X' || sensores.terreno[15] == 'X') && recargarBateria){
						pendienteGiroDcha = true;
						cout << "Zona de recarga vista a la derecha" << endl;
				}
				//Si está al lado de un muro/precipicio y encuentra una "puerta" a la izquierda, la tomará
				else if (esTransitable(sensores.terreno[1]) && !esTransitable(sensores.terreno[5]) && muroIzda){
					pendienteGiroIzda = true;
					cout << "Puerta a la izquierda" << endl;	
				}
				//Si está al lado de un muro/precipicio y encuentra una "puerta" a la derecha, la tomará
				else if (esTransitable(sensores.terreno[3]) && !esTransitable(sensores.terreno[7]) && muroDcha){
					pendienteGiroDcha = true;
					cout << "Puerta a la derecha" << endl;
				}	
			}
			//Mejora para que el personaje no se bloquee en pasajes estrechos girando todo el rato
			else if((ultimaAccion == actTURN_L || ultimaAccion == actTURN_R) && (!esTransitable(sensores.terreno[2]))){
				cout << "Giro igual al anterior para salir de sitio estrecho" << endl;
				accion = ultimaAccion;
			}
			else if(!muroDcha && muroIzda){
				cout << "Giro a la derecha (muros)" << endl;
				accion = actTURN_R;
			}
			else if(muroDcha && !muroIzda){
			cout << "Giro a la izquierda (muros)" << endl;
				accion = actTURN_L;
			}
			else{
				switch(brujula % 2){
					case 0:
						if(sinDescubrirOeste[0]){
							accion = girarADireccion(sensores.sentido, 3);
							cout << "Giro al oeste (final)" << endl;
						}
						else if(sinDescubrirEste[0]){
							accion = girarADireccion(sensores.sentido, 1);
							cout << "Giro al este (final)" << endl;
						}
						break;
					case 1:
						if(sinDescubrirNorte[0] ){
							accion = girarADireccion(sensores.sentido, 0);
							cout << "Giro al norte (final)" << endl;
						}
						else if(sinDescubrirSur[0] ){
							accion = girarADireccion(sensores.sentido, 2);
							cout << "Giro al sur (final)" << endl;
						}
						break;
				}
			}
		}
		//Si despues de aplicar todas las normas no se ha aplicado ninguna accion
		//Se elige al azar girar a la izquierda o a la derecha
		if(accion == actIDLE){
			if(girarDerecha){
				accion = actTURN_R;
				cout << "Giro a la derecha (final)" << endl;
			} else {
				accion = actTURN_L;
				cout << "Giro a la izquierda (final)" << endl;
			}	
		}
	}
	
	//Regulacion sobre excesivas repeticiones de una misma accion
	if(ultimaAccion == accion && repeticionesUltimaAccion >= tam_mapa/2 && !sinDescubrirOeste[0] && !sinDescubrirNorte[0] && !sinDescubrirEste[0] && !sinDescubrirSur[0] && girarDerecha){
		cout << "Giro por repeticion de actFORWARD > tam_mapa/2" << endl;
		if(ultimaAccion == actFORWARD && girarDerecha2)
			accion = actTURN_R;
		else
			accion = actTURN_L;
	}
	
	//Recordar muros cercanos
	if(accion == actFORWARD){
			muroIzda = !esTransitable(sensores.terreno[1]);
			muroDcha = !esTransitable(sensores.terreno[3]);
	}else if(accion == actTURN_L){
			muroDcha = !esTransitable(sensores.terreno[2]);
			if(bienSituado){
				switch(brujula){
					case 0:	muroIzda = !esTransitable(mapaResultado[fil+1][col]); break;
					case 1:	muroIzda = !esTransitable(mapaResultado[fil][col-1]); break;
					case 2:	muroIzda = !esTransitable(mapaResultado[fil-1][col]); break;
					case 3:	muroIzda = !esTransitable(mapaResultado[fil][col+1]); break;
				}
			}
			else{
				muroIzda = false;
			}
	}else if(accion == actTURN_R){
		if(bienSituado){
				switch(brujula){
					case 0:	muroDcha = !esTransitable(mapaResultado[fil+1][col]); break;
					case 1:	muroDcha = !esTransitable(mapaResultado[fil][col-1]); break;
					case 2:	muroDcha = !esTransitable(mapaResultado[fil-1][col]); break;
					case 3:	muroDcha = !esTransitable(mapaResultado[fil][col+1]); break;
				}
			}
			else{
				muroDcha = false;
			}
		muroIzda = !esTransitable(sensores.terreno[2]);
	}
	//Recordar última acción
	if(ultimaAccion == accion)
		repeticionesUltimaAccion++;
	else
		repeticionesUltimaAccion = 1;
	
	ultimaAccion = accion;
	
	cout << endl << endl;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

// ./practica1SG mapa semilla nivel fila columna sentido
// ./practica1SG ./mapas/vertigo.map 0 0 78 79 3
// ./practica1SG ./mapas/mapa100.map 0 nivel 78 79 0
// ./practica1SG ./mapas/mapa75.map 0 nivel 14 37 0
// ./practica1SG ./mapas/mapa50.map 0 nivel 20 20 0
// ./practica1SG ./mapas/mapa30.map 0 nivel 15 20 0
