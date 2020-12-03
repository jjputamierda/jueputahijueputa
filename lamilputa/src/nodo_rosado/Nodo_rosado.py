import os, sys, time, socket, select, queue
from threading import Thread
import threading

# generate random integer values
from random import seed
from random import randint
# seed random number generator

s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
hijosAg = []
vivo = 1

nodos = []  ###Variables de dijkstra
matriz = []
caminos = []
forwarding = []
'''
    @brief Metodo que recibe datos del socket por la cola de datos 
    @details Este metodo se encarga de recibir datos por medio de la cola de datos que recibieron por el socekt y 
    fueron enviados por el agente rosado
    el dato
    @param[in] N/A
    @param[out] cola de datos
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 15-09-20
'''
def respuestaTCP(serverQueue):
    paquete = []
    mensajeRecibir=None

    try:
        mensajeRecibir = serverQueue.get(block=True, timeout=20)

    except queue.Empty:
        pass

    if(mensajeRecibir!= None):

        for item in mensajeRecibir.split(","):
            print("Esto es item en python")
            print("XXXXXXXXXXXXXXXXXXX")
            print(item)
            print("XXXXXXXXXXXXXXXXXXX")
            try:
                paquete.append(int(item))
            except:
                pass
                paquete.append(111)
            #else:
                #print("Entre a else dato no es numerico")
                #paquete.append("-14")
            
    else:
        mensajeRecibir = "-1,-1,-1,-1,-1"

        for item in mensajeRecibir.split(","):
            paquete.append(int(item))

    return paquete
'''
    @brief Metodo que pasa datos para que sean enviados por el socket
    @details Este metodo se encarga de enviar datos por medio de una cola
    de datos al socket para que sean mandados al agente rosado
    @param[in] N/A
    @param[out] cola de datos , paquete con datos
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 15-09-20
'''
def solicitudTCP(paquete,clientQueue):

    if(paquete[0:2]=="70"):
        clientQueue.put(paquete)
    else:
        mensajeEnviar = str(paquete[0]) + "," + str(paquete[1]) +","+ \
        str(paquete[2]) + "," + str(paquete[3]) + "," + str(paquete[4])
        #print("Esto es mensaje enviar en python")
        #print("EEEEEEEEEEEEEEEEEEEEEEEEE")
       # print(mensajeEnviar)
        #print("EEEEEEEEEEEEEEEEEEEEEEEEE")
        clientQueue.put(mensajeEnviar)

        return paquete
'''
    @brief Metodo que reinicia el paquete de dtaos
    @details Este metodo se encarga de limpiar y reiniciar el paquete de datos
    @param[in] N/A
    @param[out] N/A
    @pre N/A
    @remark N/A
    @return N/A
    @exception N/A
    @author Juan Jose Herrera B8
    @date 15-09-20
'''
def reinicioPaquete():
    paquete = [0,0,0,0]
'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def TWHResponderPapa(numeroNodo,clientQueue,serverQueue,
reintentos,papaAg,miembroAg):

    seed(1)
    rn = randint(2,999)
    nuevoReintento = 0
    salirWhile = 0
    sn = 0
    exito = 0
    potencialHijo = 0
    while(nuevoReintento < reintentos and  salirWhile == 0):

        paquete = respuestaTCP(serverQueue)
        if (paquete[0]== 2 ):
            print("me llego una solicitud11")

            sn = paquete [1]
            salirWhile = 1
            exito = 1
        nuevoReintento = nuevoReintento + 1
    sn = sn + 1
    if(exito == 1):
        nuevoReintento = 0
        salirWhile = 0
        exito = 0
        destino = paquete [4]
        potencialHijo=destino
        paquete = [3,sn,rn,destino,numeroNodo]
        while(nuevoReintento < reintentos and  salirWhile != 1):
            solicitudTCP(paquete,clientQueue)
            paqueteR = respuestaTCP(serverQueue)
            if (paqueteR[0]== 5 and paqueteR[1] == sn
            and paqueteR[2] == rn +1 ):

                print("me llego un ok 1")
                paquete = [6,sn+1,rn+1,destino,numeroNodo]
                solicitudTCP(paquete,clientQueue)
                salirWhile = 1
                exito = 1
                global hijosAg
                hijosAg.append(potencialHijo)
                enviarBroadcast(0,potencialHijo,clientQueue)
            nuevoReintento = nuevoReintento + 1
