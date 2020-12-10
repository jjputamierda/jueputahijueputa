#include "Agente_verde.h"
pthread_mutex_t lock;
bool terminar = false;
uint16_t nodoSender;
uint16_t destinoFinal;
short destino;
//uint16_t nodoSenderTWH;

/**
    @brief Metodo que se encarga de generar un socket para recibir los
    mensajes de los vecinos
    @details Este metodo se encarga de generar un socket para recibir
    los mensajes de los vecinos y luego pasarlos a la cola de mensajes
    para que el cliente lo reciba
    @param[in] el metodo recibe un vector de un struct con los datos
    para generar el socket
    @param[out] Recibe una vector de cola de mensajes y se usa para el
    paso de mensajes
    @pre Que el vector datosNodo tenga datos
    @remark Se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void toCharArray(char * paquete_sal, CapaEnlace * paquete){
    int offset = 0;
    memmove(paquete_sal, &paquete->tipo, sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
     memmove(paquete_sal+offset, &paquete->idDestinoFinal, sizeof(paquete->idDestinoFinal));
    offset+= sizeof(paquete->idDestinoFinal);
    memmove(paquete_sal+offset, &paquete->idFuenteInmediato, sizeof(paquete->idFuenteInmediato));
    offset+= sizeof(paquete->idFuenteInmediato);
     memmove(paquete_sal+offset, &paquete->longitud, sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove(paquete_sal+offset, paquete->datos, paquete->longitud);


     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);
}

void toCharArray2(char * paquete_ent, CapaEnlace * paquete){
    int offset = 0;
    memmove( &paquete->tipo,paquete_ent ,sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
     memmove( &paquete->idDestinoFinal, paquete_ent+offset,sizeof(paquete->idDestinoFinal));
    offset+= sizeof(paquete->idDestinoFinal);
    memmove( &paquete->idFuenteInmediato, paquete_ent+offset,sizeof(paquete->idFuenteInmediato));
    offset+= sizeof(paquete->idFuenteInmediato);
     memmove( &paquete->longitud, paquete_ent+offset,sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove( paquete->datos, paquete_ent+offset,paquete->longitud);
    
     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);

}
void toCharArrayRed(char * paquete_ent, CapaRed * paquete){
    int offset = 0;
    memmove( &paquete->tipo,paquete_ent ,sizeof(paquete->tipo));
    offset+= sizeof(paquete->tipo);
     memmove( &paquete->longitud, paquete_ent+offset,sizeof(paquete->longitud));
    offset+= sizeof(paquete->longitud);
    memmove( paquete->datos, paquete_ent+offset,paquete->longitud);
    
     //memmove(paquete_sal, &paquete->latido, sizeof(paquete->latido));
    //offset+= sizeof(paquete->latido);

}

char * parserBuffer(char * buffer, size_t parseLimit,size_t  initParse,size_t newBufferSize){
    char * newBuffer = new char [newBufferSize];
    for(size_t i = initParse; i< parseLimit; i++){
        newBuffer[i] = buffer[i];
    }
    return newBuffer;
}
void recibirV(std::vector<datosNodo>* tabla,
Cola<struct CapaRed>* colaDespachadorVerde,Cola<struct Latido> *colaLatido){

    int n;
    char buffer [1047];
     
    unsigned int sock, length, fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    //struct CapaEnlace paquete;
    //struct CapaRed capaRed;
    struct CapaEnlace paquete;
    struct CapaRed capaRed;

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    length = sizeof(server);

   // fd_set lectura;
   // fcntl(sock, F_SETFL, O_NONBLOCK);
   // struct timeval tv;

  //  FD_ZERO(&lectura);
    //FD_SET(sock, &lectura);

   // tv.tv_sec = 0;
   // tv.tv_usec = 1;

   // int rv;

    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(tabla[0][0].puerto);
    if (bind(sock,(struct sockaddr *)&server,length)<0){
        std::cout << "Error en binding" << std::endl;
    }

    fromlen = sizeof(struct sockaddr_in);

    uint16_t idVecino;
    size_t i, indice;
   
    while(!terminar){
    
    

       // tv.tv_usec = 1;
       // rv = select(sock+1, &lectura, NULL, NULL, &tv);
        //Meter esto dentro del n> 0
        //if(rv == 0){
            n = recvfrom(sock, buffer, 1047, 0,
                (struct sockaddr *)&from, &fromlen);
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Hola xddddddddddd"<<std::endl;
                for(int i = 0; i < 444; i ++){
                    std::cout<<buffer[i];
                }
                std::cout<<std::endl;
                memmove( &(paquete.tipo), buffer,sizeof(paquete.tipo));
                memmove( &(paquete.idDestinoFinal),buffer+sizeof(paquete.tipo) , sizeof(paquete.idDestinoFinal));
                memmove(&(paquete.idFuenteInmediato), buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal), sizeof(paquete.idFuenteInmediato));
                memmove( &(paquete.longitud),buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal)+sizeof(paquete.idFuenteInmediato), sizeof(paquete.longitud));
                memmove( &(paquete.datos), buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal)+sizeof(paquete.idFuenteInmediato)+sizeof(paquete.longitud),paquete.longitud);
               
               // toCharArray2(buffer, &paquete);
                //int tipo=atoi(parserBuffer( buffer, 1,0,1));
                //paquete.tipo=static_cast<uint8_t>(tipo);
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Aqui llega un mensaje de broadcast"<<std::endl;
                //std::cout<<paquete.payload.payload.payloadBroadcast.payloadMensaje.mensaje<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<paquete.tipo<<std::endl;
                std::cout<<paquete.idDestinoFinal<<std::endl;
                std::cout<<paquete.idFuenteInmediato<<std::endl;
                std::cout<<paquete.longitud<<std::endl;
                //std::cout<<paquete.datos<<std::endl;
                std::cout<<"termina recepcion enlace"<<std::endl;
				std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
            
            /*
            if(paquete.tipo == RED){
                struct CapaRed capaRed;
                capaRed.tipo = paquete.tipo;
                capaRed.longitud = paquete.longitud;
            }
            */
        //}

       // if(n > 0){
            if(paquete.tipo == 0x02){
                for(size_t i = 0; i < tabla[0].size(); i++){
                    if(tabla[0][i].ID == static_cast<short>(paquete.idFuenteInmediato)){
                        pthread_mutex_lock(&lock);
                        tabla[0][i].tiempoExpiracion = 3000;
                        pthread_mutex_unlock(&lock);
                    }
                }
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Aqui llega un mensaje de broadcast"<<std::endl;
                //std::cout<<paquete.payload.payload.payloadForwarding.payload.buffer<<std::endl;
               // std::cout<<paquete.payload.payload.payloadBroadcast.payloadMensaje.mensaje<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes id fuente final e inmediato"<<std::endl;
                std::cout<<paquete.idDestinoFinal<<std::endl;
                std::cout<<paquete.idFuenteInmediato<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de nodo sender en elance"<<std::endl;
                std::cout<<"Esto antes de nodo sender en elance"<<std::endl;
                std::cout<<"Esto antes de nodo sender en elance"<<std::endl;
                std::cout<<"Esto antes de nodo sender en elance"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                nodoSender = paquete.idFuenteInmediato;
                destinoFinal =paquete.idDestinoFinal;
                std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de capaRed en enlace"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                //capaRed.tipo = paquete.payload.tipo;
                std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de capaRed en enlace1"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace1"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace1"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace1"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
               // capaRed.longitud=paquete.payload.longitud;
                std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de capaRed en enlace2"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace2"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace2"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace2"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
               // capaRed.payload.payloadForwarding.idFuenteInicial=paquete.payload.payload.payloadForwarding.idFuenteInicial;
                std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de capaRed en enlace3"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace3"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace3"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace3"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                //capaRed.payload.payloadForwarding.longitud=paquete.payload.payload.payloadForwarding.longitud;

                std::cout<<std::endl;
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de capaRed en enlace4"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace4"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace4"<<std::endl;
                std::cout<<"Esto antes de capaRed en enlace4"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                //capaRed=paquete.payload;
                 //capaRed.payload.payloadForwarding.payload.mensaje.assign(paquete.payload.payload.payloadForwarding.payload.mensaje);
                //strcpy(capaRed.payload.payloadForwarding.payload.buffer, paquete.payload.payload.payloadForwarding.payload.buffer);

                //toCharArrayRed(paquete.datos, &capaRed);
                memmove( &capaRed.tipo,paquete.datos,sizeof(capaRed.tipo));
                memmove( &capaRed.longitud,paquete.datos+sizeof(paquete.tipo),sizeof(capaRed.longitud));
                memmove( &capaRed.datos,paquete.datos+sizeof(paquete.tipo)+sizeof(capaRed.longitud),capaRed.longitud);
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                std::cout<<capaRed.tipo<<std::endl;
                std::cout<<capaRed.longitud<<std::endl;
				std::cout<<"Finalizando if"<<std::endl;
                std::cout<<"Finalizando if"<<std::endl;
                std::cout<<"Finalizando if"<<std::endl;
                std::cout<<"Finalizando if"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                //if (paquete.idDestinoFinal != (uint16_t)-1){
                
                idVecino = paquete.idFuenteInmediato;
                /*
                for(i=1; i<tabla[0].size(); i++){
                    if(tabla[0][i].ID == idVecino){
                        indice = i;
                    }
                }
                */
                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Esto antes de push en recibir en enlace"<<std::endl;
                std::cout<<paquete.idDestinoFinal<<std::endl;
                std::cout<<paquete.idFuenteInmediato<<std::endl;
                //std::cout<<capaRed.payload.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                colaDespachadorVerde->push(capaRed);

                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<"Aqui estoy despues del push de la cola"<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;


                //Modificar esto para que funciona
                /*if(paquete.tipo == 1){
                    tabla[0][indice].estado = 1;
                    tabla[0][indice].tiempoExpiracion = 120;
                    colaDespachadorVerde->push(paquete);
                } else if(paquete.estado == -1){
                    paquete.estado = 2;
                    sendto(sock,&paquete,sizeof(paquete),0,
                        (struct sockaddr *)&from,fromlen);
                }*/
           // }

       // }
                

            }
             else if(paquete.tipo == 0x01){
                struct Latido paqueteLatido;
                memmove(&paqueteLatido.tipo_latido, paquete.datos, paquete.longitud);
                destino =  static_cast<short> (paquete.idFuenteInmediato);
                colaLatido->push(paqueteLatido);
                

            }
            

            
        n = 0;
    }
    printf("Hilo recibir salió del while\n");
}

