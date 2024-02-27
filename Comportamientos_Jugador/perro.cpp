#include "../Comportamientos_Jugador/perro.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <stdlib.h>



Action ComportamientoPerro::think(Sensores sensores){

  Action accion = actIDLE;

  // Lobo comun
  if ( (sensores.agentes[11] == 'j' or sensores.agentes[12]== 'j' or sensores.agentes[13]== 'j' or sensores.agentes[6]== 'j') and
       (sensores.terreno[6] != 'P' and sensores.terreno[6] != 'M') and
       (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M')
     ){
      accion = actWALK;
      return accion;
     }

  int cual = aleatorio(10);
  switch (cual) {
	case 0: accion = actIDLE; break;
	case 1: accion = actTURN_L; break;
	case 2: accion = actTURN_SR; break;
	case 3: case 4: case 5: case 6: case 7: 
		/*if (sensores.agentes[2] == 'j')*/ accion = actWALK;
		break;
	case 8: accion = actTURN_SR; break;
	case 9: accion = actTURN_L; break;
  }

  return accion;


}

int ComportamientoPerro::interact(Action accion, int valor){
  return 0;
}
