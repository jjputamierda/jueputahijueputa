let webSocketServer = null;
const caracteres = 'abcdefghijklmnopqrstuvwxyz0123456789 ';
//Caracteres para crear mensajes aleatorios

// WebSocket ----------------------------------------------------------

/**
 * @brief Crea un elemento 'p' y lo coloca en la bitácora de mensajes.
 * @details Se crea un elemento párrafo y se le asignan las clases
 * 'mensaje' y 'clase'(parámetro), así como su texto respectivo, para
 * luego ser colocado como hijo en el div 'mensajes'.
 * @param[in] texto Mensaje textual que se coloca en 'innerText'.
 * @param[in] clase Se añade como segunda clase del mensaje creado.
 * @pre Haber recibido/enviado un mensaje, o que un error haya ocurrido
 * son sucesos que harán que esta subrutina sea invocada.
 * @remark La página web verá actualizada su bitácora
 */

function agregarMensaje(texto, clase) {
  const mensaje = document.createElement('p');
  mensaje.classList.add('mensaje');
  mensaje.classList.add(clase);
  mensaje.innerText = texto;

  const mensajes = document.getElementById('mensajes');
  mensajes.appendChild(mensaje);
}

function agregarVecinos(payload){
  vecinos = payload.substring(8);
  listaVecinos = vecinos.split(/\W+/);
  const campo_vecinos = document.getElementById('lista_vecinos');

  for(i=0; i<listaVecinos.length; i++){
    const vecino = document.createElement('input');
    vecino.type = "radio";
    vecino.name = "vecinos";
    vecino.id = "v" + listaVecinos[i];
    vecino.value = listaVecinos[i];

    const etiqueta = document.createElement('label');
    etiqueta.for = "v" + listaVecinos[i];
    etiqueta.innerText = "Vecino " + listaVecinos[i];
    campo_vecinos.appendChild(vecino);
    campo_vecinos.appendChild(etiqueta);
    
    salto_linea = document.createElement("br");
    campo_vecinos.appendChild(salto_linea);
  }

  const broadcast = document.createElement('input');
  broadcast.type = "radio";
  broadcast.name = "vecinos";
  broadcast.id = "v0";
  broadcast.value = "0";

  const etiquetaB = document.createElement('label');
  etiquetaB.for = "v0";
  etiquetaB.innerText = "Broadcast";

  campo_vecinos.appendChild(broadcast);
  campo_vecinos.appendChild(etiquetaB);
  salto_linea = document.createElement("br");
  campo_vecinos.appendChild(salto_linea);

  const aleatorio = document.createElement('input');
  aleatorio.type = "radio";
  aleatorio.name = "vecinos";
  aleatorio.id = "vA";
  aleatorio.value = "-1";

  const etiquetaA = document.createElement('label');
  etiquetaA.for = "vA";
  etiquetaA.innerText = "Aleatorio";

  campo_vecinos.appendChild(aleatorio);
  campo_vecinos.appendChild(etiquetaA);
  salto_linea = document.createElement("br");
  campo_vecinos.appendChild(salto_linea);
}

/**
 * @brief Cambia el estado de la página a 'Conectado'.
 * @details Obtiene un elemento 'p' mediante su id para así cambiar su
 * clase a 'ok' y su texto a 'Conectado', dando a entender que
 * la página mantiene una conexión con el web socket.
 * @pre Que el socket se haya abierto correctamente.
 */

function webSocketOpened() {
  const status = document.getElementById('estado');
  status.className = 'ok';
  status.innerText = 'Conectado';
}

/**
 * @brief Cambia el estado de la página a 'Desconectado'.
 * @details Obtiene un elemento 'p' mediante su id para así cambiar su
 * clase a 'error' y su texto a 'Desconectado', dando a entender que
 * la página ya no posee una conexión con el web socket.
 * @param[in] evento Posible razón de la desconexión, usada para
 * agregar un mensaje a la bitácora.
 * @pre Que el web socket se haya cerrado.
 */

function webSocketClosed(evento) {
  const status = document.getElementById('estado');
  status.className = 'error';
  status.innerText = 'Desconectado';

  agregarMensaje(`Desconectado del servidor ${evento.reason}`,'error');
  // agregarMensaje('Se intentará reconectar en 10 segundos');
  // setTimeout(function(){ setupWebSocket();}, 10000);
}

