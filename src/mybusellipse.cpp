/**
 * @brief   C++ file containing definition of MyBusEllipse class, which instances represent vehicles on route
 * @file    mybusellipse.cpp
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *  This class is doing most of the calculation of the vehicle possition etc., others objects are feeding this class with information
 */

#include "mybusellipse.h"

#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QEvent>

#include "mainwindow.h"
#include "mystreetline.h"

MyBusEllipse::MyBusEllipse(QGraphicsItem *parent) : QObject(), QGraphicsEllipseItem(parent)
{
    setAcceptHoverEvents(true);
    setPen(QPen(Qt::black, 1));
    setBrush(QBrush(Qt::green));
}


void MyBusEllipse::onTimer(qreal speed)    // symulating of vehicle movement
{
    // counting lengts travelled since last clock tick

    lenMaxTravelled += speed;
    //lenTravelled += speed;  // adding length travelled during clock tick

    MyStreetLine *activeStreet;
    for (auto* item : this->map->items()) // all objects in map scene
    {
        if (auto street = dynamic_cast<MyStreetLine*>(item); street)
        {
            if (street->stations.contains(QString(stations[onStreet]))) // finding which street is next station on
            {
                activeStreet = street;
                lenFromStation = (1 - street->stations.value(stations[onStreet])) * street->line().length();
                lenToStation = street->line().length() - lenFromStation;
                if (street->speedOnStreet != 0)
                {
                    lenTravelled += speed * street->speedOnStreet;
                }
                else    // street is completely blocked
                {
                    lenTravelled += 0;
                }
            }
        }
    }

    if (directions[onStreet])   // TRUE vehicle follows street direction p1 -> p2
    {
        qreal newPossition;
        if (toOrFromStation == true)    // vehicle goes from station to the end of the street
        {
            if (lenTravelled <= lenFromStation)
            {
                // calculating new possition of the vehicle
                newPossition = activeStreet->stations.value(stations[onStreet]) + ((1 - activeStreet->stations.value(stations[onStreet])) / (lenFromStation / lenTravelled));
                // changing to new possition
                setRect(activeStreet->line().pointAt(newPossition).x(), activeStreet->line().pointAt(newPossition).y(),busWidth,busHeigth);
            }
            else    // end of street reached
            {
                lenTravelled -= lenFromStation;
                onStreet++; // moving to next street
                if (onStreet >= stations.length())  // all the route is travelled throught
                {
                    deleteBus = true;   // vehicle can be deleted
                }
                toOrFromStation = false;    // now we go from the beginning of the street to the station
                this->onTimer(0);   // just refreshing its possition to moving any further
            }
        }
        else    // from the street beginning to the station (in street direction)
        {
            if (lenTravelled <= lenToStation)
            {
                // calculating new possition of the vehicle
                newPossition = activeStreet->stations.value(stations[onStreet]) / (lenToStation / lenTravelled);
                // changing to new possition
                setRect(activeStreet->line().pointAt(newPossition).x(), activeStreet->line().pointAt(newPossition).y(),busWidth,busHeigth);
            }
            else
            {
                if (onStreet == (stations.length() - 1))   // vehicle is in the last station
                {
                    deleteBus = true;   // vehicle can be deleted
                }
                lenTravelled -= lenToStation;
                toOrFromStation = true; // now we go from the station to the end of the street
                this->onTimer(0);   // just refreshing its possition to moving any further
            }
        }
    }
    else    // FALSE vehicle direction on the street is p2 -> p1
    {
        qreal newPossition;
        if (toOrFromStation == true)    // from station to the end of the street but with p2 -> p1 direction
        {
            if (lenTravelled <= lenToStation)
            {
                // calculating new possition of the vehicle
                newPossition = activeStreet->stations.value(stations[onStreet]) - (activeStreet->stations.value(stations[onStreet]) / (lenToStation / lenTravelled));
                // changing to new possition
                setRect(activeStreet->line().pointAt(newPossition).x(), activeStreet->line().pointAt(newPossition).y(),busWidth,busHeigth);
            }
            else    // end of the street
            {
                lenTravelled -= lenToStation;
                onStreet++;
                if (onStreet >= stations.length())  // end of the route, last station
                {
                    deleteBus = true;
                }
                toOrFromStation = false;
                this->onTimer(0);
            }
        }
        else    // from the beginning of the street to the station but with p2 -> p1 direction
        {
            if (lenTravelled <= lenFromStation)
            {
                // calculating new possition of the vehicle
                newPossition = 1 - ((1 - activeStreet->stations.value(stations[onStreet])) / (lenFromStation / lenTravelled));
                // changing to new possition
                setRect(activeStreet->line().pointAt(newPossition).x(), activeStreet->line().pointAt(newPossition).y(),busWidth,busHeigth);
            }
            else
            {
                if (onStreet == (stations.length() - 1))   // vehicle is in the last station
                {
                    deleteBus = true;
                }
                lenTravelled -= lenFromStation;
                toOrFromStation = true;
                this->onTimer(0);
            }
        }
    }

    if (deleteBus and speed != 0)  // vehicle is in the last station, it will be deleted
    {
        delete this;
    }
    //update();   // vehicle update, pada to tu
}



