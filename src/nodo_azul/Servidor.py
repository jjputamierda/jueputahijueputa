#!/usr/bin/env python3

import threading
import sys
import codecs
import os
from Mensaje import Mensaje

'''
 *	@brief Subrutina para enviar mensajes
 *	@details Esta subrutina utiliza uno de los pipes para enviarle
 *  los mensajes al agente azul, el cual se encarga de pasar este
 *  mensaje al nodo verde
 *	@param[in] [1] evento: Este evento es quien se encarga de parar
 *  la subrutina (evento.wait()) para evitar el busy waiting, cuando se
 *  quiere enviar un mensaje el hilo principal hace un evento.set() 
 *	[2] salidas: Es una lista de objetos Mensaje, para que los mensajes
 *  no se pierdan en el caso de que se requiera enviar muchos seguidos
 *	[3] mutex: Este mutex evita que hayan conflictos en la lista de 
 *  mensajes
 *	[4] pipe: Ruta para el pipe Servidor -> Agente
 *	@author Johel Phillips Ugalde B75821
 *	@date 29-09-20
 '''
def enviar(evento, salidas, mutex, pipe):

	fd = os.open(pipe, os.O_WRONLY)

	while 1:
		evento.wait()
		evento.clear()
		mutex.acquire()
		mensaje = str(salidas[0].getId())+'::'+salidas[0].getMensaje()
		salidas.pop(0)
		mutex.release()
		os.write(fd, mensaje.encode('ASCII', 'ignore')+
			'\x00'.encode('ASCII', 'ignore'))
		if(mensaje.find('-1::Salir') != -1):
			break
	os.close(fd)

'''
 *	@brief Subrutina para recibir mensajes
 *	@details Esta subrutina utiliza uno de los pipes para recibir
 *  los mensajes provenientes de otro usuario conectado a un nodo
 *  vecino
 *	@param[in] [1] entradas: Es una lista de objetos Mensaje, para que
 *  los mensajes que lleguen no se peirdan hasta que el usuario los lea
 *	[3] mutex: Este mutex evita que hayan conflictos en la lista de 
 *  mensajes
 *	[4] pipe: Ruta para el pipe Agente -> Servidor
 *	@author Johel Phillips Ugalde B75821
 *	@date 29-09-20
 '''
def recibir(entradas, mutex, pipe):
	
	codigo = '-1::Salir'.encode('ASCII', 'ignore')
	separador = '::'.encode('ASCII', 'ignore')
	fd = open(pipe, 'br')

	while 1:
		mensaje = fd.read(207)
		pos = mensaje.find('\x00'.encode('ASCII', 'ignore'))
		mensaje = mensaje[:pos]
		if(mensaje.find(codigo) != -1):
			break
		indice = mensaje.find(separador)
		idVecino = int(mensaje[0:indice])
		hilera = mensaje[indice+2:]
		mutex.acquire()
		entradas.append(Mensaje(idVecino, hilera))
		mutex.release()
	fd.close()

'''
 *	@brief Subrutina para verificar que la entrada sea un int
 *	@details Esta subrutina entra en un bucle del cual solo se
 *  puede salir si el usuario digita un número entero
 *	@author Johel Phillips Ugalde B75821
 *	@date 29-09-20
 '''
def entradaInt():
	while 1:
		try:
			entrada = int(input())
			break
		except:
			print('Error en la entrada, reintente ',end='')
	return entrada

'''
 *	@brief Subrutina para almacenar la información de los nodos
 *	@details Esta subrutina divide las datos que entran como tercer
 *	argumento del programa, para tener acceso a los ids de los nodos
 *	vecinos y el id propio
 *	@param[in] [1] Lista: Esta lista almacenará los datos de los ids
 *	[2] String: Hilera que contiene los ids de los nodos
 *	@author Johel Phillips Ugalde B75821
 *	@date 29-09-20
 '''
def almacenarIDs(ids, nodos):
	len = nodos.count(',')
	for i in range(len):
		pos = nodos.find(',')
		ids.append(nodos[:pos])
		nodos = nodos[pos+1:]

'''
 *	@brief Menú para los usuarios
 *	@details Este menú le permite a los usuarios escribir mensajes
 *  para enviarlos a otros usuarios conectados a un nodo vecino,
 *  además de notificar cuando llegan mensajes para poder visualizarlos
 *	@pre Los pipes deben ser creados con anterioridad para poder tener
 *  comunicación con el agente azul, para el paso de mensajes.
 *	@author Johel Phillips Ugalde B75821
 *	@date 29-09-20
'''
pipeEntrada = sys.argv[1]
pipeSalida = sys.argv[2]
nodos = sys.argv[3]
ids = []
salidas = []
entradas = []
almacenarIDs(ids, nodos)
mutex = threading.Lock()
evento = threading.Event()
emisor = threading.Thread(target=enviar,
		args=(evento,salidas,mutex,pipeSalida))
receptor = threading.Thread(target=recibir,
		args=(entradas,mutex,pipeEntrada))

emisor.start()
receptor.start()
print('\nBienvenido al sistema')

while 1:
	print('\nID ['+ids[0]+']\nVecinos ',end='')
	for i in range(len(ids)):
		print('['+ids[i]+'] ',end='')
	print('\nUsted tiene [' + str(len(entradas))+ '] mensajes nuevos\n'
	 	+'\t1- Escribir mensaje\n\t2- Revisar sus mensajes\n\t3- '+
	 	'Refrescar\n\t4- Salir\n¿Qué desea realizar?\t',end='')
	eleccion = entradaInt()

	if(eleccion == 1):

		print('Digite el id del usuario destino: ',end='')
		idDestino = entradaInt()
		if(idDestino > 10000 or idDestino < 1):
			idDestino = 1
			print('Dato inválido, se agignó 1 por id')

		while 1:
			mensaje = input('Redacte su mensaje:\n')
			if(len(mensaje) <= 200):
				break
			else:
				print('Su mensaje no debe sobrepasar los 200'
						' caracteres\n')

		mutex.acquire()
		salidas.append(Mensaje(idDestino, mensaje))
		mutex.release()
		evento.set()

	elif(eleccion == 2):

		if(len(entradas) != 0):
			mutex.acquire()
			print('\n\nMensaje de [',end='')
			print(entradas[0].getId(),end='')
			print(']')
			print(codecs.encode(str(entradas[0].getMensaje()),'utf-8'))
			entradas.pop(0)
			mutex.release()
		else:
			print('Usted no tiene mensajes :(')
		input('\nPresione enter para continuar\n\n')

	elif(eleccion == 3):
		print('\n\n\n\n\n\n')

	elif(eleccion == 4):

		print('\nGracias por usar el sistema :)\n')
		mutex.acquire()
		salidas.append(Mensaje(-1, 'Salir'))
		mutex.release()
		evento.set()
		break

	else:
		print('\nOpción inválida')
		input('Presione enter para continuar\n')
		print('\n\n\n\n\n\n')


emisor.join()
receptor.join()