#ifndef VIDEOSCENE_HPP
#define VIDEOSCENE_HPP

#include <QGraphicsScene>

class VideoScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit VideoScene(QObject *parent = nullptr);

signals:
    void videoClicked();

public slots:
    void reset();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    QGraphicsRectItem* rect = nullptr;

};

#endif // VIDEOSCENE_HPP
