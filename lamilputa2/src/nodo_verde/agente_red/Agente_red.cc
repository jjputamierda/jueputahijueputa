#include "Agente_red.h"

struct datosForwarding{//estructura para guardar datos para la tabla de forwarding
	uint16_t idNodo;
	uint16_t distancia;
	uint16_t camino;
};
//uint16_t nodoSender;
uint16_t nodoSenderTWH;
std::vector<struct datosForwarding> tablaForwarding;//vector para guardar los datos de forwarding

/*
    @brief Metodo que se encarga de inicializar los diferentes agentes de la capa de red
    @details Este metodo se encarga de generar hilos para que se ejecuten los diefentes agentes de la capa de red
    @param[in] N/A
    @param[out] Recibe colas de datos y una vairable para pasar datos entre los diferentes agentes del sistema
    @pre Que las colas sean correctamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
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

	Cola<struct DatosForwarding> colaForwarding;//cola para madenar datos de forwarding
	Cola<struct Broadcast> colaBroadcast;//cola para manejar datos de broadcast
	
	
	
	std::thread hiloAzul(despachadorAzul,
		colaDespachadorAzul, &colaForwarding, &colaBroadcast, 
		(*nodosIDs)[0]);//hilo que ejecuta despachador azul
	
	std::thread hiloRosado(despachadorRosado,colaDespachadorRosado,
		nodosIDs, tablaVecinos,
		colasVerdes);//hilo que ejecuta el despechador rosado
		

	std::thread hiloVerde(despachadorVerde,
	 	colaDespachadorVerde, &colaForwarding, &colaBroadcast,colaRosada,colaAzul);//hilo que ejecuta el desoachador verde
	
	std::thread hiloForwarding(forwarding, colaAzul,
		colasVerdes, &colaForwarding, nodosIDs);//hilo que ejecuta el metodo de forwarding

	std::thread hiloBroadcast(broadcast, colaAzul, colaRosada,
		colasVerdes, &colaBroadcast, nodosIDs,despachadorMiembros,
		colaAlcanzabilidad,colaEnviarAlcanzabilidad);//hilo que ejecuta el meodo de broadcast
		
    
	std::thread hiloAlcanzabilidad(enviarAlcanzabilidad,
	colaAlcanzabilidad,tablaVecinos,
	IP,&colaBroadcast,colaEnviarAlcanzabilidad);//hilo que ejecuta el metodo para enviar la alcanzabilidad
	
	std::thread hiloTablaForwarding(verificarTablaForwarding,
	colaTablaForwarding, nodosIDs);//hilo que actualiza la tabla de forwarding
	
	hiloAzul.join();
	hiloRosado.join();
	hiloVerde.join();
	hiloForwarding.join();
	hiloBroadcast.join();
	hiloAlcanzabilidad.join();
}
/**
    @brief Metodo que se encarga de actualizar la tabla de forwarding
    @details Este metodose encarga de actualizar la tabla de forwarding
    @param[in] N/A
    @param[out] Recibe una cola para menjar datos y un vector con la informacion de los nodos
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/

void verificarTablaForwarding(Cola<std::string>* colaTablaForwarding,
std::vector<int>* nodosIDs){

	std::string recibirTabla;
	std::string aux;

	while(1){
		recibirTabla=colaTablaForwarding->pop();//recibeinformacion para actaulizar la tabla

		std::stringstream s_stream(recibirTabla);
		std::vector<std::string> resultado;
		while(s_stream.good()) {
		std::string substr;
		getline(s_stream, substr, ',');
		resultado.push_back(substr);
		}
		
		
		std::cout<<resultado.size()<<std::endl;
		for(size_t indice=0;indice<resultado.size();indice++){
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
			
			tablaForwarding.push_back(nuevoDato);
			
		}
	}
}

/**
    @brief Metodo que se encarga de generar la alcanzabilidad
    @details Este metodo se encarga de genrar una soliitud de alcanzabilidad
    @param[in] N/A
    @param[out] Recibe colas para menjar datos,un vector con la informacion de los nodos y un variable con el valor de la IP
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/

void enviarAlcanzabilidad(Cola<std::string>* colaAlcanzabilidad,
std::vector<datosNodo>* tablaVecinos,
char* IP,Cola<struct Broadcast>* colaBroadcast,Cola<std::string>* colaEnviarAlcanzabilidad){

	
	std::string aux;
	std::string mensajeAlcanzabilidad;

	size_t indice;
	char host[256];
	

	while(1){
		mensajeAlcanzabilidad="";

		bzero(host,sizeof(host));
		mensajeAlcanzabilidad += "90,";//se genera solictud de alcanzabilidad interna
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
		
		colaEnviarAlcanzabilidad->push(aux);

		std::string estadoArbol=colaAlcanzabilidad->pop();//notificacion para enviar alcanzabilidad
		uint16_t prueba;
		uint16_t prueba2;
		uint8_t prueba3;
		if(estadoArbol=="45"){
			char buffer[200];
			uint16_t aux1=static_cast<uint16_t>(tablaVecinos->size()-1);
			
			memmove( buffer,&aux1 ,sizeof(aux1));

			memmove( &prueba,buffer ,sizeof(prueba));
			
			size_t offset = sizeof(aux1);
			for(indice=1;indice<tablaVecinos->size();indice++){
				uint16_t aux2=static_cast<uint16_t>((*tablaVecinos)[indice].ID);
				
				memmove( buffer+offset,&aux2 ,sizeof(aux2));
				memmove( &prueba2,buffer+offset ,sizeof(prueba2));
				

				offset=offset+sizeof(aux2);
				uint8_t aux3;
				if(((*tablaVecinos)[indice].IP).compare(IP)==0){
				
				aux3=0x01;
				
			
			if(aux3 == 0x01){
				
			}
		
			
				memmove( buffer+offset,&aux3 ,sizeof(aux3));
				memmove( &prueba3,buffer+offset ,sizeof(prueba3));//se utiliza para revisr
				if(prueba3 == 0x01){
				std::cout<<"0x01 recuperado"<<std::endl;
			}
				
			offset=offset+sizeof(aux3);
			

				}else{
				
			if(aux3 == 0x02){
				std::cout<<"0x02"<<std::endl;
			}
			
				memmove( buffer+offset,&aux3 ,sizeof(aux3));
				memmove( &prueba3,buffer+offset ,sizeof(prueba3));//esto es para revisar
				if(prueba3 == 0x02){
				std::cout<<"0x02 recuperado"<<std::endl;
			    }
				offset=offset+sizeof(aux3);
				

			}
				
			}
			
			struct Broadcast broadcast;
			broadcast.tipo = 0x02;
			broadcast.id_origen_inicial = static_cast<uint16_t>((*tablaVecinos)[0].ID);
			memmove( &broadcast.datos,buffer ,sizeof(broadcast.datos));
			broadcast.longitud=sizeof(broadcast.datos);
			colaBroadcast->push(broadcast);
		}
	}
}


/**
    @brief Metodo que recibe los mensajes de la capa de aplicaion y los redirecciona a donde correspone
    @details Este metodo se encarga de recibir los mensajes que se generan en la capa de aplicaion y los redirecciona a donde corresponde 
    @param[in] N/A
    @param[out] Recibe colas para menjar datos y el id del nodo
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void despachadorAzul(Cola<struct DatosMensaje>* colaDespachadorAzul,
		Cola<struct DatosForwarding>* colaForwarding,
		Cola<struct Broadcast>* colaBroadcast, int idPropio){

	while(1){
		struct DatosMensaje datos = colaDespachadorAzul->pop();//recibimos mensaje de capa de aplicacion
		struct Mensaje mensaje;
		strcpy(mensaje.buffer, datos.buffer); 

		if(datos.tipo == 0x01){//mensaje va para forwrding
			char buffer3 [200];
			struct DatosForwarding forwarding;
			 
			forwarding.idFuenteInicial = static_cast<uint16_t>(idPropio);
			forwarding.idDestino = datos.idDestino;
			forwarding.longitud = sizeof(datos.buffer);
			memmove( buffer3,&mensaje.buffer ,sizeof(mensaje.buffer));
			memmove( &forwarding.datos,buffer3 ,sizeof(forwarding.datos));
			
			
			colaForwarding->push(forwarding);
			
		} else if(datos.tipo == 0x02){
			char buffer3 [200];
			struct Broadcast broadcast;
			broadcast.tipo = 0x01;
			
			broadcast.id_origen_inicial = static_cast<uint16_t>(idPropio);
			memmove( buffer3,&mensaje.buffer ,sizeof(mensaje.buffer));
			memmove( &broadcast.datos,buffer3 ,sizeof(broadcast.datos));
			
			
			broadcast.longitud = sizeof(datos.buffer);
			colaBroadcast->push(broadcast);

		} else {
			std::cout << "Error!\n";
		}
	}
}

/**
    @brief Metodo que recibe las solicitudes del arbol y las envia
    @details Este metodo se encarga de recibir las solicitudes que genera el arbol y la redirecciona para que sean enviadas
    @param[in] N/A
    @param[out] Recibe colas para menjar datos y un vector con la informacion de los vecinos
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void despachadorRosado(Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
std::vector<int>* nodosIDs,std::vector<datosNodo>* tablaVecinos,std::vector<Cola<struct CapaEnlace>>* colasVerdes){

	while(1){
		struct DatosArbolGenerador datos = colaDespachadorRosado->pop();

		size_t longitud = nodosIDs->size();
				
            for(size_t i = 0; i < longitud; ++i){
				
				
                if(datos.id_destino_final == (*nodosIDs)[i]){
					char buffer [1017];
					

                    struct ArbolGenerador paqueteAg;

                    paqueteAg.tipo = datos.tipo;

					paqueteAg.SN = datos.SN;
					paqueteAg.RN =  datos.RN;

					
					struct CapaRed capaRed;
					capaRed.tipo = 0x03;

				    memmove(buffer, &paqueteAg.tipo, sizeof(paqueteAg.tipo));
					memmove(buffer+sizeof(paqueteAg.tipo), &paqueteAg.SN, sizeof(paqueteAg.SN));
					memmove(buffer+sizeof(paqueteAg.tipo)+sizeof(paqueteAg.SN), &paqueteAg.RN, sizeof(paqueteAg.RN));
					memmove( &capaRed.datos,buffer ,sizeof(capaRed.datos));
					

					capaRed.longitud =  sizeof(capaRed.datos);
					
					struct CapaEnlace paquete;

					

					paquete.tipo =0X02;
					paquete.idDestinoFinal = datos.id_destino_final;
					paquete.idFuenteInmediato = (*nodosIDs)[0];
					char buffer2[1040];
					memmove(buffer2, &capaRed.tipo, sizeof(capaRed.tipo));
					memmove(buffer2+sizeof(capaRed.tipo), &capaRed.longitud, sizeof(capaRed.longitud));
					memmove(buffer2+sizeof(capaRed.tipo)+sizeof(capaRed.longitud), &capaRed.datos, sizeof(capaRed.datos));
					memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
					paquete.longitud = sizeof(paquete.datos);


					
                    
					
	

                    colasVerdes[0][i].push(paquete);

                
				}

            }

		
		

	}
}

/**
    @brief Metodo que recibe los datos de capa de enlace y los redirecciona a donde corresponde
    @details Este metodo se encarga de recibir los datos de capa de enlace  y la redirecciona al agente que los nececita
    @param[in] N/A
    @param[out] Recibe colas para menjar datos 
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/

void despachadorVerde(Cola<struct CapaRed>* colaDespachadorVerde,
	Cola<struct DatosForwarding>* colaForwarding,
	Cola<struct Broadcast>* colaBroadcast,Cola<struct ArbolGenerador>* colaRosada,Cola<struct Mensaje>* colaAzul ){
	while(1){
		struct CapaRed capaRed =
		colaDespachadorVerde->pop();
			nodoSenderTWH=nodoSender;
		if(capaRed.tipo== 0x01){//mensaje va para forwarding

			
			
			struct DatosForwarding fowardingRecibir;
			struct Mensaje mensaje;
			
			

				memmove( &fowardingRecibir.idFuenteInicial,capaRed.datos ,sizeof(fowardingRecibir.idFuenteInicial));
     			memmove( &fowardingRecibir.longitud, capaRed.datos+ sizeof(fowardingRecibir.idFuenteInicial),sizeof(fowardingRecibir.longitud));
    			memmove( &fowardingRecibir.datos, capaRed.datos + sizeof(fowardingRecibir.idFuenteInicial)+sizeof(fowardingRecibir.longitud) ,sizeof(fowardingRecibir.datos));
				fowardingRecibir.idDestino = destinoFinal;
				
				colaForwarding->push(fowardingRecibir);
		} 
		else if (capaRed.tipo== 0x02){//mensaje va para broadcast
			
			    
			struct Broadcast nuevoBroadcast;
			memmove( &nuevoBroadcast.tipo,capaRed.datos,sizeof(nuevoBroadcast.tipo));
			memmove( &nuevoBroadcast.id_origen_inicial,capaRed.datos+sizeof(nuevoBroadcast.tipo), sizeof(nuevoBroadcast.id_origen_inicial));
			memmove( &nuevoBroadcast.longitud,capaRed.datos+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial),sizeof(nuevoBroadcast.longitud));
			memmove( &nuevoBroadcast.datos, capaRed.datos+sizeof(nuevoBroadcast.tipo)+sizeof(nuevoBroadcast.id_origen_inicial)+sizeof(nuevoBroadcast.longitud),sizeof(nuevoBroadcast.datos));
		
			
			colaBroadcast->push(nuevoBroadcast);
			
		}
		else if (capaRed.tipo== 0x03){//mensaje va para arbol
			
			
			struct ArbolGenerador paqueteAg;
			memmove( &paqueteAg.tipo, capaRed.datos ,sizeof(paqueteAg.tipo));
			memmove( &paqueteAg.SN, capaRed.datos +sizeof(paqueteAg.tipo),sizeof(paqueteAg.SN));
			memmove( &paqueteAg.RN, capaRed.datos +sizeof(paqueteAg.tipo)+sizeof(paqueteAg.SN),sizeof(paqueteAg.RN));
			nodoSenderTWH=nodoSender;
			
			colaRosada->push(paqueteAg);
			
		}  
	
		
	}
}

/**
    @brief Metodo que recibe los paquetes de forwarding y lo envia a donde corresponde
    @details Este metodo se encarga de recibir los paquetes que van para forwarding y los redireciona a conde correspone
    @param[in] N/A
    @param[out] Recibe colas para menjar datos y vector con la tabla de nodos
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void forwarding(Cola<struct Mensaje>* colaAzul,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosForwarding>* colaForwarding,
	std::vector<int>* nodosIDs){
		bool condicon=false;

	while(1){
		char buffer[1017];
	
		struct Mensaje mensaje;
		char buffer2[1040];
		char buffer3 [200];
		
		struct DatosForwarding datosForwarding = colaForwarding->pop();
		
		if(datosForwarding.idDestino == (*nodosIDs)[0]){
			char buffer4 [200];
			memmove( buffer4,&datosForwarding.datos ,sizeof(datosForwarding.datos));
			memmove( &mensaje.buffer,buffer4 ,sizeof(mensaje.buffer));
			colaAzul->push(mensaje);
			


		} else {
			
			
			size_t i = 0;
			
			while(i<tablaForwarding.size() && condicon==false){
				
				if(datosForwarding.idDestino==tablaForwarding[i].idNodo){
					
					struct Forwarding forwardingEnviar;
					
					forwardingEnviar.idFuenteInicial=datosForwarding.idFuenteInicial;
					forwardingEnviar.longitud=sizeof(datosForwarding.datos);
					
				
					
					memmove (buffer3,&datosForwarding.datos,sizeof(datosForwarding.datos) );
					memmove( &forwardingEnviar.datos,buffer3 ,sizeof(forwardingEnviar.datos));
					for(size_t j=0; j<(*nodosIDs).size(); j++){

						
						
						if((*nodosIDs)[j]==tablaForwarding[i].camino){
							
						
							memmove(buffer, &forwardingEnviar.idFuenteInicial, sizeof(forwardingEnviar.idFuenteInicial));
							memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial), &(forwardingEnviar.longitud), sizeof(forwardingEnviar.longitud));
							memmove(buffer+sizeof(forwardingEnviar.idFuenteInicial)+sizeof(forwardingEnviar.longitud), &(forwardingEnviar.datos), sizeof(forwardingEnviar.datos));
							struct CapaRed	capaRed;
							capaRed.tipo=0x01;

							capaRed.longitud=sizeof(buffer);
							
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
				
				i++;
			}
			condicon=false;
			


		}
	}
}

/**
    @brief Metodo que actualiza la tabla de broadcast con los miembros del arbol
    @details Este metodo se encarga de actualizar la tabla de forwarding con los diferentes miebros del arbol
    @param[in] N/A
    @param[out] Recibe colas para menjar datos
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
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
					
					miembrosArbol->push_back(aux);

				}
			}
		}



		
		
	}
}


/**
    @brief Metodo que genera el broadcast
    @details Este metodo se encarga de actualizar la tabla de forwarding con los diferentes miebros del arbol
    @param[in] N/A
    @param[out] Recibe colas para menjar datos y un vector con los datos de los nodos
    @pre Que las colas esten corretamente inicializadas
    @remark N/A
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
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
		

		if(nuevoBroadcast.id_origen_inicial== (*nodosIDs)[0]){
			
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
							
							colasVerdes[0][indice].push(paquete);
						}
					}
				}
			
		} else {

			
			if(nuevoBroadcast.tipo == 0x01){//mensaje aplicacion
				
				struct Mensaje nuevoMensaje;
				char buffer3 [200];
				memmove( buffer3,&nuevoBroadcast.datos ,sizeof(nuevoBroadcast.datos));
				memmove( &nuevoMensaje.buffer,buffer3 ,sizeof(nuevoMensaje.buffer));
				
				
				colaAzul->push(nuevoMensaje);
			} else if(nuevoBroadcast.tipo==0x02){//mensaje alcanzabilidad
				if(enviarDenuevo==true){
					colaAlcanzabilidad->push("45");
					enviarDenuevo=false;
				
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
					uint8_t distancia;
					memmove( &distancia,buffer3+offset ,sizeof(distancia));
					offset=offset+sizeof(distancia);
					if(distancia==0x01){
						
					char buffer[80];
   					int buffer_len = sprintf(buffer, "%d", idVecino);
					   std::string str(buffer, buffer + buffer_len );
    					
						mensajeAlcanzabilidad+=str+","+"1";
					}else if(distancia==0x02){
						
					char buffer[80];
   					int buffer_len = sprintf(buffer, "%d", idVecino);
					   std::string str(buffer, buffer + buffer_len );
    					
						mensajeAlcanzabilidad+=str+","+"2";

					}
					
					

				}
				
				
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

