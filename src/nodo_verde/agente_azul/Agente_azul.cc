#include "Agente_Azul.h"

//Variables globales
std::mutex mtx;
bool conexion;


/**
 *	@brief Subrutina del hilo agente azul
 *	@details Esta subrutina crea los pipes para la comunicación entre
 *	el agente y el servidor web, además de generar otro proceso
 *	que corre el programa Servidor.py. Además se generan hilos para
 *	realizar las comunicaciones (Con el servidor y con el agente verde)
 *	@param[in] [1] Vector de enteros: Para conocer los ids de los 
 *	vecinos y enviar los mensajes a la cola correspondiente
 *	[2] Cola de mensajes: Es un puntero a un arreglo de colas de 
 *	mensajes, da salida y entrada a los mensajes. Una de las colas
 *	es para los mensajes que recibe el usuario, las otras colas son
 *	para los mensajes de salida
 *	[3] String: Ruta para el pipe Agente -> Servidor
 *	[4] String: Ruta para el pipe Servidor -> Agente
 *	@pre Los 4 parámetros deben tener datos, deben estar las 
 *	direcciones para los pipes, además de los ids de los vecinos y
 *	de colas que deben estar creadas
 *	@remark Utiliza las colas para enviar codificaciones a lo interno
 *	como un mensaje de finalización para desconectar al cliente del 
 *	servidor (-1::Salir)
 *	@author Johel Phillips Ugalde B75821
 *	@date 15-09-20
 */
void hiloAzul(std::vector<int>*nodosIDs,Cola<struct mensaje>*colas,
		char* fifoAgenteServidor, char* fifoServidorAgente){

	pid_t pid;
	conexion = true;

	if(mkfifo(fifoAgenteServidor, 0666) == -1) 
		mostrarError("No se pudo crear el fifo Agente -> Servidor");

	if(mkfifo(fifoServidorAgente, 0666) == -1) 
		mostrarError("No se pudo crear el fifo Servidor -> Agente");

	pid = fork();
	if(!pid){
		if(execlp("python3", "python3", "src/nodo_azul/./Servidor.py",
				fifoAgenteServidor, fifoServidorAgente, (char*)0)== -1)
			mostrarError("No se pudo ejecutar Servidor.py");
	}

	std::thread emisor (enviar, colas, fifoAgenteServidor);
	std::thread receptor (recibir, colas, nodosIDs, fifoServidorAgente);

	receptor.join();
	emisor.join();
}

/**
 *	@brief Subrutina del hilo emisor
 *	@details Esta subrutina se encarga de enviarle mensajes al
 *	servidor web, y por ende al usuario, provenientes de los
 *	vecinos, además del código de finalización.
 *	@param[in] [1] Cola de mensajes: Es un puntero a un arreglo de 
 *	colas de mensajes, en este caso se sacan los mensajes de la 
 *	primer cola para ser enviados al servidor
 *	[2] String: Ruta del pipe para el paso de los mensajes
 *	Agente -> Servidor
 *	@pre Las colas deben existir y la ruta de los pipes debe ser 
 *	correcta 
 *	@remark Utiliza las colas para enviar codificaciones a lo interno
 *	como un mensaje de finalización para desconectar al cliente del 
 *	servidor (-1::Salir)
 *	@author Johel Phillips Ugalde B75821
 *	@date 15-09-20
 */
void enviar(Cola<struct mensaje>* colas, char* fifoAgenteServidor){

	int fd = open(fifoAgenteServidor, O_WRONLY);
	bool salir = false;
	struct mensaje mensaje;
	std::string buffer;

	while(!salir){
		mensaje = colas[0].pop();
		mtx.lock();
		if(conexion == false){
			salir = true;
		}
		mtx.unlock();
		buffer.clear();
		buffer.append(std::to_string(mensaje.origen));
		buffer.append("::");
		buffer.append(mensaje.buffer);
		if(buffer.size() > 207){
			buffer = buffer.substr(0,207);
		}
		write(fd, buffer.c_str(), 207);
	}
	close(fd);
}


/**
 *	@brief Subrutina del hilo receptor
 *	@details Esta subrutina se encarga de recibir mensajes del
 *	servidor web, y enviarlos a la cola de salida correspondiente
 *	@param[in] [1] Cola de mensajes: Es un puntero a un arreglo de 
 *	colas de mensajes, en este caso se envían los mensajes a la 
 *	cola correspondiente para ser enviados a un vecino
 *	[2] Vector de enteros: Contiene los ids de los vecinos, sirve
 *	para enviar los mensajes a la cola correspondiente
 *	[3] String: Ruta del pipe para el paso de los mensajes
 *	Agente -> Servidor
 *	@pre Las colas deben existir y la ruta de los pipes debe ser 
 *	correcta, además de contener la información de los ids de los
 *	vecinos
 *	@remark Utiliza las colas para enviar codificaciones a lo interno
 *	como un mensaje de finalización para desconectar al cliente del 
 *	servidor (-1::Salir)
 *	@author Johel Phillips Ugalde B75821
 *	@date 15-09-20
 */
void recibir(Cola<struct mensaje>* salida, std::vector<int>* nodosIDs,
		char* fifoServidorAgente){

	int fd = open(fifoServidorAgente, O_RDONLY);
	int longitud = nodosIDs->size();
	int posicion, destino;
	bool salir = false;
	std::string msg;
	char buffer[207];

	while(!salir){
		msg.clear();
		read(fd, buffer, 207);
		msg.append(buffer);
		if(msg.find("-1::Salir") == std::string::npos){
			posicion = msg.find("::");
			destino = std::stoi(msg.substr(0,posicion));
			for(int i = 0; i < longitud; ++i){
				if(destino == nodosIDs[0][i]){
					mtx.lock();
					salida[i].push(mensaje(msg.substr(posicion+2),
						destino));
					mtx.unlock();
				}
			}
		} else {
			mtx.lock();
			conexion = false;
			mtx.unlock();
			salir = true;
			for(unsigned long i=0; i<nodosIDs->size(); i++){
				salida[i].push(mensaje("Salir", -1));
			}
		}
	}
	close(fd);
}

void mostrarError(const char* mensaje){
	std::cerr << "Error: " << mensaje << std::endl;
}