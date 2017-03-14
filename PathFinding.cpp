#include "PathFinding.h"

#include <QTime>
#include <cmath>
#include <cfloat>


PathFinding::PathFinding() {
}

void PathFinding::initializeSurface(int size, int percentage){
    //initialize object of pathfinding with given values
    //and create area, start/goal and unpassables
    m_size = size;
    m_percentage = percentage;
    foundGoal = false;
    moveCount = 0;
    wayPassable = true;
    initializeArea();
    initializeStartGoal();
    initializeUnpassables();
}

void PathFinding::initializeArea(){
    //fill map m_area with Cells, use id as key
    for (int y = 0; y < m_size; y++){
        for (int x = 0; x < m_size; x++){
            int id = y * m_size + x;
            Cell* cell = new Cell(x, y, m_size);
            m_area[id] = cell;     
        }
    }
}

void PathFinding::initializeStartGoal(){
    //for random ints
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    
    //give m_start a random cell and mark cell in m_area as start
    int start_x = qrand() % m_size;
    int start_y = qrand() % m_size;
    int start_id = start_y * m_size + start_x;
    
    m_start = new Cell(start_x, start_y, m_size);
    m_area[start_id]->setStart();
    
    //now search candidate for m_goal but be sure
    //that start and goal have at least a distance of 19
    
    int goal_x = 0;
    int goal_y = 0;
    Cell split;
    
    do{
        goal_x=qrand() % m_size;
        goal_y=qrand() % m_size;
        split.setX(goal_x);
        split.setY(goal_y);
    } while (m_start->getManDis(&split) < 19);
    
    //finally initialize m_goal
    m_goal = new Cell(goal_x, goal_y, m_size);
    int goal_id = goal_y * m_size + goal_x;
    m_area[goal_id]->setGoal();
    
    //set Manhattan distance of start and add it to openList
    m_start->setManDis(m_start->getManDis(m_goal));
    openList.push_back(m_start);
}

void PathFinding::initializeUnpassables(){
    //for random ints
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int id = -1;
    
    //get number of necessary unpassables
    int num_unpassables = static_cast<int>(round(m_size*m_size * (m_percentage/100.0)));
    for(int i = 0; i < num_unpassables; i++){
        //make sure that new unpassable isn't already unpassable
        // and is neither start nor goal
        do{
            id = qrand() % (m_size * m_size);
        } while (!m_area[id]->isPassable() || m_area[id]->isStart() || m_area[id]->isGoal());
        m_area[id]->setUnpassable();
    }
}

void PathFinding::continuePath(){
    
    //see function getNextCell
    Cell *currentCell = getNextCell();
    
    //change the as current marked cell
    if (moveCount >= 1){
        m_area[prev->getId()]->setNotCurrent();
        m_area[currentCell->getId()]->setCurrent();
    }
    
    //Checking if CurrentCell is already the GoalCell
    if (isGoalCell(currentCell)) {
        return;
    }
    
    //delete values of previous step
    openList.clear();
    newOpenCount = 0;
    visitedCount = 0;
    openCount = 0;
   
    //look at neighbours of currentCell and add them to openList
    //right
    openPath(currentCell->getX() + 1, currentCell->getY());
    //left
    openPath(currentCell->getX() - 1, currentCell->getY());
    //down
    openPath(currentCell->getX(), currentCell->getY() + 1);
    //up
    openPath(currentCell->getX(), currentCell->getY() - 1);
    //down right
    openPath(currentCell->getX() + 1, currentCell->getY() + 1);
    //down left
    openPath(currentCell->getX() - 1, currentCell->getY() + 1);
    //up right
    openPath(currentCell->getX() + 1, currentCell->getY() - 1);
    //up left
    openPath(currentCell->getX() - 1, currentCell->getY() - 1);

    //set blocked if cell is dead end
    if (newOpenCount == 0 && visitedCount == 1 && openCount == 0){
            m_area[currentCell->getId()]->setBlocked();
    }
    
    //set blocked if roboter is jumping between two cells
    //this can happen if two cells have the least two ManhattanDistances of surrounding cells
    if (previousList.size() >= 3){
        if (previousList[0]->getId() == previousList[2]->getId() && currentCell->getId() == previousList[1]->getId()){
            m_area[currentCell->getId()]->setBlocked();
        }
        previousList.removeLast();
    }
    
    //if there are no open cells in the surrounding, there is no way passable
    if (openList.empty()){
        wayPassable = false;
    }
    
    //save currentCell in previous so it can be unmarked in the next round
    previousList.push_front(currentCell);
    moveCount += 1;
    prev = currentCell;
}

Cell* PathFinding::getNextCell(){
    double shortestDistance = DBL_MAX;
    int cellIndex = -1;
    Cell *nextCell = NULL;
    
    //search for shortest distance to goal cell, not yet visited first
    for(int i = 0; i < openList.size(); i++){
        if(openList[i]->getManDis(m_goal) < shortestDistance && !m_area[openList[i]->getId()]->isVisited()){
            shortestDistance = openList[i]->getManDis(m_goal);
            cellIndex = i;
        }
    }
    
    //if no unvisited Cells are found, search in visited
    if (cellIndex == -1){
        for(int i = 0; i < openList.size(); i++){
            if(openList[i]->getManDis(m_goal) < shortestDistance){
                shortestDistance = openList[i]->getManDis(m_goal);
                cellIndex = i;
            }
        }
    }
    
    //set nextCell as this and return it
    nextCell = openList[cellIndex];
    m_area[nextCell->getId()]->setVisited();    
    return nextCell;
}

void PathFinding::openPath(int x, int y){
    int id = y * m_size + x;
    
    //Checking if Cell is outside the area, unpassable, blocked -> return
    //else increment particular counter
    if (x < 0 || y < 0 || x >= m_size || y >= m_size){
        return;
    } else if (!m_area[id]->isPassable()) {
        return;
    } else if (m_area[id]->isBlocked()) {
        return;
    } else if (m_area[id]->isVisited()){
        visitedCount += 1;
    } else if (m_area[id]->isOpen()){
        openCount += 1;
    } else {
        newOpenCount += 1;
    }
    
    //if not returned, push Cell to openList
    Cell* newOpenCell = new Cell(x, y, m_size);
    newOpenCell->setManDis(newOpenCell->getManDis(m_goal));
    
    openList.push_back(newOpenCell);
    m_area[id]->setOpen();
}

bool PathFinding::isGoalCell(Cell* currentCell) {
    //simply check if given cell is already the goalCell
    if (currentCell->getId() == m_goal->getId()){
        foundGoal = true;
        return true;
    }
    return false;
}

//getter and requests
int PathFinding::getSize(){
    return m_size;
}

Cell* PathFinding::getCell(int id){
    return m_area[id];
}

int PathFinding::getMoveCount(){
    return moveCount;
}

bool PathFinding::isWayPassable(){
    return wayPassable;
}

bool PathFinding::isfoundGoal(){
    return foundGoal;
}