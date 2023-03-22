#include "HexCell.h"

#include <cmath>
#include <iostream>

HexCell::HexCell(int Q, int R) {
    q = Q;
    r = R;
    path_dist_from_start = 0;
    for(int i = 0; i<6; i++){
        neighbours[i] = nullptr;
        links[i] = nullptr;
    }
}

void HexCell::link(HexCell* cell) {
    //null case
    if (cell == nullptr){return;}
    int qd = cell->q-q;
    int rd = cell->r-r;

    if(qd == 0 && rd ==-1){links[N] = cell; cell->links[S] = this;}
    if(qd == 1 && rd ==-1){links[NE] = cell; cell->links[SW] = this;}
    if(qd == 1 && rd ==0){links[SE] = cell; cell->links[NW] = this;}
    if(qd == 0 && rd ==1){links[S] = cell; cell->links[N] = this;}
    if(qd == -1 && rd ==1){links[SW] = cell; cell->links[NE] = this;}
    if(qd == -1 && rd ==0){links[NW] = cell; cell->links[SE] = this;}

    return;
}

int HexCell::get_neighbour_side(const HexCell* cell) const {
    for (int i = 0; i<6; i++)
    {
        if (neighbours[i] == cell){return i;}
    }
    return -1;
}

bool HexCell::is_linked(const HexCell* cell) const {
    //null case
    if (cell == nullptr){return false;}
    for (int i = 0; i<6; i++)
    {
        if(links[i] == cell){return true;}
    }
    return false;
}

int HexCell::distance(const HexCell* cell) const {
    int qd = abs(cell->q - q);
    int rd = abs(cell->r - r);
    int sd = abs(-(cell->q-q)-(cell->r -r));
    return int((qd + rd + sd)/2);
}

bool HexCell::is_edge() const {
    for (int i = 0; i< 6; i++)
    {
        if(neighbours[i] == nullptr){return true;}
    }
    return false;
}