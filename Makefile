bin:
	mkdir bin
verde: bin
	g++ -Wall -Wextra src/nodo_verde/Nodo_verde.cc src/nodo_verde/agente_naranja/Agente_naranja.cc src/nodo_verde/agente_verde/Agente_verde.cc src/nodo_verde/agente_azul/Agente_azul.cc src/common/cola/Cola.cc -pthread -o bin/NodoVerde
run:
	bin/./NodoVerde fifoPipeAgenteNaranja fifoPipeNodoNaranja tests/Pandemic_Plan_Remastered.nodo.1.csv tests/Pandemic_Plan_Remastered.vecinos.1.csv bin/fifoPipeAgenteAzul bin/fifoPipeNodoAzul
clean:
	rm bin/fifoPipeAgenteAzul bin/fifoPipeNodoAzul
ayuda:
	bin/./NodoVerde --help
