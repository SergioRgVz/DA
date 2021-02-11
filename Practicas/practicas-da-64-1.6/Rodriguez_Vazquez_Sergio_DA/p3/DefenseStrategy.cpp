// ###### Config options ################



// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
#include <algorithm>

using namespace Asedio;         

struct C
{
    C(){}
    C(const Vector3 &position, float value): position(position), value(value){}
    Vector3 position;
    float value;
    bool operator<(const C &c) const {return value < c.value;}
    bool operator<=(const C &c) const {return value <= c.value;}
    bool operator>(const C &c) const {return value > c.value;}
    bool operator>=(const C &c) const {return value >= c.value;}
    bool operator>(int n) const{ return value < n;}
    

};

void withoutpreorder(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses);
void mergesortfunction(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses);
void Insertionsort(std::vector<C> &v, int i, int j);
void merge(std::vector<C> &v, int i, int k, int j);
std::vector<C> quicksort(std::vector<C> &v, int low, int high);
std::vector<C> merge_sort(std::vector<C> &v, int i, int j);
void quicksortfunction (bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses);
int partition(std::vector<C> &v, int low, int high);
void heapfunction(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses);

std::vector<int> merge_sortint(std::vector<int> &v, int i, int j);
void Insertionsortint(std::vector<int> &v, int i, int j);
void mergeint(std::vector<int> &v, int i, int k, int j);
std::vector<int> quicksortint(std::vector<int> &v, int low, int high);
int partitionint(std::vector<int> &v, int low, int high);
void testalgorithm();



void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight)
{
    i_out = (int)(pos.y * 1.0f / cellHeight);
    j_out = (int)(pos.x * 1.0f / cellWidth);
}

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight)
{
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);
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
        if(_distance(position, (*actual)->position) <= radio + (*actual)->radio) return false;
    }
    //Ahora hay que comprobar que la defensa no choque con nada en la posicion elegida y que no se salga del mapa



    return true;
} //esValido

float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
    , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    	
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);
    	
    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}

void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

	// cronometro c;
    // long int r = 0;

    // c.activar();
    // do {
    //     withoutpreorder(freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);	
    //     r++;
    // } while(c.tiempo() < 1.0);
    // c.parar();
    // std::cout << (nCellsWidth * nCellsHeight) << '\t' << c.tiempo() / r << '\t' << c.tiempo()*2 / r << '\t' << c.tiempo()*3 / r << '\t' << c.tiempo()*4 / r << std::endl;


    cronometro c2;
    long int r2 = 0;
    c2.activar();
    do
    {
        mergesortfunction(freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
        r2++;
    }while(c2.tiempo() < 1.0);
    c2.parar();
    std::cout << (nCellsWidth * nCellsHeight) << '\t' << c2.tiempo() / r2 << '\t' << c2.tiempo() * 2 / r2 << '\t' << c2.tiempo() * 3 / r2 << '\t' << c2.tiempo() * 4 / r2 << std::endl;


    // cronometro c3;
    // long int r3 = 0;
    // c3.activar();
    // do
    // {
    //     quicksortfunction(freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
    //     r3++;
    // }while(c3.tiempo() < 1.0);
    // c3.parar();
    // std::cout << (nCellsWidth * nCellsHeight) << '\t' << c3.tiempo() / r3 << '\t' << c3.tiempo() * 2 / r3 << '\t' << c3.tiempo() * 3 / r3 << '\t' << c3.tiempo() * 4 / r3 << std::endl;

    // cronometro c4;
    // long int r4 = 0;
    // c4.activar();
    // do
    // {
    //     heapfunction(freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses);
    //     r4++;
    // }while(c4.tiempo() < 1.0);
    // c4.parar();
    // std::cout << (nCellsWidth * nCellsHeight) << '\t' << c4.tiempo() / r4 << '\t' << c4.tiempo() * 2 / r4 << '\t' << c4.tiempo() * 3 / r4 << '\t' << c4.tiempo() * 4 / r4 << std::endl;

}


