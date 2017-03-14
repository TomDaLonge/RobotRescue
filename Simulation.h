#ifndef SIMPLEGRAPHICS_H
#define	SIMPLEGRAPHICS_H

#include "PathFinding.h"

#include <QWidget>
#include <QPaintevent>
#include <QTimer>

class Simulation : public QWidget {
    Q_OBJECT
public:
    Simulation(int size, int percentage, int battery);
    int getPathMoveCount();
    bool GoalIsFound();
    int getBatteryPercentage(); 
    
protected:
    void paintEvent(QPaintEvent *event);

private:
    PathFinding newPath;
    QTimer *timer;
    int battery;
    
    void msg_noWayPassable();
    void msg_foundGoal();
    void msg_lowBattery();
  
private slots:
    void goOn();   
};

#endif	/* SIMPLEGRAPHICS_H */

