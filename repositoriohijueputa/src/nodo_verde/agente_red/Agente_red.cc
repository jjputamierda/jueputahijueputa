#include "Agente_red.h"

struct datosForwarding{
	uint16_t idNodo;
	uint16_t distancia;
	uint16_t camino;
};
//uint16_t nodoSender;
uint16_t nodoSenderTWH;
std::vector<struct datosForwarding> tablaForwarding;

void capaRed(Cola<struct Mensaje>* colaAzul,
	Cola<struct ArbolGenerador>* colaRosada,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosMensaje>* colaDespachadorAzul,
	Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
	Cola<struct CapaRed>* colaDespachadorVerde,
	std::vector<int>* nodosIDs, Cola<std::string>* despachadorMiembros,
	Cola<std::string>* colaAlcanzabilidad,
	std::vector<datosNodo>* tablaVecinos,char* IP,
	Cola<std::string>* colaTablaForwarding){

	Cola<struct DatosForwarding> colaForwarding;
	Cola<struct Broadcast> colaBroadcast;

	(*nodosIDs)[0] += 1;
	/*
	std::thread hiloAzul(despachadorAzul,
		colaDespachadorAzul, &colaForwarding, &colaBroadcast, 
		(*nodosIDs)[0]);
	*/
	std::thread hiloRosado(despachadorRosado,colaDespachadorRosado,
		nodosIDs, tablaVecinos,
		colasVerdes);

	std::thread hiloVerde(despachadorVerde,
	 	colaDespachadorVerde, &colaForwarding, &colaBroadcast,colaRosada);
	/*
	std::thread hiloForwarding(forwarding, colaAzul, colaRosada,
		colasVerdes, &colaForwarding, nodosIDs);
	*/
	/*std::thread hiloBroadcast(broadcast, colaAzul, colaRosada,
		colasVerdes, &colaBroadcast, nodosIDs,despachadorMiembros,
		colaAlcanzabilidad);

	std::thread hiloAlcanzabilidad(enviarAlcanzabilidad,
	colaAlcanzabilidad,tablaVecinos,colaDespachadorRosado,
	colaRosada,IP);

	std::thread hiloTablaForwarding(verificarTablaForwarding,
	colaTablaForwarding, nodosIDs);*/

	//hiloAzul.join();
	hiloRosado.join();
	hiloVerde.join();
	//hiloForwarding.join();
	//hiloBroadcast.join();
}

/*void verificarTablaForwarding(Cola<std::string>* colaTablaForwarding,
std::vector<int>* nodosIDs){

	std::string recibirTabla;
	std::string aux;

	while(1){
		recibirTabla=colaTablaForwarding->pop();

		std::stringstream s_stream(recibirTabla);
		std::vector<std::string> resultado;
		while(s_stream.good()) {
		std::string substr;
		getline(s_stream, substr, ',');
		resultado.push_back(substr);
		}
		for(size_t indice=1;indice<resultado.size();indice++){
			struct datosForwarding nuevoDato;
			aux=resultado[indice];
			std::stringstream s_stream2(resultado[indice]);

			std::string substr2;
			getline(s_stream2, substr2, '-');
			for(size_t indice2=0;indice2<(*nodosIDs).size();indice2++){
				if((*nodosIDs)[indice2]==std::stoi(substr2)){
					nuevoDato.camino=std::stoi(substr2);
				}
			}

			getline(s_stream2, substr2, '-');
			nuevoDato.distancia=std::stoi(substr2);

			getline(s_stream2, substr2, '-');
			nuevoDato.idNodo=std::stoi(substr2);
			tablaForwarding.push_back(nuevoDato);
		}
	}
}*/

