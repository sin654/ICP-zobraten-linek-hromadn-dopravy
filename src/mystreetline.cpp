/**
 * @brief   C++ file containing definition of class MyStreetLine, which instances represent streets with stations
 * @file    mystreetline.cpp
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include "mystreetline.h"
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

MyStreetLine::MyStreetLine(QGraphicsItem *parent) :
    QObject(),
    QGraphicsLineItem(parent)   // was parent
{
    setFlag(QGraphicsItem::ItemIsSelectable, true); // so user is able to select street and change its speed
    setPen(QPen(Qt::red, 2));
}

void MyStreetLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // direction of the street p1 -> p2
    //painter->drawEllipse(line().pointAt(0.7), 10, 10);
    // name of the street
    painter->drawText(line().pointAt(0.5).x()+15, line().pointAt(0.5).y()+12, QString(this->name));
    // painting street line
    painter->setPen(pen());
    painter->drawLine(line());
    // painting stations on the street
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::blue));
    for (auto station : this->stations)
    {
        painter->drawEllipse(line().pointAt(station), 7, 7);
    }
}
