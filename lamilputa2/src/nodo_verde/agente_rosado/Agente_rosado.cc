#include "Agente_rosado.h"

int vivo = 1;
/**
    @brief Metodo que recibe datos del rosado y los envia a la capa de red
    
    @details Este metodo recibe datos del nodo rosado y se encarga de enviarlos a l capa de red
    @param[in] 
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void enviarEnlace(Cola<struct DatosArbolGenerador >* colaDespachadorRosado,
Cola<std::string>* recibirAgenteRosa,
Cola<std::string>* despachadorMiembros,
Cola<std::string>* colaAlcanzabilidad,
Cola<std::string>* colaTablaForwarding){
  	while(vivo == 1){
    	std::string recibir;
    	struct DatosArbolGenerador enviar;
    	std::string aux;
    	std::vector<std::string> resultado;
    	recibir = recibirAgenteRosa->pop();

		

		std::string auxTabla=recibir;
  		std::stringstream s_stream(recibir);
  		while(s_stream.good()){
   			std::string substr;
   			getline(s_stream, substr, ',');
   			resultado.push_back(substr);
		}//Mensajes para Broadcast
		if(strcmp(resultado[0].c_str(),"44")==0 ||
		strcmp(resultado[0].c_str(),"43")==0||
		strcmp(resultado[0].c_str(),"46")==0 ||
		strcmp(resultado[0].c_str(),"47")==0){

  			aux=resultado[0]+","+resultado[3];
  			despachadorMiembros->push(aux);

		}else{//Alcanzabilidad
  			if(strcmp(resultado[0].c_str(),"70")==0){
				
				auxTabla=auxTabla.substr(3);
				colaTablaForwarding->push(auxTabla);
				
			} else if(strcmp(resultado[0].c_str(),"45")==0){
    			aux=resultado[0];
    			colaAlcanzabilidad->push(aux);
  			}else{
				int nuevoIntIdDestino(std::stoi(resultado[3]));
    			enviar.id_destino_final= static_cast<uint16_t>(nuevoIntIdDestino);
				
				//Tipos de AG
				if(resultado[0][0] == '2'){
					enviar.tipo = 0x02;
					
				}
				else if(resultado[0][0] == '3'){
					enviar.tipo = 0x03;
				}
				else if(resultado[0][0] == '4'){
					enviar.tipo = 0x04;
				}
				else if(resultado[0][0] == '5'){
					enviar.tipo = 0x05;
				}
				else if(resultado[0][0] == '6'){
					enviar.tipo = 0x06;
				}
				else if(resultado[0][0] == '7'){
					enviar.tipo = 0x07;
				}
				

    			
				int nuevoSN(std::stoi(resultado[1]));
				
				int nuevoRN(std::stoi(resultado[2]));
				enviar.RN = static_cast<uint16_t>(nuevoRN);
				
    			colaDespachadorRosado->push(enviar);
  			}
  		}
  	}
}

/**
    @brief Metodo que recibe datos del rosado y los envia al nodo rosado
    
    @details Este metodo recibe datos de la capa de red y los pasa para que sean enviados a nodo rosado
    @param[in] 
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void recibirEnlace(Cola<struct ArbolGenerador>* colaRosada,
Cola<std::string>* envioAgenteRosa){

  	int bandera = 1;

  	while(bandera == 1){
		
    	struct ArbolGenerador recibir;
    	recibir = colaRosada->pop();
		

    	std::string SN=std::to_string(recibir.SN);
		std::string RN=std::to_string(recibir.RN);
		std::string tipo;
		if(recibir.tipo == 0x02){
			tipo = "2";
					
		}
		else if(recibir.tipo == 0x03){
			tipo = "3";
					
		}
		else if(recibir.tipo == 0x04){
			tipo = "4";
					
		}
		else if(recibir.tipo == 0x05){
			tipo = "5";
					
		}
		else if(recibir.tipo == 0x06){
			tipo = "6";
					
		}
		else if(recibir.tipo == 0x07){
			tipo = "7";
					
		}
		
		
    	std::string enviar= tipo +","+ SN+","+ RN;

		

		

     	if(strcmp(enviar.c_str(),"-100,-100,-100")==0){
      		bandera = 0;
      		vivo =0;
      		enviar=enviar + ",-100,-100";
      		envioAgenteRosa->push(enviar);
    	} else {
	  		if(strcmp((enviar.substr(0,2)).c_str(),"90")!=0){
			
		  		enviar=enviar+",-111,"+std::to_string(nodoSenderTWH);
			
	  		}
			
      		envioAgenteRosa->push(enviar);
    	}
  	}
}

/**
    @brief Metodo que envia la alcanzabilidad al nodo rosado
    
    @details Este metodo recibe la alcanzabilidad para enviarlo alnodo rosado para ejecutar dikjstra
    @param[in] 
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void enviarAlcanzabilidad2(Cola<std::string> *colaEnviarAlcanzabilidad,Cola<std::string>* envioAgenteRosa){

std::string mensajeAlcanzabilidad;
	while(1){
		mensajeAlcanzabilidad=colaEnviarAlcanzabilidad->pop();
		envioAgenteRosa->push(mensajeAlcanzabilidad);

	}





}

/**
    @brief Metodo que genera al nodo rosado e hilos para diferentes funciones
    @details Este metodo se encarga de generar al nodo rosado e hilos para manejar diferentes fuciones
    @param[in] 
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void trabajoAgenteRosado(std::vector<datosNodo>* tablaVecinos,
Cola<struct ArbolGenerador>* colaRosada,
Cola<struct DatosArbolGenerador>* colaDespachadorRosado,char* puerto1,
char* puerto2,Cola<std::string>* despachadorMiembros,
Cola<std::string>* colaAlcanzabilidad,
Cola<std::string>* colaTablaForwarding,Cola<std::string> *colaEnviarAlcanzabilidad){

  	Cola<std::string> recibirAgenteRosa;
  	Cola<std::string> envioAgenteRosa;

  	std::string numeroNodo = std::to_string(tablaVecinos[0][0].ID);

  	int pid = fork();//Se crea el nodo rosado
  	if(pid == 0){
      	execlp(INTERPRETADOR,INTERPRETADOR,
      	CAMINO_NODO_ROSADO,numeroNodo.c_str(),puerto1,puerto2,
      	(char*)NULL);
      	exit(1);
  	}else if(pid > 0){//Agente_rosado

	  
    	size_t i,j;
    	std::vector<indiceNodo> ids;
    	indiceNodo nodoId, pivote;
    	for(i=1; i<tablaVecinos->size(); i++){
      		nodoId.id = (*tablaVecinos)[i].ID;
      		nodoId.indice = i;
      		ids.push_back(nodoId);
    	}

    	for(i=0; i<ids.size()-1; i++){
      		for(j=0; j<ids.size()-i-1; j++){
        		if(ids[j].id > ids[j+1].id){
          			pivote = ids[j];
          			ids[j] = ids[j+1];
          			ids[j+1] = pivote;
        		}
      		}
    	}
		

        Cola<std::string> colaInicializacion;

        std::thread hiloCliente;

        hiloCliente = std::thread(clienteTCP,&envioAgenteRosa,
        atoi(puerto1));
        std::thread hiloServer;

        hiloServer = std::thread(serverTCP,&recibirAgenteRosa,
        &colaInicializacion,atoi(puerto2));

        std::thread enlace1;
        enlace1=std::thread(enviarEnlace,colaDespachadorRosado,
        &recibirAgenteRosa,despachadorMiembros,colaAlcanzabilidad,
        colaTablaForwarding);

        std::thread enlace2;
        enlace2=std::thread(recibirEnlace,colaRosada,&envioAgenteRosa);

		
		std::thread hiloAlcanzabilidad2(enviarAlcanzabilidad2,colaEnviarAlcanzabilidad,&envioAgenteRosa);


        inicializarNodoRosado(numeroNodo,ids,&envioAgenteRosa,
        &colaInicializacion,tablaVecinos);

        hiloCliente.join();
        hiloServer.join();
        enlace1.join();
        enlace2.join();
		hiloAlcanzabilidad2.join();
  	}else{
    	throw std::runtime_error(ERR_CREAR_NR);
  	}
	waitpid(pid,NULL,0);//Se espera por del nodo naranja
}

/**
    @brief Metodo que genera informacion para inicializar el nodo rosado
    @details Este metodo se encarga de generar informacion para que se pueda inicializar el nodo rosado
    @param[in] 
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void inicializarNodoRosado(std::string numeroNodo,
std::vector<indiceNodo> ids,
Cola<std::string>* envioAgenteRosa,
Cola<std::string>* colaInicializacion,
std::vector<datosNodo>* tablaVecinos){

  	std::string confirmacion= "-13,-13,-13,-13.-13";

  	while(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
		//Se manda cantidad de reintentos al nodo rosado
		confirmacion = colaInicializacion->pop();
    	std::string reintentos;
    	reintentos = "0,0,0,0,9";
    	envioAgenteRosa->push(reintentos);
		
    	confirmacion = colaInicializacion->pop();

		

    	if(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
      		//2WH con el nodo rosado
			std::cout<<"ERROR"<<std::endl;
    	}

  	}
	
  	if(stoi(numeroNodo) != 1){
		

  		while (true){

    		confirmacion = "-13,-13,-13,-13,-13";
    		confirmacion = colaInicializacion->pop();

  			while(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
    		//lo de la tabla

    			std::string papa = "-2,-2,-2,-2,-2";

    			size_t indice = 0;
				size_t indice2 = 0;
    			int bandera = 1;
				

				while(indice2 <ids.size() && bandera == 1){
					//Se muestra la tabla de los vecinos
					int indiceTabla = ids[indice2].indice;
					std::cout<<(*tablaVecinos)[indiceTabla].estado<<std::endl;
					
					
					
		


    
      				
      				indice2++;
    			}

    			
				while(indice <ids.size() && bandera == 1){

      				int indiceTabla = ids[indice].indice;
      				if((*tablaVecinos)[indiceTabla].estado == 1){
        				//Se envía un potencial papá
						sleep(5);
        				bandera = 0;
        				papa="0,1,1,1,"+std::to_string(ids[indice].id);
      				}
      				indice++;
    			}
				
				
				
    			envioAgenteRosa->push(papa);
    			confirmacion = colaInicializacion->pop();

    			if(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
      				std::cout<<"ERROR1"<<std::endl;
    			}
				

  			}
	        
			int bandera3 = 0;
			confirmacion = "-13,-13,-13,-13,-13";
			confirmacion = colaInicializacion->pop();
			if(strcmp(confirmacion.c_str(),"1,1,1,1,1")==0){
      			std::cout<<"BANDERA3"<<std::endl;
				  bandera3 = 1;
				  //registro del nodo rosado para ver si algo salió mal 
				  //en el 3WH
	           
			}
			if(bandera3 == 1){
				int bandera4 = 0;
				confirmacion = "-13,-13,-13,-13,-13";
				confirmacion = colaInicializacion->pop();
				if(strcmp(confirmacion.c_str(),"1,1,1,1,1")==0){
      				std::cout<<"BANDERA4"<<std::endl;
				 	 bandera4 = 1;
					  //registro del nodo rosado para ver si algo salió mal 
				  //en el 3WH
	           
				}
				if(bandera4 == 1){
					confirmacion = "-13,-13,-13,-13,-13";
					
					int bandera2 = 0;
					while(bandera2==0){
						size_t indice = 0;
						while(indice <ids.size() && bandera2 == 0){

      						int indiceTabla = ids[indice].indice;
      						
							  
							if((*tablaVecinos)[indiceTabla].estado == 0){
        						std::cout<<"MUERTE DE UN VECINO"<<std::endl;
								
								std::cout<<(*tablaVecinos)[indiceTabla].estado<<std::endl;
								sleep(2);
        						bandera2 = 0;
        						std::string miembroMuerto="8,8,8,8,"+std::to_string(ids[indice].id);
								//Se le manda al nodo rosaso un vecino muestro para que lo verifique
								envioAgenteRosa->push(miembroMuerto);
								confirmacion = colaInicializacion->pop();

								if (strcmp(confirmacion.c_str(),"9,9,9,9,9")==0){
									//Si el muerto fue un papá, vuelva a reiniciar la inicialización
									//Con el nodo rosado
									bandera2 = 1;
								}
      						}
      						indice++;
    					}
					}
				}
				

				
			}
			
 		
		 }
		sleep(5);
	}			
				

				
}

/**
    @brief Metodo que envia inormacion l nodo rosado por medio de un socket
    @details Este metodo se encarga de enviar informacion al nodo rosado por medio de un socket
    @param[in] Recibe el puerto
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void clienteTCP(Cola<std::string>* envioAgenteRosa,int puerto1){

  	int serverSock=socket(AF_INET, SOCK_STREAM, 0);

  	sockaddr_in serverAddr;
  	sockaddr_in clieAddr;
  	bzero((char *) &serverAddr, sizeof(serverAddr));

  	serverAddr.sin_family = AF_INET;
  	serverAddr.sin_port = htons(puerto1);
  	serverAddr.sin_addr.s_addr = INADDR_ANY;

  	bind(serverSock, (struct sockaddr*)&serverAddr,sizeof(serverAddr));

  	listen(serverSock,5);

  	int len=sizeof(clieAddr);

  	int newserverSock=accept(serverSock,(struct sockaddr*)&clieAddr,
  	(socklen_t*)&len);

  	int bandera = 1;
  	while(bandera == 1){
    	std::string msj = envioAgenteRosa->pop();
		

      	char const * idLocalChar = msj.c_str();
      	send(newserverSock,idLocalChar,strlen(idLocalChar),0);

    	if(strcmp(msj.c_str(),"-100,-100,-444,-100,-100")==0){
      		bandera = 0;
    	}
  	}
}

/**
    @brief Metodo que recibe informacion que es enviada desde el rosado ppr medio de un socket
    @details Este metodo se encarga de recibir datos en un socket que fueron enviados desde el agente rosado
    @param[in] Recibe el puerto
    @param[out] Recibe colas de datos para manejar informacion
    @pre Que las colas esten correctamente inicializadas
    @remark Se modifica las colas de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 Juan Jose Herrera B83878
    @date 4-12-2020
*/
void serverTCP(Cola<std::string>* recibirAgenteRosa,
Cola<std::string>* colaInicializacion,int puerto2){

  	int serverSock=socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock==-1) {
    	perror("Create socket");
	}

  	ioctl(serverSock, FIONBIO, 0);

  	struct sockaddr_in serverAddr;
  	serverAddr.sin_family = AF_INET;
  	serverAddr.sin_port = htons(puerto2);
  	serverAddr.sin_addr.s_addr = INADDR_ANY;
  	int largo=sizeof(serverAddr);

  	bind(serverSock, (struct sockaddr*)&serverAddr,
  	sizeof(struct sockaddr));

  	listen(serverSock,1);
  	int newserverSock=accept(serverSock,(struct sockaddr*)&serverAddr,
  	(socklen_t*)&largo);

  	ioctl(newserverSock, FIONBIO, 0);

  	while (vivo == 1){
    	std::string mensaje;
    	char buffer [1024];
    	bzero(&buffer, 1024);
    	read(newserverSock,buffer, 1024); //n =
		
		
    	std::string ret(buffer, 1024);
    	mensaje = ret;
    	std::vector<std::string> resultado;
    	std::stringstream s_stream(mensaje);
    	while(s_stream.good()) {
      		std::string substr;
      		getline(s_stream, substr, ',');
      		resultado.push_back(substr);
    	}
		//Mensajes para cola de inicialización
     	if((strcmp(mensaje.c_str(),"1,1,1,1,1")==0) ||
		 (strcmp(mensaje.c_str(),"9,9,9,9,9")==0) ||
		 (strcmp(mensaje.c_str(),"10,10,10,10,10")==0) ||
     	((strcmp(resultado[0].c_str(),"-101")==0)&&
     	(strcmp(resultado[1].c_str(),"-101")==0)&&
     	(strcmp(resultado[2].c_str(),"-101")==0))){

        	colaInicializacion->push(mensaje);

     	}else{
			 //Mensajes para otros nodos
       		recibirAgenteRosa->push(mensaje);
     	}
	}
}

