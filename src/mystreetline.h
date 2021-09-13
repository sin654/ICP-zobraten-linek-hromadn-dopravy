#ifndef MYSTREETLINE_H
#define MYSTREETLINE_H

/**
 * @brief   This file is a header file for mystreetline.cpp
 * @file    mystreetline.h
 * @author  Jan Doležel <xdolez81@stud.fit.vutbr.cz>
 *
 *
 */

#include <QGraphicsLineItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>

/**
 * @brief The MyStreetLine class, which is a class of the street on the map
 */
class MyStreetLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:

    /**
     * @brief class instance constructor
     * @param parent
     */
    explicit MyStreetLine(QGraphicsItem *parent = nullptr);

    QString name;   // name of the street
    QMap<QString,qreal> stations;   // dictionary of stations on the street
    qreal speedOnStreet{1}; // default is 1, when speed on street is changes it can be (0-1)

protected:

    /**
     * @brief overriden virtual fuction to paint street different way than just QGraphicsLineItem (painting stations on the street)
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;



signals:

private slots:




};

#endif // MYSTREETLINE_H
