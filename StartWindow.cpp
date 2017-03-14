#include "StartWindow.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QIntValidator>
#include <QPainter>
#include <QTimer>

StartWindow::StartWindow() {
    //create mainLayout and add the three child layouts
    QGridLayout *mainLayout = new QGridLayout;
    
    QVBoxLayout *leftLayout = new QVBoxLayout();
    mainLayout->addLayout(leftLayout, 15, 1);
    QVBoxLayout *centerLayout = new QVBoxLayout();
    mainLayout->addLayout(centerLayout, 15, 2);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout, 15, 3);
    
    this->setLayout(mainLayout);
    
    //create roboter
    roboter = new QPixmap(":images/robot.png");
    pic = 0;
    x = 0;
    
    //set default values. can be changed per lineedit
    i_size = 25;
    i_percentage = 45;
    i_battery = 80;
    
    //create timer for moving robot
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveRobot()));
    timer->start(70);
    
    //create labels in leftLayout (tags for lineedits)
    QLabel *size_label = new QLabel( trUtf8("Größe:"));
    leftLayout->addWidget(size_label);
    
    QLabel *percentage_label = new QLabel( trUtf8("% unpassierbare Felder:"));
    leftLayout->addWidget(percentage_label);
    
    QLabel *battery_label = new QLabel( trUtf8("Anzahl möglicher Züge:"));
    leftLayout->addWidget(battery_label);
    
    //initialize lineedits for centerLayout
    size_edit = new QLineEdit();
    size_edit->insert("25");
    size_edit->setValidator( new QIntValidator(20, 80, this));
    centerLayout->addWidget(size_edit);
    
    percentage_edit = new QLineEdit();
    percentage_edit->insert("40");
    percentage_edit->setValidator( new QIntValidator(1, 90, this));
    centerLayout->addWidget(percentage_edit);
    
    battery_edit = new QLineEdit();
    battery_edit->insert("80");
    battery_edit->setValidator( new QIntValidator(20, 200, this));
    centerLayout->addWidget(battery_edit);
    
    //create labels for rightLayout (acceptable values info)
    
    QLabel *pot_size_label = new QLabel( trUtf8("(20 -  80)"));
    rightLayout->addWidget(pot_size_label);
    
    QLabel *pot_percentage_label = new QLabel( trUtf8("(  1 -  90)"));
    rightLayout->addWidget(pot_percentage_label);
    
    QLabel *pot_battery_label = new QLabel( trUtf8("(20 - 200)"));
    rightLayout->addWidget(pot_battery_label);
}

void StartWindow::paintEvent(QPaintEvent* event){
    //paintevent for robot
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(x, 15, 150, 150, *roboter, pic*100, 0, 120, 120);
    painter.end();
}

void StartWindow::moveRobot(){
    //slot to move the robot at every signal sent by timer
    x = x + 6;
    pic++;
    pic = pic % 1;
    
    QSize window_size = this->size();
    if(x > window_size.width()){
        x = 0;
    }
    update();
}

//save values of lineedits and return them
//used in MainWindow to get content of lineedits when starting simulation
int StartWindow::getSize(){
    i_size = size_edit->text().toInt();
    return i_size;
}

int StartWindow::getPercentage(){
    i_percentage = percentage_edit->text().toInt();
    return i_percentage;
}

int StartWindow::getBattery(){
    i_battery = battery_edit->text().toInt();
    return i_battery;
}

bool StartWindow::hasAcceptableInput(){
    //check if input of all three lineedits is in the range
    return size_edit->hasAcceptableInput() && percentage_edit->hasAcceptableInput() && battery_edit->hasAcceptableInput();
}