'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def enviarBroadcast(esPapa,miembroArbol,clientQueue):
    if (esPapa==1):
        paquete=[43,43,43,miembroArbol,43]
    else:
        paquete=[44,44,44,miembroArbol,44]

    solicitudTCP(paquete,clientQueue)
'''
    @brief Metodo que genera un paquete que indica que ya se es parte del arbol
    @details Este metodo se encarga de generar un paquete que indica que ya se es parte del
    arbol y se pasa el paquete para que sea enviado por el socket al agente rosado
    @param[in] N/A
    @param[out] cola de datos 
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 15-09-20
'''
def enivarConfirmacionArbol(clientQueue):
    paquete=[45,45,45,45,45]
    solicitudTCP(paquete,clientQueue)
'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def TWHNoRespondePapa(numeroNodo,clientQueue,serverQueue,reintentos):
    paquete = reinicioPaquete()
    seed(1)
    rn = randint(2,999)
    nuevoReintento = 0
    salirWhile = 0
    sn = 0
    exito = 0
    potencialHijo = 0
    while(nuevoReintento < reintentos and  salirWhile == 0):
        paquete = respuestaTCP(serverQueue)
        if (paquete[0]== 2):
            print("me llego una solicitud22")
            sn = paquete [1]
            salirWhile = 1
            exito = 1
        nuevoReintento = nuevoReintento + 1
    sn = sn + 1
    if(exito == 1):
        nuevoReintento = 0
        salirWhile = 0
        exito = 0
        destino = paquete [4]
        paquete = [4,sn,rn,destino,numeroNodo]
        while(nuevoReintento < reintentos and  salirWhile == 0):

            solicitudTCP(paquete,clientQueue)
            paqueteR = respuestaTCP(serverQueue)
            if (paqueteR[0]== 5 and paqueteR[1] == sn
            and paqueteR[2] == rn +1 ):
                print("me llego un ok22")
                paquete = [6,sn+1,rn+1,destino,numeroNodo]
                salirWhile = 1
                exito = 1

            nuevoReintento = nuevoReintento + 1

'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def TWHSolicitudPapa(numeroNodo,potencialPapa,clientQueue,
serverQueue,reintentos):

    seed(1)
    sn = randint(2,999)
    paquete = [2,sn,0,potencialPapa,numeroNodo]
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    viejoPaqueteOp = 0
    papaAg = -1
    miembroAg = 0
    while(nuevoReintento < reintentos and salirWhile ==0):
        solicitudTCP(paquete,clientQueue)
        paqueteR = respuestaTCP(serverQueue)
        if (paqueteR[0]== 3 and paqueteR[1] == sn + 1):
            print("Aceptaron ser mi papa")
            salirWhile = 1
            exito = 1
            viejoPaqueteOp = paqueteR[0]
        else:
            if(paqueteR[0]== 4 and paqueteR[1] == sn + 1):
                print("Aceptaron ser mi papa no")
                salirWhile = 1
                exito = 1
                viejoPaqueteOp = paqueteR[0]

        nuevoReintento = nuevoReintento + 1

    #sn = paqueteR[1]
    #rn = paqueteR[2] + 1
    if(exito == 1):
        sn = paqueteR[1]
        rn = paqueteR[2] + 1
        nuevoReintento = 0
        salirWhile = 0
        exito = 0
        paquete = [5,sn,rn,potencialPapa,numeroNodo]
        while(nuevoReintento < reintentos and salirWhile == 0):
            solicitudTCP(paquete,clientQueue)
            paqueteR = respuestaTCP(serverQueue)

            if (paqueteR[0]== 6 and paqueteR[1] == sn + 1):
                print("me llego un ok de papa")
                salirWhile = 1
                exito = 1
                if(viejoPaqueteOp == 2 ):
                    papaAg = potencialPapa
                    #time.sleep(2)
                    enviarBroadcast(1,papaAg,clientQueue)
                    miembroAg = 1
            nuevoReintento = nuevoReintento + 1

    vectorRespuestaP = [exito,papaAg,miembroAg]
    return vectorRespuestaP
