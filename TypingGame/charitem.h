#ifndef CHARITEM_H
#define CHARITEM_H

#include <QGraphicsItem>

class CharItem : public QGraphicsItem
{
public:
    CharItem(QGraphicsItem *parent = nullptr);  // 构造函数，可选参数为父项

    QRectF boundingRect() const override;  // 重写boundingRect函数，返回项的边界矩形
    QPainterPath shape() const override;   // 重写shape函数，返回项的绘制路径
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;  // 重写paint函数，绘制项的外观
    void setSpeed(qreal speed) { this->speed = speed; }  // 设置项的速度
    int getKey() const { return key; }  // 获取项对应的键值
    int getSize() const { return size; }  // 获取项的尺寸

protected:
    void advance(int step) override;  // 重写advance函数，处理项的移动逻辑

private:
    QPixmap backgroundPixmap; // 新的成员变量来存储图像
    int key;  // 项对应的键值
    int size;  // 项的尺寸
    qreal speed;  // 项的移动速度
    QColor colorChar;  // 项字符的颜色
    QColor colorBackground;  // 项背景的颜色
    QColor colorFrame;  // 项边框的颜色
};

#endif
