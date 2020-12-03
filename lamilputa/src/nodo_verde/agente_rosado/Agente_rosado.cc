#include "Agente_rosado.h"

//uint16_t nodoSender;
//uint16_t nodoSenderTWH;

int vivo = 1;
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

		std::cout<<"Esto es recibir en enviar enlace en agente rosado"<<std::endl;
		std::cout<<recibir<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;

		std::string auxTabla=recibir;
  		std::stringstream s_stream(recibir);
  		while(s_stream.good()){
   			std::string substr;
   			getline(s_stream, substr, ',');
   			resultado.push_back(substr);
		}
		if(strcmp(resultado[0].c_str(),"44")==0 ||
		strcmp(resultado[0].c_str(),"43")==0){

  			aux=resultado[0]+","+resultado[3];
  			despachadorMiembros->push(aux);

		}else{
  			if(strcmp(resultado[0].c_str(),"70")==0){
				//auxTabla=auxTabla.substr(2);
				//colaTablaForwarding->push(auxTabla);
			} else if(strcmp(resultado[0].c_str(),"45")==0){
    			//aux=resultado[0];
    			//colaAlcanzabilidad->push(aux);
  			}else{
				int nuevoIntIdDestino(std::stoi(resultado[3]));
    			enviar.id_destino_final= static_cast<uint16_t>(nuevoIntIdDestino);
				std::cout<<"Esto es enviar id destino fianl en enviar enlace en agente rosado"<<std::endl;
				std::cout<<enviar.id_destino_final<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

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
				std::cout<<"Esto es tipo en enviar enlace en agente rosado"<<std::endl;
				std::cout<<enviar.tipo<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;

    			//std::string str = resultado[0];
				//enviar.tipo=str[0];
				int nuevoSN(std::stoi(resultado[1]));
				enviar.SN = static_cast<uint16_t>(nuevoSN);
				std::cout<<"Esto es SN en enviar enlace en agente rosado"<<std::endl;
				std::cout<<enviar.SN<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				int nuevoRN(std::stoi(resultado[2]));
				enviar.RN = static_cast<uint16_t>(nuevoRN);
				std::cout<<"Esto es RN en enviar enlace en agente rosado"<<std::endl;
				std::cout<<enviar.RN<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
    			colaDespachadorRosado->push(enviar);
  			}
  		}
  	}
}

void recibirEnlace(Cola<struct ArbolGenerador>* colaRosada,
Cola<std::string>* envioAgenteRosa){

  	int bandera = 1;

  	while(bandera == 1){
		//Seguir trabajnod en esta parte
    	struct ArbolGenerador recibir;
    	recibir = colaRosada->pop();
		std::cout<<"Esto es recibir en recibir enlace en agente rosado"<<std::endl;
		std::cout<<recibir.SN<<std::endl;
		std::cout<<recibir.RN<<std::endl;
		std::cout<<recibir.tipo<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;

		//int num = (int)recibir.tipo - (int)'0';
		std::cout<<"Esto es antes del primer to string"<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;

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

		std::cout<<"Esto es enviar en recibir enlace en agente rosado1111"<<std::endl;
			std::cout<<enviar<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;

		

     	if(strcmp(enviar.c_str(),"-100,-100,-100")==0){
      		bandera = 0;
      		vivo =0;
      		enviar=enviar + ",-100,-100";
      		envioAgenteRosa->push(enviar);
    	} else {
	  		if(strcmp((enviar.substr(0,2)).c_str(),"90")!=0){
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es enviar en recibir enlace en agente rosado antes del to string"<<std::endl;
			std::cout<<enviar<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
		  		enviar=enviar+",-111,"+std::to_string(nodoSenderTWH);
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Esto es enviar en recibir enlace en agente rosado despues del to string"<<std::endl;
			std::cout<<enviar<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
	  		}
			std::cout<<"Esto es enviar en recibir enlace en agente rosado2222"<<std::endl;
			std::cout<<enviar<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;
      		envioAgenteRosa->push(enviar);
    	}
  	}
}


void trabajoAgenteRosado(std::vector<datosNodo>* tablaVecinos,
Cola<struct ArbolGenerador>* colaRosada,
Cola<struct DatosArbolGenerador>* colaDespachadorRosado,char* puerto1,
char* puerto2,Cola<std::string>* despachadorMiembros,
Cola<std::string>* colaAlcanzabilidad,
Cola<std::string>* colaTablaForwarding){

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

        inicializarNodoRosado(numeroNodo,ids,&envioAgenteRosa,
        &colaInicializacion,tablaVecinos);

        hiloCliente.join();
        hiloServer.join();
        enlace1.join();
        enlace2.join();
  	}else{
    	throw std::runtime_error(ERR_CREAR_NR);
  	}
	waitpid(pid,NULL,0);//Se espera por del nodo naranja
}

