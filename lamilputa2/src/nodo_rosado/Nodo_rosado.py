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
salirNodo = False
reinicioPapa = 0
papaAg = -1

nodos = []  ###Variables de dijkstra
matriz = []
caminos = []
forwarding = []
'''
    @brief Metodo que recibe datos del socket por la cola de datos 
    @details Este metodo se encarga de recibir datos por medio de 
    la cola de datos que recibieron por el socekt y 
    fueron enviados por el agente rosado
    el dato
    @param[out] N/A
    @param[in] cola de datos
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return paquete
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 15-09-20
'''
def respuestaTCP(serverQueue):
    paquete = []
    mensajeRecibir=None

    try:
        #Se recibe un paquete de la cola de server
        mensajeRecibir = serverQueue.get(block=True, timeout=20)

    except queue.Empty:
        pass

    if(mensajeRecibir!= None):
        
        for item in mensajeRecibir.split(","):
            try:
                paquete.append(int(item))
            except:
                pass
                paquete.append(111)
            
            
    else:
        #Se recibó un mensaje vació
        mensajeRecibir = "-1,-1,-1,-1,-1"

        for item in mensajeRecibir.split(","):
            paquete.append(int(item))

    return paquete
'''
    @brief Metodo que pasa datos para que sean enviados por el socket
    @details Este metodo se encarga de enviar datos por medio de una cola
    de datos al socket para que sean mandados al agente rosado
    @param[out] N/A
    @param[in] cola de datos , paquete con datos
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return paquete
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 15-09-20
'''
def solicitudTCP(paquete,clientQueue):
    #Mensaje de alcanzabilidad
    if(paquete[0:2]=="70"):
        clientQueue.put(paquete)
    else:
        #Mensaje que no es de alcanzabilidad
        mensajeEnviar = str(paquete[0]) + "," + str(paquete[1]) +","+ \
        str(paquete[2]) + "," + str(paquete[3]) + "," + str(paquete[4])
        
        clientQueue.put(mensajeEnviar)

        return paquete

'''
    @brief Metodo verifica si la desconexión del
    arbol generador fue de un hijo o del papa y decide
    que le va a reportar al módulo de broadcast para
    avisarle de la desconexión
    @details Se revisan solicitudes de todo tipo de 
    posibles nodos muertos del árbol generador y se responden 
    debidamente
    @param[out] N/A
    @param[in] numeroNodo,clientQueue,candidatoMuerto
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos y el paquete a enviar
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B83878
    @date 02-11-20
'''
def revisarQuienMurio(numeroNodo,clientQueue,candidatoMuerto):
    global hijosAg
    global papaAg
    #Caso se murio el papa
    if (candidatoMuerto == papaAg and numeroNodo != 1):
        avisarHijosDesconexion(numeroNodo,clientQueue)
        #Mensaje para indicar que se murió el papa
        paquete=[9,9,9,9,9]
        solicitudTCP(paquete,clientQueue)
    else:
        i = 0
        bandera = 0
        while (i < len(hijosAg) and bandera == 0):
            #Caso se murió un hijo
            if(hijosAg[i]==candidatoMuerto):
                hijosAg.remove(i)
                bandera = 1
                #Indicador de que se murió un hijo
                paquete=[46,46,46,hijosAg[i],46]
                solicitudTCP(paquete,clientQueue)
        #Caso ya no hay hijos
        if(len(hijosAg)== 0):
            global reinicioPapa
            reinicioPapa = 1
            if(numeroNodo != 1):
                paquete=[9,9,9,9,9]
                solicitudTCP(paquete,clientQueue)
        else:
            #Caso no se murió nadie
            if(numeroNodo != 1):
                paquete=[1,1,1,1,1]
                solicitudTCP(paquete,clientQueue)


        
        
       
        






    