'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''

def reconfiguracionPapa(serverQueue,clientQueue,reintentos):
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    potencialPapa = -1
    vectorRespuestaP = [exito,potencialPapa]
    paqueteL = [-101,-101,-101,-101,-101]
    while(nuevoReintento < reintentos and salirWhile == 0):
        solicitudTCP(paqueteL,clientQueue)

        paquete = respuestaTCP(serverQueue)

        if (paquete[0] ==  0 and paquete[1] == 1 and
        paquete[2] == 1 ):#[0,1,1,1,3]
            salirWhile = 1
            potencialPapa = paquete[4]
            paquete = [1,1,1,1,1]
            solicitudTCP(paquete,clientQueue)
            exito = 1
        nuevoReintento = nuevoReintento + 1
    vectorRespuestaP = [exito,potencialPapa]

    if(salirWhile == 0):
        paquete = [-20,-20,-20,-20,-20]
        solicitudTCP(paquete,clientQueue)
        time.sleep(0.3)
    return vectorRespuestaP

'''
    @brief Metodo que genera una solicutud para averiguar la cantidad de reintentes para 
    realizar preguntas a un vecino si es parte del arbol
    @details Este metodo se encarga de generar una solicitud para averiguar cual es la cantidad de veces que se le 
    va a pregunatr a un vecino si es parte del arbol
    @param[in] N/A
    @param[out] dos cola de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan Jose Herrera B8
    @date 15-09-20
'''
def obtenerReintentos(serverQueue,clientQueue):
    reintentos = 7
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    paqueteL = [-101,-101,-101,-101,-101]

    while(nuevoReintento < reintentos and salirWhile == 0):

        solicitudTCP(paqueteL,clientQueue)
        paquete = respuestaTCP(serverQueue)  #[0,0,0,1]

        if(paquete[0] ==  0 and paquete[1] == 0 and
            paquete[2] == 0 ):

            reintentos = paquete[4]
            salirWhile = 1
            paquete = [1,1,1,1,1]
            solicitudTCP(paquete,clientQueue)
            exito = 1
        nuevoReintento = nuevoReintento + 1

    if(salirWhile == 0):
        paquete = [-20,-20,-20,-20,-20]
        solicitudTCP(paquete,clientQueue)
        time.sleep(0.3)
    vectorRespuesta = [exito,reintentos]
    return vectorRespuesta


def buscarPapa(numeroNodo,serverQueue,clientQueue,reintentos):
    exito = 0
    vectorRespuestaP = reconfiguracionPapa(serverQueue,
    clientQueue,reintentos)

    potencialPapa=vectorRespuestaP[1]
    exito = vectorRespuestaP[0]
    if(exito == 1):
        vectorRespuestaD = TWHSolicitudPapa(numeroNodo,potencialPapa,
        clientQueue,serverQueue,reintentos)

        vectorRespuestaP = vectorRespuestaD

    return vectorRespuestaP


def revisarNodosMuertos(numeroNodo,clientQueue,serverQueue,reintentos):
    muertos = 0
    reintentos = 7
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    while(nuevoReintento < reintentos or salirWhile == 0):
        paquete = respuestaTCP(serverQueue)
        if (paquete[0] >  -1 and paquete[1] == -1 and
        paquete[2] == -1 ):
            nodoMuerto = paquete[3]
            salirWhile = 1
            if(nodoMuerto == papaAg):
                muertos ==1
                for x in hijosAg:
                    paquete = [-1.-1.-1,x,numeroNodo]
                    solicitudTCP(paquete,clientQueue)
            else:
                i = 0
                while (i < len(hijosAg)):
                    if(hijosAg[i]==nodoMuerto):
                        hijosAg.remove(i)

        nuevoReintento = nuevoReintento + 1

