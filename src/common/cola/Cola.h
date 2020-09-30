//Modificado de: https://gist.github.com/ictlyh/f8473ad0cb1008c6b32c41f3dea98ef5#file-producer-consumer-L25
//Autor Original: Yuanhao Luo "ictlyh"
//Fecha de última modificación: 9/9/20

#ifndef COLA_H
#define COLA_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#define LONGITUD 200

struct mensaje{
    unsigned long ipFuente;
    short int puertoFuente;
    unsigned long ipDestino;
    short int puertoDestino;
    short int idFuente;
    short int idDestino;
    char buffer[LONGITUD] = {0};
    mensaje(std::string str = "",short int idDestino = -1,short int idFuente = -1){
        this->idFuente = idFuente;
        this->idDestino = idDestino;
        str.copy(buffer, LONGITUD);
    }
};



struct datosNodo
{
    int ID;
    std::string IP;
    int puerto;
};

template <typename T>

class Cola{

    private:
        std::queue<T> cola;
        std::mutex m;
        std::condition_variable condicion;

    public:

        Cola()=default;
        Cola(const Cola&) = delete;

        T pop();
        void pop(T&);
        void push(const T&);
};

#endif //COLA_H