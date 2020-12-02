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

	
	
	std::thread hiloAzul(despachadorAzul,
		colaDespachadorAzul, &colaForwarding, &colaBroadcast, 
		(*nodosIDs)[0]);
	/*
	std::thread hiloRosado(despachadorRosado,colaDespachadorRosado,
		nodosIDs, tablaVecinos,
		colasVerdes);
		*/

	std::thread hiloVerde(despachadorVerde,
	 	colaDespachadorVerde, &colaForwarding, &colaBroadcast,colaRosada,colaAzul);
	
	std::thread hiloForwarding(forwarding, colaAzul,
		colasVerdes, &colaForwarding, nodosIDs);
/*
	std::thread hiloBroadcast(broadcast, colaAzul, colaRosada,
		colasVerdes, &colaBroadcast, nodosIDs,despachadorMiembros,
		colaAlcanzabilidad);
		*/
    /*
	std::thread hiloAlcanzabilidad(enviarAlcanzabilidad,
	colaAlcanzabilidad,tablaVecinos,colaDespachadorRosado,
	colaRosada,IP);

	std::thread hiloTablaForwarding(verificarTablaForwarding,
	colaTablaForwarding, nodosIDs);
	*/
	hiloAzul.join();
	//hiloRosado.join();
	hiloVerde.join();
	hiloForwarding.join();
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

