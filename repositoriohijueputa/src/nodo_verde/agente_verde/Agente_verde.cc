#include "Agente_verde.h"

bool terminar = false;

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

void recibirV(std::vector<datosNodo>* tabla,
Cola<struct CapaEnlace>* colaDespachadorVerde){

    int n;
    unsigned int sock, length, fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    struct CapaEnlace paquete;

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    length = sizeof(server);

    fd_set lectura;
    fcntl(sock, F_SETFL, O_NONBLOCK);
    struct timeval tv;

    FD_ZERO(&lectura);
    FD_SET(sock, &lectura);

    tv.tv_sec = 0;
    tv.tv_usec = 1;

    int rv;

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

        tv.tv_usec = 1;
        rv = select(sock+1, &lectura, NULL, NULL, &tv);
        //Meter esto dentro del n> 0
        if(rv == 0){
            n = recvfrom(sock, &paquete, sizeof(paquete), 0,
                (struct sockaddr *)&from, &fromlen);
            /*
            if(paquete.tipo == RED){
                struct CapaRed capaRed;
                capaRed.tipo = paquete.tipo;
                capaRed.longitud = paquete.longitud;
            }
            */
        }

        if(n > 0){
            if(paquete.tipo == RED){
                nodoSender = paquete.idFuenteInmediato;
                struct CapaRed capaRed = *(paquete.payload);
                

            }
            

            if (paquete.idDestinoFinal != (uint16_t)-1){

                idVecino = paquete.idFuenteInmediato;
                for(i=1; i<tabla[0].size(); i++){
                    if(tabla[0][i].ID == idVecino){
                        indice = i;
                    }
                }
                
                colaDespachadorVerde->push(paquete);
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
            }

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
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    std::string p;
    //struct estructuraEnlace mensaje;
    struct CapaEnlace datos;

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

            if(datos.tipo == 2){ // Capa de red
                n = sendto(sock, &datos, sizeof(capaEnlace),
                    0, (struct sockaddr*)&server, length);
                if(n < 0){
                    std::cout << "Error en envío" << std::endl;
                }
            } else if(datos.tipo == 1){
                std::cout << "Latido!!!\n";
            } else {
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

/*void latido(std::vector<datosNodo>* tabla,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes){

    struct capaEnlace mEstado;
    size_t i;
    while(!terminar){
        for(i=1; i<tabla[0].size(); i++){
            if(tabla[0][i].tiempoExpiracion <= 0){
                mEstado.estado = -1;
                colasDeMensajes[0][i].push(mEstado);
            } else {
                tabla[0][i].tiempoExpiracion--;
            }
        }
        sleep(tabla[0].size());
    }
}*/

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

void hiloVerde(std::vector<datosNodo>* tablaVecinos,
std::vector<Cola<struct CapaEnlace>>* colasDeMensajes,
Cola<struct CapaRed>* colaDespachadorVerde){

    std::thread conexiones[tablaVecinos[0].size()-1];
    std::thread recibir = std::thread(recibirV, tablaVecinos,
        colaDespachadorVerde);

    size_t i;

    for(i=0; i<tablaVecinos[0].size()-1; i++){
        conexiones[i] = std::thread(enviarV, i+1,
        tablaVecinos, colasDeMensajes);
    }

    /*std::thread hiloLatido = std::thread(latido, tablaVecinos,
    colasDeMensajes);*/

    for(i=0; i<tablaVecinos[0].size()-1; i++){
        conexiones[i].join();
    }
    recibir.join();
    //hiloLatido.join();
}
