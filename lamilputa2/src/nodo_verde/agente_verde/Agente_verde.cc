#include "Agente_verde.h"
pthread_mutex_t lock;
bool terminar = false;
uint16_t nodoSender;
uint16_t destinoFinal;
short destino;

/**
    @brief Metodo que se encarga de generar un socket para recibir los
    mensajes de los vecinos
    @details Este metodo se encarga de generar un socket para recibir
    los mensajes de los vecinos y luego pasarlos a la cola de mensajes
    para que el cliente lo reciba
    @param[out] El metodo recibe un vector de un struct con los datos
    para generar el socket
    @param[out] Cola despachadora para dejar los mensajes entrantes a
    capa de red
    @param[out] Cola latido para solicitudes latido
    @pre Que el vector datosNodo tenga datos
    @remark Se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 4-12-20
*/

void recibirV(std::vector<datosNodo>* tabla,
Cola<struct CapaRed>* colaDespachadorVerde,
Cola<struct Latido>* colaLatido){

    int n;
    char buffer [1047];
     
    unsigned int sock, length, fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    struct CapaEnlace paquete;
    struct CapaRed capaRed;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    length = sizeof(server);

    // fd_set lectura;
    // fcntl(sock, F_SETFL, O_NONBLOCK);
    // struct timeval tv;

    //FD_ZERO(&lectura);
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
    size_t i;
   
    while(!terminar){
    
        // tv.tv_usec = 1;
        // rv = select(sock+1, &lectura, NULL, NULL, &tv);
        //Meter esto dentro del n> 0
        //if(rv == 0){

            n = recvfrom(sock, buffer, 1047, 0,
                (struct sockaddr *)&from, &fromlen);

                //Traspaso de datos recibidos al paquete de enlace

                memmove(&(paquete.tipo), buffer,sizeof(paquete.tipo));
                memmove(&(paquete.idDestinoFinal), buffer +
                sizeof(paquete.tipo) , sizeof(paquete.idDestinoFinal));

                memmove(&(paquete.idFuenteInmediato), buffer +
                sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal),
                sizeof(paquete.idFuenteInmediato));

                memmove( &(paquete.longitud),buffer +
                sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal) +
                sizeof(paquete.idFuenteInmediato),
                sizeof(paquete.longitud));

                memmove(&(paquete.datos), buffer +
                sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal) +
                sizeof(paquete.idFuenteInmediato) +
                sizeof(paquete.longitud),sizeof(paquete.datos));

        //}

        //if(n > 0){

            if(paquete.tipo == 0x02){
                for(i = 0; i < tabla[0].size(); i++){
                    if(tabla[0][i].ID ==
                    static_cast<short>(paquete.idFuenteInmediato)){

                        pthread_mutex_lock(&lock);
                        tabla[0][i].tiempoExpiracion = 30000;
                        pthread_mutex_unlock(&lock);
                    }
                }

                nodoSender = paquete.idFuenteInmediato;
                destinoFinal =paquete.idDestinoFinal;

                memmove(&capaRed.tipo,paquete.datos,
                sizeof(capaRed.tipo));

                memmove(&capaRed.longitud,paquete.datos +
                sizeof(paquete.tipo),sizeof(capaRed.longitud));

                memmove( &capaRed.datos,paquete.datos +
                sizeof(paquete.tipo)+sizeof(capaRed.longitud),
                sizeof(capaRed.datos));
                
                idVecino = paquete.idFuenteInmediato;

                colaDespachadorVerde->push(capaRed);

        // }

            } else if(paquete.tipo == 0x01){
                struct Latido paqueteLatido;
                memmove(&paqueteLatido.tipo_latido, paquete.datos,
                sizeof(paqueteLatido.tipo_latido));

                destino =
                static_cast<short> (paquete.idFuenteInmediato);

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
    @param[in] Identificador que contiene el número del hilo respectivo
    @param[out] Recibe un vector datosNodo con la informacion de los
    vecinos
    @param[out] Cola de mensajes para recoger mensajes entrantes
    @pre Que el vector datosNodo tenga datos
    @remark se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 4-12-20
*/

void enviarV(size_t identificador, std::vector<datosNodo>* tabla,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes){

    int sock, n;
    char buffer [1047];
    unsigned int length;
    struct sockaddr_in server;
    struct hostent *hp;
    std::string p;
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

                memmove(buffer, &(datos.tipo), sizeof(datos.tipo));
                memmove(buffer + sizeof(datos.tipo),
                &(datos.idDestinoFinal), sizeof(datos.idDestinoFinal));

                memmove(buffer + sizeof(datos.tipo) +
                sizeof(datos.idDestinoFinal),
                &(datos.idFuenteInmediato),
                sizeof(datos.idFuenteInmediato));

                memmove(buffer + sizeof(datos.tipo) +
                sizeof(datos.idDestinoFinal) +
                sizeof(datos.idFuenteInmediato), &(datos.longitud),
                sizeof(datos.longitud));

                memmove(buffer+sizeof(datos.tipo) +
                sizeof(datos.idDestinoFinal) +
                sizeof(datos.idFuenteInmediato) +
                sizeof(datos.longitud), &(datos.datos),
                sizeof(datos.datos));

                memmove(&(paquete.tipo), buffer,sizeof(paquete.tipo));
                memmove(&(paquete.idDestinoFinal), buffer +
                sizeof(paquete.tipo) , sizeof(paquete.idDestinoFinal));

                memmove(&(paquete.idFuenteInmediato), buffer +
                sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal),
                sizeof(paquete.idFuenteInmediato));

                memmove(&(paquete.longitud),buffer +
                sizeof(paquete.tipo)+sizeof(paquete.idDestinoFinal) +
                sizeof(paquete.idFuenteInmediato),
                sizeof(paquete.longitud));

                memmove(&(paquete.datos), buffer +
                sizeof(paquete.tipo) +sizeof(paquete.idDestinoFinal) +
                sizeof(paquete.idFuenteInmediato) +
                sizeof(paquete.longitud),sizeof(paquete.datos));

                n = sendto(sock, buffer, sizeof(buffer),
                    0, (struct sockaddr*)&server, length);
                if(n < 0){
                    std::cout << "Error en envío" << std::endl;
                }
            }
            else {
                 std::cout << "Error!!!!\n";
            }
        }
    }
    printf("Hilo %ld salió del while\n", identificador);
}