'''
    @brief Metodo que avisa a sus hijos que ya no forman parte del árbol generador
    @details Se envía a los demás miembros del árbol generador que ya no van a 
    formar parte del árbol generador
    @param[out] N/A
    @param[in] numeroNodo,clientQueue
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos y el paquete a enviar
    @return N/A
    @exception N/A
    @author Juan Jose Herrera B83878
    @date 15-11-20
'''
def avisarHijosDesconexion(numeroNodo,clientQueue):
    global hijosAg
    for x in hijosAg:
        paquete = [7,0,0,x,numeroNodo]
        solicitudTCP(paquete,clientQueue)
        
    #Se avisa a broadcast que todos se murieron
    paquete=[47,47,47,47,47]
    solicitudTCP(paquete,clientQueue)
    global reinicioPapa
    reinicioPapa = 1


'''
    @brief Metodo se encarga de hacer el TWH 
    para responderle a los potenciales hijos y respondar solicitudes de potenciales nodos muertos
    @details Este método responde las solicitudes del TWH de 
    los vecinos que las envian y atiende alertas de nodos muertos
    @param[out] N/A
    @param[in] numeroNodo,clientQueue,serverQueue,
    reintentos
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan José Herrera B83878
    @date 15-09-20
'''
def TWHResponderPapa(numeroNodo,clientQueue,serverQueue,
reintentos):
    global reinicioPapa
    global salirNodo
    seed(1)
    rn = randint(2,999)
    nuevoReintento = 0
    salirWhile = 0
    #Sn para guardar datos de solicitudes
    sn = 0
    exito = 0
    potencialHijo = 0
    while(nuevoReintento < reintentos and  salirWhile == 0 and salirNodo == False and reinicioPapa == 0):
        #Se obtiene una solicitud de un nuevo nodo
        paquete = respuestaTCP(serverQueue)
        if (paquete[0]== 2 ):
        #Llegó una solictud nueva para tener papá

            sn = paquete [1]
            salirWhile = 1
            exito = 1
        else:
            #Llegó un mensaje para revisar si el vecino que se murió es parte del arbol generador
            if(paquete[0]== 8 and paquete[1]== 8 and paquete[2]== 8 and paquete[3]== 8 ):
                
                revisarQuienMurio(numeroNodo,clientQueue,paquete[4])
            else:
                #Solicitud para salirse del arbol generador
                if(paquete[0] == 7):
 
                    avisarHijosDesconexion(numeroNodo,clientQueue)
        nuevoReintento = nuevoReintento + 1
    sn = sn + 1
    #Si la primera parte del TWH fué exitodso, se continúa a la segunda parte
    if(exito == 1):
        nuevoReintento = 0
        salirWhile = 0
        exito = 0
        destino = paquete [4]
        potencialHijo=destino
        paquete = [3,sn,rn,destino,numeroNodo]
        while(nuevoReintento < reintentos and  salirWhile != 1 and salirNodo == False and reinicioPapa == 0):
            solicitudTCP(paquete,clientQueue)
            paqueteR = respuestaTCP(serverQueue)
            if (paqueteR[0]== 5 and paqueteR[1] == sn
            and paqueteR[2] == rn +1 ):

    
                paquete = [6,sn+1,rn+1,destino,numeroNodo]

                solicitudTCP(paquete,clientQueue)
                salirWhile = 1
                exito = 1
                global hijosAg
                #Se agrega un nuevo miembro para el árbol generador
                hijosAg.append(potencialHijo)
                enviarBroadcast(0,potencialHijo,clientQueue)
            else:
                if(paquete[0]== 8 and paquete[1]== 8 and paquete[2]== 8 and paquete[3]== 8 ):
                    
                    revisarQuienMurio(numeroNodo,clientQueue,paquete[4])
                else:
                    if(paquete[0] == 7):
                        
                        avisarHijosDesconexion(numeroNodo,clientQueue)    
            nuevoReintento = nuevoReintento + 1
'''
    @brief Método que envía los miembros del árbol al módulo del broadcast
    @details Este metodo se encarga de generar un socket para recibir datos
    que son enviados desde el agente rosado
    @param[out] N/A
    @param[in] esPapa,miembroArbol,clientQueue
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan José Herrera B83878
    @date 15-09-20
'''
def enviarBroadcast(esPapa,miembroArbol,clientQueue):
    if (esPapa==1):
        paquete=[43,43,43,miembroArbol,43]
    else:
        paquete=[44,44,44,miembroArbol,44]

    solicitudTCP(paquete,clientQueue)