/**
 * @brief Añade un mensaje a la bitácora
 * @details Se espera a la llegada de un mensaje entrante (evento) para
 * así añadirlo a la bitácora mediante la subrutina 'agregarMensaje'.
 * @param[in] evento El mensaje recibido que se añade a la bitácora.
 * @pre Que un mensaje haya llegado por el web socket.
 */

function webSocketMessageReceived(evento) {
  payload = evento.data;
  verificacion = payload.substring(0,1);
  if(isNaN(verificacion) == false){
    agregarMensaje(evento.data, 'normal');
  } else if (verificacion.localeCompare('i') == 0){
    id = payload.substring(3);
    const header1 = document.getElementById('head1');
    header1.innerText = 'Nodo ' + id;
    const title1 = document.getElementById('title1');
    title1.innerText = 'Nodo ' + id;
  } else {
    agregarVecinos(payload);
  }
}

/**
 * @brief Notifica de un error relacionado con el web socket.
 * @details Mediante la subrutina 'agregarMensaje' se notifica mediante
 * la bitácora que ha ocurrido un error de conexión.
 * @pre Que haya ocurrido un error con el web socket.
 */

function webSocketErrorOcurrido() {
  agregarMensaje('Error de conexión con el nodo verde', 'error');
}

/**
 * @brief Subrutina encargada de iniciar el web socket y de responder
 * a eventos relacionados con este.
 * @details Se crea un 'WebSocket' con una dirección ip y un puerto,
 * que luego deberán ser obtenidos mediante el agente azul. Luego se
 * añaden subrutinas para responder a eventos específicos, como un
 * mensaje entrante o un error relacionado con el web socket.
 * @pre El único requisito para la invocación de esta subrutina es que
 * la página se haya cargado.
 */

function setupWebSocket() {
  const datosWebSocket = document.body.dataset.ws;
  console.log(datosWebSocket);
  webSocketServer = new WebSocket(`ws://${datosWebSocket}`);

  webSocketServer.addEventListener('open', webSocketOpened);
  webSocketServer.addEventListener('message',webSocketMessageReceived);
  webSocketServer.addEventListener('error', webSocketErrorOcurrido);
  webSocketServer.addEventListener('close', webSocketClosed);
}

// Form ---------------------------------------------------------------

function sendMessage() {

  let numero = 1;
  let plantilla, tipo, mensajeAleatorio, cantidadCaracteres;
  const messageText = document.getElementById('message_field').value;
  primerCaracter = messageText.charAt(0);
  if(primerCaracter == '/'){
    numero = parseInt(messageText.substring(1));
  }

  let neighbor =
  document.querySelector('input[name="vecinos"]:checked').value;

  if(neighbor == -1){
    indice = Math.floor(Math.random() * listaVecinos.length);
    neighbor = listaVecinos[indice];
    plantilla = `Para ${neighbor}: `;
    tipo = 'enviado';
  } else if(neighbor == 0){
    plantilla = `Mensaje por Broadcast: `;
    tipo = 'broadcast';
  } else {
    plantilla = `Para ${neighbor}: `;
    tipo = 'enviado';
  }

  if(primerCaracter == '/'){
    for(i=0; i<numero; i++){
      mensajeAleatorio = '';
      cantidadCaracteres = Math.floor(Math.random() * 200);
      for(j=0; j<cantidadCaracteres; j++){
        mensajeAleatorio += caracteres.charAt(Math.floor(Math.random()
        * caracteres.length));
      }
      agregarMensaje(plantilla + mensajeAleatorio, tipo);
      webSocketServer.send(`${neighbor}::${mensajeAleatorio}`);
    }
  } else if(primerCaracter == '#'){
      mensajeAleatorio = '';
      cantidadCaracteres = Math.floor(Math.random() * 200);
      for(j=0; j<cantidadCaracteres; j++){
        mensajeAleatorio += caracteres.charAt(Math.floor(Math.random()
        * caracteres.length));
      }
      agregarMensaje(`Para 3: ` + mensajeAleatorio, tipo);
      webSocketServer.send(`3::${mensajeAleatorio}`);
  } else {
    agregarMensaje(plantilla + messageText, tipo);
    webSocketServer.send(`${neighbor}::${messageText}`);
  }
}

function setupClient() {
  setupWebSocket();

  const boton = document.getElementById('send_button');
  boton.addEventListener('click', sendMessage);
  const campoTexto = document.getElementById('message_field');
  campoTexto.addEventListener('keyup', function(evento){
    if(evento.key === 'Enter'){
      evento.preventDefault();
      sendMessage();
    }
  });
}

window.addEventListener('load', setupClient);