/**
    @brief Metodo que genera sockets para poder conectarse con los
    vecinos
    @details Este metodo recibe la informacion de los vecinos del nodo
    y lo ejecutan varios hilos para conectarse a los diferentes vecinos
    @param[in] Recibe un vector datosNodo con la informacion de los
    vecinos
    @param[out] Recibe una referencia al vector de colas
    @pre Que el vector datosNodo tenga datos
    @remark se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void enviarV(size_t identificador, std::vector<datosNodo>* tabla,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes){

    int sock, n;
    char buffer [1047];
    //char buffer2 [65542];
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    std::string p;
    //struct estructuraEnlace mensaje;
    struct CapaEnlace datos;
    struct CapaEnlace paquete;

    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        std::cout << "Error al crear el socket" << std::endl;
    }

    server.sin_family = AF_INET;
    hp = gethostbyname(tabla[0][identificador].IP.data());

    if (hp==0){
        std::cout << "Error hots desconocido" << std::endl;
    }

    bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
    server.sin_port = htons(tabla[0][identificador].puerto);
    length=sizeof(struct sockaddr_in);
    while(!terminar){

        datos = colasDeMensajes[0][identificador].pop();

        if(datos.idDestinoFinal == (uint16_t)-1){
            terminar = true;
        } else {

              
            if(datos.tipo == 0x01 ||datos.tipo == 0x02 ){
              std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                
				std::cout<<"Esto antes del send en enlace"<<std::endl;
                std::cout<<datos.tipo<<std::endl;
                std::cout<<datos.idDestinoFinal<<std::endl;
                std::cout<<datos.idFuenteInmediato<<std::endl;
                //std::cout<<datos.payload.payload.payloadForwarding.payload.buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                //toCharArray(buffer, &datos);
                /*
                sprintf (buffer2, "%u", datos.tipo);
				strcat(buffer, buffer2);
                sprintf (buffer2, "%u", datos.idDestinoFinal);
				strcat(buffer, buffer2);
                sprintf (buffer2, "%u", datos.idFuenteInmediato);
				strcat(buffer, buffer2);
                sprintf (buffer2, "%u", datos.longitud);
				strcat(buffer, buffer2);
                sprintf (buffer2, "%s", datos.datos);
				strcat(buffer, buffer2);
                */
                memmove(buffer, &(datos.tipo), sizeof(datos.tipo));
                memmove(buffer+sizeof(datos.tipo), &(datos.idDestinoFinal), sizeof(datos.idDestinoFinal));
                memmove(buffer+sizeof(datos.tipo)+sizeof(datos.idDestinoFinal), &(datos.idFuenteInmediato), sizeof(datos.idFuenteInmediato));
                memmove(buffer+sizeof(datos.tipo)+sizeof(datos.idDestinoFinal)+sizeof(datos.idFuenteInmediato), &(datos.longitud), sizeof(datos.longitud));
                memmove(buffer+sizeof(datos.tipo)+sizeof(datos.idDestinoFinal)+sizeof(datos.idFuenteInmediato)+sizeof(datos.longitud), &(datos.datos),datos.longitud);

                std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                memmove( &(paquete.tipo), buffer,sizeof(paquete.tipo));
                memmove( &(paquete.idDestinoFinal),buffer+sizeof(paquete.tipo) , sizeof(paquete.idDestinoFinal));
                memmove(&(paquete.idFuenteInmediato), buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal), sizeof(paquete.idFuenteInmediato));
                memmove( &(paquete.longitud),buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal)+sizeof(paquete.idFuenteInmediato), sizeof(paquete.longitud));
                memmove( &(paquete.datos), buffer+sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal)+sizeof(paquete.idFuenteInmediato)+sizeof(paquete.longitud),paquete.longitud);
                std::cout<<paquete.tipo<<std::endl;
                std::cout<<paquete.idDestinoFinal<<std::endl;
                std::cout<<paquete.idFuenteInmediato<<std::endl;
                std::cout<<paquete.longitud<<std::endl;
                std::cout<<"Uwu send antes del sockect"<<std::endl;
                //std::cout<<buffer<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
				std::cout<<std::endl;
                for(int i = 0; i < 444; i ++){
                    std::cout<<buffer[i];
                }
                std::cout<<std::endl;
                n = sendto(sock, buffer, sizeof(buffer),
                    0, (struct sockaddr*)&server, length);
                if(n < 0){
                    std::cout << "Error en envío" << std::endl;
                }
            }
            else {
                 std::cout << "Error!!!!\n";
            }


            /*if(datos.estado == 1){
                mensaje.cabeceraRed = datos.cabeceraRed;
                mensaje.idDestino = datos.idDestino;
                mensaje.idFuente = tabla[0][0].ID;
                mensaje.estado = datos.estado;
                n = sendto(sock, &mensaje, sizeof(mensaje), 0,
                        (struct sockaddr*)&server, length);
                if(n < 0){
                    std::cout << "Error enviar mensaje" << std::endl;
                }
            } else if(datos.estado == -1){

                mensaje.estado = datos.estado;

                sendto(sock, &mensaje, sizeof(mensaje), 0,
                (struct sockaddr*)&server, length);

                recvfrom(sock, &mensaje, sizeof(mensaje), MSG_DONTWAIT,
                (struct sockaddr*)&from, &length);

                if(mensaje.estado == -1){
                    tabla[0][identificador].estado = 0;
                    tabla[0][identificador].tiempoExpiracion = 0;
                    //std::cout << "Vecino " <<
                    //tabla[0][identificador].ID <<
                    //" está muerto" << std::endl;
                } else if(mensaje.estado == 2){
                    tabla[0][identificador].estado = 1;
                    tabla[0][identificador].tiempoExpiracion = 120;
                    //std::cout << "Vecino " <<
                    //tabla[0][identificador].ID <<
                    //" está vivo" << std::endl;
                }
            }*/


        }
    }
    printf("Hilo %ld salió del while\n", identificador);
}

