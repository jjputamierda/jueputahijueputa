//Modificado de: https://gist.github.com/ictlyh/f8473ad0cb1008c6b32c41f3dea98ef5#file-producer-consumer-L25
//Autor Original: Yuanhao Luo "ictlyh"
//Fecha de última modificación: 10/11/20

#ifndef COLA_H
#define COLA_H

#include <queue>
#include <mutex>
#include <condition_variable>

#define SIZE 200

extern uint16_t nodoSender;
extern uint16_t nodoSenderTWH;

struct estructuraAplicacion{
    char buffer[SIZE] = {};
};

struct estructuraRed{
    uint8_t tipo;
    uint8_t destinoRed;
    struct estructuraAplicacion cabeceraAplicacion;
};

struct estructuraEnlace{
    uint16_t idFuente;
    uint16_t idDestino;
    int estado = 1;
    struct estructuraRed cabeceraRed;
};

//Azul->despachador || Rosado->despachador
struct capaSuperior{
    struct estructuraAplicacion cabeceraAplicacion;
    uint8_t destinoRed;
    uint16_t idDestino;
};

//despachador->forwarding || despachador->broadcast
struct capaRed{
    struct estructuraRed cabeceraRed;
    uint16_t idDestino;
    uint16_t idFuente;
    uint8_t tipo;
};

//forwarding->Verde || broadcast->verde
struct capaEnlace{
    struct estructuraRed cabeceraRed;
    uint16_t idDestino;
    int estado = 1;
};

//forwarding->Azul || broadcast->Azul
struct capaAplicacion{
    struct estructuraAplicacion cabeceraAplicacion;
    uint16_t idFuente;
};

// enlace->des == enlace

struct datosNodo{
    short ID;
    std::string IP;
    short puerto;
    uint8_t estado = 1;
    uint8_t tiempoExpiracion = 0;
};











// NUEVAS ESTRUCTURAS



// Estructuras oficiales

// APLICACION
struct CapaAplicacion{
std::string mensaje;
};

// RED -> Tipos -> utilidades
struct Mensaje{
   // uint16_t idFuente;
    //char buffer[200] = {};
    char buffer[200] = {};
    //char buffer[1013] = {};
};

struct Alcanzabilidad{
    uint16_t longitud;
    uint16_t idVecino;
    uint8_t distancia;
};

// RED -> Tipos
struct Forwarding{
    uint16_t idFuenteInicial;
     //unsigned short idFuenteInicial;
    uint16_t longitud;
    //struct Mensaje payload;
    char datos [200];
   
};

struct ArbolGenerador{
    uint8_t tipo;
    uint16_t SN;
    uint16_t RN;
    //Se puede quitar?
};


struct BroadcastA{
    uint8_t tipo;              // Mensaje normal 1 || Alcanzabilidad 2
    uint16_t idFuente;
    uint16_t longitud;
    struct Mensaje payload;
};

struct BroadcastB{
    uint8_t tipo;              // Mensaje normal 1 || Alcanzabilidad 2
    uint16_t idFuente;
    uint16_t longitud;
    struct Alcanzabilidad payload;
};

// RED
struct CapaRedA{
    uint8_t tipo;
    uint16_t longitud;
    struct Forwarding payload;


};

struct CapaRedB{
    uint8_t tipo;
    uint16_t longitud;
    //struct Broadcast paylaod;
};

struct CapaRedC{
    uint8_t tipo;
    uint16_t longitud;
    struct ArbolGenerador payload;
};


// ENLACE
/*struct CapaEnlaceA{
    uint8_t tipo;               // Latido 1 || Capa red 2
    uint16_t idDestinoFinal;
    uint16_t idFuenteInmediato;
    uint16_t longitud;
    uint8_t latido;
};*/

struct CapaEnlaceB{
    uint8_t tipo;               // Latido 1 || Capa red 2
    uint16_t idDestinoFinal;
    uint16_t idFuenteInmediato;
    uint16_t longitud;
    struct CapaRedA payload;
};

/*struct CapaEnlaceC{
    uint8_t tipo;               // Latido 1 || Capa red 2
    uint16_t idDestinoFinal;
    uint16_t idFuenteInmediato;
    uint16_t longitud;
    struct CapaRedB payload;
};

struct CapaEnlaceD{
    uint8_t tipo;               // Latido 1 || Capa red 2
    uint16_t idDestinoFinal;
    uint16_t idFuenteInmediato;
    uint16_t longitud;
    struct CapaRedC payload;
};*/


// Estructuras especiales
struct Latido{
    uint8_t tipo_latido; 
}
// Agente azul -> Despachador azul
struct DatosMensaje{
    //char buffer[200] = {};
    char buffer[200] = {};
    uint16_t idDestino;
    uint8_t tipo;               // Forwarding 1 || Broadcast 2
};
struct Broadcast{
    uint8_t tipo;              // Mensaje normal 1 || Alcanzabilidad 2
    uint16_t id_origen_inicial;
    uint16_t longitud;
    char datos[200] = {};
    
};
struct CajaNegraRed{
   
   struct Broadcast payloadBroadcast;
  // struct ArbolGenerador payloadArbol;
    struct Forwarding payloadForwarding;
};
// Agente verde -> Despachador verde
struct CapaRed{
    uint8_t tipo;
    uint16_t longitud;
    //struct CajaNegraRed payload;
    char datos [1017];
};


struct DatosForwarding{
    uint16_t idFuenteInicial;
   //unsigned short idFuenteInicial;
    uint16_t idDestino;
    uint16_t longitud;
    //struct Mensaje payload;
    char datos [200];
    // buffer mensaje

};
struct DatosArbolGenerador{
    uint8_t tipo;
    uint16_t SN;
    uint16_t RN;
    //Se puede quitar?
    uint16_t id_destino_final;


};



struct CapaEnlace{
    uint8_t tipo;               // Latido 1 || Capa red 2
    uint16_t idDestinoFinal;
    uint16_t idFuenteInmediato;
    uint16_t longitud;
   // uint8_t latido = 0;
   // struct CapaRed payload;
    //char datos [65535];
    char datos [1040];//cambiar

    
};















template <typename T>

class Cola{

    private:
        std::queue<T> cola;
        std::mutex *m = new std::mutex();
        std::condition_variable *condicion =
        new std::condition_variable();

    public:

        Cola()=default;
        //Cola(const Cola&) = delete;

        T pop();
        void push(const T&);
};

#endif //COLA_H