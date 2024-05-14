#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include "timer.h"
#include "charitem.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    // 构造函数，创建游戏场景对象
    GameScene(QObject *parent = nullptr);

    // 析构函数，释放游戏场景对象
    ~GameScene();

public slots:
    //爆炸特效函数
    void showExplosionEffect(const QPoint &position);
    // 游戏开始槽函数，处理游戏开始逻辑
    void gameStart();

    // 游戏升级槽函数，处理游戏升级逻辑
    void gameLevelUp();

    // 游戏结束槽函数，处理游戏结束逻辑
    void gameOver();

    // 游戏暂停槽函数，处理游戏暂停逻辑
    void gamePause();

    // 添加新字符项槽函数，用于在游戏中添加新的字符项
    void addNewCharItem();

    // 更新分数槽函数，用于更新游戏分数显示
    void updateScore();

    // 游戏进展函数，处理游戏的每一帧逻辑
    void advance();

protected:
    // 键盘按下事件处理函数，处理用户按键输入
    void keyPressEvent(QKeyEvent *keyEvent);
private:
    // 场景默认大小
    QSize size;
    // 分数显示
    QGraphicsSimpleTextItem scoreItem;
    // 关卡显示
    QGraphicsSimpleTextItem levelItem;
    // 提示
    QGraphicsSimpleTextItem tipItem;

    // 字母Item表
    QList<CharItem *> charItemList;//这里注意是类指针类型的数组，迭代器返回还有用->来调用成员

    // 落字母定时器
    Timer timerDown;
    // 刷新界面定时器
    Timer timerUpdate;

    // 游戏标识，0标识未开始，1标识进行中，2表示过关，-1表示游戏已结束
    int flag;
    // 关卡
    uint level;
    // 分数
    int score;
    // 失误数
    uint miss;

    // 关卡落字母数
    int charNum;
    // 当前关卡已出字母数
    int charNumAppeared;
    // 落字母间隔时间(ms)
    int timeDown;
    // 刷新界面的间隔时间(ms)
    int timeUpdate;
    // 落子速度
    qreal itemSpeed;
};

#endif // GAMESCENE_H