List<C>::const_iterator searchbest(const List<C> &Candidates)
{
    std::list<C>::const_iterator candidate = Candidates.begin();
    for (std::list<C>::const_iterator it = Candidates.begin(); it != Candidates.end(); ++it)
    {  
        if((*it).value > (*candidate).value)
            candidate = it;
    }
    return candidate;
}

void withoutpreorder(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{
     float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;

    List<C> Candidates;

    //Hacemos lista de celdas (candidatos) para colocar las defensas    
    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses));
            Candidates.push_back(c);
        }
    }


    std::list<C>::const_iterator candidate;
    List<Defense*>::iterator currentDefense = defenses.begin();

    while(!Candidates.empty() && currentDefense != defenses.end())
    {
        candidate = searchbest(Candidates);

        Candidates.erase(candidate);
            
            if(factible(defenses, obstacles, mapWidth, mapHeight, (*candidate).position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, ultima_colocada)){
                (*currentDefense)->position.x = (*candidate).position.x;
                (*currentDefense)->position.y = (*candidate).position.y;
                (*currentDefense)->position.z = 0;
                ++currentDefense;
                ultima_colocada++;               
            }
    }
}


std::vector<C> merge_sort(std::vector<C> &v, int i, int j)
{
    int n = j - i ; 
    int k;
    if(n <= v.size())
    {
        Insertionsort(v, i, j);
    }
    else
    {
        k = i - 1 + n/2;
        merge_sort(v, i, k);
        merge_sort(v, k + 1, j);
        merge(v, i, k, j);
    }
    return v;
}

void Insertionsort(std::vector<C> &v, int i, int j)
{
    int key;
    for(i; i < j; i++)
    {
        key = v[i].value;
        j = i - 1;

        while(j >= 0 && v[j] > key)
        {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1].value = key;
    }
    
}

void merge(std::vector<C> &v, int i, int k, int j)
{
    int n, p, q;
    n = j - i + 1;
    p = i;
    q = k + 1;
    std::vector<C> w(n);
    for(int l = 0; l < n; l++)
    {
        if(p <= k && (q > j || v[p] <= v[q]))
        {
            w[l] = v[p];
            p++;
        }
        else
        {
            w[l] = v[q];
            q++;
        }
    }
    
    for(int l = 0; l < n; l++)
    {
        v[i - 1 + l] = w[l];
    }
}


void mergesortfunction(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;  

    std::vector<C> Candidates;

    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses));
            Candidates.push_back(c);
        }
    }  
    Candidates = merge_sort(Candidates, 0, Candidates.size());

    List<Defense*>::iterator currentDefense = defenses.begin();

    int i = 0;

    while( currentDefense != defenses.end() && i < Candidates.size())
    {
        C &candidate = Candidates[i];
        i++;
            
            if(factible(defenses, obstacles, mapWidth, mapHeight, candidate.position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, ultima_colocada)){
                (*currentDefense)->position.x = candidate.position.x;
                (*currentDefense)->position.y = candidate.position.y;
                (*currentDefense)->position.z = 0;
                ++currentDefense;
                ultima_colocada++;               
            }
    }

}

void quicksortfunction (bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;

    std::vector<C> Candidates;
    
    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses));
            Candidates.push_back(c);
        }
    } 

    Candidates = quicksort(Candidates, 0, Candidates.size()); 

    List<Defense*>::iterator currentDefense = defenses.begin();

    int i = 0;

    while( currentDefense != defenses.end() && i < Candidates.size())
    {
        C &candidate = Candidates[i];
        i++;
            
            if(factible(defenses, obstacles, mapWidth, mapHeight, candidate.position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, ultima_colocada)){
                (*currentDefense)->position.x = candidate.position.x;
                (*currentDefense)->position.y = candidate.position.y;
                (*currentDefense)->position.z = 0;
                ++currentDefense;
                ultima_colocada++;               
            }
    }
}

