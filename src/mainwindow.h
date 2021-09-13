#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @brief   This file is a header file for mainwindow.cpp
 * @file    mainwindow.h
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */


#include <QMainWindow>
#include <QTimer>
#include <QMap>
#include <QCloseEvent>
#include "mymapscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class, which is class of the Qt application window object
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor of QMainWindow class instance (basic setting, connecting UI to signals etc.)
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyMapScene *scene;  // declaration of pointer to QGraphicsScene class instance

private:
    Ui::MainWindow *ui;
    QTimer *appClock;   // timer for application clock
    double symulationSpeed{1000};   // default symulation speed is 1000ms = 1s, lower means faster etc.

    // clock
    int hours{0};   // from qint16
    int minutes{0};
    int seconds{0};

protected:
    virtual void closeEvent(QCloseEvent *event) override;


private slots:
    /**
     * @brief Zomms in QGraphicsScene class instance
     */
    void zoomIn();

    /**
     * @brief Zooms out QGraphicsScene class instance
     */
    void zoomOut();

    /**
     * @brief Called every time application time emits signal timeout() and propagates this insformation to QGraphicsScene instance
     */
    void onTimer();

    /**
     * @brief Speeds up symulation by decreasing symulationSpeed
     */
    void speedUpSymulation();

    /**
     * @brief Slows down symulation by increasing symulationSpeed
     */
    void slowDownSymulation();

    /**
     * @brief Resets application clock to 0:0:0 (default time) and propagates this information to QGraphicsScene instance
     */
    void setDefaultTime();

    /**
     * @brief Sets symulationSpeed back to 1000ms = 1s
     */
    void setDefaultSpeed();

    /**
     * @brief when you move with the slider and some street is selected its speed will be changed
     * @param val
     */
    void onStreetSpeedChange(int val);

    /**
     * @brief pauses the symulation or continues it
     */
    void startStop();
};
#endif // MAINWINDOW_H
