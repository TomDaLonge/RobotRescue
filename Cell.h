#ifndef CELL_H
#define	CELL_H


class Cell {
public:
    Cell();
    Cell(int x, int y, int size);   
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getId();
    double getH();
    double getManDis(Cell *goal);
    void setManDis(double H);
    void setUnpassable();
    bool isPassable();
    void setVisited();
    bool isVisited();
    void setStart();
    bool isStart();
    void setGoal();
    bool isGoal();
    void setOpen();
    bool isOpen();
    void setBlocked();
    bool isBlocked();
    void setCurrent();
    void setNotCurrent();
    bool isCurrent();
    
private:
    int m_x;
    int m_y;
    int m_id;
    double m_H;
    bool passable;
    bool visited;
    bool open;
    //bool onPathToGoal;
    bool start;
    bool goal;
    bool blocked;
    bool current;
};

#endif	/* CELL_H */

