#include "MainWindow.h"

#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow() {
    //create mainLayout & childLayouts, add them to mainLayout
    QGridLayout *mainLayout = new QGridLayout();
    
    QHBoxLayout *topLayout = new QHBoxLayout();
    menuLayout = new QVBoxLayout();
    screenLayout = new QHBoxLayout();
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    
    mainLayout->addLayout(topLayout, 1, 1, 1, 8);
    mainLayout->addLayout(menuLayout, 2, 1, 6, 2);
    mainLayout->addLayout(screenLayout, 2, 3, 6, 6);
    mainLayout->addLayout(bottomLayout, 8, 1, 1, 8);
    
    this->setLayout(mainLayout);
    
    setWindowTitle("Robot Rescue");
    setWindowIcon(QIcon(trUtf8(":/images/r-logo.png"))); 
    //logo found on http://www.spiralknights.com:82/wiki-images/1/1c/Placemarker-R.png
    this->setMinimumSize(550, 550);
    
    //initialize top and bottom layouts
    QLabel *topLabel = new QLabel("<h1>ROBOT RESCUE</h1>");
    topLabel->setFont(QFont("Calibri"));
    topLayout->addWidget(topLabel, 1, Qt::AlignHCenter);
    
    QLabel *bottomLabel = new QLabel(trUtf8("© Dominik Meyer & Tom Daniek, Intelligent Systems Design, 3.Semester, HSHL, 2015"));
    bottomLabel->setFixedHeight(25);
    bottomLayout->addWidget(bottomLabel, 1,  Qt::AlignHCenter);
    
    //excluded in function, got too confusing
    initializeMenu();
    
    //when starting programm, screenLayout should show the startWindow
    firstWindow = new StartWindow;
    screenLayout->addWidget(firstWindow);
}

void MainWindow::startSimulation(){
    //user pushes startbutton -> content of screenLayout changes to
    //simulation and starts it, startbutton is now disabled
    if (firstWindow->hasAcceptableInput()){
        mainPercentage = firstWindow->getPercentage();
        mainSize = firstWindow->getSize();
        mainBattery = firstWindow->getBattery();
        screenLayout->removeWidget(firstWindow);
    } else {
        wrongInput();
        return;
    }
    delete firstWindow;
    graphics = new Simulation(mainSize, mainPercentage, mainBattery);
    screenLayout->addWidget(graphics);
    start->setDisabled(true);
    restart->setDisabled(false);
    back->setDisabled(false);
    timer->start(400);
}

void MainWindow::restartSimulation(){
    //only possible if simulationwindow is already open
    screenLayout->removeWidget(graphics);
    delete graphics;
    graphics = new Simulation(mainSize, mainPercentage, mainBattery);
    screenLayout->addWidget(graphics);
    timer->start(400);
}

void MainWindow::goBack(){
    //only possible if simulationwindow is already open
    screenLayout->removeWidget(graphics);
    delete graphics;
    firstWindow = new StartWindow();
    screenLayout->addWidget(firstWindow);
    start->setDisabled(false);
    restart->setDisabled(true);
    back->setDisabled(true);
    count->setText("0");
    timer->stop();
    batteryBar->setValue(100);
}

void MainWindow::exitApplication() {
    close();
}

void MainWindow::wrongInput(){
    QMessageBox error;
    
    error.setWindowTitle("Falsche Werte");
    error.setText(trUtf8("Bitte geben Sie für die Variablen gültige Werte ein."));
    
    error.exec();
}

void MainWindow::initializeMenu(){
    //whole menubar on the left
    QLabel *upspace = new QLabel;
    upspace->setFixedHeight(100);
    menuLayout->addWidget(upspace);
    
    start = new QPushButton("Start", this);
    start->setFixedWidth(90);
    menuLayout->addWidget(start);
    connect(start, SIGNAL(clicked()), this, SLOT(startSimulation()));
    
    restart = new QPushButton("Neustart", this);
    restart->setFixedWidth(90);
    menuLayout->addWidget(restart);
    connect(restart, SIGNAL(clicked()), this, SLOT(restartSimulation()));
    restart->setDisabled(true);
    
    back = new QPushButton(trUtf8("Zurück"), this);
    back->setFixedWidth(90);
    menuLayout->addWidget(back);
    connect(back, SIGNAL(clicked()), this, SLOT(goBack()));
    back->setDisabled(true);
    
    QPushButton *end = new QPushButton("Beenden", this);
    end->setFixedWidth(90);
    menuLayout->addWidget(end);
    connect(end, SIGNAL(clicked()), this, SLOT(exitApplication()));
 
    QLabel *count_label = new QLabel(trUtf8("Anzahl Züge:"));
    count_label->setFixedWidth(90);
    count_label->setFixedHeight(15);
    menuLayout->addWidget(count_label);
 
    count = new QLabel("0");
    count->setFixedWidth(90);
    count->setFixedHeight(15);
    count->setAlignment(Qt::AlignRight);
    menuLayout->addWidget(count);
    
    QLabel *battery_label = new QLabel(trUtf8("Akkuladestand:"));
    count_label->setFixedWidth(75);
    count_label->setFixedHeight(15);
    menuLayout->addWidget(battery_label);
    
    batteryBar = new QProgressBar();
    batteryBar->setRange(0, 100);
    batteryBar->setValue(100);
    batteryBar->setTextVisible(true);
    
    menuLayout->addWidget(batteryBar);
    
    QLabel *downspace = new QLabel;
    downspace->setFixedHeight(100);
    menuLayout->addWidget(downspace);
    
    //timer for changing movecount and batterystatus
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(setMoveCount()));
    connect(timer, SIGNAL(timeout()), this, SLOT(batteryLevelChanged()));
}

void MainWindow::setMoveCount(){
    //changes content of label every signals sent by timer
    pathCount = QString::number(graphics->getPathMoveCount());
    count->setText(trUtf8("%1 / %2").arg(pathCount).arg(mainBattery));
    update();
}

void MainWindow::batteryLevelChanged(){
    //changes status of battery bar at every signal sent by timer and stops timer if goal is found
    batteryBar->setValue(graphics->getBatteryPercentage());  
    
    if (graphics->GoalIsFound()){
        timer->stop();
        return;
    }
}