#include "charitem.h"

#include <QGraphicsScene>
#include <QPainter>

CharItem::CharItem(QGraphicsItem *parent ) :
    QGraphicsItem(parent),
    speed(2.8), // 初始化速度为2.8
    size(48),   // 初始化大小为48
    colorBackground(Qt::yellow), // 设置背景颜色为黄色
    colorChar(Qt::darkGray),     // 设置字符颜色为深灰色
    colorFrame(Qt::black)        // 设置边框颜色为黑色
{
    key = rand() % 26 + 0x41; // 生成一个随机字符，ASCII码范围为0x41到0x5A（A到Z）
}

QRectF CharItem::boundingRect() const
{
    return QRectF(-size/2, -size/2, size, size); // 返回包围盒，中心点为原点，大小为size*size
}

QPainterPath CharItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect()); // 返回一个矩形路径
    return path;
}

void CharItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Frame
    painter->setBrush(colorFrame); // 设置画刷颜色为边框颜色
    painter->drawRoundedRect(boundingRect(), size*0.125, size*0.125); // 绘制圆角矩形边框

    // Background
    painter->setBrush(colorBackground); // 设置画刷颜色为背景颜色
    painter->drawRoundedRect(-size*0.475, -size*0.475, size*0.95, size*0.95, size*0.118, size*0.118); // 绘制圆角矩形背景

    // Text
    QFont font("Arial", size*0.56); // 设置字体为Arial，大小为size*0.56
    painter->setBrush(colorChar);   // 设置画刷颜色为字符颜色
    painter->setFont(font);          // 设置字体
    painter->drawText(boundingRect(), Qt::AlignCenter, QChar(key)); // 在中心位置绘制字符
}

void CharItem::advance(int step)
{
    if (!step)
        return;

    setPos(mapToParent(0, speed)); // 每次步进更新位置
}
