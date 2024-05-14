#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

/**
 * @brief GameView 类表示用于处理鼠标事件的自定义 QGraphicsView。
 */
class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief 构造一个带有父窗口的 GameView 对象。
     * @param parent 父窗口。
     */
    GameView(QWidget *parent = nullptr);

    /**
     * @brief 构造一个带有 QGraphicsScene 和父窗口的 GameView 对象。
     * @param scene 与视图关联的 QGraphicsScene。
     * @param parent 父窗口。
     */
    GameView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    /**
     * @brief 重写 QGraphicsView 来处理鼠标移动事件。
     * @param mouseEvent 鼠标移动事件。
     */
    void mouseMoveEvent(QMouseEvent *mouseEvent) override;

    /**
     * @brief 重写 QGraphicsView 来处理鼠标按下事件。
     * @param mouseEvent 鼠标按下事件。
     */
    void mousePressEvent(QMouseEvent *mouseEvent) override;

    /**
     * @brief 重写 QGraphicsView 来处理鼠标释放事件。
     * @param mouseEvent 鼠标释放事件。
     */
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;

private:
    bool mouseLeftButtonDown; /**< 标志，指示左鼠标按钮当前是否按下。 */
    QPoint lastPos; /**< 鼠标光标的最后已知位置。 */
};

#endif // GAMEVIEW_H
