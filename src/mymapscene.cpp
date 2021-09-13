/**
 * @brief   C++ file containing class MyMapScene, it is canvas for other graphics items
 * @file    mymapscene.cpp
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include "mymapscene.h"
#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QPen>
#include <QJsonObject>
#include <QJsonValue>
#include "mystreetline.h"
#include "mybusellipse.h"
#include <vector>

QMap<int, MyBusEllipse*> busDictionary; // dictionary with information when each vehicles starts its journey
QString stationsFile = "../examples/stations.json"; // path to .json file with information about routes,stations,vehicles
QString mapFile = "../examples/map.json";   // path to .json file with information about streets

MyMapScene::MyMapScene(QObject *parent) : QGraphicsScene(parent)
{
    initMapScene();
}

void MyMapScene::initMapScene()
{
    // reading JSON file of streets
    QString jsonFile;
    QFile file;
    file.setFileName(mapFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonFile = file.readAll();
    file.close();

    QJsonDocument document =  QJsonDocument::fromJson(jsonFile.toUtf8());
    QJsonObject allJson = document.object();
    QJsonValue streets = allJson.take(QString("streets"));
    QJsonArray streets2 = streets.toArray();

    QJsonObject street;
    MyStreetLine *newStreet{nullptr};
    while (streets2.size() > 0)
    {
        street = streets2.takeAt(0).toObject();
        newStreet = new MyStreetLine;
        newStreet->name = street.value(QString("street_name")).toString();
        // stations setting
        QJsonArray stations = street.take(QString("stations")).toArray();
        QJsonObject station;
        while (stations.size() > 0)     // putting stations to streets
        {
            station = stations.takeAt(0).toObject();
            newStreet->stations.insert(station.value(QString("station_name")).toString(), station.value(QString("location")).toDouble());
        }
        // street p1[x,y] p2[s,y]
        newStreet->setLine(street.value(QString("p1x")).toDouble(), street.value(QString("p1y")).toDouble(), street.value(QString("p2x")).toDouble(), street.value(QString("p2y")).toDouble());
        // counting min,max
        if (street.value(QString("p1x")).toDouble() < minX)
        {
            minX = street.value(QString("p1x")).toDouble();
        }
        if  (street.value(QString("p2x")).toDouble() < minX)
        {
            minX = street.value(QString("p2x")).toDouble();
        }
        if (street.value(QString("p1x")).toDouble() > maxX)
        {
            maxX = street.value(QString("p1x")).toDouble();
        }
        if  (street.value(QString("p2x")).toDouble() > maxX)
        {
            maxX = street.value(QString("p2x")).toDouble();
        }


        if (street.value(QString("p1y")).toDouble() < minY)
        {
            minY = street.value(QString("p1y")).toDouble();
        }
        if  (street.value(QString("p2y")).toDouble() < minY)
        {
            minY = street.value(QString("p2y")).toDouble();
        }
        if (street.value(QString("p1y")).toDouble() > maxY)
        {
            maxY = street.value(QString("p1y")).toDouble();
        }
        if  (street.value(QString("p2y")).toDouble() > maxY)
        {
            maxY = street.value(QString("p2y")).toDouble();
        }


        // adding street to scene
        addItem(newStreet);
    }



    // timeschedule line
    orderLine  = new MyOrderLine;
    orderLine->setLine(maxX + 100, minY, maxX + 100, maxY);
    addItem(orderLine);





    // reading JSON file with routes, stations, vehicles
    file.setFileName(stationsFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonFile = file.readAll();
    file.close();

    document =  QJsonDocument::fromJson(jsonFile.toUtf8());
    allJson = document.object();
    QJsonArray routes = allJson.take(QString("routes")).toArray();

    QJsonObject route;
    while (routes.size() > 0)
    {
        route = routes.takeAt(0).toObject().value("route").toObject();  // one route
        QJsonArray next_stations = route.value("stations").toArray();
        QJsonObject next_station;
        while (next_stations.size() > 0)    // setting stations and directions to route
        {
            next_station = next_stations.takeAt(0).toObject();
            tmpStations.append(next_station.value("station").toString());
            tmpDirections.push_back(next_station.value("direction").toBool());
        }
        QJsonArray busses = route.value("busses").toArray();
        QJsonObject nextBus;
        while (busses.size() > 0)    // vehicles creating
        {
            nextBus = busses.takeAt(0).toObject();

            // constructing vehicle
            MyBusEllipse *newBus = new MyBusEllipse;
            // setting information obout map scene ->map=this
            newBus->map = this;
            // name
            newBus->name = nextBus.value("name").toString();
            // stations a directions assigning
            for (auto item : tmpStations)   // stations
            {
                newBus->stations.append(item);
            }
            for (auto item : tmpDirections)     // directions
            {
                newBus->directions.push_back(item);
            }
            // adding vehicle to vehicle dictionary
            int timeInSec = nextBus.value("departure_hour").toInt()*3600 + nextBus.value("departure_min").toInt()*60 + nextBus.value("departure_sec").toInt();
            busDictionary.insertMulti(timeInSec, newBus);
        }
        // deleting tmpStations a tmpDirections (needs to be fresh for next route)
        tmpStations.clear();
        tmpDirections.clear();
    }
}

void MyMapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto* item : items()) // all the objects in the scene
    {
        if (auto street = dynamic_cast<MyStreetLine*>(item); street)    // streets
        {
            street->setSelected(false); // unselect all the streets
            // default street pen
            street->setPen(QPen(Qt::red, 2));
            street->update();
        }
    }

    for (auto* item : items(event->scenePos())) // all the objects in the scene
    {
        if (auto street = dynamic_cast<MyStreetLine*>(item); street)    // streets
        {
            street->setSelected(true);
            street->setPen(QPen(Qt::green, 3));
        }
    }


    QGraphicsScene::mousePressEvent(event);
}


void MyMapScene::clockEvent(int hours, int minutes, int seconds)
{
    if (hours == 0 and minutes == 0 and seconds == 0)   // we looped throught the whole day
    {
        timeResetEvent();   // restart everything in the scene
    }

    int timeInSeconds = hours*3600 + minutes*60 + seconds;
    // adding vehicles with departure time timeInSeconds to map scene
    QMap<int, MyBusEllipse*>::const_iterator i = busDictionary.find(timeInSeconds);
    while (i != busDictionary.end() && i.key() == timeInSeconds)
    {
        i.value()->dep_hours = hours;
        i.value()->dep_minutes = minutes;
        i.value()->dep_seconds = seconds;
        addItem(i.value());
        ++i;
    }


    for (auto* item : items()) // all the objects in the scene
    {
        if (auto street = dynamic_cast<MyBusEllipse*>(item); street)    // to send request to all vehicles on the journey to symulate its new possition
        {
            street->hours = hours;
            street->minutes = minutes;
            street->seconds = seconds;
            street->onTimer(speed);
        }
    }
    update();   // updating map scene
}

void MyMapScene::timeResetEvent()
{
    for (auto* item : items())  // clearing whole scene
    {
        delete item;
    }
    busDictionary.clear();  // clearing vehicle dictionary
    initMapScene();     // new init of the scene
}

void MyMapScene::onStreetSpeedChange(int newSpeed)
{
    for (auto* item : items())
    {
        if (auto tmpStreet = dynamic_cast<MyStreetLine*>(item); tmpStreet)
        {
            if (tmpStreet->isSelected())   // street is selected
            {
                // changing street speed
                tmpStreet->speedOnStreet = newSpeed / 100.0;
            }
        }
    }
}

void MyMapScene::deleteMapExit()
{
    for (auto* item : items())
    {
        delete item;
    }
}

void MyMapScene::stationOrderShow(std::vector<double> order, QVector<QString> stas, QString name, qreal routeLength, qreal lenTravelled)
{
    // feeding needing information to timeschedule line
    orderLine->stations = stas;
    orderLine->stationTime = order;
    orderLine->name = name;
    orderLine->routeLength = routeLength;
    orderLine->lenTravelled = lenTravelled;
    orderLine->show();
    orderLine->update();
    update();

}

void MyMapScene::stationOrderHide()
{
    orderLine->hide();
    orderLine->update();
    update();
}
