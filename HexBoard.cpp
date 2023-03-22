#include "HexBoard.h"

#include <iostream>
#include <queue>

#include "queue.h"
#include "stack.h"

HexBoard::HexBoard(int radius) {
    this->radius = radius;
    create_board();
    configure_cells();
    start_coord = {0,0};
    end_coord = {0,0};
    longest_path_dist = 0;
}

void HexBoard::create_board() {
    //zero case
    if(radius == 0){return;}
    //other case
    HexCell *newcell = new HexCell(0,0);
    cells[{0,0}] = newcell;
    for(int i = 2; i<=radius; i++){
        for(int q = 0; q <= (i-1); q++){HexCell *newcell = new HexCell(q, -(i-1));
                                        cells[{q, -(i-1)}] = newcell;}
        for(int q = 0; q >= -(i-1); q--){HexCell *newcell = new HexCell(q,(i-1));
                                        cells[{q, (i-1)}] = newcell;}
        for(int r = 0; r< (i-1); r++){HexCell *newcell = new HexCell(-(i-1),r);
                                        cells[{-(i-1),r}] = newcell;}
        for(int r = 0; r > -(i-1); r--){HexCell *newcell = new HexCell((i-1),r);
                                        cells[{(i-1),r}] = newcell;}
        for(int q = 1; q<(i-1); q++){HexCell *newcell = new HexCell(q,(i-1)-q);
                                        cells[{q,(i-1)-q}] = newcell;}
        for(int q = -1; q>-(i-1); q--){HexCell *newcell = new HexCell(q,-(i-1)-q);
                                        cells[{q,-(i-1)-q}] = newcell;}
    }
}

void HexBoard::configure_cells() {
    for(auto it : cells)
    {
        pair<int, int> coord = it.first;
        HexCell* cell = it.second; 

        if(cells.count({coord.first + 1, coord.second -1})){cell->neighbours[1] = cells[{coord.first + 1, coord.second -1}];}
        if(cells.count({coord.first + 1, coord.second})){cell->neighbours[2] = cells[{coord.first + 1, coord.second}];}
        if(cells.count({coord.first, coord.second +1})){cell->neighbours[3] = cells[{coord.first, coord.second +1}];}
        if(cells.count({coord.first - 1, coord.second +1})){cell->neighbours[4] = cells[{coord.first - 1, coord.second +1}];}
        if(cells.count({coord.first - 1, coord.second})){cell->neighbours[5] = cells[{coord.first - 1, coord.second}];}
        if(cells.count({coord.first, coord.second -1})){cell->neighbours[0] = cells[{coord.first, coord.second -1}];}
    }
}

void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    map<pair<int,int>, bool> visited;
    Stack<pair<int,int>> order;
    this->start_coord = start_coord;
    order.push(start_coord);
    visited[start_coord] = true;

    int to_edge = 0;
    longest_path_dist = 0;

    while(!order.isEmpty()){
        HexCell * curr_cell = cells[order.peek()];
        HexCell * neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob);

        if(neigh_cell != nullptr){
            curr_cell->link(neigh_cell);
            visited[make_pair(neigh_cell->q, neigh_cell->r)] = true;
            neigh_cell->path_dist_from_start = curr_cell->path_dist_from_start+1;
            if(neigh_cell->path_dist_from_start>longest_path_dist){
                longest_path_dist = neigh_cell->path_dist_from_start;
            }
            if(neigh_cell->is_edge() && neigh_cell->path_dist_from_start >= to_edge){
                to_edge = neigh_cell->path_dist_from_start;
                end_coord = {neigh_cell->q, neigh_cell->r};
            }
            order.push(make_pair(neigh_cell->q, neigh_cell->r));
        }
        else{order.pop();}
    }
}

void HexBoard::solve_maze() {
    /**
     * @todo Your code here!
     */
    Stack<HexCell*> possible;
    map<HexCell*, HexCell*> visited; //key is current, value previous
    possible.push(cells[start_coord]);
    visited[cells[start_coord]] = NULL;

    while(!possible.isEmpty()){
        //check if end has been found.
        if(possible.peek() == cells[end_coord]){break;}

        //puts possible next options in 
        //pq ordered from worst choice to best choice.
        std::priority_queue<pair<int, HexCell*>> pq;
        for(auto &i : possible.peek()->links){
            //if i is null or already visited
            if(i == nullptr || visited.find(i) != visited.end()){continue;}
            int score = i->path_dist_from_start + i->distance(cells[end_coord]);
            pq.push(make_pair(score, i));
            visited[i] = possible.peek();
        }
        //if no remaining neighbours to visit, go back.
        if(pq.empty()){possible.pop();}

        //puts pq on possible if it is not empty.
        while(!pq.empty()){
            possible.push(pq.top().second);
            pq.pop();
        }
    }

    //backtracking
    Stack<pair<int,int>> reverse;
    HexCell* current = cells[end_coord];
    while(true){
        pair<int,int> t(current->q, current->r);
        reverse.push(t);
        current = visited[current];
        if(current == NULL){break;}
    }
    while(!reverse.isEmpty()){
        solution.push_back(reverse.pop());
    }

}