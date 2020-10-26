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

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight)
{
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);
}

void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight)
{
    i_out = (int)(pos.y * 1.0f / cellHeight);
    j_out = (int)(pos.x * 1.0f / cellWidth);
}

float cellValue(int row, int col, bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, List<Object *> obstacles, List<Defense *> defenses)
{
    return 0; // implemente aqui la funci�n que asigna valores a las celdas
}

bool factible(std::list<Defense *> defenses, std::list<Object *> obstacles, float mapWidth, float mapHeight, Vector3 position, float radio, bool **freeCells, int CellsWidth, int CellsHeight)
{
    if (position.x > mapWidth || position.y > mapHeight)
        return false; //Si la posicion se sale del mapa
    int candidatei, candidatej;
    positionToCell(position, candidatei, candidatej, CellsWidth, CellsHeight);
    if (!freeCells[candidatei, candidatej])
        return false; //Si devuelve false, significa que el centro esta ocupado seguro asi que no es factible

    //Ahora hay que comprobar que la defensa no choque con nada en la posicion elegida y que no se salga del mapa

} //esValido

void DEF_LIB_EXPORTED placeDefenses(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{ //Freecells: mapa de celdas con centros libres, tener en cuenta si es true; nCellsWidth: numero de celdas en ancho; nCellsHeight: n celdas alto; mapWidth: anchura del mapa entero; mapHeight: altura del mapa entero, obstacles: lista de obstaculos, son vectores basicamente; defenses:

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    int maxAttemps = 1000;
    List<Defense *>::iterator currentDefense = defenses.begin();
    while (currentDefense != defenses.end() && maxAttemps > 0)
    {

        (*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
        (*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
        (*currentDefense)->position.z = 0;
        ++currentDefense;
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
