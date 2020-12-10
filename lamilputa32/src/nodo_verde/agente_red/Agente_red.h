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
void capaRed(Cola<struct ForwardingAplicacion>* colaAzul,
	Cola<struct ArbolGenerador>* colaRosada,
	std::vector<Cola<struct CapaEnlace>>* colasVerdes,
	Cola<struct DatosForwardingAplicacion>* colaDespachadorAzul,
	Cola<struct DatosArbolGenerador>* colaDespachadorRosado,
	Cola<struct CapaRed>* colaDespachadorVerde,
	std::vector<int>* nodosIDs, Cola<std::string>* despachadorMiembros,
	Cola<std::string>* colaAlcanzabilidad,
	std::vector<datosNodo>* tablaVecinos,char* IP,
	Cola<std::string>* colaTablaForwarding,Cola<std::string>* colaEnviarAlcanzabilidad  );

//despachador agente azul
void despachadorAzul(Cola<struct DatosForwardingAplicacion>*,
	Cola<struct DatosForwarding>*, Cola<struct Broadcast>*, int);

//despachador agente rosado
void despachadorRosado(Cola<struct DatosArbolGenerador>* ,
std::vector<int>*,std::vector<datosNodo>* tablaVecinos,std::vector<Cola<struct CapaEnlace>>*);


//despachador agente verde
void despachadorVerde(Cola<struct CapaRed>* ,
	Cola<struct DatosForwarding>* ,
	Cola<struct Broadcast >* ,
	Cola<struct ArbolGenerador>*,Cola<struct ForwardingAplicacion>* colaAzul );


//forwarding
void forwarding(Cola<struct ForwardingAplicacion>*,
 std::vector<Cola<struct CapaEnlace>>*,
Cola<struct DatosForwarding>*, std::vector<int>*g);
void toCharArrayForwarding(char * paquete_sal, Forwarding * paquete);
void toCharArrayForwarding2(char * paquete_ent, Forwarding * paquete);
void toCharArrayBroadcast(char * paquete_sal, Broadcast * paquete);
void toCharArrayBroadcast2(char * paquete_ent, Broadcast * paquete);
void toCharArray(char * paquete_sal, CapaRed * paquete);
void toCharArray2(char * paquete_ent, CapaRed * paquete);



//broadcast

void broadcast(Cola<struct ForwardingAplicacion>* colaAzul,
		Cola<struct ArbolGenerador>* colaRosada,
		std::vector<Cola<struct CapaEnlace>>* colasVerdes,
		Cola<struct Broadcast>* colaBroadcast,
		std::vector<int>* nodosIDs,
		Cola<std::string>* despachadorMiembros,
		Cola<std::string>* colaAlcanzabilidad,Cola<std::string>* colaEnviarAlcanzabilidad);


void verificarEstructura(Cola<std::string>*,std::vector<int>*);

void enviarAlcanzabilidad(Cola<std::string>* colaAlcanzabilidad,
std::vector<datosNodo>* tablaVecinos,char*,Cola<struct Broadcast>*,Cola<std::string>* colaEnviarAlcanzabilidad);

void verificarTablaForwarding(Cola<std::string>*,
std::vector<int>*);

void timeout();


#endif //AGENTE_RED_H
