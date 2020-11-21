#ifndef CAPA_RED_H
#define CAPA_RED_H

#include <thread>
#include <mutex>
#include "../../common/cola/Cola.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#define FORWARDING 1
#define BROADCAST 2
#define ARBOL 3
#define MENSAJE 1
#define ALCANZABILIDAD 2
#define RED 2

// Funciones

//Función principal de la capa de red
void capaRed(Cola<struct Mensaje>* colaAzul,
	Cola<struct ArbolGenerador>* colaRosada,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosMensaje>* colaDespachadorAzul,
	Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
	Cola<struct CapaRed>* colaDespachadorVerde,
	std::vector<int>* nodosIDs, Cola<std::string>* despachadorMiembros,
	Cola<std::string>* colaAlcanzabilidad,
	std::vector<datosNodo>* tablaVecinos,char* IP,
	Cola<std::string>* colaTablaForwarding );

//despachador agente azul
void despachadorAzul(Cola<struct DatosMensaje>*,
	Cola<struct DatosForwarding>*, Cola<struct Broadcast>*, int);

//despachador agente rosado
void despachadorRosado(Cola<struct DatosArbolGenerador>* ,
std::vector<int>*,std::vector<datosNodo>* tablaVecinos,std::vector<Cola<struct CapaEnlace>>*);


//despachador agente verde
void despachadorVerde(Cola<struct CapaRed>* ,
	Cola<struct DatosForwarding>* ,
	Cola<struct Broadcast >* ,
	Cola<struct ArbolGenerador>* );


//forwarding
void forwarding(Cola<struct Mensaje>*,
Cola<struct capaAplicacion>*, std::vector<Cola<struct CapaEnlace>>*,
Cola<struct DatosForwarding>*, std::vector<int>*);

//broadcast
/*void broadcast(Cola<struct Mensaje>*,
Cola<struct capaAplicacion>*, std::vector<Cola<struct CapaEnlace>>*,
Cola<struct capaRed>*, std::vector<int>*,Cola<std::string>*,
Cola<std::string>* colaAlcanzabilidad);
*/

/*void verificarEstructura(Cola<std::string>*,std::vector<int>*);

void enviarAlcanzabilidad(Cola<std::string>* colaAlcanzabilidad,
std::vector<datosNodo>* tablaVecinos,Cola<struct capaSuperior>*,
Cola<struct capaAplicacion>*,char*);

void verificarTablaForwarding(Cola<std::string>*,
std::vector<int>*);
*/

#endif //AGENTE_RED_H