std::vector<C> quicksort(std::vector<C> &v, int low, int high)
{
    if(low < high)
    {
        //pivot es el indice de particion
        int pivot = partition(v, low, high);

        //Separadamente ordenar elementos antes de la particion y despues de la particion
        quicksort(v, low, pivot - 1);
        quicksort(v, pivot + 1, high);
    }
    return v;
}

int partition(std::vector<C> &v, int low, int high)
{
    C pivot = v[high];
    int i = (low - 1);

    for(int j = low; j <= high - 1; j++)
    {
        if(v[j] <= pivot)
        {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return (i + 1);
}


void heapfunction(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;  

    std::vector<C> Candidates;

    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses));
            Candidates.push_back(c);
        }
    } 
    std::make_heap(Candidates.begin(), Candidates.end());
    std::sort_heap(Candidates.begin(), Candidates.end());

    List<Defense*>::iterator currentDefense = defenses.begin();

    int i = 0;

    while( currentDefense != defenses.end() && i < Candidates.size())
    {
        C &candidate = Candidates[i];
        i++;
            
            if(factible(defenses, obstacles, mapWidth, mapHeight, candidate.position, (*currentDefense)->radio, freeCells, cellWidth, cellHeight, ultima_colocada)){
                (*currentDefense)->position.x = candidate.position.x;
                (*currentDefense)->position.y = candidate.position.y;
                (*currentDefense)->position.z = 0;
                ++currentDefense;
                ultima_colocada++;               
            }
    }

}

void testalgorithm()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> w = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    do{
        merge_sortint(v, 0, 8);
        if(!std::equal(v.begin(), v.end(), w.begin()))
        {
            std::cout << "v != w" << std::endl;
        }
    }while(std::next_permutation(v.begin(), v.end()));

    do{
        quicksortint(v, 0, 8);
        if(!std::equal(v.begin(), v.end(), w.begin()))
        {
            std::cout << "v != w" << std::endl;
        }
    }while(std::next_permutation(v.begin(), v.end()));    
}



std::vector<int> merge_sortint(std::vector<int> &v, int i, int j)
{
    int n = j - i ; 
    int k;
    if(n <= v.size())
    {
        Insertionsortint(v, i, j);
    }
    else
    {
        k = i - 1 + n/2;
        merge_sortint(v, i, k);
        merge_sortint(v, k + 1, j);
        mergeint(v, i, k, j);
    }
    return v;
}

void Insertionsortint(std::vector<int> &v, int i, int j)
{
    int key;
    for(i; i < j; i++)
    {
        key = v[i];
        j = i - 1;

        while(j >= 0 && v[j] > key)
        {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
    
}

void mergeint(std::vector<int> &v, int i, int k, int j)
{
    int n, p, q;
    n = j - i + 1;
    p = i;
    q = k + 1;
    std::vector<int> w(n);
    for(int l = 0; l < n; l++)
    {
        if(p <= k && (q > j || v[p] <= v[q]))
        {
            w[l] = v[p];
            p++;
        }
        else
        {
            w[l] = v[q];
            q++;
        }
    }
    
    for(int l = 0; l < n; l++)
    {
        v[i - 1 + l] = w[l];
    }
}

std::vector<int> quicksortint(std::vector<int> &v, int low, int high)
{
    if(low < high)
    {
        //pivot es el indice de particion
        int pivot = partitionint(v, low, high);

        //Separadamente ordenar elementos antes de la particion y despues de la particion
        quicksortint(v, low, pivot - 1);
        quicksortint(v, pivot + 1, high);
    }
    return v;
}

int partitionint(std::vector<int> &v, int low, int high)
{
    int pivot = v[high];
    int i = (low - 1);

    for(int j = low; j <= high - 1; j++)
    {
        if(v[j] <= pivot)
        {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return (i + 1);
}