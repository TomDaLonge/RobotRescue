#include "Simulation.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <cfloat>
#include <cmath>
#include <QPainter>
#include <QColor>

Simulation::Simulation(int size, int percentage, int battery) {
    newPath.initializeSurface(size, percentage);
    this->battery = battery;
    
    //at every signal sent by timer, roboter makes next move
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(goOn()));
    timer->start(400);

    //stop timer if roboter has found goal
    if (newPath.isfoundGoal()){
        timer->stop();
    }
}

void Simulation::paintEvent(QPaintEvent *event) {
    //initialize painter and sizes
    QPainter painter;
    painter.begin(this);
    QPen mPen;
    QBrush mBrush;
    
    QSize size = this->size();
    int cellWidth = size.width() / newPath.getSize();
    int cellHeight = size.height() / newPath.getSize();
    int entire = newPath.getSize() * newPath.getSize();
    
    for (int i = 0; i < entire; i++) {
        Cell* currentCell = newPath.getCell(i);
        //inititalize color of pen for each Cell
        if (!currentCell->isPassable()) {
            mBrush.setColor(Qt::black);
        } else if (currentCell->isCurrent()) {
            mBrush.setColor(Qt::yellow);
        } else if (currentCell->isStart()) {
            mBrush.setColor(Qt::green);
        } else if (currentCell->isGoal()) {
            mBrush.setColor(Qt::red);
        } else if (currentCell->isBlocked()){
            mBrush.setColor(Qt::magenta);
        } else if (currentCell->isVisited()) {
            mBrush.setColor(Qt::blue);
        } else if (currentCell->isOpen()) {
            mBrush.setColor(Qt::lightGray);
        } else {
            mBrush.setColor(Qt::darkGray);
        }
        
        //finally paint Cell
        mBrush.setStyle(Qt::SolidPattern);
        painter.setPen(Qt::white);
        painter.setBrush(mBrush);
        //width and height depend on window size
        painter.drawRect(currentCell->getX() * cellWidth, currentCell->getY() * cellHeight, cellWidth - 1, cellHeight - 1);
    }
    painter.end();
}

void Simulation::goOn(){
    
    //if goal is not found yet, a passable cell around current exists 
    //and battery is not empty, go on and update,
    //else stop timer and show particular message
    
    if (!newPath.isfoundGoal()){
        if (newPath.isWayPassable() && newPath.getMoveCount() < battery){
            newPath.continuePath();
        } else if (newPath.getMoveCount() >= battery){
            timer->stop();
            msg_lowBattery();
        } else {
            timer->stop();
            msg_noWayPassable();
        }
    } else {
        timer->stop();
        msg_foundGoal();
    }
    update();
}

void Simulation::msg_noWayPassable(){
    QMessageBox error;
    
    error.setWindowTitle("Ein Fehler ist aufgetreten");
    error.setText("Der Roboter konnte seinen Weg nicht finden! Bitte erneut versuchen!");
    
    error.exec();
}

void Simulation::msg_foundGoal(){
    QMessageBox goal;
    
    goal.setWindowTitle("Ziel erreicht");
    goal.setText(trUtf8("Der Roboter hat sein Ziel erreicht. Er hat dafür %1 Züge gebraucht. Sein Akkuladestand beträgt %2 %.").arg(newPath.getMoveCount()).arg(getBatteryPercentage()));
    
    goal.exec();
}

void Simulation::msg_lowBattery(){
    QMessageBox goal;
    
    goal.setWindowTitle("Akku leer");
    goal.setText(trUtf8("Der Akku des Roboters ist leer! Bitte erneut versuchen!"));
    
    goal.exec();
}

int Simulation::getBatteryPercentage(){
    //calculate actual battery status and return it.
    //problematic because percentage is rounded to zero before 
    //it gets multiplied with 100 if both values are int
    double actualBattery = double(battery) - double(newPath.getMoveCount());
    int batteryStatus = static_cast<int>(round((actualBattery / double(battery)) * 100));
    return batteryStatus;
}

bool Simulation::GoalIsFound(){
    return newPath.isfoundGoal();
}

int Simulation::getPathMoveCount(){
    return newPath.getMoveCount();
}