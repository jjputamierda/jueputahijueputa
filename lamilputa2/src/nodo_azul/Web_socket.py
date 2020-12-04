#!/usr/bin/env python3

from autobahn.twisted.websocket import WebSocketServerProtocol, \
	WebSocketServerFactory
from twisted.python import log
from twisted.internet import reactor
import threading, sys, codecs, os, random, time, queue

CODIGO = '-1::Salir'.encode('ASCII', 'ignore')
LONGITUD = 207
conexiones = []

class WebSocketServer(WebSocketServerProtocol):
	
	'''
	*  @brief Muestra que se conectó un cliente
	*  details Se imprime en salida estandar cada vez que se conecta un
	*  nuevo cliente
	*  @param[in] request: Brinda información específica como la ip del 
	*  cliente
	*  @param[out] N/A
 	*  @remark N/A
 	*  @return N/A
 	*  @exception N/A
	*  @author Jeisson Hidalgo
	*  @date 28-09-20
	'''
	def onConnect(self, request):
		print(f"Client connecting: {request.peer}")
		conexiones.append(self)
	
	'''
	*  @brief Envía la información propia y la de los vecinos
	*  @details Se imprime en salida estandar un mensaje de 
	*  confirmación además de enviarle a la página web la información
	*  del ID propio y los IDs de los vecinos
	*  @param[in] N/A
	*  @param[out] N/A
 	*  @remark N/A
 	*  @return N/A
 	*  @exception N/A
	*  @author Jeisson Hidalgo
	*  @date 28-09-20
	'''
	def onOpen(self):
		print("WebSocket connection open.")
		self.sendMessage(f"id\t{ids[0]}".encode('utf8'), False)
		print("Envié # nodo")
		idVecinos = 'vecinos'
		for i in range(1, len(ids)):
			idVecinos += ' ' + ids[i]
		self.sendMessage(f"{idVecinos}".encode('utf8'), False)
		print("Envié # vecinos")

	'''
	*  @brief Imprime en salida estandar los mensajes que llegan
	*  @details Esta subrutina imprime información de los mensajes que
	*  llegan desde el web socket con destino al agente azul, para
	*  luego ser enviados a su respectivo nodo de destino
	*  @param[in] [1] payload: Es el cuerpo del mensaje
	*  [2] isBinary: Nos dice si el mensaje es binario o no
	*  @param[out] N/A
 	*  @remark N/A
 	*  @return N/A
 	*  @exception N/A
	*  @author Jeisson Hidalgo
	*  @date 28-09-20
	'''
	def onMessage(self, payload, isBinary):
		if isBinary:
			print(f"Binary message received: {len(payload)} bytes")
		else:
			print(f"Text message received: {payload.decode('utf8')}")
		salidas.put(payload)

	'''
	*  @brief Imprime en salida estandar cuando se cierra una conexión
	*  details Esta subrutina imprime un mensaje cuando un cliente 
	*  cierra el navegador.
 	*  @remark N/A
 	*  @return N/A
 	*  @exception N/A
	*  @author Jeisson Hidalgo
	*  @date 28-9-20
	'''
	def onClose(self, wasClean, code, reason):
		print(f"WebSocket connection closed: {reason}")
		conexiones.pop(0)
		salidas.put(CODIGO)

	'''
	*  @brief Envía mensajes a la página web
	*  @details Esta subrutina envía mensajes a la página web por medio
	*  del web socket, para que el usuario pueda visualizarlo
	*  @param[in] [1] mensaje: Es el mensaje que se envía a la página
	*  web, para que el usuario pueda visualizarlo
	*  @param[out] N/A
 	*  @remark N/A
 	*  @return N/A
 	*  @exception N/A
	*  author Jeisson Hidalgo
	*  date 15-11-20
	'''
	def enviarMensaje(self, mensaje):
		mensaje = mensaje.decode('utf8')
		self.sendMessage(f'{mensaje}'.encode('utf8'), False)