void inicializarNodoRosado(std::string numeroNodo,
std::vector<indiceNodo> ids,
Cola<std::string>* envioAgenteRosa,
Cola<std::string>* colaInicializacion,
std::vector<datosNodo>* tablaVecinos){

  	std::string confirmacion= "-13,-13,-13,-13.-13";

  	while(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
		
		confirmacion = colaInicializacion->pop();
    	std::string reintentos;
    	reintentos = "0,0,0,0,9";
    	envioAgenteRosa->push(reintentos);
		std::cout<<"Llegue a despues de push"<<std::endl;
		std::cout<<reintentos<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
    	confirmacion = colaInicializacion->pop();

		std::cout<<"Llegue a despues de pop en inicializar nodo rosado"<<std::endl;
		std::cout<<confirmacion<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;

    	if(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
      		std::cout<<"ERROR"<<std::endl;
    	}

  	}
	std::cout<<"Llegue a antes del while en inicializar nodo rosado"<<std::endl;
  	if(stoi(numeroNodo) != 1){

  		while (strcmp(confirmacion.c_str(),
  		"-322,-23E234324,-3242343,-3,-2")!= 0){

    		confirmacion = "-13,-13,-13,-13,-13";
    		confirmacion = colaInicializacion->pop();

  			while(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
    		//lo de la tabla

    			std::string papa = "-2,-2,-2,-2,-2";

    			size_t indice = 0;
				size_t indice2 = 0;
    			int bandera = 1;
				

				while(indice2 <ids.size() && bandera == 1){
					std::cout<<"ESTO ES PRUEBA TABLA AGENTE ROSADO"<<std::endl;
					std::cout<<"ID NODO"<<std::endl;
					std::cout<<ids[indice2].id<<std::endl;
					std::cout<<"ESTADO"<<std::endl;
					int indiceTabla = ids[indice2].indice;
					std::cout<<*(tablaVecinos)[indiceTabla].estado<<std::endl;
					
					
					
		


    
      				
      				indice2++;
    			}

    			
				while(indice <ids.size() && bandera == 1){

      				int indiceTabla = ids[indice].indice;
      				if((*tablaVecinos)[indiceTabla].estado == 1){
        				sleep(5);
        				bandera = 0;
        				papa="0,1,1,1,"+std::to_string(ids[indice].id);
      				}
      				indice++;
    			}
				
				
				//papaFalso
				//papa="0,1,1,1,1";
    			envioAgenteRosa->push(papa);
    			confirmacion = colaInicializacion->pop();

    			if(strcmp(confirmacion.c_str(),"1,1,1,1,1")!=0){
      				std::cout<<"ERROR1"<<std::endl;
    			}
				std::cout<<"Esto es confirmacion"<<std::endl;
				std::cout<<"oooooooooooooooooooooooooo"<<std::endl;
				std::cout<<confirmacion<<std::endl;
				std::cout<<"oooooooooooooooooooooooooo"<<std::endl;

  			}
 		}
	}			
				

				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Sali del if en inicializar nodo rosado"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
}

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
		std::cout<<"Llegue a despues de pop en cliente"<<std::endl;
		std::cout<<msj<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		
		std::cout<<msj.c_str()<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;

      	char const * idLocalChar = msj.c_str();
      	send(newserverSock,idLocalChar,strlen(idLocalChar),0);

    	if(strcmp(msj.c_str(),"-100,-100,-444,-100,-100")==0){
      		bandera = 0;
    	}
  	}
}

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
		std::cout<<"Llegue a despues de read en server TCP agente rosado"<<std::endl;
		std::cout<<buffer<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		
    	std::string ret(buffer, 1024);
    	mensaje = ret;
    	std::vector<std::string> resultado;
    	std::stringstream s_stream(mensaje);
    	while(s_stream.good()) {
      		std::string substr;
      		getline(s_stream, substr, ',');
      		resultado.push_back(substr);
    	}
     	if((strcmp(mensaje.c_str(),"1,1,1,1,1")==0) ||
     	((strcmp(resultado[0].c_str(),"-101")==0)&&
     	(strcmp(resultado[1].c_str(),"-101")==0)&&
     	(strcmp(resultado[2].c_str(),"-101")==0))){

        	colaInicializacion->push(mensaje);

     	}else{
       		recibirAgenteRosa->push(mensaje);
     	}
	}
}
