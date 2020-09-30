#include "Agente_verde.h"

int terminar = 0;

/**
    @brief Metodo que se encarga de generar un socket para recibir los mensajes de los vecinos
    @details Este metodo se encarga de generar un socket para recibir los mensajes de los vecinos y luego pasarlos a la cola de mensajes para que el cliente lo reciba
    @param[in] el metodo recibe un vector de un struct con los datos para generar el socket 
    @param[out] Recibe una vector de cola de mensajes y se usa para el paso de mensajes
    @pre Que el vector datosNodo tenga datos
    @remark Se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void recibirV(std::vector<datosNodo> tabla,Cola<struct mensaje>* colasDeMensajes){

    unsigned int sock, length, fromlen;
    int n;
    struct sockaddr_in server;
    struct sockaddr_in from;
    struct mensaje mRecibido("No llegó mensaje (Esta es una plantilla)", -1);

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    length = sizeof(server);

    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(tabla[0].puerto);
    if (bind(sock,(struct sockaddr *)&server,length)<0){
        std::cout << "Error en binding" << std::endl;
    }

    fromlen = sizeof(struct sockaddr_in);
    while (terminar!=1) {
        n = recvfrom(sock,&mRecibido,sizeof(mRecibido),MSG_DONTWAIT,(struct sockaddr *)&from,&fromlen);
        if (n > 0){
            colasDeMensajes[0].push(mRecibido);
        }
    }
    usleep(250000);
}

/**
    @brief Metodo que genera sockets para poder conectarse con los vecinos
    @details Este metodo recibe la informacion de los vecinos del nodo y lo ejecutan varios hilos para conectarse a los diferentes vecinos
    @param[in] Recibe un vector datosNodo con la informacion de los vecinos
    @param[out] Recibe una referencia al vector de colas
    @pre Que el vector datosNodo tenga datos
    @remark se modifica la cola de mensajes
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void enviarV(int identificador,std::vector<datosNodo> tabla,Cola<struct mensaje>* colasDeMensajes){

    int sock, n;
    unsigned int length;
    struct sockaddr_in server;
    struct hostent *hp;
    std::string p;
    struct mensaje miMensaje(" ", -2);

    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        std::cout << "Error al crear el socket" << std::endl;
    }

    server.sin_family = AF_INET;
    hp = gethostbyname(tabla[identificador].IP.data());

    if (hp==0){
        std::cout << "Error hots desconocido" << std::endl;
    }

    bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
    server.sin_port = htons(tabla[identificador].puerto);
    length=sizeof(struct sockaddr_in);
    while(terminar!=1){
        miMensaje=colasDeMensajes[identificador].pop();
        if(miMensaje.idDestino == -1){
            terminar = 1;
        } else {
            n=sendto(sock,&miMensaje,sizeof(miMensaje),0,(struct sockaddr *)&server,length);
            if(n<0){
                std::cout << "Error al enviar el mensaje" << std::endl;
            }
            //sleep(2);
        }
    }
}

/**
    @brief Metodo del agente verde
    @details Metodo que ejecuta el agente verde que se encarga de generar los hilos de enviar y recibir
    @param[in] recibe un int con la cantidad de vecinos y un vector con los datos del nodo y los vecinos
    @param[out] Recibe la cola de mensajes
    @pre Que el vector info tenga datos
    @remark se generan los hilos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
*/

void hiloVerde(  long unsigned int i2,std::vector<std::string> info,Cola<struct mensaje>* colasDeMensajes){

    std::vector<datosNodo> tablaVecinos;
    long unsigned int i;
    datosNodo datosPropios;
    tablaVecinos.push_back(datosPropios);
    tablaVecinos[0].ID = 0;

    char *linea = (char*)info[0].c_str();

    char* p;

    p = strtok(linea,",");

    tablaVecinos[0].ID = atoi(p);
    p = strtok(NULL, ",");
    tablaVecinos[0].puerto = atoi(p);

    for(i=1; i<info.size(); i++){
        datosNodo vecino;
        tablaVecinos.push_back(vecino);

        linea = (char*)info[i].c_str();
        p = strtok(linea,",");
        tablaVecinos[i].ID = atoi(p);

        p = strtok(NULL,",");
        tablaVecinos[i].IP = p;

        p = strtok(NULL,",");
        tablaVecinos[i].puerto = atoi(p);
    }

    std::thread conexiones[i2];
    std::thread recibir=std::thread(recibirV,tablaVecinos,colasDeMensajes);

    for(long unsigned int indice=0;indice<i2;indice++){
        conexiones[indice]=std::thread(enviarV,indice+1,tablaVecinos,colasDeMensajes);
    }

    for(long unsigned int indice2=0;indice2<i2;indice2++){
        conexiones[indice2].join();
    }

    recibir.join();
}