'''
    @brief Metodo que inidica cuando se es parte del arbol generador
    @details Este método se utiliza como bandera para indicar a modulos
    esxternos que el nodo es parte del árbol generador
    @param[out] N/A
    @param[in] cola de datos 
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan Jose Herrera B8
    @date 29-11-20
'''
def enivarConfirmacionArbol(clientQueue):
    paquete=[45,45,45,45,45]
    solicitudTCP(paquete,clientQueue)
'''
    @brief Metodo se encarga de hacer el TWH 
    para responderle a los potenciales hijos que no se es
    parte del árbol genrador
    @details Este método responde las solicitudes del TWH de 
    los vecinos que las envian y se reponde que no se es 
    parte del árbol generador
    @param[out] N/A
    @param[in] numeroNodo,clientQueue,serverQueue,
    reintentos
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan José Herrera B83878
    @date 31-11-20
'''
def TWHNoRespondePapa(numeroNodo,clientQueue,serverQueue,reintentos):

    seed(1)
    rn = randint(2,999)
    nuevoReintento = 0
    salirWhile = 0
    sn = 0
    exito = 0
    potencialHijo = 0
    
    while(nuevoReintento < reintentos and  salirWhile == 0):
        paquete = respuestaTCP(serverQueue)
        #LLega una solicitud nueva solicitud
        if (paquete[0]== 2):
            sn = paquete [1]
            salirWhile = 1
            exito = 1
        nuevoReintento = nuevoReintento + 1
    sn = sn + 1
    if(exito == 1):
        #Se responde la primera parte del TWH
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
                #Se envia respuesta final de ack 
                paquete = [6,sn+1,rn+1,destino,numeroNodo]
                salirWhile = 1
                exito = 1

            nuevoReintento = nuevoReintento + 1

'''
    @brief Metodo que realiza el TWH para enviar solicitudes un
    potencial papá del árbol generador
    @details Evía solicitudes a otros miembros del árbol generador
    y realiza el TWH para las solicitudes
    @param[out] N/A
    @param[in] numeroNodo,potencialPapa,clientQueue,
    serverQueue,reintentos
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return vectorRespuestaP
    @exception N/A
    @author Juan José Herrera Rodríguez B83878
    @date 31-11-20
'''
def TWHSolicitudPapa(numeroNodo,potencialPapa,clientQueue,
serverQueue,reintentos):

    seed(1)
    sn = randint(2,999)
    #Se crea paquete para iniciar el TWH con una solicitud
    paquete = [2,sn,0,potencialPapa,numeroNodo]
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    viejoPaqueteOp = 0
    papaAg = -1
    miembroAg = 0
    #Se espera una respuesta de la solicitud que se enviará 
    #cada cierta cantidad de veces (reintentos)
    while(nuevoReintento < reintentos and salirWhile ==0):
        #Se envía una solicitud
        solicitudTCP(paquete,clientQueue)
        paqueteR = respuestaTCP(serverQueue)
        if (paqueteR[0]== 3 and paqueteR[1] == sn + 1):
            #Solicitud con sí de potencial papá
            salirWhile = 1
            exito = 1
            viejoPaqueteOp = paqueteR[0]
        else:
            if(paqueteR[0]== 4 and paqueteR[1] == sn + 1):
                #Solicitud con no de potencial papá
                salirWhile = 1
                exito = 1
                viejoPaqueteOp = paqueteR[0]

        nuevoReintento = nuevoReintento + 1

    if(exito == 1):
        #Se continua si se envió un sí o no del potencial papá
        paquete = [1,1,1,1,1]
        solicitudTCP(paquete,clientQueue)
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
                salirWhile = 1
                exito = 1
                if(viejoPaqueteOp == 3 ):
                    papaAg = potencialPapa
                 
                    enviarBroadcast(1,papaAg,clientQueue)
                    miembroAg = 1
            
            nuevoReintento = nuevoReintento + 1
        if(salirWhile ==0):
            paquete = [-1,-1,-1,-1,-1]
            solicitudTCP(paquete,clientQueue)
        else:
            paquete = [1,1,1,1,1]
            solicitudTCP(paquete,clientQueue)

    else:
        paquete = [-1,-1,-1,-1,-1]
        solicitudTCP(paquete,clientQueue)

    vectorRespuestaP = [exito,papaAg,miembroAg]
    return vectorRespuestaP
