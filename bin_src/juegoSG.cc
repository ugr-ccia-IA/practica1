#include "motorlib.hpp"
#include <fstream>

MonitorJuego monitor;

int main(int argc, char ** argv){

  if (argc < 6){
    cout << "Faltan parametros..." << endl;
    cout << "Se necesita indicar: " << endl;
    cout << "1) Mapa" << endl << "2) Semilla" << endl << "3) Nivel (0, 1, 2 o 3)" << endl;
    cout << "4) Origen (F)" << endl << "5) Origen (C)" << "6) Orientacion " << endl;
  }
  else {
    // 1 -> mapa
    // 2 -> semilla
    // 3 -> nivel
    // 4,5 -> origen
    // 6 -> origentacion (0 = norte, 1 = noreste, 2 = este, 3 = sureste, 4 = sur, 5 = suroeste, 6 = oeste, 7 = noroeste)

    monitor.setMapa(argv[1]);
    monitor.startGame(atoi(argv[3]));

    ifstream ifile;
    ifile.open(argv[1], ios::in);

    unsigned int colSize, filSize;
    ifile >> colSize;
    ifile >> filSize;

    vector<vector<unsigned char> > mapAux;
    vector<unsigned char> colAux(colSize);

    for (unsigned int i = 0; i < filSize; i++) {
      mapAux.push_back(colAux);
    }

    for (unsigned int i = 0; i < filSize; i++) {
      for (unsigned int j = 0; j < colSize; j++) {
        ifile >> mapAux[i][j];
      }
    }
    ifile.close();
    // Reviso si es una posicion valida
    if (atoi(argv[4])!=-1){
      if (mapAux[atoi(argv[4])][atoi(argv[5])] == 'P' or mapAux[atoi(argv[4])][atoi(argv[5])] == 'M'){
	       cout << "Posicion (" << argv[4] << "," << argv[5] << ") no es de tipo válido para el jugador" << endl;
         return 0;
       }
     }

    monitor.inicializar(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[2]));

    monitor.get_entidad(0)->setBateria(5000);
    monitor.get_entidad(0)->Cogio_Bikini(false);
    monitor.get_entidad(0)->Cogio_Zapatillas(false);
    monitor.setPasos(3000);
    monitor.setRetardo(0);


    monitor.juegoInicializado();

    lanzar_motor_juego2(monitor); // Se lanza el juego
  }
  exit(EXIT_SUCCESS);
}
