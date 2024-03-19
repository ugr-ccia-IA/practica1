#include "motorlib/monitor.hpp"

#include <fstream>

void MonitorJuego::cambiarOrientacion(unsigned char entidad, unsigned char orientacion)
{
  Orientacion aux;
  switch (orientacion)
  {
  case 0:
    aux = norte;
    break;

  case 1:
    aux = noreste;
    break;

  case 2:
    aux = este;
    break;

  case 3:
    aux = sureste;
    break;

  case 4:
    aux = sur;
    break;

  case 5:
    aux = suroeste;
    break;

  case 6:
    aux = oeste;
    break;

  case 7:
    aux = noroeste;
    break;

  }
  entidades[entidad]->setOrientacion(aux);
}

void MonitorJuego::setMapa(const char *file)
{
  ifstream ifile;
  ifile.open(file, ios::in);

  unsigned int colSize, filSize;
  ifile >> colSize;
  ifile >> filSize;

  vector<vector<unsigned char>> mapAux;
  vector<unsigned char> colAux(colSize);

  for (unsigned int i = 0; i < filSize; i++)
  {
    mapAux.push_back(colAux);
  }

  for (unsigned int i = 0; i < filSize; i++)
  {
    for (unsigned int j = 0; j < colSize; j++)
    {
      ifile >> mapAux[i][j];
    }
  }
  ifile.close();

  mapa = new Mapa(mapAux, &entidades);
}

bool MonitorJuego::inicializarJuego()
{
  bool aux = empezarJuego;
  if (empezarJuego)
  {
    empezarJuego = false;
    resultados = false;
  }
  return aux;
}

void MonitorJuego::generate_a_valid_cell(int &pos_fila, int &pos_col, int &ori)
{
  pos_col = -1;
  pos_fila = -1;
  char celdaRand = '_';
  do
  {
    pos_fila = aleatorio(getMapa()->getNFils() - 1);
    pos_col = aleatorio(getMapa()->getNCols() - 1);

    celdaRand = getMapa()->getCelda(pos_fila, pos_col);
  } while ((celdaRand == 'P' or celdaRand == 'M'));
  ori = aleatorio(7);
}

bool MonitorJuego::is_a_valid_cell_like_goal(int f, int c)
{
  if (f < 0 or f >= getMapa()->getNFils())
    return false;
  if (c < 0 or c >= getMapa()->getNCols())
    return false;
  char cell = getMapa()->getCelda(f, c);
  if (cell == 'M' or cell == 'P')
    return false;
  return true;
}

void MonitorJuego::decPasos()
{
  if (get_entidad(0)->fin())
  {
    jugando = false;
    resultados = true;
  }
  else
  {
    if (pasos > 0)
    {
      pasos--;
      pasosTotales++;
    }

    if (!get_entidad(0)->vivo() or get_entidad(0)->getBateria() == 0)
    {
      jugando = false;
      resultados = true;
    }
  }
}