/*void enviarAlcanzabilidad(Cola<std::string>* colaAlcanzabilidad,
std::vector<datosNodo>* tablaVecinos,
Cola<struct capaSuperior>* colaDespachadorRosado,
Cola<struct capaAplicacion>* colaRosada,char* IP){

	struct capaSuperior enviar;
	struct capaAplicacion enviarRosado;
	std::string aux;
	std::string mensajeAlcanzabilidad;

	size_t indice;
	char host[256];
	struct capaSuperior enrutamiento;
	struct estructuraRed cabeceraRed;

	while(1){
		mensajeAlcanzabilidad="";

		bzero(host,sizeof(host));
		mensajeAlcanzabilidad += "90,";
		mensajeAlcanzabilidad += std::to_string((*tablaVecinos)[0].ID);
		mensajeAlcanzabilidad += "," + 
		std::to_string((tablaVecinos->size()-1));

		for(indice=1;indice<tablaVecinos->size();indice++){
			mensajeAlcanzabilidad += "," +
			std::to_string((*tablaVecinos)[indice].ID);

			if(((*tablaVecinos)[indice].IP).compare(IP)==0){
				mensajeAlcanzabilidad+=",1";
			}else{
				mensajeAlcanzabilidad+=",2";
			}
		}
		aux = mensajeAlcanzabilidad;
		strcpy(enviarRosado.cabeceraAplicacion.buffer,aux.c_str());
		enviarRosado.idFuente=(*tablaVecinos)[0].ID;
		colaRosada->push(enviarRosado);

		std::string estadoArbol=colaAlcanzabilidad->pop();
		if(estadoArbol=="45"){

			enviar.idDestino=0;
			enviar.destinoRed=0;
	    	aux=mensajeAlcanzabilidad;
	    	strcpy(enviar.cabeceraAplicacion.buffer,aux.c_str());

			colaDespachadorRosado->push(enviar);
		}
	}
}*/
/*
void despachadorAzul(Cola<struct DatosMensaje>* colaDespachadorAzul,
		Cola<struct DatosForwarding>* colaForwarding,
		Cola<struct Broadcast>* colaBroadcast, int idPropio){

	while(1){
		struct DatosMensaje datos = colaDespachadorAzul->pop();
		struct Mensaje mensaje;
		mensaje.mensaje.assign(datos.mensaje);
		mensaje.idFuente = idPropio; 

		if(datos.tipo == FORWARDING){
			struct DatosForwarding forwarding;
			forwarding.idFuenteInicial = idPropio;
			forwarding.idDestino = datos.idDestino;
			forwarding.longitud = datos.mensaje.size() + 2; // 2 bytes
			//Cambiar nombre
			forwarding.payload = mensaje;
			fowarding.tipo = 


			colaForwarding->push(forwarding);

		} else if(datos.tipo == BROADCAST){
			struct BroadcastA broadcast;
			broadcast.tipo = MENSAJE;
			broadcast.idFuente = idPropio;
			broadcast.longitud = datos.mensaje.size() + 2; // 2 bytes
			broadcast.payload = mensaje;

			//colaBroadcast->push(broadcast);

		} else {
			std::cout << "Error!\n";
		}
	}
}
*/
void despachadorRosado(Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
std::vector<int>* nodosIDs,std::vector<datosNodo>* tablaVecinos,std::vector<Cola<struct CapaEnlace>>* colasVerdes){

	while(1){
		struct DatosArbolGenerador datos = colaDespachadorRosado->pop();

		size_t longitud = nodosIDs->size();

            for(size_t i = 0; i < longitud; ++i){

                if(datos.id_destino_final == (*nodosIDs)[i]){

                    struct ArbolGenerador paqueteAg;

                    paqueteAg.tipo = datos.tipo;

					paqueteAg.SN = datos.SN;
					paqueteAg.RN =  datos.RN;

					struct CapaRed capaRed;
					capaRed.tipo = ARBOL;

					capaRed.payload->payloadArbol = &(paqueteAg);
					capaRed.longitud =  sizeof(paqueteAg);
					struct CapaEnlace paquete;



					paquete.tipo = RED;
					paquete.idDestinoFinal = datos.id_destino_final;
					paquete.idFuenteInmediato = (*nodosIDs)[0];
					paquete.longitud = sizeof(capaRed);
					paquete.payload = &capaRed;


                    
					
	

                    colasVerdes[0][i].push(paquete);

                
				}

            }

		
		

	}
}