'''
    @brief Metodo que genera una solicutud para averiguar la cantidad de reintentes para 
    realizar preguntas a un vecino si es parte del arbol
    @details Este metodo se encarga de generar una solicitud para averiguar cual es la cantidad de veces que se le 
    va a pregunatr a un vecino si es parte del arbol
    @param[in] N/A
    @param[out] dos cola de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan Jose Herrera B8
    @date 15-09-20
'''
def responderSolicitudesPapa(numeroNodo,clientQueue,serverQueue,
reintentos,papaAg,miembroAg):

    muertos = 0
    global vivo
    while(vivo == 1):
        #print("Respondiendo solicitudes")
        enivarConfirmacionArbol(clientQueue)

        TWHResponderPapa(numeroNodo,clientQueue,serverQueue,
        reintentos,papaAg,miembroAg)

'''
    @brief Metodo que genera una matriz con los datos de los nodos
    
    @details Este metodo se encarga de generar una con los datos de los nodos
    @param[in] N/A
    @param[out] cola de datos y paquete de datos
    @pre Que la cola de datos fuera creada y que el paquete fuera correcftamente inicializado
    @remark N/A
    @return N/A
    @exception N/A
    @author Johel Phillipes B8
    @date 15-09-20
'''
def generarMatriz(data,clientQueue):
    datos = data.split(sep=',')
    indice = 0

    while indice < len(datos):
      nuevoNodo = True
      j = 0
      while nuevoNodo and j < len(nodos):
        if nodos[j] == int(datos[indice]):
          nuevoNodo = False
        j += 1
      if nuevoNodo:
        nodos.append(int(datos[indice]))

      indice += 2

    while len(matriz) < len(nodos):
      matriz.append([])
      forwarding.append([])

    for i in range(len(matriz)):
      while len(matriz[i]) < len(matriz)-1:
        matriz[i].append(1000)
        forwarding[i].append(1000)

    nuevo = 0
    for i in range(len(nodos)):
      if int(datos[0]) == nodos[i]:
        nuevo = i

    indice = 2
    for i in range(int(datos[1])):
      for j in range(len(nodos)):
        if int(datos[indice]) == nodos[j]:
          if j == 0:
            matriz[0][nuevo-1] = int(datos[indice+1])
          else:
            matriz[nuevo][j-1] = int(datos[indice+1])
      indice += 2

    for i in range(len(nodos)-1):
      caminos.append([0, 1000])


    dijkstra(matriz, caminos, forwarding,clientQueue)

'''
    @brief Metodo que ejecuta el algoritmo de dijstra
    @details Este metodo se encarga de ejecutar el algoritmo de dijkstra para encontrar la ruta mas corta a los nodos
    @param[in] N/A
    @param[out] cola de datos
    @pre Que la cola de datos fuera creada
    @remark 
    @return N/A
    @exception N/A
    @author Johel Phillipes B8
    @date 15-09-20
'''
def dijkstra(pesos, caminos, forwarding,clientQueue):
    i = 0
    j = 0
    acumulado = []
    indiceI = []
    indiceJ = []
    padre = 0
   
    while i < len(matriz):
      if len(indiceI):
        i = indiceI[-1]
        j = indiceJ[-1]
        indiceI.pop(-1)
        indiceJ.pop(-1)
        acumulado.pop(-1)
        padre = 0
      while j < len(matriz)-1:
        if pesos[i][j]:
          acumuladoActual = 0
          if not padre:
              padre = j + 1
          if i <= j:

            acumulado.append(pesos[i][j])
            for n in range(len(acumulado)):
              acumuladoActual += acumulado[n]
            if acumuladoActual < forwarding[i][j]:
              forwarding[i][j] = acumuladoActual
              if caminos[j][1] > acumuladoActual:
                caminos[j][0] = padre
                caminos[j][1] = acumuladoActual
            indiceI.append(i)
            indiceJ.append(j+1)
            i = j + 1
            j = 0
          else:
            for n in range(len(acumulado)):
              acumuladoActual += acumulado[n]
            acumuladoActual += pesos[i][j]
            if acumuladoActual < forwarding[i][j]:
              forwarding[i][j] = acumuladoActual
              if caminos[j][1] > acumuladoActual:
                caminos[j][0] = padre
                caminos[j][1] = acumuladoActual
            j += 1
        else:
          forwarding[i][j] = 1000
          j += 1
      if not len(indiceI):
        i += 1

    paquete="70"
    for i in range(len(matriz)-1):
        paquete += "," + str(nodos[caminos[i][0]]) + "-" + \
        str(nodos[caminos[i][1]]) + "-" + str(nodos[i+1])

    solicitudTCP(paquete,clientQueue)
