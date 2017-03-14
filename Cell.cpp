#include "Cell.h"
 
#include <cmath>


Cell::Cell(){
    
}

Cell::Cell(int x, int y, int size){
    //initialize new Cell with given values and set other values on default
    m_x = x;
    m_y = y;
    m_id = m_y * size + m_x;
    m_H = 0;
    passable = true;
    visited = false;
    open = false;
    //onPathToGoal = false;
    start = false;
    goal = false;
    blocked = false;
    current = false;
}

//all getters, setters and other requests

int Cell::getX(){
    return m_x;
}

void Cell::setX(int x) {
    m_x = x;
}

int Cell::getY(){
    return m_y;
}

void Cell::setY(int y) {
    m_y = y;
}

int Cell::getId(){
    return m_id;
}

double Cell::getManDis(Cell *goal){
    double x_dif = std::fabs(this->m_x - goal->getX());
    double y_dif = std::fabs(this->m_y - goal->getY());
    return x_dif + y_dif;
}

void Cell::setManDis(double H){
    m_H = H;
}

void Cell::setUnpassable(){
    passable = false;
}

bool Cell::isPassable(){
    return passable;
}

void Cell::setVisited(){
    visited = true;
}

bool Cell::isVisited(){
    return visited;
}

void Cell::setStart(){
    start = true;
}

bool Cell::isStart(){
    return start;
}

void Cell::setGoal(){
    goal = true;
}

bool Cell::isGoal(){
    return goal;
}

void Cell::setOpen() {
    open = true;
}

bool Cell::isOpen(){
    return open;
}

void Cell::setBlocked(){
    blocked = true;
}

bool Cell::isBlocked(){
    return blocked;
}

void Cell::setCurrent(){
    current = true;
}

void Cell::setNotCurrent(){
    current = false;
}

bool Cell::isCurrent(){
    return current;
}