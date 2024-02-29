#include <fstream>
#include "motorlib.hpp"

bool fileExists( std::string const& name )
{
    ifstream f(name.c_str());
    return f.good();
}

bool PosicionAgenteCorrecta(EnLinea argumentos){
    ifstream ifile;
    ifile.open(argumentos.ubicacion_mapa, ios::in);

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
    if (argumentos.fil_inicial!=-1){
      if (mapAux[argumentos.fil_inicial][argumentos.col_inicial] == 'P' or mapAux[argumentos.fil_inicial][argumentos.col_inicial] == 'M'){
	       cout << "Posicion (" << argumentos.fil_inicial << "," << argumentos.col_inicial << ") no es de tipo válido para el jugador" << endl;
         return false;
       }
     }
  return true;
}



int main(int argc, char ** argv) {
  EnLinea argumentos;

  if (argc < 2){
    srand(1);
    lanzar_motor_grafico(argc, argv);
  }
  else {
    srand(atoi(argv[1]));
    argumentos.ubicacion_mapa = argv[1];
    if (!fileExists(argumentos.ubicacion_mapa)){
      std::cout << "El mapa no existe\n";
      exit(1);
    }
    argumentos.semilla = atoi(argv[2]);
    argumentos.level = atoi(argv[3]);
    argumentos.fil_inicial = atoi(argv[4]);
    argumentos.col_inicial = atoi(argv[5]);
    argumentos.ori_inicial = atoi(argv[6]);
    int i = 7;

    if (!PosicionAgenteCorrecta(argumentos)) return 0;


    lanzar_motor_grafico_verOnline(argc, argv, argumentos);
  }

  exit(EXIT_SUCCESS);
}
