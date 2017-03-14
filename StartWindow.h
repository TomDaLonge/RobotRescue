#ifndef MOVINGROBOTER_H
#define	MOVINGROBOTER_H

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QLineEdit>

class StartWindow : public QWidget {
    Q_OBJECT
public:
    StartWindow();
    int getSize();
    int getPercentage();
    int getBattery();
    bool hasAcceptableInput();
    
protected:
    void paintEvent(QPaintEvent *event);
   
private:
    int pic;
    int x;
    int i_size;
    int i_percentage;
    int i_battery;
    QPixmap *roboter;
    QLineEdit *size_edit;
    QLineEdit *percentage_edit;
    QLineEdit *battery_edit;
    
private slots:    
    void moveRobot();

    
};

#endif	/* MOVINGROBOTER_H */