void MonitorJuego::inicializar(int pos_fila, int pos_col, int bruj, int seed)
{
  clear(); // Elimina el verctor de entidades
  srand(seed); // OK
  int tama = getMapa()->getNCols();

  // Si es la primera vez que se llama se fija el numero de lobos y aldeanos al 10 % del numero de columnas
  if (get_n_aldeanos() == -1)
  {
    set_n_aldeanos(tama / 10);
    set_n_lobos(tama / 10);
  }

  unsigned char celdaRand;

  // Primero SIEMPRE se coloca al jugador. SIEMPRE.
  if ((pos_fila == -1) or (pos_col == -1))
  {
    do
    {
      pos_fila = aleatorio(getMapa()->getNFils() - 1);
      pos_col = aleatorio(getMapa()->getNCols() - 1);
      celdaRand = getMapa()->getCelda(pos_fila, pos_col);
    } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_fila, pos_col) != '_'));
  }

  Orientacion paDonde;


  if ((nivel == 1 or nivel == 2) and bruj != norte and bruj != -1){
    paDonde = static_cast<Orientacion>(bruj);
    cout << "Se cambió la orientación inicial del agente a Norte. Orientación inicial obligatoria en este nivel.\n";
    bruj = norte;
  }
  else if (bruj == -1){
    bruj = rand() % 8;
  }

  if (nivel == 3 and (bruj%2 !=0) ){
    paDonde = static_cast<Orientacion>(bruj);
    bruj = (bruj +1)%8;
  }

  nueva_entidad(new Entidad(jugador, jugador_, static_cast<Orientacion>(bruj), pos_fila, pos_col, new Jugador3D(""), new ComportamientoJugador(getMapa()->getNFils()), 3000));

  if (nivel == 3 and paDonde != bruj){
    cout << "Se fijó la orientación inicial a " << get_entidad(0)->stringOrientacion(static_cast<Orientacion>(bruj)) << "." << endl;
  }
 
  // Para el nivel 2 genero los aldeanos y los lobos
  if (nivel >= 2)
  {
    for (int i = 0; i < get_n_aldeanos(); i++)
    {
      do
      {
        pos_fila = aleatorio(getMapa()->getNFils() - 1);
        pos_col = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_fila, pos_col);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_fila, pos_col) != '_'));
      bruj = rand() % 8;
      nueva_entidad(new Entidad(npc, aldeano, static_cast<Orientacion>(bruj), pos_fila, pos_col, new Aldeano3D(""), new ComportamientoAldeano(), 3000));
    }

     for (int i = 0; i < get_n_lobos(); i++)
    {
      do
      {
        pos_fila = aleatorio(getMapa()->getNFils() - 1);
        pos_col = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_fila, pos_col);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_fila, pos_col) != '_'));
      bruj = rand() % 8;
      nueva_entidad(new Entidad(npc, lobo, static_cast<Orientacion>(bruj), pos_fila, pos_col, new Perro3D(), new ComportamientoPerro(), 3000));
    }

  }



  get_entidad(0)->setVision(getMapa()->vision(0));

  srand(seed); // OK?
}

double MonitorJuego::CoincidenciaConElMapa()
{
  unsigned int aciertos = 0, totalCasillas = 0;

  for (unsigned int i = 0; i < getMapa()->getNFils(); i++)
  {
    for (unsigned int j = 0; j < getMapa()->getNCols(); j++)
    {
      if (getMapa()->getCelda(i, j) == get_entidad(0)->getMapaResultado()[i][j])
      {
        aciertos++;
      }
      totalCasillas++;
    }
  }
  return (aciertos * 100.0 / totalCasillas);
}

double MonitorJuego::CoincidenciaConElMapaContandoErrores()
{
  int aciertos = 0, totalCasillas = 0;

  for (unsigned int i = 0; i < getMapa()->getNFils(); i++)
  {
    for (unsigned int j = 0; j < getMapa()->getNCols(); j++)
    {
      if (getMapa()->getCelda(i, j) == get_entidad(0)->getMapaResultado()[i][j])
      {
        aciertos++;
      }
      else if (get_entidad(0)->getMapaResultado()[i][j] != '?')
      { // Puso un valor distinto de desconocido en mapaResultado y no acertó
        aciertos--;
      }
      totalCasillas++;
    }
  }

  return (max(aciertos,0) * 100.0 / totalCasillas);
}

void MonitorJuego::PintaEstadoMonitor()
{
  cout << "*********************************************\n";
  cout << "Pos Fil: " << get_entidad(0)->getFil() << endl;
  cout << "Pos Col: " << get_entidad(0)->getCol() << endl;
  cout << "Brujula: " << get_entidad(0)->getOrientacion() << endl;
  cout << "aldeanos: " << numero_entidades() << endl;
  for (int i = 1; i < numero_entidades(); i++)
  {
    cout << "     Tipo: " << get_entidad(i)->getSubTipo() << endl;
    cout << "  Pos Fil: " << get_entidad(i)->getFil() << endl;
    cout << "  Pos Col: " << get_entidad(i)->getCol() << endl;
    cout << "  Brujula: " << get_entidad(i)->getOrientacion() << endl
         << endl;
  }
  cout << "*********************************************\n";
}

void MonitorJuego::init_casillas_especiales(unsigned int f, unsigned int c){
  unsigned char celda_inicial;
  celda_inicial = getMapa()->getCelda(f, c);
  if (celda_inicial == 'D'){
    get_entidad(0)->Cogio_Zapatillas(true);
  }
  else if (celda_inicial == 'K'){
    get_entidad(0)->Cogio_Bikini(true);
  }
  else if (celda_inicial == 'G'){
    get_entidad(0)->notify();
  }
}