#include <QGraphicsScene>
#include <QMouseEvent>
#include "gameview.h"

GameView::GameView(QWidget *parent) :
    QGraphicsView(parent), // 使用父类构造函数初始化 QGraphicsView
    mouseLeftButtonDown(false) // 初始化鼠标左键按下标志为 false
{
    setObjectName("view"); // 设置对象名为 "view"
    setRenderHint(QPainter::Antialiasing); // 设置渲染提示为抗锯齿
    setStyleSheet("QGraphicsView#view{border-image:url(:/image/bcg.png)}" ); // 设置样式表，设置背景图片
    setCacheMode(QGraphicsView::CacheBackground); // 设置缓存模式为缓存背景
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); // 设置视口更新模式为根据边界矩形更新
    setFixedSize(scene()->width(), scene()->height()); // 设置固定大小为场景大小
    // 无窗口模式
    setWindowFlags(Qt::FramelessWindowHint); // 设置窗口标志为无边框窗口
}

GameView::GameView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent) // 使用父类构造函数初始化 QGraphicsView，并传入场景和父组件
{
    setObjectName("view"); // 设置对象名为 "view"
    setRenderHint(QPainter::Antialiasing); // 设置渲染提示为抗锯齿
    setStyleSheet("QGraphicsView#view{border-image:url(:/image/bcg.png)}" ); // 设置样式表，设置背景图片
    setCacheMode(QGraphicsView::CacheBackground); // 设置缓存模式为缓存背景
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); // 设置视口更新模式为根据边界矩形更新
    setFixedSize(scene->width(), scene->height()); // 设置固定大小为场景大小
    // 无窗口模式
    setWindowFlags(Qt::FramelessWindowHint); // 设置窗口标志为无边框窗口
}
void GameView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    // 如果左键按下，移动视图位置
    if (mouseLeftButtonDown) {
        move(x() + (mouseEvent->globalX() - lastPos.x()),
             y() + (mouseEvent->globalY() - lastPos.y()));
        lastPos = mouseEvent->globalPos();
    }
}

void GameView::mousePressEvent(QMouseEvent *mouseEvent)
{
    // 如果是左键按下，记录当前位置
    if (mouseEvent->button() == Qt::LeftButton) {
        mouseLeftButtonDown = true;
        lastPos = mouseEvent->globalPos();
    }
}

void GameView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    // 如果是左键释放，重置标志位
    if (mouseEvent->button() == Qt::LeftButton)
        mouseLeftButtonDown = false;
}
