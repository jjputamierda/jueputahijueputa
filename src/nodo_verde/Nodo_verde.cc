#include "Nodo_verde.h"

std::vector<std::string> datosNodos;

void depositarDatos(std::vector<std::string>nuevosDatos){
   datosNodos = nuevosDatos;
}

/**
    @brief Rutina principal para el funcionamiento de la red.
    @details Esta rutina se encarga de la creación de los 3 agentes (hilos naranja, azul y verde) para que cada uno ejecute
    su rutina. Se reciben los datos del agente naranja y este finaliza (join), mientras que los agentes azul y verde deben
    mantenerse en ejecución durante toda la rutina.
    @param[in] argc Cantidad de parámetros al momento de la ejecución. Se espera que solo se reciban 2.
    @param[out] argv Lista de parámetros. El primero sería el programa en sí, mientras que el segundo es la posible entrada
    "--help", el cual da una pequeña descripción de ayuda sobre el programa.
    @pre En teoría, no habría ningún prerrequisito para la ejecución de esta rutina.
    @remark Ninguna observación destacable, al invocar esta rutina se invocan todas las demás.
    @return Si la rutina finaliza correctamente, debería retornar un 0, representando una salida exitosa.
    En caso de un error, es probable que el la rutina finalice de forma brusca.
    @exception La rutina podría terminar abruptamente en caso de un error.
    @author Isaac Herrera B43332 (Pandemic Plan Remastered)
    @date 13/9/20
*/

int main(int argc,char *argv[]){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            std::cout << "Argumentos: nombrePipeAgenteNaranja(a elegir) nombrePipeNodoNaranja(a elegir) ruta/infoNodo.csv ruta/vecinos.csv nombrePipeAgenteAzul(a elegir) nombrePipeNodoAazul(a elegir)" << std::endl;

            return 0;
        } else {
            std::cout << "Commando inválido, ejecute el programa con \"--help\" para recibir ayuda" << std::endl;
        }
    } else {
        if(argc != 7){
            std::cout << "Hacen falta argumentos, ejecute el programa con \"..help\" para recibir ayuda" << std::endl;
        }
    }

    char* rutaFifoAgenteNaranja = argv[1];
    char* rutaFifoNodoNaranja = argv[2];
    char* rutaNodoVerde = argv[3];
    char* rutaVecinos = argv[4];

    std::thread agenteNaranja(trabajoAgenteNaranja,rutaFifoAgenteNaranja,
    rutaFifoNodoNaranja,rutaNodoVerde,rutaVecinos);

    agenteNaranja.join();
    for(int i = 0; i < datosNodos.size();i++){
      std::cout<<datosNodos[i].data()<<std::endl;
    }
    /*
    std::vector<int> nodosIDS;

    int posicion,datos;
    for(unsigned long int indice2=0;indice2<datosNodos.size();indice2++){
        posicion=datosNodos[indice2].find(",");
        datos= std::stoi(datosNodos[indice2].substr(0,posicion));
        nodosIDS.push_back(datos);
    }

    Cola<struct mensaje> colasDeMensajes[datosNodos.size()];
    std::thread agenteAzul;
    agenteAzul = std::thread(hiloAzul, &nodosIDS, &colasDeMensajes[0],argv[5],argv[6]);

    std::thread agenteVerde;
    agenteVerde = std::thread(hiloVerde,datosNodos.size()-1,datosNodos,&colasDeMensajes[0]);

    agenteAzul.join();
    agenteVerde.join();
    */
    return 0;
}
