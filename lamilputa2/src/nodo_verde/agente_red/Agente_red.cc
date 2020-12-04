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
	Cola<std::string>* colaTablaForwarding,Cola<std::string>* colaEnviarAlcanzabilidad){

	Cola<struct DatosForwarding> colaForwarding;
	Cola<struct Broadcast> colaBroadcast;
	
	
	
	std::thread hiloAzul(despachadorAzul,
		colaDespachadorAzul, &colaForwarding, &colaBroadcast, 
		(*nodosIDs)[0]);
	
	std::thread hiloRosado(despachadorRosado,colaDespachadorRosado,
		nodosIDs, tablaVecinos,
		colasVerdes);
		

	std::thread hiloVerde(despachadorVerde,
	 	colaDespachadorVerde, &colaForwarding, &colaBroadcast,colaRosada,colaAzul);
	
	std::thread hiloForwarding(forwarding, colaAzul,
		colasVerdes, &colaForwarding, nodosIDs);

	std::thread hiloBroadcast(broadcast, colaAzul, colaRosada,
		colasVerdes, &colaBroadcast, nodosIDs,despachadorMiembros,
		colaAlcanzabilidad,colaEnviarAlcanzabilidad);
		
    
	std::thread hiloAlcanzabilidad(enviarAlcanzabilidad,
	colaAlcanzabilidad,tablaVecinos,
	IP,&colaBroadcast,colaEnviarAlcanzabilidad);
	
	std::thread hiloTablaForwarding(verificarTablaForwarding,
	colaTablaForwarding, nodosIDs);
	
	hiloAzul.join();
	hiloRosado.join();
	hiloVerde.join();
	hiloForwarding.join();
	hiloBroadcast.join();
	hiloAlcanzabilidad.join();
}

void verificarTablaForwarding(Cola<std::string>* colaTablaForwarding,
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
			std::cout<<"PUSH FORWARDING"<<std::endl;
			std::cout<<tablaForwarding.size()<<std::endl;
			tablaForwarding.push_back(nuevoDato);
			std::cout<<"LUEGO PUSH FORWARDING"<<std::endl;
			std::cout<<tablaForwarding.size()<<std::endl;
		}
	}
}