'''
    @brief Metodo para obtener un candidato a ser papá
    @details Método que realiza un 2WH para obtener el 
    potencial papá
    
    @param[in] serverQueue,clientQueue,reintentos
    @param[out] N/A
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return vectorRespuestaP
    @exception N/A
    @author Diego Barquero Quesada B80961/Juan José Herrera Rodríguez B83878
    @date 15-09-20
'''

def reconfiguracionPapa(serverQueue,clientQueue,reintentos):
    nuevoReintento = 0
    salirWhile = 0
    exito = 0
    potencialPapa = -1
    vectorRespuestaP = [exito,potencialPapa]
    #Paquete para comunicarse con el agente rosado y
    #avisarle que está listo para recibir un potencial
    #papá
    paqueteL = [-101,-101,-101,-101,-101]

    while(nuevoReintento < reintentos and salirWhile == 0):
        
        solicitudTCP(paqueteL,clientQueue)

        paquete = respuestaTCP(serverQueue)

        if (paquete[0] ==  0 and paquete[1] == 1 and
        paquete[2] == 1 ):
            #Se recibe el mensaje con el potencial papá
            salirWhile = 1
            potencialPapa = paquete[4]
            paquete = [1,1,1,1,1]
            #Se envía una confirmación al agente
            solicitudTCP(paquete,clientQueue)
            exito = 1
        nuevoReintento = nuevoReintento + 1
    vectorRespuestaP = [exito,potencialPapa]

    if(salirWhile == 0):

        paquete = [-20,-20,-20,-20,-20]
        #No llegó un potencial papá
        solicitudTCP(paquete,clientQueue)
        time.sleep(0.3)
    return vectorRespuestaP

'''
    @brief Metodo que obtiene la contidad de reintentos
    de su respectivo agente
    @details Como parte de iniciación del nodo rosado, se 
    obtiene la cantidad de reintentos para futuros métodos
    @param[out] N/A
    @param[in] dos cola de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return vectorRespuesta
    @exception N/A
    @author Juan Jose Herrera B83878
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
        paquete = respuestaTCP(serverQueue)  

        if(paquete[0] ==  0 and paquete[1] == 0 and
            paquete[2] == 0 ):
            #Llegó paquete con cantidad de reintentos
            reintentos = paquete[4]
            salirWhile = 1
            paquete = [1,1,1,1,1]
            solicitudTCP(paquete,clientQueue)
            exito = 1
        nuevoReintento = nuevoReintento + 1

    if(salirWhile == 0):
        #Nunca llegó el paquete de reintentos esperados
        paquete = [-20,-20,-20,-20,-20]
        solicitudTCP(paquete,clientQueue)
        time.sleep(0.3)
    vectorRespuesta = [exito,reintentos]
    return vectorRespuesta

'''
    @brief Metodo que tiene la estructura para buscar un potencial
    papá
    @details Este método tiene la estructura para obtener el 
    potencial papá y luego hacer el TWH respectivo
    @param[out] N/A
    @param[in] numeroNodo,serverQueue,clientQueue,reintentos
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return vectorRespuestaP
    @exception N/A
    @author Juan Jose Herrera B83878
    @date 15-09-20
'''
def buscarPapa(numeroNodo,serverQueue,clientQueue,reintentos):
    exito = 0
    vectorRespuestaP = reconfiguracionPapa(serverQueue,
    clientQueue,reintentos)

    potencialPapa=vectorRespuestaP[1]
    exito = vectorRespuestaP[0]
    if(exito == 1):
        #Se obtuvo un potencial papá
        vectorRespuestaD = TWHSolicitudPapa(numeroNodo,potencialPapa,
        clientQueue,serverQueue,reintentos)
        #Contiene a potencial papá y el aviso si
        #obtuvo exito en el TWH
        vectorRespuestaP = vectorRespuestaD

    return vectorRespuestaP



'''
    @brief Metodo que genera una solicutud para averiguar la cantidad de reintentes para 
    realizar preguntas a un vecino si es parte del arbol
    @details Este metodo se encarga de generar una solicitud para averiguar cual es la cantidad de veces que se le 
    va a pregunatr a un vecino si es parte del arbol
    @param[in] N/A
    @param[out] numeroNodo,clientQueue,serverQueue,
    reintentos
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Juan Jose Herrera B83878
    @date 15-09-20