void despachadorVerde(Cola<struct CapaRed>* colaDespachadorVerde,
	Cola<struct DatosForwarding>* colaForwarding,
	Cola<struct Broadcast>* colaBroadcast,Cola<struct ArbolGenerador>* colaRosada){
	while(1){
		struct CapaRed capaRed =
		colaDespachadorVerde->pop();
		if(capaRed.tipo== FORWARDING){
			struct  CajaNegraRed payloadRed= *(capaRed.payload);
			struct Forwarding forwarding= *(payloadRed.payloadForwarding);
			//colaForwarding->push(forwarding);
		} 
		else if (capaRed.tipo== BROADCAST){
			struct  CajaNegraRed payloadRed= *(capaRed.payload);
			struct Broadcast broadcast= *(payloadRed.payloadBroadcast);
			//colaBroadcast->push(broadcast);
		}
		else if (capaRed.tipo== ARBOL){
			struct  CajaNegraRed payloadRed= *(capaRed.payload);
			struct ArbolGenerador arbol= *(payloadRed.payloadArbol);
			nodoSenderTWH=nodoSender;
			//nodoSenderTWH = nodoSender;
			colaRosada->push(arbol);
		}  
	
		
	}
}
/*
void forwarding(Cola<struct Mensaje>* colaAzul,
	Cola<struct capaAplicacion>* colaRosada,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosForwarding>* colaForwarding,
	std::vector<int>* nodosIDs){


	while(1){
		struct DatosForwarding datosForwarding = colaForwarding->pop();

		if(datosForwarding.idDestino == (*nodosIDs)[0]){
			colaAzul->push(datosForwarding.payload);

		} else {
			for(size_t i = 0; i < tablaForwarding.size(); ++i){
				if(datosForwarding.idDestino == 
					tablaForwarding[i].idNodo){
					struct CapaEnlace paquete;
					struct CapaRedA capaRed;
					struct Forwarding forwarding;
					forwarding.idFuenteInicial = 
					datosForwarding.idFuenteInicial;
					forwarding.longitud = datosForwarding.longitud;
					forwarding.payload = datosForwarding.payload;
					capaRed.tipo = FORWARDING;
					//Por que
					capaRed.longitud = forwarding.longitud + 4;
					capaRed.payload = forwarding;
					paquete.tipo = RED;
					paquete.idDestinoFinal = datosForwarding.idDestino;
					paquete.idFuenteInmediato = (*nodosIDs)[0];
					paquete.longitud = capaRed.longitud + 3;
					paquete.payloadForwarding = &capaRed;
					for(size_t j=0; j<(*nodosIDs).size(); j++){
						if((*nodosIDs)[j]==tablaForwarding[i].camino){
							colasVerdes[0][j].push(paquete);
							break;
						}
					}
				}
				break;
			}
		}
	}
}
*/

/*void verificarEstructura(Cola<std::string>* despachadorMiembros,
std::vector<int>* miembrosArbol){

	int salir=0;
	int aux;
	std::string nuevoMiembro;
	while(salir==0){
		nuevoMiembro=despachadorMiembros->pop();
		std::stringstream s_stream(nuevoMiembro);
		std::vector<std::string> resultado;
		while(s_stream.good()) {
			std::string substr;
			getline(s_stream, substr, ',');
			resultado.push_back(substr);
		}
		aux=stoi(resultado[1]);
		miembrosArbol->push_back(aux);
	}
}*/

/*void broadcast(Cola<struct capaAplicacion>* colaAzul,
		Cola<struct capaAplicacion>* colaRosada,
		std::vector<Cola<struct capaEnlace>>* colasVerdes,
		Cola<struct capaRed>* colaBroadcast,
		std::vector<int>* nodosIDs,
		Cola<std::string>* despachadorMiembros,
		Cola<std::string>* colaAlcanzabilidad){

		std::vector<int> miembrosArbol;
		std::thread controladorTablaRosado(verificarEstructura,
		despachadorMiembros,&miembrosArbol);

		bool enviarDenuevo=true;

	while(1){
		struct capaRed enrutamiento = colaBroadcast->pop();
		struct capaEnlace paquete;
		size_t longitud = nodosIDs->size();

		paquete.cabeceraRed = enrutamiento.cabeceraRed;
		paquete.idDestino = enrutamiento.idDestino;

		if(enrutamiento.idFuente == (*nodosIDs)[0]){
			if(enrutamiento.idDestino != (uint16_t)-1){
				for(size_t i = 0; i < miembrosArbol.size(); ++i){
					for(size_t indice=1;indice<longitud;indice++){
						if(miembrosArbol[i]==(*nodosIDs)[indice]){
							colasVerdes[0][indice].push(paquete);
						}
					}
				}
			} else {
				for(size_t i = 1; i < longitud; ++i){
					colasVerdes[0][i].push(paquete);
				}
			}
		} else {

			struct capaAplicacion mensaje;
			mensaje.cabeceraAplicacion =
				enrutamiento.cabeceraRed.cabeceraAplicacion;
			mensaje.idFuente = enrutamiento.idFuente;
			if(enrutamiento.tipo == MENSAJE){
				colaAzul->push(mensaje);
			} else {
				if(enviarDenuevo==true){
					colaAlcanzabilidad->push("45");
					enviarDenuevo=false;
				}
				colaRosada->push(mensaje);
			}

			for(size_t i = 0; i < miembrosArbol.size(); ++i){
				for(size_t indice=1;indice<longitud;indice++){
					if(miembrosArbol[i]==(*nodosIDs)[indice] &&
					(*nodosIDs)[indice] != enrutamiento.idFuente ){

						colasVerdes[0][indice].push(paquete);
					}
				}
			}
		}
	}
	controladorTablaRosado.join();
}*/
