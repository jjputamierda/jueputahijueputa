#ifndef AGENTE_AZUL_H
#define AGENTE_AZUL_H

	#include <fcntl.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <string.h>
	#include <iostream>
	#include <thread>
	#include <mutex>
	#include <fstream>
	#include "../../common/cola/Cola.h"

	#define LONGITUD 207

	//Variables globales
	//bool conexion;
	//std::mutex mtx;

	// Funciones
	void hiloAzul(std::vector<int>*,Cola<struct mensaje>*,char*,char*);
	void enviar(Cola<struct mensaje>*, char*);
	void recibir(Cola<struct mensaje>*, std::vector<int>*, char*);
	void mostrarError(const char*);

#endif
