#ifndef AGENTE_VERDE_H
#define AGENTE_VERDE_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h> //exit function
#include <string.h> //strlen
#include <unistd.h> //write/read functions
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "../../common/cola/Cola.h"

void recibirV(std::vector<datosNodo>,Cola<struct mensaje>*);
void enviarV( int ,std::vector<datosNodo>,Cola<struct mensaje>*);
void hiloVerde( long unsigned int,std::vector<std::string>,
Cola<struct mensaje>*);

#endif
