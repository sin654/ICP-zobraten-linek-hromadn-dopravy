#ifndef MYORDERLINE_H
#define MYORDERLINE_H

/**
 * @brief   This file is a header file for myorderline.cpp
 * @file    myorderline.h
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include <QGraphicsLineItem>
#include <QObject>


/**
 * @brief The MyOrderLine class, which is a class of time schedule line of the vehicle
 */
class MyOrderLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    explicit MyOrderLine(QGraphicsLineItem *parent = nullptr);

    // arrays
    QVector<QString> stations;  // stations of the route
    std::vector<double> stationTime;    // time to get to each station of the route
    QString name;   // name of the vehicle

    qreal routeLength;  // vehicle route length
    qreal lenTravelled; // length already travelled by the vehicle

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

signals:

};

#endif // MYORDERLINE_H