void MyBusEllipse::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    for (auto* item : this->map->items()) // all the objects in the map scene
    {
        if (auto street = dynamic_cast<MyStreetLine*>(item); street)
        {
            for (auto i : stations)  // all the stations of the vehicle
            {
                if (street->stations.contains(i))   // street has station of this vehicle
                {
                    street->setPen(QPen(Qt::blue, 4));  // highliting of the street in the route
                    street->update();
                }
            }
        }
    }



    if (!stationOrderDone)  // i need to count this just once
    {
        // vehicle ride order
        vehicleOrderVector.push_back(0);
        MyStreetLine *activeStreet;
        while (onStreetOrder < stations.length())
        {
            for (auto* item : this->map->items()) // all the objects in the map scene
            {
                if (auto street = dynamic_cast<MyStreetLine*>(item); street)
                {
                    if (street->stations.contains(stations[onStreetOrder]))
                    {
                        activeStreet = street;
                    }
                }
            }

            if (fromOrToStationOrder == true)   // ze zastávky do konce ulice
            {
                if (directions[onStreetOrder] == true) // po směru
                {
                    lenBetwStations += activeStreet->line().length() * (1 - activeStreet->stations.value(stations[onStreetOrder]));
                }
                else    // pro směru
                {
                    lenBetwStations += activeStreet->line().length() * activeStreet->stations.value(stations[onStreetOrder]);
                }
                fromOrToStationOrder = false;
                onStreetOrder++;
            }
            else   // ze začátku ulice do zastávky
            {
                if (directions[onStreetOrder] == true) // po směru
                {
                    lenBetwStations += activeStreet->line().length() * activeStreet->stations.value(stations[onStreetOrder]);
                }
                else    // pro směru
                {
                    lenBetwStations += activeStreet->line().length() * (1 - activeStreet->stations.value(stations[onStreetOrder]));
                }
                fromOrToStationOrder = true;
                vehicleOrderVector.push_back(lenBetwStations);
                // for order
                routeLenghtOrder = lenBetwStations;
            }
        }
        stationOrderDone = true;
        for (unsigned long i=0; i < vehicleOrderVector.size(); i++) // recounting to time from departure time
        {
            vehicleOrderVector[i] = (dep_hours*3600+dep_minutes*60+dep_seconds) + vehicleOrderVector[i]/5;
        }
    }
    // showing vehicle order
    map->stationOrderShow(vehicleOrderVector, stations, name, routeLenghtOrder, lenMaxTravelled);
}

void MyBusEllipse::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    for (auto* item : this->map->items()) // all the objects in the map scene
    {
        if (auto street = dynamic_cast<MyStreetLine*>(item); street)
        {
            for (auto name : stations)  // all the stations of the vehicle
            {
                if (street->stations.contains(name))   // street has station of this vehicle
                {
                    street->setPen(QPen(Qt::red, 2));  //  UN highliting of the street in the route
                    street->update();
                }
            }
        }
    }
    map->stationOrderHide();    // hiding vehicle order
}