'''
    @brief Metodo que genera un socket para recibir datos del agente rosado
    @details Este metodo se encarga de generar un socket para recibir datos que son enviados desde el agente rosado
    @param[in] N/A
    @param[out] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def serverTcp(serverQueue,clientQueue):
    bandera = 1
    while(bandera == 1):

        data = s1.recv(1024)
        data=data.decode("utf-8")
        prueba=data[0:2]
        if(prueba == "90"):
            data=data[3:]
            generarMatriz(data,clientQueue)
        elif (prueba != "90"):
            serverQueue.put(data)
            if(data == "-100,-100,-100,-100,-100"):
                global vivo
                vivo = 1

'''
    @brief Metodo que genera un socket para enviar datos al agente rosado
    @details Este metodo se encarga de generar un socket para enviar datos al agente rosado
    @param[in] N/A
    @param[out]  cola de datos 
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-09-20
'''
def clientTcp(clientQueue):

    global vivo
    while vivo == 1:
        data=None
        try:
            data = clientQueue.get(block = True,timeout=30)
            print("Esto es data en python")
            print("aaaaaaaaaaaaaaaaaaa")
            print("aaaaaaaaaaaaaaaaaaa")
            print(data)
            print("aaaaaaaaaaaaaaaaaaa")
            print("aaaaaaaaaaaaaaaaaaa")

        except queue.Empty:
            pass
        if data != None:
            s2.send(data.encode())


def arGe(serverQueue,clientQueue):

    numeroNodo = -1
    numeroNodo = int(sys.argv[1])
    miembroAg = 0
    m = str(numeroNodo) + "numero nodo Python"
    papaAg = -1

    reintentos = -1
    bandera = True
    global vivo
    while(bandera == True and vivo == 1):
        vectorRespuesta = obtenerReintentos(serverQueue,clientQueue)
        exito = vectorRespuesta[0]
        reintentos = vectorRespuesta[1]
        if(exito ==1 ):
            bandera = False
    while(vivo == 1):
        #numero de nodo se puede pasar por parametro
        papaAg = -1
        global hijosAg
        hijosAg = []
        numeroAg = 0
        if(numeroNodo == 1):
            miembroAg =1

        if(miembroAg == 0):
            vectorRespuestaP = buscarPapa(numeroNodo,serverQueue,
            clientQueue,reintentos)

            if(vectorRespuestaP[0] == 1):
                papaAg = vectorRespuestaP[1]
                miembroAg = vectorRespuestaP[1]

                responderSolicitudesPapa(numeroNodo,clientQueue,
                serverQueue,reintentos,papaAg,miembroAg)
        else:
            responderSolicitudesPapa(numeroNodo,clientQueue,
            serverQueue,reintentos,papaAg,miembroAg)

if __name__=='__main__':

    time.sleep(5)

    HOST,PORT = 'localhost', int(sys.argv[2])
    s1.connect((HOST, PORT))

    HOST2,PORT2 = 'localhost', int (sys.argv[3])

    s2.connect((HOST2, PORT2))

    clientQueue = queue.Queue()
    serverQueue = queue.Queue()
    serverSolicitudes = queue.Queue()

    hiloCliente = Thread(target = clientTcp, args = (clientQueue,))
 
    hiloCliente.start()

    hiloServer=Thread(target=serverTcp,args=(serverQueue,clientQueue,))

    hiloServer.start()

    aG = Thread(target = arGe, args = (serverQueue,clientQueue,))

    aG.start()