void toCharArray2(char * paquete_ent, CapaRed * paquete){
    int offset = 0;
    memmove( &paquete->tipo,paquete_ent ,sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
     memmove( &paquete->longitud, paquete_ent+offset,sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove( paquete->datos, paquete_ent+offset,paquete->longitud);
    
     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);

}

void toCharArray(char * paquete_sal, CapaRed * paquete){
    int offset = 0;
    memmove(paquete_sal, &paquete->tipo, sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
    memmove(paquete_sal+offset, &paquete->longitud, sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove(paquete_sal+offset, paquete->datos, paquete->longitud);


     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
}








void toCharArrayBroadcast2(char * paquete_ent, Broadcast * paquete){
	/*
    int offset = 0;
    memmove( &paquete->tipo,paquete_ent ,sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
     memmove( &paquete->longitud, paquete_ent+offset,sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove( paquete->datos, paquete_ent+offset,paquete->longitud);
    
     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
	*/

}

void toCharArrayBroadcast(char * paquete_sal, Broadcast * paquete){
	/*
    int offset = 0;
    memmove(paquete_sal, &paquete->tipo, sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
    memmove(paquete_sal+offset, &paquete->longitud, sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove(paquete_sal+offset, paquete->datos, paquete->longitud);


     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
	*/
}

void toCharArrayForwarding2(char * paquete_ent, Forwarding * paquete){
	
    int offset = 0;
    memmove( &paquete->idFuenteInicial,paquete_ent ,sizeof(paquete->idFuenteInicial));
    offset+= sizeof(paquete->idFuenteInicial);
     memmove( &paquete->longitud, paquete_ent+offset,sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove( paquete->datos, paquete_ent+offset,paquete->longitud);
    
     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
	

}

void toCharArrayForwarding(char * paquete_sal, Forwarding * paquete){
	
    int offset = 0;
    memmove(paquete_sal, &paquete->idFuenteInicial, sizeof(paquete->idFuenteInicial));
    offset+= sizeof(paquete->idFuenteInicial);
    memmove(paquete_sal+offset, &paquete->longitud, sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove(paquete_sal+offset, paquete->datos, paquete->longitud);


     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
	
}




void despachadorAzul(Cola<struct DatosMensaje>* colaDespachadorAzul,
		Cola<struct DatosForwarding>* colaForwarding,
		Cola<struct Broadcast>* colaBroadcast, int idPropio){

	while(1){
		struct DatosMensaje datos = colaDespachadorAzul->pop();
		struct Mensaje mensaje;
		strcpy(mensaje.buffer, datos.buffer);
		//memmove( &paquete->idFuenteInicial,paquete_ent ,sizeof(paquete->idFuenteInicial));
		
		 
		//mensaje.mensaje.assign(datos.mensaje);
		//mensaje.idFuente = idPropio; 

		if(datos.tipo == '1'){
			
			struct DatosForwarding forwarding;
			 //forwarding.idFuenteInicial = static_cast<uint16_t>(idPropio);
			forwarding.idFuenteInicial = static_cast<uint16_t>(idPropio);
			forwarding.idDestino = datos.idDestino;
			forwarding.longitud = strlen(datos.buffer);//datos.mensaje.size(); // 2 bytes
			memmove( &forwarding.datos,&mensaje.buffer ,strlen(forwarding.datos));
			//forwarding.payload = mensaje;
			//fowarding.tipo = 
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Antes del push en despachador azul en if forwarding"<<std::endl;
			std::cout<<forwarding.idFuenteInicial<<std::endl;
			std::cout<<forwarding.idDestino<<std::endl;
			std::cout<<forwarding.longitud<<std::endl;
			//std::cout<<forwarding.payload.buffer<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			colaForwarding->push(forwarding);
			
		} else if(datos.tipo == '2'){

			struct Broadcast broadcast;
			broadcast.tipo = '1';
			//std::string aux=std::to_string(idPropio);
			broadcast.id_origen_inicial = static_cast<uint16_t>(idPropio);
			broadcast.longitud = strlen(datos.buffer);//datos.mensaje.size(); // 2 bytes
			broadcast.payloadMensaje = mensaje;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Dentro de if en despachador azul antes de broadcast"<<std::endl;
			std::cout<<broadcast.id_origen_inicial<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			colaBroadcast->push(broadcast);

		} else {
			std::cout << "Error!\n";
		}
	}
}
/*
void despachadorRosado(Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
std::vector<int>* nodosIDs,std::vector<datosNodo>* tablaVecinos,std::vector<Cola<struct CapaEnlace>>* colasVerdes){

	while(1){
		struct DatosArbolGenerador datos = colaDespachadorRosado->pop();

		size_t longitud = nodosIDs->size();
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto es antes del for en agende red"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
            for(size_t i = 0; i < longitud; ++i){
				
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto es despues del for en agende red"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                if(datos.id_destino_final == (*nodosIDs)[i]){
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es despues adentro del if en agende red"<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;

                    struct ArbolGenerador paqueteAg;

                    paqueteAg.tipo = datos.tipo;

					paqueteAg.SN = datos.SN;
					paqueteAg.RN =  datos.RN;

					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es despues de struc arbol generador  en agende red"<<std::endl;
					std::cout<<paqueteAg.tipo<<std::endl;
					std::cout<<paqueteAg.SN<<std::endl;
					std::cout<<paqueteAg.RN<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					struct CapaRed capaRed;
					capaRed.tipo = '3';

					capaRed.payload.payloadArbol = paqueteAg;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es despues de paiload arbol en agende red"<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;

					capaRed.longitud =  sizeof(paqueteAg);
					
					struct CapaEnlace paquete;

					for(size_t i2=0;i2<nodosIDs->size();i2++){
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<"Esto es Nodos ids"<<std::endl;
						std::cout<<(*nodosIDs)[i2]<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<"Esto es datosNodo"<<std::endl;
						std::cout<<(*tablaVecinos)[i2].ID<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
						std::cout<<std::endl;
					}

					paquete.tipo ='2';
					paquete.idDestinoFinal = datos.id_destino_final;
					paquete.idFuenteInmediato = (*nodosIDs)[0];
					paquete.longitud = sizeof(capaRed);
					paquete.payload = capaRed;

					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es despues de struc capa Enlace y anets push en agende red"<<std::endl;
					std::cout<<"Antes de tipo"<<std::endl;
					std::cout<<paquete.tipo<<std::endl;
					std::cout<<paquete.idDestinoFinal<<std::endl;
					std::cout<<paquete.idFuenteInmediato<<std::endl;
					std::cout<<paquete.longitud<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
                    
					
	

                    colasVerdes[0][i].push(paquete);

                
				}

            }

		
		

	}
}
*/
void despachadorVerde(Cola<struct CapaRed>* colaDespachadorVerde,
	Cola<struct DatosForwarding>* colaForwarding,
	Cola<struct Broadcast>* colaBroadcast,Cola<struct ArbolGenerador>* colaRosada,Cola<struct Mensaje>* colaAzul ){
	while(1){
		struct CapaRed capaRed =
		colaDespachadorVerde->pop();
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
				std::cout<<"Adentro de depachador verde"<<std::endl;
                std::cout<<capaRed.tipo<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			nodoSenderTWH=nodoSender;
		if(capaRed.tipo== '1'){

			
			//struct  CajaNegraRed cajaNegra;
			//cajaNegra=capaRed.payload;
			struct Forwarding fowardingRecibir;
			struct Mensaje mensaje;
			//fowardingRecibir=cajaNegra.payloadForwarding;
			std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

				memmove( &fowardingRecibir.idFuenteInicial,capaRed.datos ,sizeof(fowardingRecibir.idFuenteInicial));
     			memmove( &fowardingRecibir.longitud, capaRed.datos+ sizeof(fowardingRecibir.idFuenteInicial),sizeof(fowardingRecibir.longitud));

    			memmove( &fowardingRecibir.datos, capaRed.datos + sizeof(fowardingRecibir.idFuenteInicial)+sizeof(fowardingRecibir.longitud) ,strlen(fowardingRecibir.datos));
				//toCharArrayForwarding2(capaRed.datos, &fowardingRecibir);
				std::cout<<"Buernas antes de push a cola azul en despachador verde"<<std::endl;
				//std::cout<<fowardingRecibir.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			    std::cout<<fowardingRecibir.idFuenteInicial<<std::endl;
				std::cout<<fowardingRecibir.longitud<<std::endl;
				std::cout<<fowardingRecibir.datos<<std::endl;
				memmove( &mensaje.buffer,&fowardingRecibir.datos ,sizeof(mensaje.buffer));
				std::cout<<mensaje.buffer<<std::endl;
				std::cout<<"Saludos despues de mmemori forwarding a mensaje"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			colaAzul->push(mensaje);
			//struct Forwarding forwarding= *(payloadRed.payloadForwarding);
			//colaForwarding->push(forwarding);
		} 
		else if (capaRed.tipo== '2'){
			/*
			    std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Eentre a if a ahverle push a cola broadcast"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			struct Broadcast nuevoBroadcast;
			struct  CajaNegraRed payloadRed= capaRed.payload;
			nuevoBroadcast=payloadRed.payloadBroadcast;
			//struct Broadcast broadcast= *(payloadRed.payloadBroadcast);
			colaBroadcast->push(nuevoBroadcast);
			*/
		}
		else if (capaRed.tipo== '3'){
			/*
			struct  CajaNegraRed payloadRed= capaRed.payload;
			struct ArbolGenerador arbol= payloadRed.payloadArbol;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes del pusha la cola rosada en la capa de red"<<std::endl;
                std::cout<<arbol.SN<<std::endl;
                std::cout<<arbol.RN<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			nodoSenderTWH=nodoSender;
			//nodoSenderTWH = nodoSender;
			colaRosada->push(arbol);
			*/
		}  
	
		
	}
}

void forwarding(Cola<struct Mensaje>* colaAzul,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosForwarding>* colaForwarding,
	std::vector<int>* nodosIDs){
		bool condicon=false;

	while(1){
		char buffer[1017];
		//char buffer3[1017];

		char buffer2[1040];
		struct DatosForwarding datosForwarding = colaForwarding->pop();

		if(datosForwarding.idDestino == (*nodosIDs)[0]){
			//colaAzul->push(datosForwarding.payload);


		} else {

				size_t longitud = nodosIDs->size();
			for(size_t i = 0; i < longitud; ++i){
				if(datosForwarding.idDestino == (*nodosIDs)[i]){
					//struct Mensaje auxMensaje;
					struct Forwarding forwardingEnviar;
					//struct Broadcast auxBroadcast;
					//auxBroadcast.payloadMensaje=auxMensaje;
					//struct ArbolGenerador auxArbol;
					forwardingEnviar.idFuenteInicial=datosForwarding.idFuenteInicial;
					forwardingEnviar.longitud=sizeof(datosForwarding.datos);
					
				
					//forwardingEnviar.payload=datosForwarding.payload;
					memmove( &forwardingEnviar.datos,&datosForwarding.datos ,strlen(forwardingEnviar.datos));

					
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Me encuentro despues de forwarding enviar"<<std::endl;
                //std::cout<<forwardingEnviar.datos<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

				//nodoSenderTWH=nodoSender;
					//struct CajaNegraRed cajaNegra;
					//cajaNegra.payloadForwarding=forwardingEnviar;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Ayuda  despues de caja negra"<<std::endl;
               // std::cout<<cajaNegra.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
					//cajaNegra.payloadBroadcast=auxBroadcast;
					//cajaNegra.payloadArbol=auxArbol;
					 //toCharArrayForwarding(buffer, &forwardingEnviar);
					 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Voy antes de funcion especial"<<std::endl;
				//int offset = 0;
				//char aux=forwardingEnviar.idFuenteInicial;

    			memmove(buffer, &forwardingEnviar.idFuenteInicial, sizeof(forwardingEnviar.idFuenteInicial));
				//sprintf (buffer, "%u", forwardingEnviar.idFuenteInicial);
				//strcat(buffer3, buffer);
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
					 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Despues del primer memory move"<<std::endl;
				//std::cout<<buffer3<<std::endl;
				 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
   				 //offset+= sizeof(forwardingEnviar.idFuenteInicial);
   				 memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial), &(forwardingEnviar.longitud), sizeof(forwardingEnviar.longitud));
			
				//sprintf (buffer, "%u", forwardingEnviar.longitud);
				//strcat(buffer3, buffer);
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
					 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Despues del segundo memory move"<<std::endl;
				//std::cout<<buffer3<<std::endl;
				 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
   				 //offset+= sizeof(forwardingEnviar.longitud);
    			memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial)+sizeof(forwardingEnviar.longitud), &(forwardingEnviar.datos), strlen(forwardingEnviar.datos));
				//sprintf (buffer, "%s", forwardingEnviar.datos);
				//strcat(buffer3, buffer);
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
					 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Despues del tercer memory move"<<std::endl;
				//std::cout<<buffer3<<std::endl;
				 std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				//std::cout<<buffer<<std::endl;

               // std::cout<<cajaNegra.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
					struct CapaRed	capaRed;
					capaRed.tipo=0x01;

					capaRed.longitud=sizeof(buffer);
					//sprintf (capaRed.datos, "%s", buffer3);
					memmove( &capaRed.datos,buffer ,strlen(capaRed.datos));
					//capaRed.payload=cajaNegra;

					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				std::cout<<"Conoce  despues de capaRed"<<std::endl;
				//std::cout<<capaRed.datos<<std::endl;
                //std::cout<<capaRed.payload.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

				//toCharArray(buffer2, &capaRed);
				memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
				memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
				memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, strlen(capaRed.datos));
				//sprintf (buffer, "%u", capaRed.tipo);
				//strcat(buffer2, buffer);
				//sprintf (buffer, "%u", capaRed.longitud);
				//strcat(buffer2, buffer);
				//sprintf (buffer, "%s", capaRed.datos);
				//strcat(buffer2, buffer);
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
				//std::cout<<"voy a imprimir buffer2"<<std::endl;
				//std::cout<<buffer2<<std::endl;
                //std::cout<<capaRed.payload.payloadForwarding.payload.buffer<<std::endl;
				//std::cout<<std::endl;
				//std::cout<<std::endl;
				//std::cout<<std::endl;
					struct CapaEnlace paquete;
					paquete.tipo=0x02;
					paquete.idDestinoFinal=datosForwarding.idDestino;
					paquete.idFuenteInmediato=datosForwarding.idFuenteInicial;
					paquete.longitud=sizeof(buffer2);
					memmove(&paquete.datos, buffer2, strlen(paquete.datos));
					//sprintf (paquete.datos, "%s", buffer2);
					//memmove( paquete.datos,buffer2 ,sizeof(paquete.datos));
					
					//paquete.payload=capaRed;
					std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Aqui estoy en fowarding antes de hcerle push para enviar a enlace"<<std::endl;
                std::cout<<paquete.idDestinoFinal<<std::endl;
                std::cout<<paquete.idFuenteInmediato<<std::endl;
				//std::cout<<paquete.datos<<std::endl;
				//std::cout<<paquete.payload.payload.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

					colasVerdes[0][i].push(paquete);
				}
			}

			/*
			size_t i = 0;
			while(i<tablaForwarding.size() && condicon==false){
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"Antes de entrara a if"<<std::endl;
				std::cout<<enrutamiento.idDestino<<std::endl;
				std::cout<<tablaForwarding[i].idNodo<<std::endl;
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				if(enrutamiento.idDestino==tablaForwarding[i].idNodo){
					std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"Despues de entrar a if"<<std::endl;
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				
					struct capaEnlace paquete;
					paquete.cabeceraRed = enrutamiento.cabeceraRed;
					paquete.idDestino = enrutamiento.idDestino;
					for(size_t j=0; j<(*nodosIDs).size(); j++){

						std::cout<<"Esto es camino"<<std::endl;
						std::cout<<"MMMMMMMMMMMMMMM"<<std::endl;
						std::cout<<"MMMMMMMMMMMMMMM"<<std::endl;
						std::cout<<tablaForwarding[i].camino<<std::endl;
						std::cout<<"MMMMMMMMMMMMMMM"<<std::endl;
						std::cout<<"MMMMMMMMMMMMMMM"<<std::endl;

						std::cout<<"Esto es nodos ID"<<std::endl;
						std::cout<<"hhhhhhhhhhhhhhh"<<std::endl;
						std::cout<<"hhhhhhhhhhhhhh"<<std::endl;
						std::cout<<(*nodosIDs)[j]<<std::endl;
						std::cout<<"hhhhhhhhhhhhhhhhhhh"<<std::endl;
						std::cout<<"hhhhhhhhhhhhhhhh"<<std::endl;
						
						if((*nodosIDs)[j]==tablaForwarding[i].camino){
							std::cout<<"Entre a enviar paquete"<<std::endl;
							std::cout<<"XXXXXXXXXXXXX"<<std::endl;
							std::cout<<"XXXXXXXXXXXXXXXX"<<std::endl;
							std::cout<<tablaForwarding[i].camino<<std::endl;
							std::cout<<"XXXXXXXXXXXXX"<<std::endl;
							std::cout<<"XXXXXXXXXXXXXXXX"<<std::endl;
						colasVerdes[0][j].push(paquete);
						condicon=true;
							break;
						}
					}
				}
				//break;
				i++;
			}
			condicon=false;
			*/

			/*
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
			*/
		}
	}
}