'''
def responderSolicitudesPapa(numeroNodo,clientQueue,serverQueue,
reintentos):

    global salirNodo
    global reinicioPapa
    enivarConfirmacionArbol(clientQueue)
    while(salirNodo== False and reinicioPapa == 0):
        #Esto se realiza un TWH de solicitud hasta que ya no forme
        #parate del árbol generador
        TWHResponderPapa(numeroNodo,clientQueue,serverQueue,reintentos)
        #Evitar cuello de botella
        time.sleep(0.3)

'''
    @brief Metodo que genera una matriz con los datos de los nodos
    
    @details Este metodo se encarga de generar una con los datos de los nodos
    @param[in] N/A
    @param[out] cola de datos y paquete de datos
    @pre Que la cola de datos fuera creada y que el paquete fuera correcftamente inicializado
    @remark N/A
    @return N/A
    @exception N/A
    @author Johel Phillipes B75821
    @date 15-09-20
'''

'''
 *  @brief Este método genera una matriz con los datos de la
 *  alcanzabilidad de los nodos emisores
 *  @details Se genera una matriz de tamaño n-1 x n que representa
 *  las conexiones directas entre los nodos y sus respectivos pesos,
 *  esta matriz ayuda a calcular la menor distancia entre este nodo
 *  y cualquier otro.
 *  @param[in] [1] data: Se recibe la información en forma de string,
 *  como mínimo llegan 3 datos. El primer dato es el nodo emisor del
 *  mensaje, luego llega la cantidad de vecinos a los que el nodo
 *  emisor puede  alcanzar, luego llegan parejas de datos que son
 *  dichos nodos con su respectivo peso
 *  [2] clientQueue: Es una referencia a la cola de salida del nodo
 *  rosado, para sacar los resultados de dijkstra
 *  @author Johel Phillips Ugalde B75821
 *  @date 2-12-20
 '''

def generarMatriz(data, clientQueue):
    global nodos, matriz, caminos, forwarding

    datos = data.split(sep=',') # Separa los datos
    indice = 0

    while indice < len(datos):
      nuevoNodo = True
      j = 0
      while nuevoNodo and j < len(nodos):
        if nodos[j] == int(datos[indice]): # Ya existe el registro
          nuevoNodo = False
        j += 1
      if nuevoNodo:
        nodos.append(int(datos[indice])) # Se agrega si no existía

      indice += 2 # Itera entre los nodos

    while len(matriz) < len(nodos): # Se agragan campos a la matriz
      matriz.append([])
      forwarding.append([])

    for i in range(len(matriz)): # Se llena la matriz con nulos
      while len(matriz[i]) < len(matriz)-1:
        matriz[i].append(1000)
        forwarding[i].append(1000)

    nuevo = 0
    for i in range(len(nodos)): # Se busca el índice del nodo emisor
      if int(datos[0]) == nodos[i]:
        nuevo = i

    indice = 2
    for i in range(int(datos[1])): # Cantidad de pares
      for j in range(len(nodos)):
        if int(datos[indice]) == nodos[j]:
          if j == 0:
            matriz[0][nuevo-1] = int(datos[indice+1])
          else:
            matriz[nuevo][j-1] = int(datos[indice+1])
      indice += 2

    for i in range(len(nodos)-1):
      caminos.append([0, 1000])


    dijkstra(clientQueue)



'''
    @brief Metodo que ejecuta el algoritmo de dijstra
    @details Este metodo se encarga de ejecutar el algoritmo de dijkstra para encontrar la ruta mas corta a los nodos
    @param[out] N/A
    @param[in] cola de datos
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
        if matriz[i][j]: # Si no es parte de la "diagonal"
          acumuladoActual = 0
          if not padre: # Si no hay un padre
              padre = j + 1 # El padre es el siguiente elemento
          if i <= j: # Derecha de la "diagonal" ("recursivo")

            acumulado.append(matriz[i][j])
            for n in range(len(acumulado)):
              acumuladoActual += acumulado[n]
            if acumuladoActual < forwarding[i][j]: # Ruta más corta
              forwarding[i][j] = acumuladoActual
              if caminos[j][1] > acumuladoActual: # Mejor ruta
                caminos[j][0] = padre
                caminos[j][1] = acumuladoActual
            indiceI.append(i)# Acumular índice i
            indiceJ.append(j+1)# Acumular índice j
            i = j + 1
            j = 0
          else: # Izquierda de la "diagonal" (no "recursivo")
            for n in range(len(acumulado)):
              acumuladoActual += acumulado[n]
            acumuladoActual += pesos[i][j]
            if acumuladoActual < forwarding[i][j]:
              forwarding[i][j] = acumuladoActual
              if caminos[j][1] > acumuladoActual:
                caminos[j][0] = padre
                caminos[j][1] = acumuladoActual
            j += 1
        else: # La "diagonal" es nula
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
    @param[out] N/A
    @param[in] dos colas de datos 
    @pre Que las colas de datos fueran creadas
    @remark se modifican las colas de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-10-20
