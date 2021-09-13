/**
 * @brief   C++ file cointaining time shedule line, line is formated for each vehicle here
 * @file    myorderline.cpp
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */


#include "myorderline.h"
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QFont>

MyOrderLine::MyOrderLine(QGraphicsLineItem *parent) : QObject(), QGraphicsLineItem(parent)
{
    hide();
    setPen(QPen(Qt::blue, 3));
}


void MyOrderLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->drawLine(line());

   // name
   painter->setFont(QFont("Courier", 15));
   painter->drawText(line().pointAt(0).x(), line().pointAt(0).y() - 50, "vehicle: " + name);

   qreal x = 0;
   qreal m;
   QString text;    // text next to each station

   int hours;
   int minutes;
   int seconds;
   while (x <  stations.length())
   {
        // recounting to hours:mins:seconds
        seconds = stationTime[x];
        hours = seconds / 3600;
        seconds -= hours * 3600;
        minutes = seconds / 60;
        seconds -= minutes * 60;

        // drawing stations with its description
        m = x / (stations.length() - 1);
        painter->setBrush(QBrush(Qt::green));
        painter->drawEllipse(line().pointAt(m), 10,10);   // station
        text = stations[x]+ " - " + QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds);
        painter->drawText(line().pointAt(m).x() + 20 ,line().pointAt(m).y(), text);
        x++;

        // location of the vehicle on the line
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawEllipse(line().pointAt(lenTravelled / routeLength), 10,10);
   }
}
