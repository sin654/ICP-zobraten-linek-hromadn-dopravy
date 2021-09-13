#ifndef MYMAPSCENE_H
#define MYMAPSCENE_H

#include <myorderline.h>

/**
 * @brief   This file is a header file for mymapscene.cpp
 * @file    mymapscene.h
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include <QGraphicsScene>
#include <QObject>

/**
 * @brief The MyMapScene class, which is a class of the map of the streets
 */
class MyMapScene : public QGraphicsScene
{
    Q_OBJECT
public:


    MyOrderLine *orderLine; // timeschedule line


    explicit MyMapScene(QObject *parent = nullptr);
    /**
     * @brief reaction to application clock timeout() signal, symulating moving of the vehicles on the scene
     * @param hours
     * @param minutes
     * @param seconds
     */
    void clockEvent(int hours, int minutes, int seconds);

    /**
     * @brief reaction to changing application clock time to default 0:0:0 (deleting all the vecicles on the road)
     */
    void timeResetEvent();

    /**
     * @brief changes speed of the selected street
     * @param newSpeed
     */
    void onStreetSpeedChange(int newSpeed);

    /**
     * @brief called when you exit application
     */
    void deleteMapExit();

    /**
     * @brief shows timetable of the hovered vehicle
     * @param order vector of time to get to each station of the route
     * @param stas vector of stations
     * @param name name of the vehicle
     * @param routeLength
     * @param lenTravelled length already travelled
     */
    void stationOrderShow(std::vector<double> order, QVector<QString> stas, QString name, qreal routeLength, qreal lenTravelled);
    /**
     * @brief hides the timetable
     */
    void stationOrderHide();

private:
    /**
     * @brief reads .json files with streets and routes+vehicles and inites the scene with these information
     */
    void initMapScene();

    qreal speed{5}; // speed of pixel/(application clock timeout())
    QVector<QString> tmpStations;   // stores stations order in route
    std::vector<bool> tmpDirections;    // stores direction which vehicle comes to the street (true ... p1 -> p2) (false ... p2 -> p1)

    // min and max coordinates of the streets on the map
    double minX{0};
    double maxX{0};
    double minY{0};
    double maxY{0};

protected:
    /**
     * @brief unselects all the items in the scene and if street was clicked then it is selected (changed color etc.)
     * @param event event info
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:

private slots:

};

#endif // MYMAPSCENE_H