void enviarAlcanzabilidad(Cola<std::string>* colaAlcanzabilidad,
std::vector<datosNodo>* tablaVecinos,
char* IP,Cola<struct Broadcast>* colaBroadcast,Cola<std::string>* colaEnviarAlcanzabilidad){

	//struct capaSuperior enviar;
	//struct capaAplicacion enviarRosado;
	std::string aux;
	std::string mensajeAlcanzabilidad;

	size_t indice;
	char host[256];
	//struct capaSuperior enrutamiento;
	//struct estructuraRed cabeceraRed;

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
		//trcpy(enviarRosado.cabeceraAplicacion.buffer,aux.c_str());
		//enviarRosado.idFuente=(*tablaVecinos)[0].ID;
		colaEnviarAlcanzabilidad->push(aux);

		std::string estadoArbol=colaAlcanzabilidad->pop();
		uint16_t prueba;
		uint16_t prueba2;
		uint8_t prueba3;
		if(estadoArbol=="45"){
			char buffer[200];
			uint16_t aux1=static_cast<uint16_t>(tablaVecinos->size()-1);
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es aux1"<<std::endl;
			std::cout<<aux1<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			memmove( buffer,&aux1 ,sizeof(aux1));

			memmove( &prueba,buffer ,sizeof(prueba));
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es Prueba"<<std::endl;
			std::cout<<prueba<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			size_t offset = sizeof(aux1);
			for(indice=1;indice<tablaVecinos->size();indice++){
				uint16_t aux2=static_cast<uint16_t>((*tablaVecinos)[indice].ID);
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es aux2"<<std::endl;
			std::cout<<aux2<<std::endl;
				memmove( buffer+offset,&aux2 ,sizeof(aux2));
				memmove( &prueba2,buffer+offset ,sizeof(prueba2));
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es Prueba2"<<std::endl;
			std::cout<<prueba2<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;

				offset=offset+sizeof(aux2);
				uint8_t aux3;
				if(((*tablaVecinos)[indice].IP).compare(IP)==0){
				//mensajeAlcanzabilidad+=",1";
				aux3=0x01;
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es aux3 0x01"<<std::endl;
			if(aux3 == 0x01){
				std::cout<<"0x01"<<std::endl;
			}
		
			std::cout<<std::endl;
			std::cout<<std::endl;
				memmove( buffer+offset,&aux3 ,sizeof(aux3));
				memmove( &prueba3,buffer+offset ,sizeof(prueba3));
				if(prueba3 == 0x01){
				std::cout<<"0x01 recuperado"<<std::endl;
			}
				
				offset=offset+sizeof(aux3);
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			//std::cout<<"Esto es Prueba3"<<std::endl;
			//std::cout<<prueba3<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;

				}else{
				//mensajeAlcanzabilidad+=",2";
				aux3=0x02;
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es aux3 0x02"<<std::endl;
			if(aux3 == 0x02){
				std::cout<<"0x02"<<std::endl;
			}
			std::cout<<aux3<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
				memmove( buffer+offset,&aux3 ,sizeof(aux3));
				memmove( &prueba3,buffer+offset ,sizeof(prueba3));
				if(prueba3 == 0x02){
				std::cout<<"0x02 recuperado"<<std::endl;
			    }
				offset=offset+sizeof(aux3);
				std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			//std::cout<<"Esto es Prueba3"<<std::endl;
			//std::cout<<prueba3<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;

			}
				
			}
			/*
			enviar.idDestino=0;
			enviar.destinoRed=0;
	    	aux=mensajeAlcanzabilidad;
	    	strcpy(enviar.cabeceraAplicacion.buffer,aux.c_str());
			*/
			struct Broadcast broadcast;
			broadcast.tipo = 0x02;
			//std::string aux=std::to_string(idPropio);
			broadcast.id_origen_inicial = static_cast<uint16_t>((*tablaVecinos)[0].ID);
			memmove( &broadcast.datos,buffer ,sizeof(broadcast.datos));
			broadcast.longitud=sizeof(broadcast.datos);
			colaBroadcast->push(broadcast);
		}
	}
}

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

		if(datos.tipo == 0x01){
			char buffer3 [200];
			struct DatosForwarding forwarding;
			 //forwarding.idFuenteInicial = static_cast<uint16_t>(idPropio);
			forwarding.idFuenteInicial = static_cast<uint16_t>(idPropio);
			forwarding.idDestino = datos.idDestino;
			forwarding.longitud = sizeof(datos.buffer);//datos.mensaje.size(); // 2 bytes
			memmove( buffer3,&mensaje.buffer ,sizeof(mensaje.buffer));
			memmove( &forwarding.datos,buffer3 ,sizeof(forwarding.datos));
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
			
		} else if(datos.tipo == 0x02){
			char buffer3 [200];
			struct Broadcast broadcast;
			broadcast.tipo = 0x01;
			//std::string aux=std::to_string(idPropio);
			broadcast.id_origen_inicial = static_cast<uint16_t>(idPropio);
			memmove( buffer3,&mensaje.buffer ,sizeof(mensaje.buffer));
			memmove( &broadcast.datos,buffer3 ,sizeof(broadcast.datos));
			
			//broadcast.payloadMensaje = mensaje;
			broadcast.longitud = sizeof(datos.buffer);//datos.mensaje.size(); // 2 bytes
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
					char buffer [1017];
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
					capaRed.tipo = 0x03;

				    memmove(buffer, &paqueteAg.tipo, sizeof(paqueteAg.tipo));
					memmove(buffer+sizeof(paqueteAg.tipo), &paqueteAg.SN, sizeof(paqueteAg.SN));
					memmove(buffer+sizeof(paqueteAg.tipo)+sizeof(paqueteAg.SN), &paqueteAg.RN, sizeof(paqueteAg.RN));
					memmove( &capaRed.datos,buffer ,sizeof(capaRed.datos));
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es despues de paiload arbol en agende red"<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;

					capaRed.longitud =  sizeof(capaRed.datos);
					
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

					paquete.tipo =0X02;
					paquete.idDestinoFinal = datos.id_destino_final;
					paquete.idFuenteInmediato = (*nodosIDs)[0];
					char buffer2[1040];
					memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
					memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
					memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
					memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
					paquete.longitud = sizeof(paquete.datos);


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
				std::cout<<capaRed.longitud<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			nodoSenderTWH=nodoSender;
		if(capaRed.tipo== 0x01){

			
			//struct  CajaNegraRed cajaNegra;
			//cajaNegra=capaRed.payload;
			struct DatosForwarding fowardingRecibir;
			struct Mensaje mensaje;
			//fowardingRecibir=cajaNegra.payloadForwarding;
			std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

				memmove( &fowardingRecibir.idFuenteInicial,capaRed.datos ,sizeof(fowardingRecibir.idFuenteInicial));
     			memmove( &fowardingRecibir.longitud, capaRed.datos+ sizeof(fowardingRecibir.idFuenteInicial),sizeof(fowardingRecibir.longitud));
    			memmove( &fowardingRecibir.datos, capaRed.datos + sizeof(fowardingRecibir.idFuenteInicial)+sizeof(fowardingRecibir.longitud) ,sizeof(fowardingRecibir.datos));
				fowardingRecibir.idDestino = destinoFinal;
				//toCharArrayForwarding2(capaRed.datos, &fowardingRecibir);
				std::cout<<"Buernas antes de push a cola azul en despachador verde"<<std::endl;
				//std::cout<<fowardingRecibir.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			    std::cout<<fowardingRecibir.idFuenteInicial<<std::endl;
				std::cout<<fowardingRecibir.longitud<<std::endl;
				//std::cout<<fowardingRecibir.datos<<std::endl;
				
				
				//colaAzul->push(mensaje);
				//struct Forwarding forwarding= *(payloadRed.payloadForwarding);
				colaForwarding->push(fowardingRecibir);
		} 
		else if (capaRed.tipo== 0x02){
			
			    std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Eentre a if a ahverle push a cola broadcast"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			struct Broadcast nuevoBroadcast;
			memmove( &nuevoBroadcast.tipo,capaRed.datos,sizeof(nuevoBroadcast.tipo));
			memmove( &nuevoBroadcast.id_origen_inicial,capaRed.datos+sizeof(nuevoBroadcast.tipo), sizeof(nuevoBroadcast.id_origen_inicial));
			memmove( &nuevoBroadcast.longitud,capaRed.datos+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial),sizeof(nuevoBroadcast.longitud));
			memmove( &nuevoBroadcast.datos, capaRed.datos+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial)+sizeof(nuevoBroadcast.longitud),sizeof(nuevoBroadcast.datos));
		
			//struct Broadcast broadcast= *(payloadRed.payloadBroadcast);
			colaBroadcast->push(nuevoBroadcast);
			
		}
		else if (capaRed.tipo== 0x03){
			
			
			struct ArbolGenerador paqueteAg;
			memmove( &paqueteAg.tipo, capaRed.datos ,sizeof(paqueteAg.tipo));
			memmove( &paqueteAg.SN, capaRed.datos +sizeof(paqueteAg.tipo),sizeof(paqueteAg.SN));
			memmove( &paqueteAg.RN, capaRed.datos +sizeof(paqueteAg.tipo)+sizeof(paqueteAg.SN),sizeof(paqueteAg.RN));
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes del pusha la cola rosada en la capa de red"<<std::endl;
                std::cout<<paqueteAg.SN<<std::endl;
                std::cout<<paqueteAg.RN<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
			nodoSenderTWH=nodoSender;
			//nodoSenderTWH = nodoSender;
			colaRosada->push(paqueteAg);
			
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
		struct Mensaje mensaje;
		char buffer2[1040];
		char buffer3 [200];
		
		struct DatosForwarding datosForwarding = colaForwarding->pop();
		std::cout<<"VOY A ENVIAR MENSAJE"<<std::endl;

		if(datosForwarding.idDestino == (*nodosIDs)[0]){
			char buffer4 [200];
			memmove( buffer4,&datosForwarding.datos ,sizeof(datosForwarding.datos));
			memmove( &mensaje.buffer,buffer4 ,sizeof(mensaje.buffer));
			colaAzul->push(mensaje);
			std::cout<<"mensaje va para el azul"<<std::endl;
			std::cout<<mensaje.buffer<<std::endl;


		} else {
			/*	
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
					memmove (buffer3,&datosForwarding.datos,sizeof(datosForwarding.datos) );
					memmove( &forwardingEnviar.datos,buffer3 ,sizeof(forwardingEnviar.datos));

					
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
    			memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial)+sizeof(forwardingEnviar.longitud), &(forwardingEnviar.datos), sizeof(forwardingEnviar.datos));
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


				struct Forwarding fowardingRecibir;

				memmove( &fowardingRecibir.idFuenteInicial,buffer ,sizeof(fowardingRecibir.idFuenteInicial));
     			memmove( &fowardingRecibir.longitud, buffer+ sizeof(fowardingRecibir.idFuenteInicial),sizeof(fowardingRecibir.longitud));

    			memmove( &fowardingRecibir.datos, buffer + sizeof(fowardingRecibir.idFuenteInicial)+sizeof(fowardingRecibir.longitud) ,sizeof(fowardingRecibir.datos));
				std::cout<<"revision fowaring"<<std::endl;
				std::cout<<fowardingRecibir.idFuenteInicial<<std::endl;
				std::cout<<fowardingRecibir.longitud<<std::endl;
				//std::cout<<fowardingRecibir.datos<<std::endl;





					struct CapaRed	capaRed;
					capaRed.tipo=0x01;

					capaRed.longitud=sizeof(buffer);
					//sprintf (capaRed.datos, "%s", buffer3);
					memmove( &capaRed.datos,buffer ,sizeof(capaRed.datos));
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
				memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
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
					memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
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
			*/
			
			size_t i = 0;
			std::cout<<"SIZE TABLA FORWARDING"<<std::endl;
			std::cout<<tablaForwarding.size()<<std::endl;
			while(i<tablaForwarding.size() && condicon==false){
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"Antes de entrara a if"<<std::endl;
				std::cout<<datosForwarding.idDestino<<std::endl;
				std::cout<<tablaForwarding[i].idNodo<<std::endl;
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				if(datosForwarding.idDestino==tablaForwarding[i].idNodo){
					std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"Despues de entrar a if"<<std::endl;
				std::cout<<"\n";
				std::cout<<"\n";
				std::cout<<"\n";
				
					//struct capaEnlace paquete;
					//paquete.cabeceraRed = enrutamiento.cabeceraRed;
					//paquete.idDestino = enrutamiento.idDestino;

					//struct Mensaje auxMensaje;
					struct Forwarding forwardingEnviar;
					//struct Broadcast auxBroadcast;
					//auxBroadcast.payloadMensaje=auxMensaje;
					//struct ArbolGenerador auxArbol;
					forwardingEnviar.idFuenteInicial=datosForwarding.idFuenteInicial;
					forwardingEnviar.longitud=sizeof(datosForwarding.datos);
					
				
					//forwardingEnviar.payload=datosForwarding.payload;
					memmove (buffer3,&datosForwarding.datos,sizeof(datosForwarding.datos) );
					memmove( &forwardingEnviar.datos,buffer3 ,sizeof(forwardingEnviar.datos));
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
						
							memmove(buffer, &forwardingEnviar.idFuenteInicial, sizeof(forwardingEnviar.idFuenteInicial));
							memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial), &(forwardingEnviar.longitud), sizeof(forwardingEnviar.longitud));
							memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial)+sizeof(forwardingEnviar.longitud), &(forwardingEnviar.datos), sizeof(forwardingEnviar.datos));
							struct CapaRed	capaRed;
							capaRed.tipo=0x01;

							capaRed.longitud=sizeof(buffer);
							//sprintf (capaRed.datos, "%s", buffer3);
							memmove( &capaRed.datos,buffer ,sizeof(capaRed.datos));
						
							memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
							memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
							memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
							struct CapaEnlace paquete;
							paquete.tipo=0x02;
							paquete.idDestinoFinal=datosForwarding.idDestino;
							paquete.idFuenteInmediato=datosForwarding.idFuenteInicial;
							paquete.longitud=sizeof(buffer2);
							memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
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