'''
 *	@brief Subrutina para enviar mensajes al agente azul
 *	@details Esta subrutina utiliza uno de los pipes para enviarle
 *  los mensajes al agente azul, el cual se encarga de pasar este
 *  mensaje al nodo verde, estos mensajes llegan por el web socket
 *  por parte del usuario
 *  @param[in] N/A
 *  @param[out] N/A
 *  @pre Este método es ejecutado por un hilo, debe permanecer
 *  corriendo durante toda la ejecucuión
 *  @remark Debe implementar sleep para que el pipe no falle
 *  @return N/A
 *  @exception N/A
 *	@author Johel Phillips Ugalde B75821
 *	@date 01-12-20
 '''
def enviar():

	fd = os.open(pipeSalida, os.O_WRONLY)

	while True:
		mensaje = salidas.get(block = True)
		salidas.task_done()
		if len(mensaje) >= LONGITUD:
			mensaje = mensaje[:LONGITUD-1]
		os.write(fd, mensaje + '\x00'.encode('ASCII', 'ignore'))
		time.sleep(0.01)
		if(mensaje.find(CODIGO) != -1):
			break
	os.close(fd)

'''
 *	@brief Subrutina para recibir mensajes del agente azul y 
 *  reenviarlos por el web socket a la página web
 *	@details Esta subrutina utiliza uno de los pipes para recibir
 *  los mensajes provenientes de otro usuario conectado a un nodo
 *  vecino y reenviarlos a la página web, para que el usuario conectado
 *  a este nodo pueda visualizar sus mensajes
 *  @param[in] N/A
 *  @param[out] N/A
 *  @pre Este método es ejecutado por un hilo, debe permanecer
 *  corriendo durante toda la ejecucuión
 *  @remark Debe implementar sleep para que el pipe no falle
 *  @return N/A
 *  @exception N/A
 *	@author Johel Phillips Ugalde B75821
 *	@date 01-12-20
 '''
def recibir():
	
	final = '\x00'.encode('ASCII', 'ignore')
	fd = open(pipeEntrada, 'br')

	while True:
		mensaje = fd.read(LONGITUD)
		time.sleep(0.01)
		if(mensaje[2] != "'"):
			if(mensaje.find(CODIGO[:3]) != -1):
				reactor.callFromThread(reactor.stop)
				break
			posicion = mensaje.find(final)
			mensaje = mensaje[:posicion]
			conexiones[0].enviarMensaje(mensaje)
	fd.close()

'''
 *	@brief Subrutina para almacenar la información de los nodos
 *	@details Esta subrutina divide las datos que entran como tercer
 *	argumento del programa, para tener acceso a los ids de los nodos
 *	vecinos y el id propio
 *	@param[in] [1] ids: Esta lista almacenará los datos de los ids
 *	[2] nodos: Hilera que contiene los ids de los nodos
 *	@author Johel Phillips Ugalde B75821
 *	@date 20-11-20
 '''
def almacenarIDs(ids, nodos):
	len = nodos.count(',')
	for i in range(len):
		pos = nodos.find(',')
		ids.append(nodos[:pos])
		nodos = nodos[pos+1:]


if __name__ == '__main__':

	pipeEntrada = sys.argv[1]
	pipeSalida = sys.argv[2]
	nodos = sys.argv[3]
	host = sys.argv[4]
	port = int(sys.argv[5])

	ids = []
	salidas = queue.Queue()

	emisor = threading.Thread(target = enviar)
	receptor = threading.Thread(target = recibir)
	almacenarIDs(ids, nodos)

	log.startLogging(sys.stdout)
	factory = WebSocketServerFactory(f"ws://{host}:{port}")
	factory.protocol = WebSocketServer
	reactor.listenTCP(port, factory)
	
	emisor.start()
	receptor.start()
	reactor.run()

	emisor.join()
	receptor.join()
	salidas.join()