'''
def serverTcp(serverQueue,clientQueue):
    bandera = 1
    while(bandera == 1):

        data = s1.recv(1024)
        data=data.decode("utf-8")
        prueba=data[0:2]
        if(prueba == "90"):
            data=data[3:]
            #Se prepara dijkstra
            generarMatriz(data,clientQueue)
        elif (prueba != "90"):
            #mensaje normal para ag
            serverQueue.put(data)
            #terminar el while
            if(data == "-100,-100,-100,-100,-100"):
                global salirNodo
                salirNodo = True
                bandera = 0
        #Evitar los cuellos de botella
        time.sleep(0.3)

'''
    @brief Metodo que genera un socket para enviar datos al agente rosado
    @details Este metodo se encarga de generar un socket para enviar datos al agente rosado
    @param[out] N/A
    @param[in]  cola de datos 
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Diego Barquero Quesada B80961
    @date 15-10-20
'''
def clientTcp(clientQueue):

    global salirNodo
    while salirNodo == False:
        data=None
        try:
            data = clientQueue.get(block = True,timeout=30)
            

        except queue.Empty:
            pass
        if data != None:
            s2.send(data.encode())
        #Evitar cuello de botella
        time.sleep(0.3)

'''
    @brief Metodo que funciona como Main del árbol generador
    @details Realiza las principales funciones del árbol generador
    @param[out] N/A
    @param[in]  cola de datos 
    @pre Que la cola de datos fuera creada
    @remark se modifica la cola de datos
    @return N/A
    @exception N/A
    @author Juan José Herrera Rodríguez
    @date 01-12-20
'''
def arGe(serverQueue,clientQueue):
    #Reinicio de principales estructuras
    numeroNodo = -1
    numeroNodo = int(sys.argv[1])
    miembroAg = 0
   
    global papaAg
    papaAg = -1

    reintentos = -1
    bandera = True

    while(bandera == True):
        vectorRespuesta = obtenerReintentos(serverQueue,clientQueue)
        exito = vectorRespuesta[0]
        reintentos = vectorRespuesta[1]
        if(exito ==1 ):
            bandera = False
    global salirNodo
    while( salirNodo == False):
        #La cantidad de reintentos fue obtenida
        global reinicioPapa
        reinicioPapa = 0
        #numero de nodo se puede pasar por parametro
        papaAg = -1
        global hijosAg
        hijosAg = []
        numeroAg = 0
        if(numeroNodo == 1):
            miembroAg =1

        if(miembroAg == 0):
            #no soy el nodo 1
            vectorRespuestaP = buscarPapa(numeroNodo,serverQueue,
            clientQueue,reintentos)

            if(vectorRespuestaP[0] == 1):
                papaAg = vectorRespuestaP[1]
                miembroAg = vectorRespuestaP[1]

                responderSolicitudesPapa(numeroNodo,clientQueue,serverQueue,reintentos)
        else:
            #Soy el nodo número 1
            responderSolicitudesPapa(numeroNodo,clientQueue,serverQueue,reintentos)

if __name__=='__main__':
    #Se levantan los hilos del nodos rosado y sus respectivas colas
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