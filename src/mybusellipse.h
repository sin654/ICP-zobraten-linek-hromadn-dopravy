#ifndef MYBUSELLIPSE_H
#define MYBUSELLIPSE_H

/**
 * @brief   This file is a header file for mybusellipse.cpp
 * @file    mybusellipse.h
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include "mymapscene.h"
#include <vector>

/**
 * @brief The MyBusEllipse class, which is a class of vehicle travelling on the map
 */
class MyBusEllipse : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit MyBusEllipse(QGraphicsItem *parent = nullptr);
    MyMapScene *map;     // pointer to its map scene
    QVector<QString> stations;  // stations which this vehicle goes throught
    std::vector<bool> directions;   // direction which this vehicle goes throught the streets with stations above
    QString name;   // name of the vehicle

    /**
     * @brief reaction to application clock
     * @param length travelled between clock ticks
     */
    void onTimer(qreal speed);

    // departure time
    int dep_hours;
    int dep_minutes;
    int dep_seconds;

    // time now
    int hours;
    int minutes;
    int seconds;

protected:
    /**
     * @brief when vehicle is hovered its route is highlited and timeschedule is shown
     * @param event event info
     */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief unhighlites the vehicle route and hides timeschedule
     * @param event event info
     */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    int onStreet{0};  // index to stations vector, 0 -> first station etc
    qreal lenFromStation;   // length fromm station to the end of the street (taking in street direction p1 -> p2)
    qreal lenToStation;     // length from the start of the street to the station (taking in street direction p1 -> p2)
    qreal lenTravelled{0};     // lenght travelled of vehicle (resetted to 0 when vehicle is on the end of the street or in the station)
    qreal lenMaxTravelled{0};   // debugging purposes

    bool toOrFromStation{true}; // starting from station to the end of the street

    bool deleteBus{false};  // if true then vehicle is deleted

    // vehicle sizes
    qreal busWidth{15};
    qreal busHeigth{15};

    // timeschedule calculating
    std::vector<double> vehicleOrderVector; // time form then first station to next ones
    bool fromOrToStationOrder{true};
    int onStreetOrder{0};
    double lenBetwStations{0};
    bool stationOrderDone{false};
    qreal routeLenghtOrder; // length of the whole route of this vehicle


signals:

};

#endif // MYBUSELLIPSE_H
