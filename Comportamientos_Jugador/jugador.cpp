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

Action ComportamientoJugador::girarADireccion(int nuevoSentido){
	Action accion;
	int aux = brujula - nuevoSentido;
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

void ComportamientoJugador::registrarMapa(Sensores sensores, vector<vector<unsigned char>> & mapa){
		mapa[fil][col] = sensores.terreno[0];
		int coef = 1;
		if(brujula == NORTE || brujula == OESTE)
			coef = -1;
		int k = 0;
	
		if(brujula == NORTE || brujula == SUR){
			for(int x = 0; x < 4; x++){
				int aux_1 = fil+x*coef;
				for(int y = -x; y <= x; y++){
					int aux_2 = col-y*coef;
					mapa[aux_1][aux_2] = sensores.terreno[k];
					k++;
				}
			}	
		}
		else{
			for(int x = 0; x < 4; x++){
				int aux_1 = col+x*coef;
				for(int y = -x; y <= x; y++){
					int aux_2 = fil+y*coef;
					mapa[aux_2][aux_1] = sensores.terreno[k];
					k++;
				}
			}
		}
}
Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	// Determinar el efecto de la ultima accion enviada
	
	//Si se produce un reset, damos a todas las variables de estado su valor inicial
	if(sensores.reset){
		fil = col = tam_mapa;
		brujula = NORTE;
		random1 = random2 = false;
		if(sensores.nivel != 0){
			bienSituado  = false;
		}
		tengoBikini = tengoZapatillas = false;
		ultimaAccion = actIDLE;
		pendienteGiroIzda = pendienteGiroDcha = false;
		for(int i = 0; i < 4; i++)
      	bloqueadoDireccion[i] = 0;
	}
	
	//Actualizamos los valores aleatorios
	random1 = (rand() % 2 == 0);
	random2 = (rand() % 2 == 0);
	
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
					case NORTE: fil--; break;
					case ESTE:  col++; break;
					case SUR:   fil++; break;
					case OESTE: col--; break;
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
	
	//Si se pasa por una casilla de localización por primera vez, se guarda la posicion y orientación 
	//del jugador y se actualiza mapaResultado con los datos de mapaPrelocalización
	if(sensores.terreno[0] == 'G' && !bienSituado){
		//cout << "Relocalizacion" << endl;
		int dif_fil = fil - sensores.posF;
		int dif_col = col - sensores.posC;
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
		bienSituado = true;
		for(int i = 0; i < tam_mapa; i++){
			for(int j = 0; j < tam_mapa; j++){
				unsigned char valor_actual = mapaPreLocalizacion[i+dif_fil][j+dif_col];
				if( valor_actual != '?'){
					mapaResultado[i][j] = valor_actual;					//Pasamos la info de preloc. al mapaResultado
					mapaPreLocalizacion[i+dif_fil][j+dif_col] = '?';	//Dejamos el mapaPrelocalizacion limpio
				}
			}
		}
	}
	
	if(sensores.terreno[0] == 'K')
		tengoBikini = true;
	
	if(sensores.terreno[0] == 'D')
		tengoZapatillas = true;
		
	//Actualización del mapa (vision)
	if(bienSituado){
		registrarMapa(sensores, mapaResultado);
	}
	else{
		registrarMapa(sensores, mapaPreLocalizacion);
	}
	//Actualizacion de si se necesita recargar bateria
	recargarBateria = (sensores.bateria < 4500 && sensores.vida > 1500) ||
	                  (sensores.bateria < 1500 && sensores.vida > 1000) ||
	                  (sensores.bateria < 500 && sensores.vida > 200);
	
	//Comprobación de sectores cercanos sin explorar
	vector<vector<unsigned char>> *mapaActual = &mapaResultado;
	if(!bienSituado)
		mapaActual = &mapaPreLocalizacion;
	for(int i = 0; i < 2; i++){
		int coef = 4*(i+1);
		if(fil>=coef && (*mapaActual)[fil-coef][col] == '?')
			sinDescubrir[NORTE][i]  = true;
		else
			sinDescubrir[NORTE][i]  = false;	
		if(col<mapaActual->size()-coef && (*mapaActual)[fil][col+coef] == '?' )
			sinDescubrir[ESTE][i]  = true;
		else
			sinDescubrir[ESTE][i]  = false;
		
		if(fil<mapaActual->size()-coef && (*mapaActual)[fil+coef][col] == '?')
			sinDescubrir[SUR][i]  = true;
		else
			sinDescubrir[SUR][i]  = false;
		
		if(col>=coef && (*mapaActual)[fil][col-coef] == '?' )
			sinDescubrir[OESTE][i]  = true;
		else
			sinDescubrir[OESTE][i]  = false;
	}
	
	
	//Comprobación de muros/obstaculos en frente del jugador
	for(int i = 0; i < 4; i++)
		bloqueadoDireccion[i]--;
	
	if(muroDcha && bloqueadoDireccion[(brujula+1)%4] <= 0){
		bloqueadoDireccion[(brujula+1)%4]+=2;
	}
	if(muroIzda && bloqueadoDireccion[(brujula+3)%4] <= 0){
		bloqueadoDireccion[(brujula+3)%4]+=2;
	}
	if((!esTransitable(sensores.terreno[1])) &&
	   (!esTransitable(sensores.terreno[2]) || !esTransitable(sensores.terreno[6])) &&
	   (!esTransitable(sensores.terreno[3]))
	){
		if(bloqueadoDireccion[brujula] > -20)
			bloqueadoDireccion[brujula] = tam_mapa/2;
		else
			bloqueadoDireccion[brujula] = 20;
	  }
	  
    noHaySalida = (bloqueadoDireccion[NORTE] > 0) && (bloqueadoDireccion[ESTE] > 0) && (bloqueadoDireccion[SUR] > 0) && (bloqueadoDireccion[OESTE] > 0);
	if(muroDcha)
		contador_muroDcha++;
	else
		contador_muroDcha = 0;
	if(muroIzda)
		contador_muroIzda++;
	else	
		contador_muroIzda = 0;

	contadorDescubrir--;
	//Actualizamos la variable contadorObjeto
	contadorObjeto--;
	if (
		((sensores.terreno[1] == 'K' || sensores.terreno[4] == 'K' || sensores.terreno[9] == 'K') && !tengoBikini)     || 
		((sensores.terreno[1] == 'D' || sensores.terreno[4] == 'D' || sensores.terreno[9] == 'D') && !tengoZapatillas) ||
		((sensores.terreno[1] == 'G' || sensores.terreno[4] == 'G' || sensores.terreno[9] == 'G') && !bienSituado)
		)
			contadorObjeto = 2;
	else if (
		((sensores.terreno[3] == 'K' || sensores.terreno[8] == 'K' || sensores.terreno[15] == 'K') && !tengoBikini)     || 
		((sensores.terreno[3] == 'D' || sensores.terreno[8] == 'D' || sensores.terreno[15] == 'D') && !tengoZapatillas) ||
		((sensores.terreno[3] == 'G' || sensores.terreno[8] == 'G' || sensores.terreno[15] == 'G') && !bienSituado)
		)
			contadorObjeto = 2;
	else if ((sensores.terreno[1] == 'X' || sensores.terreno[4] == 'X' || sensores.terreno[9] == 'X' ||
			  sensores.terreno[3] == 'X' || sensores.terreno[8] == 'X' || sensores.terreno[15]== 'X') 
			  && recargarBateria)
			contadorObjeto = 2;
		
	
	//Salida por pantalla de algunas variables de estado
	//Usado para depurar el programa y buscar soluciones e ideas
	/*
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case NORTE: cout << "Norte" << endl; break;
		case ESTE:  cout << "Este" << endl; break;
		case SUR:   cout << "Sur " << endl; break;
		case OESTE: cout << "Oeste" << endl; break;
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
	cout << "Sin descubrir Norte: \t0->" << sinDescubrir[NORTE][0] << "\t1-> " << sinDescubrir[NORTE][1]<< "\t2->" << sinDescubrir[NORTE][2] << endl;
	cout << "Sin descubrir Este:  \t0->"  << sinDescubrir[ESTE][0] << "\t1-> " << sinDescubrir[ESTE][1]<< "\t2->" << sinDescubrir[ESTE][2]  << endl;
	cout << "Sin descubrir Sur:   \t0->"   << sinDescubrir[SUR][0] << "\t1-> " << sinDescubrir[SUR][1]<< "\t2->" << sinDescubrir[SUR][2]   << endl;
	cout << "Sin descubrir Oeste: \t0->" << sinDescubrir[OESTE][0] << "\t1-> " << sinDescubrir[OESTE][1]<< "\t2->" << sinDescubrir[OESTE][2] << endl;
	
	cout << "Bloqueado Norte: " << bloqueadoDireccion[NORTE] << endl;
	cout << "Bloqueado Este: "  << bloqueadoDireccion[ESTE]  << endl;
	cout << "Bloqueado Sur: "   << bloqueadoDireccion[SUR]   << endl;
	cout << "Bloqueado Oeste: " << bloqueadoDireccion[OESTE] << endl;
	cout << "Contador Objeto: " << contadorObjeto << endl;
	
	//Definir la nueva acción
	cout << "Regla tomada: ";
	*/
	//Si el jugador está en agua/bosque sin tener bikini/zapatillas, avanza hasta salir de ahí
	if(((sensores.terreno[0]=='A' && !tengoBikini) || (sensores.terreno[0]=='B' && !tengoZapatillas)) 
	 && (sensores.terreno[2] != 'P' && sensores.terreno[2] != 'M')){
		accion = actFORWARD;
		//cout << "Huyendo de agua/bosque" << endl;
	}
	//Si en el anterior movimiento decidimos hacer un giro a posteriori, se ejecuta ahora
	else if(pendienteGiroDcha){
		accion = actTURN_R;
		pendienteGiroDcha = false;
		//cout << "Giro Pendiente Derecha" << endl;
	}
	else if (pendienteGiroIzda){
		accion = actTURN_L;
		pendienteGiroIzda = false;	
		//cout << "Giro Pendiente Izquierda" << endl;
	}
	//Si se necesita recargar bateria y estas en un punto de recarga, quedate en el punto de recarga
	else if(sensores.terreno[0] == 'X' && recargarBateria){
		accion = actIDLE;
		//cout << "Recargando..." << endl;
	}
	else{
		//Si el personaje está frente a un callejón sin salida, hacemos que gire
		if (!esTransitable(sensores.terreno[1])
			&& !esTransitable(sensores.terreno[3])
			&& !esTransitable(sensores.terreno[6])
			){
			//cout << "Detectado callejon 136" << endl;
			if(ultimaAccion == actTURN_R && !muroIzda && bloqueadoDireccion[(brujula+3)%4] < 0){
				accion = actTURN_L;
				//cout << "Giro con continuidad" << endl;
			}
			else if (ultimaAccion == actTURN_L && !muroDcha && bloqueadoDireccion[(brujula+1)%4] < 0){
				accion = actTURN_R;
				//cout << "Giro con continuidad" << endl;
			}
			else if(!muroDcha && muroIzda){
				//cout << "Giro a la derecha (muros)" << endl;
				accion = actTURN_R;
			}
			else if(muroDcha && !muroIzda){
			//cout << "Giro a la izquierda (muros)" << endl;
				accion = actTURN_L;
			}
			else if (random1){
				accion = actTURN_R;
			}
			else{
				accion = actTURN_L;
			}
		}
		
		//Dirigete a donde haya zona sin descubrir el 50% de las veces en el caso de no estar yendo a por un objeto
		if(random1 && contadorObjeto<0 && contadorDescubrir<0){
			bool salirBucle = false;
			for(int i = 0; i < 2 && !salirBucle; i++){
				salirBucle = sinDescubrir[OESTE][i] || sinDescubrir[NORTE][i] || sinDescubrir[ESTE][i]  || sinDescubrir[SUR][i];
			//Si solo al norte hay una zona sin descubrir, ve hacia el norte
				if (!sinDescubrir[OESTE][i]  && sinDescubrir[NORTE][i]  && !sinDescubrir[ESTE][i]  && !sinDescubrir[SUR][i]  && bloqueadoDireccion[NORTE] <= 0){
					accion = girarADireccion(NORTE);
					//cout << "Yendo al Norte para descubrir mapa" << endl;
				}
				//Si solo al este hay una zona sin descubrir, ve hacia el este
				else if (sinDescubrir[ESTE][i]  && !sinDescubrir[NORTE][i]  && !sinDescubrir[SUR][i]  && !sinDescubrir[OESTE][i] && bloqueadoDireccion[ESTE] <= 0){
					accion = girarADireccion(ESTE);
					//cout << "Yendo al Este para descubrir mapa" << endl;
				}
				//Si solo al sur hay una zona sin descubrir, ve hacia el sur
				else if (!sinDescubrir[OESTE][i]  && !sinDescubrir[NORTE][i]  && !sinDescubrir[ESTE][i]  && sinDescubrir[SUR][i] && bloqueadoDireccion[SUR] <= 0){
					accion = girarADireccion(SUR);
					//cout << "Yendo al Sur para descubrir mapa" << endl;
				}
				//Si solo al oeste hay una zona sin descubrir, ve hacia al oeste
				else if (sinDescubrir[OESTE][i]  && !sinDescubrir[NORTE][i]  && !sinDescubrir[ESTE][i]  && !sinDescubrir[SUR][i] && bloqueadoDireccion[OESTE] <= 0){
					accion = girarADireccion(OESTE);
					//cout << "Yendo al Oeste para descubrir mapa" << endl;
				}
				//Si hay al norte y al sur zona sin descubrir, ve hacia alguna de ellas al azar
				else if (sinDescubrir[NORTE][i] && sinDescubrir[SUR][i]
				&& !sinDescubrir[ESTE][i] && !sinDescubrir[OESTE][i] &&
				(bloqueadoDireccion[NORTE] <= 0 || bloqueadoDireccion[SUR] <= 0)){
					if(bloqueadoDireccion[NORTE] <= 0 && bloqueadoDireccion[SUR] <= 0){
						if(random2){
							accion = girarADireccion(NORTE);
							//cout << "Yendo al Norte para descubrir mapa (v. dos posibilidades)." << endl;
						}
						else{
							accion = girarADireccion(SUR);
							//cout << "Yendo al Sur para descubrir mapa (v. dos posibilidades)." << endl;
						}
					}
					else if(bloqueadoDireccion[NORTE] <= 0 && bloqueadoDireccion[SUR] > 0){
						accion = girarADireccion(NORTE);
						//cout << "Yendo al Norte para descubrir mapa (sur bloqueado)." << endl;
					}
					else{
						accion = girarADireccion(SUR);
						//cout << "Yendo al Sur para descubrir mapa (norte bloqueado)." << endl;

					}
				}
				//Si hay al este y al oeste zona sin descubrir, ve hacia alguna de ellas al azar
				else if (sinDescubrir[OESTE][i] && sinDescubrir[ESTE][i] && 
				!sinDescubrir[NORTE][i] && !sinDescubrir[SUR][i] &&
				(bloqueadoDireccion[ESTE] <= 0 || bloqueadoDireccion[OESTE] <= 0)){
					if(bloqueadoDireccion[ESTE] <= 0 && bloqueadoDireccion[OESTE] <= 0){
						if(random2){
							accion = girarADireccion(ESTE);
							//cout << "Yendo al Este para descubrir mapa (v. dos posibilidades)." << endl;
						}
						else{
							accion = girarADireccion(OESTE);
							//cout << "Yendo al Oeste para descubrir mapa (v. dos posibilidades)." << endl;
						}
					}
					else if(bloqueadoDireccion[ESTE] <= 0 && bloqueadoDireccion[OESTE] > 0){
						accion = girarADireccion(ESTE);
						//cout << "Yendo al Este para descubrir mapa (oeste bloqueado)." << endl;
					}
					else{
						accion = girarADireccion(OESTE);
						//cout << "Yendo al Oeste para descubrir mapa (este bloqueado)." << endl;
					}
				}
			}
			//Si se ha hecho un giro para descubrir mapa
			if(accion != actIDLE){
				contadorDescubrir = 1;	//Actualizamos contador
			}
		}
		if (accion == actIDLE){
			if(esTransitable(sensores.terreno[2]) && (sensores.superficie[2]=='_')){
				accion = actFORWARD;
				//cout << "Hacia delante" << endl;
				//Si se tiene un objeto a la izquierda, se girará al siguiente movimiento a la izquierda
				if(
				((sensores.terreno[1] == 'K' || sensores.terreno[4] == 'K' || sensores.terreno[9] == 'K') && !tengoBikini)     || 
				((sensores.terreno[1] == 'D' || sensores.terreno[4] == 'D' || sensores.terreno[9] == 'D') && !tengoZapatillas) ||
				((sensores.terreno[1] == 'G' || sensores.terreno[4] == 'G' || sensores.terreno[9] == 'G') && !bienSituado)
				){
					pendienteGiroIzda = true;
					contadorObjeto=2;
					//cout << "Objeto visto a la izquierda" << endl;
				}
				//Si se tiene un objeto a la derecha, se girará al siguiente movimiento a la derecha
				else if (
				((sensores.terreno[3] == 'K' || sensores.terreno[8] == 'K' || sensores.terreno[15] == 'K') && !tengoBikini)     || 
				((sensores.terreno[3] == 'D' || sensores.terreno[8] == 'D' || sensores.terreno[15] == 'D') && !tengoZapatillas) ||
				((sensores.terreno[3] == 'G' || sensores.terreno[8] == 'G' || sensores.terreno[15] == 'G') && !bienSituado)
				)
				{
					pendienteGiroDcha = true;
					contadorObjeto=2;
					//cout << "Objeto visto a la derecha" << endl;
				}
				//Si ves una zona de recarga y la necesitas, ve hacia ella
					//Si hay una zona de recarga a la izquierda, ve hacia ella
				else if ((sensores.terreno[1] == 'X' || sensores.terreno[4] == 'X' || sensores.terreno[9] == 'X') && recargarBateria){
							pendienteGiroIzda = true;
							//cout << "Zona de recarga vista a la izquierda" << endl;
							contadorObjeto=2;
					}
					//Si hay una zona de recarga a la derecha, ve hacia ella
					else if ((sensores.terreno[3] == 'X' || sensores.terreno[8] == 'X' || sensores.terreno[15] == 'X') && recargarBateria){
							pendienteGiroDcha = true;
							//cout << "Zona de recarga vista a la derecha" << endl;
							contadorObjeto=2;
					}
				
				//Si está al lado de un muro/precipicio y encuentra una "puerta" a la izquierda, la tomará
				else if (esTransitable(sensores.terreno[1]) && !esTransitable(sensores.terreno[5]) && muroIzda){
					pendienteGiroIzda = true;
					//cout << "Puerta a la izquierda" << endl;	
				}
				//Si está al lado de un muro/precipicio y encuentra una "puerta" a la derecha, la tomará
				else if (esTransitable(sensores.terreno[3]) && !esTransitable(sensores.terreno[7]) && muroDcha){
					pendienteGiroDcha = true;
					//cout << "Puerta a la derecha" << endl;
				}
				//Si lleva mucho tiempo con un muro a un lado, cuando deje de haber muro giramos a esa direccion (caso general puertas)
				else if(contador_muroDcha > 5 && esTransitable(sensores.terreno[3])){
					pendienteGiroDcha = true;
					//cout << "Puerta grande a la derecha" << endl;
				}
				else if(contador_muroIzda > 5 && esTransitable(sensores.terreno[1])){
					pendienteGiroIzda = true;
					//cout << "Puerta grande a la izquierda" << endl;
				}
			}
			//Mejora para que el personaje no se bloquee en pasajes estrechos girando todo el rato
			else if((ultimaAccion == actTURN_L || ultimaAccion == actTURN_R) && (!esTransitable(sensores.terreno[2])) && (random1 || random2)){
				//cout << "Giro igual al anterior para salir de sitio estrecho" << endl;
				accion = ultimaAccion;
			}
			else if(!muroDcha && muroIzda){
				//cout << "Giro a la derecha (muros)" << endl;
				accion = actTURN_R;
			}
			else if(muroDcha && !muroIzda){
			//cout << "Giro a la izquierda (muros)" << endl;
				accion = actTURN_L;
			}
			else{
				switch(brujula % 2){
					case 0:
						if(sinDescubrir[OESTE][0]){
							accion = girarADireccion(OESTE);
							//cout << "Giro al oeste (final)" << endl;
						}
						else if(sinDescubrir[ESTE][0]){
							accion = girarADireccion(ESTE);
							//cout << "Giro al este (final)" << endl;
						}
						break;
					case 1:
						if(sinDescubrir[NORTE][0]){
							accion = girarADireccion(NORTE);
							//cout << "Giro al norte (final)" << endl;
						}
						else if(sinDescubrir[SUR][0] ){
							accion = girarADireccion(SUR);
							//cout << "Giro al sur (final)" << endl;
						}
						break;
				}
			}
		}
		//Si despues de aplicar todas las normas no se ha aplicado ninguna accion
		//Se elige al azar girar a la izquierda o a la derecha
		if(accion == actIDLE){
			if(random1){
				accion = actTURN_R;
				//cout << "Giro a la derecha (final)" << endl;
			} else {
				accion = actTURN_L;
				//cout << "Giro a la izquierda (final)" << endl;
			}	
		}
	}
	
	//Regulacion sobre excesivas repeticiones de una misma accion
	if(ultimaAccion == accion && repeticionesUltimaAccion >= tam_mapa/2 && !sinDescubrir[OESTE][0] && !sinDescubrir[NORTE][0] && !sinDescubrir[ESTE][0] && !sinDescubrir[SUR][0] && random1){
		//cout << "Giro por repeticion de actFORWARD > tam_mapa/2" << endl;
		if(ultimaAccion == actFORWARD && random2)
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
					case NORTE:	muroIzda = !esTransitable((*mapaActual)[fil+1][col]); break;
					case ESTE :	muroIzda = !esTransitable((*mapaActual)[fil][col-1]); break;
					case SUR  :	muroIzda = !esTransitable((*mapaActual)[fil-1][col]); break;
					case OESTE:	muroIzda = !esTransitable((*mapaActual)[fil][col+1]); break;
				}
			}
			else{
				muroIzda = false;
			}
	}else if(accion == actTURN_R){
		if(bienSituado){
				switch(brujula){
					case NORTE:	muroDcha = !esTransitable((*mapaActual)[fil+1][col]); break;
					case ESTE:	muroDcha = !esTransitable((*mapaActual)[fil][col-1]); break;
					case SUR:  	muroDcha = !esTransitable((*mapaActual)[fil-1][col]); break;
					case OESTE:	muroDcha = !esTransitable((*mapaActual)[fil][col+1]); break;
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
	
	//cout << endl << endl;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
