#ifndef PATHFINDING_H
#define	PATHFINDING_H

#include "Cell.h"

#include <QList>
#include <QMap>

class PathFinding {
public:
    PathFinding();
    void initializeSurface(int size, int percentage);
    void continuePath();
    
    int getSize();
    Cell* getCell(int id);
    int getMoveCount();
    bool isWayPassable();
    bool isfoundGoal();
    
private:
    Cell *m_start;
    Cell *m_goal;
    Cell *prev;
    QMap<int, Cell*> m_area;
    QList<Cell*> openList;
    QList<Cell*> previousList;
    //QList<Cell*> blockedList;
    int m_size;
    int m_percentage;
    int newOpenCount;
    int visitedCount;
    int openCount;
    int moveCount;
    bool wayPassable;
    bool foundGoal;
    
    void initializeArea();
    void initializeStartGoal();
    void initializeUnpassables();
    Cell* getNextCell();
    void openPath(int x, int y);
    bool isGoalCell(Cell* currentCell);
};

#endif	/* PATHFINDING_H */