/*
void verificarEstructura(Cola<std::string>* despachadorMiembros,
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
}
*/

/*
void broadcast(Cola<struct Mensaje>* colaAzul,
		Cola<struct ArbolGenerador>* colaRosada,
		std::vector<Cola<struct CapaEnlace>>* colasVerdes,
		Cola<struct Broadcast>* colaBroadcast,
		std::vector<int>* nodosIDs,
		Cola<std::string>* despachadorMiembros,
		Cola<std::string>* colaAlcanzabilidad){

		std::vector<int> miembrosArbol;
		std::thread controladorTablaRosado(verificarEstructura,
		despachadorMiembros,&miembrosArbol);

		bool enviarDenuevo=true;

	while(1){
		//struct capaRed enrutamiento = colaBroadcast->pop();
		struct Broadcast nuevoBroadcast=colaBroadcast->pop();
		struct CapaRed capaRed;
		struct CapaEnlace paquete;
		size_t longitud = nodosIDs->size();
		capaRed.tipo='2';
		capaRed.longitud=sizeof(nuevoBroadcast);
		capaRed.payload.payloadBroadcast=nuevoBroadcast;
		//paquete.cabeceraRed = enrutamiento.cabeceraRed;
		//paquete.idDestino = enrutamiento.idDestino;

		if(nuevoBroadcast.id_origen_inicial== (*nodosIDs)[0]){
			//if(enrutamiento.idDestino != (uint16_t)-1){
				for(size_t i = 0; i < miembrosArbol.size(); ++i){
					for(size_t indice=1;indice<longitud;indice++){
						if(miembrosArbol[i]==(*nodosIDs)[indice]){
							paquete.tipo='2';
							paquete.idFuenteInmediato=nuevoBroadcast.id_origen_inicial;
							paquete.idDestinoFinal=static_cast<uint16_t>(miembrosArbol[i]);
							paquete.longitud=sizeof(capaRed);
							paquete.payload=capaRed;
							std::cout<<std::endl;
		                    std::cout<<std::endl;
		                    std::cout<<std::endl;
	                        std::cout<<"Voy a enviar en broadcast"<<std::endl;
							std::cout<<miembrosArbol[i]<<std::endl;
							std::cout<<paquete.tipo<<std::endl;
							std::cout<<paquete.idFuenteInmediato<<std::endl;
							std::cout<<paquete.idDestinoFinal<<std::endl;
							std::cout<<paquete.longitud<<std::endl;
		                    std::cout<<std::endl;
		                    std::cout<<std::endl;
		                    std::cout<<std::endl;
							colasVerdes[0][indice].push(paquete);
						}
					}
				}
			//} else {
				//for(size_t i = 1; i < longitud; ++i){
					//colasVerdes[0][i].push(paquete);
				//}
			//}
		} else {

			//struct capaAplicacion mensaje;
			//mensaje.cabeceraAplicacion =
				//enrutamiento.cabeceraRed.cabeceraAplicacion;
			//mensaje.idFuente = enrutamiento.idFuente;
			if(nuevoBroadcast.tipo == '1'){
				
				struct Mensaje nuevoMensaje=nuevoBroadcast.payloadMensaje;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				
				colaAzul->push(nuevoMensaje);
			} else {
				//if(enviarDenuevo==true){
					//colaAlcanzabilidad->push("45");
					//enviarDenuevo=false;
					//colaRosada->push(mensaje);
				//}
				
			}
			
			for(size_t i = 0; i < miembrosArbol.size(); ++i){
				for(size_t indice=1;indice<longitud;indice++){
					if(miembrosArbol[i]==(*nodosIDs)[indice] &&
					(*nodosIDs)[indice] != nuevoBroadcast.id_origen_inicial ){
                            paquete.tipo='2';
							paquete.idFuenteInmediato=nuevoBroadcast.id_origen_inicial;
							paquete.idDestinoFinal=static_cast<uint16_t>(miembrosArbol[i]);
							paquete.payload=capaRed;
						colasVerdes[0][indice].push(paquete);
					}
				}
			}
			
		}
	}
	controladorTablaRosado.join();
}
*/