void verificarEstructura(Cola<std::string>* despachadorMiembros,
std::vector<int>* miembrosArbol){

	int salir=0;
	int aux;
	int opcion;
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
		opcion = stoi(resultado[0]);
		if(opcion == 46){
			int bandera = 0;
			size_t i = 0;
			while(i< miembrosArbol->size()&& bandera == 0){
				if((*miembrosArbol)[i] == aux){
					miembrosArbol->erase (miembrosArbol->begin()+i);
					bandera = 1;
				}
				i++;
			}
		}
		else{
			if(opcion == 47){
				size_t i = 0;
				while(i< miembrosArbol->size()){
					
					miembrosArbol->erase (miembrosArbol->begin()+i);
					
				i++;
				}
			}
			else{
				if(opcion == 43 ||opcion == 44){
					std::cout<<"Entre a pushear a miembros arbol "+ aux<<std::endl;
					miembrosArbol->push_back(aux);

				}
			}
		}



		
		
	}
}



void broadcast(Cola<struct Mensaje>* colaAzul,
		Cola<struct ArbolGenerador>* colaRosada,
		std::vector<Cola<struct CapaEnlace>>* colasVerdes,
		Cola<struct Broadcast>* colaBroadcast,
		std::vector<int>* nodosIDs,
		Cola<std::string>* despachadorMiembros,
		Cola<std::string>* colaAlcanzabilidad,Cola<std::string>* colaEnviarAlcanzabilidad){

		std::vector<int> miembrosArbol;
		std::thread controladorTablaRosado(verificarEstructura,
		despachadorMiembros,&miembrosArbol);

		bool enviarDenuevo=true;

	while(1){
		char buffer2[1040];
		//struct capaRed enrutamiento = colaBroadcast->pop();
		struct Broadcast nuevoBroadcast=colaBroadcast->pop();
		struct CapaRed capaRed;
		struct CapaEnlace paquete;
		size_t longitud = nodosIDs->size();
		capaRed.tipo=0x02;
		char buffer [1017];
		memmove(buffer, &nuevoBroadcast.tipo, sizeof(nuevoBroadcast.tipo));
		memmove(buffer+sizeof(nuevoBroadcast.tipo), &nuevoBroadcast.id_origen_inicial, sizeof(nuevoBroadcast.id_origen_inicial));
		memmove(buffer+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial), &nuevoBroadcast.longitud, sizeof(nuevoBroadcast.longitud));
		memmove(buffer+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial)+sizeof(nuevoBroadcast.longitud), &nuevoBroadcast.datos, sizeof(nuevoBroadcast.datos));
		
		
		memmove( &capaRed.datos,buffer ,sizeof(capaRed.datos));
		capaRed.longitud=sizeof(capaRed.datos);
		//capaRed.payload.payloadBroadcast=nuevoBroadcast;
		//paquete.cabeceraRed = enrutamiento.cabeceraRed;
		//paquete.idDestino = enrutamiento.idDestino;

		if(nuevoBroadcast.id_origen_inicial== (*nodosIDs)[0]){
			//if(enrutamiento.idDestino != (uint16_t)-1){
				for(size_t i = 0; i < miembrosArbol.size(); ++i){
					for(size_t indice=1;indice<longitud;indice++){
						if(miembrosArbol[i]==(*nodosIDs)[indice]){
							paquete.tipo=0x02;
							paquete.idFuenteInmediato=nuevoBroadcast.id_origen_inicial;
							paquete.idDestinoFinal=static_cast<uint16_t>(miembrosArbol[i]);
							paquete.longitud=sizeof(paquete.datos);
							memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
				            memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
				            memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
							memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
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
			if(nuevoBroadcast.tipo == 0x01){
				
				struct Mensaje nuevoMensaje;
				char buffer3 [200];
				memmove( buffer3,&nuevoBroadcast.datos ,sizeof(nuevoBroadcast.datos));
				memmove( &nuevoMensaje.buffer,buffer3 ,sizeof(nuevoMensaje.buffer));
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<"aqui dentro de if en broadcast para enviar a azul"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				
				colaAzul->push(nuevoMensaje);
			} else if(nuevoBroadcast.tipo==0x02){
				if(enviarDenuevo==true){
					colaAlcanzabilidad->push("45");
					enviarDenuevo=false;
					//colaRosada->push(mensaje);
				}
				char buffer3 [200];
				memmove( buffer3,&nuevoBroadcast.datos ,sizeof(nuevoBroadcast.datos));
				uint16_t cantiadaVecinos;
				std::string mensajeAlcanzabilidad;
				memmove( &cantiadaVecinos,buffer3 ,sizeof(cantiadaVecinos));
				mensajeAlcanzabilidad = "90,"+std::to_string(nodoSender)+","+std::to_string(cantiadaVecinos)+",";
				size_t offset=sizeof(cantiadaVecinos);
				for(uint16_t indice4=0;indice4<cantiadaVecinos;indice4++){
					uint16_t idVecino;
					memmove( &idVecino,buffer3+offset ,sizeof(idVecino));
					offset=offset+sizeof(idVecino);
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es idVecino"<<std::endl;
					std::cout<<idVecino<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					uint8_t distancia;
					memmove( &distancia,buffer3+offset ,sizeof(distancia));
					offset=offset+sizeof(distancia);
					if(distancia==0x01){
						std::cout<<std::endl;
						std::cout<<"Entre a if tipo 0x01"<<std::endl;
						std::cout<<std::endl;
					std::cout<<std::endl;
					char buffer[80];
   					int buffer_len = sprintf(buffer, "%d", idVecino);
					   std::string str(buffer, buffer + buffer_len );
    					
						mensajeAlcanzabilidad+=str+","+"1";
					}else if(distancia==0x02){
						std::cout<<std::endl;
						std::cout<<"Entre a if tipo 0x02"<<std::endl;
						std::cout<<std::endl;
					std::cout<<std::endl;
					char buffer[80];
   					int buffer_len = sprintf(buffer, "%d", idVecino);
					   std::string str(buffer, buffer + buffer_len );
    					
						mensajeAlcanzabilidad+=str+","+"2";

					}
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<"Esto es distancia"<<std::endl;
					std::cout<<distancia<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					
					std::cout<<"Esto es mensaje alcanzabilidad"<<std::endl;
					std::cout<<mensajeAlcanzabilidad<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					

				}
				
				//colaRosada->push(mensaje);
				colaEnviarAlcanzabilidad->push(mensajeAlcanzabilidad);
				
			}
			
			for(size_t i = 0; i < miembrosArbol.size(); ++i){
				for(size_t indice=1;indice<longitud;indice++){
					if(miembrosArbol[i]==(*nodosIDs)[indice] &&
					(*nodosIDs)[indice] != nuevoBroadcast.id_origen_inicial ){
							paquete.tipo=0x02;
							paquete.idFuenteInmediato=nuevoBroadcast.id_origen_inicial;
							paquete.idDestinoFinal=static_cast<uint16_t>(miembrosArbol[i]);
							
							memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
				            memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
				            memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
							memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
							paquete.longitud=sizeof(paquete.datos);
						colasVerdes[0][indice].push(paquete);
					}
				}
			}
			
		}
	}
	controladorTablaRosado.join();
}
