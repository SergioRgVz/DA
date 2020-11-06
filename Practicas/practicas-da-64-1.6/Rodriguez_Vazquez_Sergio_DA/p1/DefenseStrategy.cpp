// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;


struct C
{
    C(){}
    C(const Vector3 &position, float value): position(position), value(value){}
    Vector3 position;
    float value;
};

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight)
{
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);
}

void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight)
{
    i_out = (int)(pos.y * 1.0f / cellHeight);
    j_out = (int)(pos.x * 1.0f / cellWidth);
}


float cellValue(int row, int col, int nCellsWidth, int nCellsHeight, float cellWidth, float cellHeight)
{     
    // Vector3 center = cellCenterToPosition(nCellsWidth/2, nCellsHeight/2, cellWidth, cellHeight);

    // return _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), center);
    Vector3
    cornerUpLeft = cellCenterToPosition(0, 0, cellWidth, cellHeight),
    cornerUpRight = cellCenterToPosition(0, nCellsWidth, cellWidth, cellHeight),
    cornerDownLeft = cellCenterToPosition(nCellsHeight, 0, cellWidth, cellHeight),
    cornerDownRight = cellCenterToPosition(nCellsHeight, nCellsWidth, cellWidth, cellHeight);


    // float CUpLeftDistance, CUpRightDistance, CDownLeftDistance, CDownRightDistance;
    float
    CUpLeftDistance = _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), cornerUpLeft),
    CUpRightDistance = _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), cornerUpRight),
    CDownLeftDistance = _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), cornerDownLeft),
    CDownRightDistance = _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), cornerDownRight);

    return std::min({CUpLeftDistance, CUpRightDistance, CDownLeftDistance, CDownRightDistance});

    // return std::max(std::max(CUpLeftDistance, CUpRightDistance), std::max(CDownLeftDistance, CDownRightDistance)); //Pondriamos esto para calcular la distancia a una de las esquinas
    //Lo que falta en esta funcion es ordenar las distancias a las esquinas y devolver la mayor, en base a eso hay que hacer otra funcion cellValue que ponga puntuacion a las demas defensas

     // implemente aqui la funci�n que asigna valores a las celdas
}

float cellValue(int row, int col, float cellWidth, float cellHeight, Vector3 position)
{
    return _distance(cellCenterToPosition(row, col, cellWidth, cellHeight), position); 
}


bool factible(std::list<Defense *> defenses, std::list<Object *> obstacles, float mapWidth, float mapHeight, Vector3 position, float radio, bool **freeCells, int CellsWidth, int CellsHeight, int ultima_colocada)
{
    if (position.x + radio > mapWidth || position.y + radio > mapHeight || position.x - radio < 0 || position.y - radio < 0)
        return false; //Si la posicion se sale del mapa
    int candidaterow, candidatecol, i = 0;
    positionToCell(position, candidaterow, candidatecol, CellsWidth, CellsHeight);
    if (!freeCells[candidaterow, candidatecol]) return false; //Si devuelve false, significa que el centro esta ocupado seguro asi que no es factible

    List<Defense*>::const_iterator defactual = defenses.begin();
    while(i <= ultima_colocada)
    {
        if(_distance(position, (*defactual)->position) <= radio + (*defactual)->radio) return false;
        ++defactual;
        i++;
    }


    for(List<Object*>::const_iterator actual = obstacles.begin(); actual != obstacles.end(); ++actual)
    {
        if(abs(_distance(position, (*actual)->position)) <= radio + (*actual)->radio) return false;
    }
    //Ahora hay que comprobar que la defensa no choque con nada en la posicion elegida y que no se salga del mapa



    return true;
} //esValido

bool isMinor(const C& A, const C& B)
{
    return (A.value < B.value);
}

void DEF_LIB_EXPORTED placeDefenses(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{ //Freecells: mapa de celdas con centros libres, tener en cuenta si es true; nCellsWidth: numero de celdas en ancho; nCellsHeight: n celdas alto; mapWidth: anchura del mapa entero; mapHeight: altura del mapa entero, obstacles: lista de obstaculos, son vectores basicamente; defenses:

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;

    List<C> Candidates;

    //Hacemos lista de celdas (candidatos) para colocar el resto de defensas    
    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, cellValue(i, j, nCellsWidth, nCellsHeight, cellWidth, cellHeight));
            Candidates.push_back(c);
        }
    }

    // for(auto c:Candidates)
    // {
    //     std::cout << c.position.x << " " << c.position.y << std::endl;
    // }

    Candidates.sort(isMinor);
    C cfirstpromising;


    List<Defense*>::iterator currentDefense = defenses.begin();
    while(!Candidates.empty() && ultima_colocada == -1 )
    { 
        cfirstpromising = Candidates.back();
        Candidates.pop_back();
        if(factible(defenses, obstacles, mapWidth, mapHeight, cfirstpromising.position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, -1))
        {
            (*currentDefense)->position.x = cfirstpromising.position.x;
            (*currentDefense)->position.y = cfirstpromising.position.y;
            (*currentDefense)->position.z = 0;
            ultima_colocada++;

        }
    }
    Candidates = {};

    //Hemos colocado la primera defensa
    
    // List<C> Candidates;



    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, cellValue(i, j, cellWidth, cellHeight, (*currentDefense)->position));
            Candidates.push_back(c);
        }
    }

    ++currentDefense;
    Candidates.sort(isMinor);
    //Ponemos las demas defensas
    C cpromising;
    while(currentDefense != defenses.end() && !Candidates.empty())
    {
        cpromising = Candidates.front();
        Candidates.pop_front();
        if(factible(defenses, obstacles, mapWidth, mapHeight, cpromising.position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, ultima_colocada))
        {
            (*currentDefense)->position.x = cpromising.position.x;
            (*currentDefense)->position.y = cpromising.position.y;
            (*currentDefense)->position.z = 0;
            ++currentDefense;
            ultima_colocada++;
        }
    }

#ifdef PRINT_DEFENSE_STRATEGY

    float **cellValues = new float *[nCellsHeight];
    for (int i = 0; i < nCellsHeight; ++i)
    {
        cellValues[i] = new float[nCellsWidth];
        for (int j = 0; j < nCellsWidth; ++j)
        {
            cellValues[i][j] = ((int)(cellValue(i, j))) % 256;
        }
    }
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells, cellValues, std::list<Defense *>(), true);

    for (int i = 0; i < nCellsHeight; ++i)
        delete[] cellValues[i];
    delete[] cellValues;
    cellValues = NULL;

#endif
}