void latido(std::vector<datosNodo>* tabla,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes){

    struct CapaEnlace paquete;
    struct Latido paqueteLatido;
    size_t i;
    while(!terminar){
        for(i=1; i<tabla[0].size(); i++){
            if(tabla[0][i].tiempoExpiracion <= 0){
                paqueteLatido.tipo_latido = 0x01;
                paquete.tipo = 0x01;
                paquete.idDestinoFinal = static_cast<uint16_t>(tabla[0][i].ID);
                paquete.idFuenteInmediato = static_cast<uint16_t>(tabla[0][0].ID);
                char buffer2 [1040];
                //memmove(buffer2, &paqueteLatido.tipo_latido, sizeof(paqueteLatido.tipo_latido));
                //memmove(&paquete.datos, buffer2, sizeof(paquete.datos));
                std::cout<<"Entre a latido"<<std::endl;
                memmove(&paquete.datos, &paqueteLatido.tipo_latido, sizeof(paqueteLatido.tipo_latido));
                paquete.longitud = strlen(paquete.datos);
                colasDeMensajes[0][i].push(paquete);
            } else {
                pthread_mutex_lock(&lock);
                tabla[0][i].tiempoExpiracion--;
                pthread_mutex_unlock(&lock);
            }
            
        }
        
        sleep(tabla[0].size());
        for(i=1; i<tabla[0].size(); i++){
            if(tabla[0][i].tiempoExpiracion <= 0 ){
                std::cout<<"Estoy muerto"<<tabla[0][i].ID<<std::endl;
                tabla[0][i].estado = 0;
            }
            else{
                std::cout<<"Estoy vivo"<<tabla[0][i].ID<<std::endl;
                std::cout<<"Timepo"<<tabla[0][i].tiempoExpiracion<<std::endl;
            }
        }
    }
}

