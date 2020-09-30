class Mensaje:

    def __init__(self, id_vecino, mensaje):
    	self.id_vecino = id_vecino
    	self.mensaje = mensaje

    def getId(self):
    	return self.id_vecino

    def getMensaje(self):
   		return self.mensaje