/**
    @brief Genera solicitudes de tipo latido
    @details Se encarga de generar solicitudes de tipo latido
    para enviarla a sus vecinos
    @param[out] Recibe la cola de mensajes para comunicación
    con los otros nodos
    @param[out] Tabla con los datos de los vecinos
    @pre Que el vector info tenga datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 / Isaac Herrera B43332
    @date 4-12-20
*/

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
                paquete.idDestinoFinal =
                static_cast<uint16_t>(tabla[0][i].ID);

                paquete.idFuenteInmediato =
                static_cast<uint16_t>(tabla[0][0].ID);

                char buffer2 [1040];
                memmove(buffer2, &paqueteLatido.tipo_latido,
                sizeof(paqueteLatido.tipo_latido));

                memmove(&paquete.datos, buffer2,
                sizeof(paquete.datos));

                paquete.longitud = sizeof(paquete.datos);
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
                std::cout << "Vecino " <<tabla[0][i].ID <<
                " muerto" << std::endl;
                tabla[0][i].estado = 0;
            }
            else{
                std::cout << "Vecino " << tabla[0][i].ID <<
                " vivo" << std::endl;
            }
        }
    }
}

/**
    @brief Verifica solicitudes de tipo latido
    @details Se encarga de recibir solicitudes de tipo latido
    para responderlas pertinentemente
    @param[out] Recibe la cola de mensajes para comunicación
    con los otros nodos
    @param[out] Tabla con los datos de los vecinos
    @param[out] Cola de tipo latido para las solicitudes
    @pre Que el vector info tenga datos
    @remark se generan los hilos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 / Isaac Herrera B43332
    @date 4-12-20
*/

void verificarLatido(
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes,
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
                    paquete.idDestinoFinal =
                    static_cast<uint16_t>(tabla[0][i].ID);

                    paquete.idFuenteInmediato =
                    static_cast<uint16_t>(tabla[0][0].ID);

                    char buffer2 [1040];
                    memmove(buffer2, &nuevoPaqueteLatido.tipo_latido,
                    sizeof(nuevoPaqueteLatido.tipo_latido));

                    memmove(&paquete.datos, buffer2,
                    sizeof(paquete.datos));

                    paquete.longitud = sizeof(paquete.datos);
                    colasDeMensajes[0][i].push(paquete);
                }
            }
        }
        else if(paqueteLatido.tipo_latido == 0X02 ){
            for(size_t i = 0; i<tabla[0].size(); i ++){
                if(tabla[0][i].ID == destino){
                    tabla[0][i].estado = 1;
                    pthread_mutex_lock(&lock);
                    tabla[0][i].tiempoExpiracion = 200;
                    pthread_mutex_unlock(&lock);
                }
            }
        }
    }
}

/**
    @brief Rutina principal del hilo verde
    @details Se encarga de generar los hilos de las subrutinas
    de latido y el envío/recepción de mensajes
    @param[out] Recibe la cola de mensajes para comunicación
    con los otros nodos
    @param[out] Tabla con los datos de los vecinos
    @param[out] Cola despachadora para comunicación con capa de red
    @pre Que el vector info tenga datos
    @remark se generan los hilos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961 / Isaac Herrera B43332
    @date 4-12-20
*/

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

    std::thread verificarLatidoHilo = std::thread(verificarLatido,
    colasDeMensajes,tablaVecinos,&colaLatido);

    for(i=0; i<tablaVecinos[0].size()-1; i++){
        conexiones[i].join();
    }

    recibir.join();
    verificarLatidoHilo.join();
    hiloLatido.join();
    pthread_mutex_destroy(&lock);
}