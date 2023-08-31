#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

#include "VideoScene.hpp"

VideoScene::VideoScene(QObject *parent)
    : QGraphicsScene{parent}
{
}

void VideoScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton) {
        return;
    } else {
        emit videoClicked();
    }

    update();
}

void VideoScene::reset()
{
    float scale = 0.5;
    if (!rect) {
        rect = addRect(QRectF(0, 0, 1280 * scale, 720 * scale), QPen(), QBrush(Qt::black));
    }
    rect->setVisible(true);
    rect->setZValue(100);

    update();
}