/**
    @brief Metodo del agente verde
    @details Metodo que ejecuta el agente verde que se encarga de
    generar los hilos de enviar y recibir
    @param[in] recibe un int con la cantidad de vecinos y un vector con
    los datos del nodo y los vecinos
    @param[out] Recibe la cola de mensajes
    @pre Que el vector info tenga datos
    @remark se generan los hilos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void verificarLatido(std::vector<Cola<struct CapaEnlace>>* colasDeMensajes,
std::vector<datosNodo>* tabla,Cola<struct Latido> * colaLatido){
    
    while(!terminar){
        struct Latido paqueteLatido = colaLatido->pop();
        if(paqueteLatido.tipo_latido == 0X01 ){
            for(size_t i = 0; i<tabla[0].size(); i ++){
                if(tabla[0][i].ID == destino){
                struct Latido nuevoPaqueteLatido;
                nuevoPaqueteLatido.tipo_latido = 0X02;
                struct CapaEnlace paquete;
                paquete.tipo = 0x01;
                paquete.idDestinoFinal = static_cast<uint16_t>(tabla[0][i].ID);//cambiar
                paquete.idFuenteInmediato = static_cast<uint16_t>(tabla[0][0].ID);
                char buffer2 [1040];
                memmove(&paquete.datos, &paqueteLatido.tipo_latido, sizeof(paqueteLatido.tipo_latido));
                paquete.longitud = strlen(paquete.datos);
                colasDeMensajes[0][i].push(paquete);
                }
            }


        }
        else if(paqueteLatido.tipo_latido == 0X02 ){
            for(size_t i = 0; i<tabla[0].size(); i ++){
                if(tabla[0][i].ID == destino){
                    tabla[0][i].estado = 1;
                    pthread_mutex_lock(&lock);
                    tabla[0][i].tiempoExpiracion = 3000;
                    pthread_mutex_unlock(&lock);


                }
            }
        }
    }

}

void hiloVerde(std::vector<datosNodo>* tablaVecinos,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes,
Cola<struct CapaRed>* colaDespachadorVerde){
    Cola<struct Latido> colaLatido;
    
   
    std::thread conexiones[tablaVecinos[0].size()-1];
    std::thread recibir = std::thread(recibirV, tablaVecinos,
        colaDespachadorVerde,&colaLatido);
    


    
    pthread_mutex_init(&lock, NULL);
     

    




    size_t i;

    for(i=0; i<tablaVecinos[0].size()-1; i++){
        conexiones[i] = std::thread(enviarV, i+1,
        tablaVecinos, colasDeMensajes);
    }

    std::thread hiloLatido = std::thread(latido, tablaVecinos,
    colasDeMensajes);
    std::thread verificarLatidoHilo = std::thread(verificarLatido,colasDeMensajes,tablaVecinos,&colaLatido);
    for(i=0; i<tablaVecinos[0].size()-1; i++){
        conexiones[i].join();
    }
    recibir.join();
    verificarLatidoHilo.join();
    hiloLatido.join();
    pthread_mutex_destroy(&lock);
}