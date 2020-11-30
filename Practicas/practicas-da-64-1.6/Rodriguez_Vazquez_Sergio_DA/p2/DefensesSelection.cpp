// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

struct DefenseCost
{
    DefenseCost();
    DefenseCost(Defense *defense, float cost): defense_(defense), cost_(cost){}
    Defense *defense_;
    float cost_;
};

float calculatecost(Defense *defense)
{
    return ((defense->damage * defense->attacksPerSecond) + defense->health + defense->range );
    // return defense->health;
}


void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs, float mapWidth, float mapHeight, std::list<Object*> obstacles) 
{
    std::vector<DefenseCost> defensecosts;
    
    std::list<Defense*>::iterator it = defenses.begin();

    //Quitamos la primera defensa porque no la tenemos que tener en cuenta
 
    selectedIDs.push_back((*it)->id);
    ases = ases - (*it)->cost;
    it++;


    //Recorremos la lista de defensas que nos dan y vamos creando la lista de candidatos para aplicar el algoritmo de la mochila
    while(it != defenses.end())
    {
        DefenseCost defenseiterated((*it), calculatecost((*it)));
        defensecosts.push_back(defenseiterated);
        ++it;
    }

    //algoritmo de la mochila
    float F[defensecosts.size()][ases + 1];


    for(int j = 0; j <= ases; j++)
    {
        if(j < defensecosts[0].defense_->cost)
            F[0][j] = 0;
        else 
            F[0][j] = defensecosts[0].cost_;
    }
    int i, j;
    for(i = 1; i < defensecosts.size(); i++)
    {
        for(j = 0; j <= ases; j++)
        {
            if (j < defensecosts[i].defense_->cost)
                F[i][j] = F[i - 1][j];
            else
                F[i][j] = std::max(F[i - 1][j],F[i - 1][j - defensecosts[i].defense_->cost] + defensecosts[i].defense_->cost);
        }
    }

    // std::list<Defense*>::iterator it = defenses.begin();

    i = defensecosts.size() - 1;
    j = ases;

    // while(F[1][j] != 0)
    // {
        while(i > 0)
        {
            if(F[i][j] != F[i - 1][j])
            {
                selectedIDs.push_back(defensecosts[i].defense_->id);
                j -= defensecosts[i].defense_->cost;
            }
            --i;
        }
    // }

}
