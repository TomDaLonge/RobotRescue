#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "StartWindow.h"
#include "Simulation.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QProgressBar>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow();
   
private:
    QTimer *timer;
    StartWindow *firstWindow;
    Simulation *graphics;
    QVBoxLayout *menuLayout;
    QHBoxLayout *screenLayout;
    QPushButton *start;
    QPushButton *restart;
    QPushButton *back;
    QLabel *count;
    QString pathCount;
    int mainSize;
    int mainPercentage;
    int mainBattery;
    QProgressBar *batteryBar;
    
    void wrongInput();
    void initializeMenu();
    
private slots:
    void startSimulation();
    void restartSimulation();
    void goBack();
    void exitApplication();
    void setMoveCount();
    void batteryLevelChanged();
};

#endif	/* MAINWINDOW_H */

