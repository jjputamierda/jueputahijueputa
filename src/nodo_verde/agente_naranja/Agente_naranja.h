#ifndef AGENTE_NARANJA_H
#define AGENTE_NARANJA_H
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cerrno>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdexcept>
#include <thread>
#include <sstream>
#include "../Nodo_verde.h"
//constantes numéricas
#define STD_SIZE 2
#define MAX_BUF 1024
#define PERMISOS_FIFO 0666
#define CANT_MIN_ARCH 7
#define CONF_SIZE 3
#define MIN_BYTES_VER 10
#define MAX_REINICIO 5
#define INTERPRETADOR "python3"
#define CAMINO_NODO_NARANJA "src/nodo_naranja/Nodo_naranja.py"
//Constantes de texto
#define COMP_2_BYTES "0"
#define CODIGO_CONFIRMACION "777"
#define CODIGO_AVISO_ENVIO_PID "100"
#define CODIGO_FINALIZACION_PID "101"
#define CODIGO_AVISO_ENV_VER "102"
#define CODIGO_AVISO_REC_VER "103"
#define CODIGO_AVISO_ENV_VEC "104"
#define CODIGO_AVISO_REC_VEC "106"
#define ERR_CREAR_NN "No se pudo crear el nodo naranja"
#define ERR_PIPE_CREAR_L "No se creó pipe de lectura"
#define ERR_PIPE_CREAR_E "No se creó pipe de escritura"
#define ERR_PIPE_ABRIR_L "No se abrió el pipe de lectura"
#define ERR_PIPE_BORRAR_L "No se eliminó pipe de lectura"
#define ERR_PIPE_BORRAR_E "No se eliminó pipe de escritura"
#define ERR_PIPE_ABRIR_L "No se abrió el pipe de lectura"
#define ERR_PIPE_ABRIR_E "No se abrió el pipe de escritura"
#define ERR_PIPE_CERRAR_L "No se cerró pipe de lectura"
#define ERR_PIPE_CERRAR_E "No se cerró pipe de escritura"


//se definen las funciones a utilizar
void trabajoAgenteNaranja(char*caminoFifoAgente,
char *caminoFifoNodo,char* caminoNodoVerde,char* caminoVecinos);
void enviarConfirmacion(int fdl, int fde,
char *caminoFifoAgente,char * caminoFifoNodo);
void quitarPipe(char * camino,std::string e);
std::vector< std::string > comunicarConNodoNaranja(int pid,
char *caminoFifoAgente,char *caminoFifoNodo);
void comunicacionPid(char *caminoFifoAgente,
char *caminoFifoNodo,int pid);
void cerrarPipe(int fd,std::string e,char *caminoFifo,
std::string eBorrar);
void escribirPipe(std::string msj,std::string e,
  char *caminoFifoAgente,char *caminoFifoNodo);
void escribirPipeTam(std::string msj,std::string e,
  char *caminoFifoAgente,char *caminoFifoNodo);
void escribirPipeMen(std::string msj,std::string e,
  char *caminoFifoAgente,char *caminoFifoNodo);
 std::string leerPipe(std::string e,
 char *caminoFifoAgente,char *caminoFifoNodo);
size_t leerPipeTam(std::string e,
char *caminoFifoAgente,char *caminoFifoNodo);
std::string leerPipeMen(std::string e,
char *caminoFifoAgente,char *caminoFifoNodo,
size_t tamanoMenReal);
void revisarCodigos(
std::string codEnv, std::string codCor,std::string e);

std::vector< std::string > comunicacionInfoVerde(
  char *caminoFifoAgente,char *caminoFifoNodo,
  std::vector < std::string > infoVerde);

std::vector< std::string > comunicacionVecinos(
  char *caminoFifoAgente,char *caminoFifoNodo,
  std::vector < std::string > infoVerde);

std::vector<std::string> recibirInfoArchivo(
std::string codigo,std::string e,
char* caminoFifoAgente
,char *caminoFifoNodo,std::vector < std::string > infoVerde);
#endif
