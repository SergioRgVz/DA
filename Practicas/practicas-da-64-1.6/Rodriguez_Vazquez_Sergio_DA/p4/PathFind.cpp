// ###### Config options ################

#define PRINT_PATHS 1

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_PATHS
#include "ppm.h"
#include <algorithm>
#endif

using namespace Asedio;

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ 
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); 
}

void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;

    for(int i = 0 ; i < cellsHeight ; ++i) {
        for(int j = 0 ; j < cellsWidth ; ++j) {
            Vector3 cellPosition = cellCenterToPosition(i, j, cellWidth, cellHeight);
            float cost = 0;
            if( (i+j) % 2 == 0 ) {
                cost = cellWidth * 100;
            }
            
            additionalCost[i][j] = cost;
        }
    }
}



List<Vector3> recuperar (AStarNode *origen, AStarNode *destino)
{
    std::list<Vector3> path;
    AStarNode* aux = destino;
    path.push_back(aux->position);
    while(aux->parent != origen)
    {
        path.push_front(aux -> position);

        aux = aux -> parent;
    }

    return path;
}


bool belongs(std::vector<AStarNode*> &vector, AStarNode* node)
{
    std::vector<AStarNode*>::iterator aux;

    aux = std::find(vector.begin(), vector.end(), node);

    return (aux != vector.end());
}


bool criterion(AStarNode* A, AStarNode* B)
{
    return A->F < B->F;
}

void push_mont(AStarNode* node, std::vector<AStarNode*> &vector)
{
    vector.push_back(node);
    std::push_heap(vector.begin(), vector.end(), criterion);
}

void pop_mont(std::vector<AStarNode*> &vector)
{
    std::pop_heap(vector.begin(), vector.end(), criterion);
    vector.pop_back();
}

void update(std::vector<AStarNode*> &vector)
{
    std::sort_heap(vector.begin(), vector.end(), criterion);
}



float estimatedDistance(AStarNode* a, AStarNode* b)
{
    return abs(a->position.x - b->position.x) + abs(a->position.y - b->position.y);
}

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode, int cellsWidth, int cellsHeight, float mapWidth, float mapHeight, float** additionalCost, std::list<Vector3> &path)
{
    AStarNode* actual = originNode;
    actual->G = 0;
    actual->H = _distance(actual->position, targetNode->position);    
    actual->F = actual->G + actual->H;
    float d = 0;

    std::vector<AStarNode*> closed, opened;
    opened.push_back(actual);

    std::make_heap(opened.begin(), opened.end());
    
    bool found = false;

    while(!found && opened.size() > 0)
    {
        actual = opened.front();
        pop_mont(opened);
        closed.push_back(actual);

        if(actual == targetNode)
        {
            found = true;
        }
        else
        {   
            for(std::list<AStarNode*>::iterator it = actual->adjacents.begin(); it != actual->adjacents.end(); ++it)
            {                
                std::vector<AStarNode*>::iterator aux;
                aux = std::find(closed.begin(), closed.end(), *it);

                if(aux == closed.end()) 
                {
                    aux = std::find(opened.begin(), opened.end(), *it);

                    if(aux == opened.end()) 
                    {
                        (*it)->parent = actual;
                        (*it)->G = actual->G + _distance(actual->position, (*it)->position);
                        (*it)->H = estimatedDistance((*it), targetNode);
                        (*it)->F = (*it)->G + (*it)->H;
                        push_mont((*it), opened);
                    }
                    else
                    {
                         d = _distance(actual->position, (*it)->position);
                        if((*it)->G > (actual->G + d))
                        {
                            (*it)->parent = actual;
                            (*it)->G = actual->G + d;
                            (*it)->F = (*it)->G + (*it)->H;
                            update(opened);
                        }
                    }
                    
                }
            }
        }
        
    }
    path = recuperar(originNode, targetNode);
}


