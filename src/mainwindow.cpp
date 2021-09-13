/**
 * @brief   C++ file containing definition of the application window
 * @file    mainwindow.cpp
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new MyMapScene;
    ui->graphicsView->setScene(scene);

    // enhances rendering in graphicsViev
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);

    // start stop button
    ui->startStopButton->setText("PAUSE");
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::startStop);

    // setting time to
    ui->timeLabel->setText(QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds));
    ui->timeLabel->setStyleSheet("font: 20pt;");

    // connect zoom buttons
    connect(ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    ui->zoomInButton->setStyleSheet("background-color:magenta");
    connect(ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    ui->zoomOutButton->setStyleSheet("background-color:brown");

    // connect speedUp slowDown buttons
    connect(ui->speedUpButton, &QPushButton::clicked, this, &MainWindow::speedUpSymulation);
    ui->speedUpButton->setStyleSheet("background-color:green");
    connect(ui->slowDownButton, &QPushButton::clicked, this, &MainWindow::slowDownSymulation);
    ui->slowDownButton->setStyleSheet("background-color:red");

    // connect default time button
    connect(ui->defaultTimeButton, &QPushButton::clicked, this, &MainWindow::setDefaultTime);
    ui->defaultTimeButton->setStyleSheet("background-color:blue");

    // connect default speed button
    connect(ui->defaultSpeedButton, &QPushButton::clicked, this, &MainWindow::setDefaultSpeed);
    ui->defaultSpeedButton->setStyleSheet("background-color:yellow");

    // timer settings
    appClock = new QTimer(this);
    appClock->setInterval(symulationSpeed); // 1000ms default
    appClock->start();
    connect(appClock, &QTimer::timeout, this, &MainWindow::onTimer);

    // change speed on the street slider
    connect(ui->changeStreetSpeedSlider, &QSlider::valueChanged, this, &MainWindow::onStreetSpeedChange);

    // slider label
    ui->sliderLabel->setStyleSheet("font: 18pt;");

    // so whole map is visible in small window
    zoomOut();
    zoomOut();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // delete all the items in the scene and window itself?
    scene->deleteMapExit();
    delete scene;
    event->accept();
}

void MainWindow::zoomIn()
{
    ui->graphicsView->scale(1.25, 1.25);
}

void MainWindow::zoomOut()
{
    ui->graphicsView->scale(0.8, 0.8);
}

void MainWindow::onTimer()
{
    // clock
    seconds++;
    if (seconds == 60)
    {
        minutes++;
        seconds = 0;
    }
    if (minutes == 60)
    {
        hours++;
        minutes = 0;
    }
    if (hours == 24)
    {
        hours = 0;
    }

    ui->timeLabel->setText(QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds));   // setting new time

    // propagates to street map scene
    scene->clockEvent(hours, minutes, seconds);
}

void MainWindow::speedUpSymulation()
{
    // 2x speed
    symulationSpeed /= 2;
    appClock->setInterval(symulationSpeed);
}

void MainWindow::slowDownSymulation()
{
    // half speed
    symulationSpeed *= 2;
    appClock->setInterval(symulationSpeed);
}

void MainWindow::setDefaultTime()
{
    // reset time to 0:0:0
    seconds = 0;
    minutes = 0;
    hours = 0;

    // have to delete all the vehicles in the map scene , or they would be added again -> inconsistent
    scene->timeResetEvent();
}

void MainWindow::setDefaultSpeed()
{
    // refresh every 1s
    symulationSpeed = 1000;
    appClock->setInterval(symulationSpeed);
}

void MainWindow::onStreetSpeedChange(int val)
{
    scene->onStreetSpeedChange(val);
}

void MainWindow::startStop()
{
    if (ui->startStopButton->text() == "PAUSE")
    {
        appClock->stop();
        ui->startStopButton->setText("CONTINUE");
    }
    else if (ui->startStopButton->text() == "CONTINUE")
    {
        appClock->start();
        ui->startStopButton->setText("PAUSE");
    }
}

