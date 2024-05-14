#include <ctime>
#include "gamescene.h"
#include "qgraphicsproxywidget.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QSoundEffect>
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsScene>

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent),
    size(1024,768),
    flag(0),
    level(0),
    score(0),
    miss(0),
    charNum(50),
    charNumAppeared(0),
    timeUpdate(8),
    itemSpeed(3)
{
    setSceneRect(0, 0, size.width(), size.height());

    // 添加得分显示
    scoreItem.setFont(QFont("Arial", 16, QFont::Normal));
    scoreItem.setBrush(QBrush(Qt::gray));
    scoreItem.setPos(10,8);
    scoreItem.setZValue(1);
    addItem(&scoreItem);
    updateScore();

    // 添加提示
    tipItem.setFont(QFont("Arial", 10, QFont::Light, true));
    tipItem.setBrush(QBrush(Qt::gray));
    tipItem.setZValue(1);
    addItem(&tipItem);

    // 添加关卡显示
    levelItem.setFont(QFont("Arial", 36, QFont::Bold));
    levelItem.setBrush(QBrush(Qt::gray));
    levelItem.setZValue(1);
    addItem(&levelItem);

    // 关联定时器与落字母
    connect(&timerDown, SIGNAL(timeout()), this, SLOT(addNewCharItem()));

    // scene的advance来使子部件动起来
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(advance()));

    // 初始化随机数
    srand(time(0));

    // 启动
    gameLevelUp();
}

GameScene::~GameScene()
{
}

void GameScene::showExplosionEffect(const QPoint &position) {
    // 加载爆炸特效图片
    QPixmap explosionPixmap(":/image/chicken.png");

    // 创建 QLabel 控件用于显示爆炸特效图片
    QLabel *explosionLabel = new QLabel;
    explosionLabel->setPixmap(explosionPixmap);

    // 将 QLabel 控件添加到场景中，并通过 QGraphicsProxyWidget 进行管理
    QGraphicsProxyWidget *proxyWidget = addWidget(explosionLabel);
    // 设置 QLabel 控件在场景中的位置，使其以爆炸位置为中心显示
    proxyWidget->setPos(position - QPoint(explosionPixmap.width() / 2, explosionPixmap.height() / 2));
    // 设置不透明度为0.5
    proxyWidget->setOpacity(0.5);
    // 创建爆炸特效的动画
    QPropertyAnimation *animation = new QPropertyAnimation(proxyWidget);
    // 设置动画持续时间为0.5秒
    animation->setDuration(500);
    // 设置动画起始值为当前控件的几何属性
    animation->setStartValue(proxyWidget->geometry());
    // 设置动画结束值为控件位置不变，但缩小
    animation->setEndValue(QRectF(proxyWidget->pos(), explosionPixmap.size() * 1));
    // 设置动画的缓动曲线，使动画效果更加流畅
    animation->setEasingCurve(QEasingCurve::OutQuad);

    // 连接动画结束信号到槽函数，确保动画结束后删除爆炸特效控件
    connect(animation, &QPropertyAnimation::finished, [=]() {
        delete proxyWidget; // 删除代理控件，同时会删除其管理的 QLabel 控件
    });

    // 启动动画
    animation->start();
}

void GameScene::gameStart()
{
    // 如果游戏已经开始，恢复计时器并返回
    if (flag == 1) {
        timerDown.resume();
        timerUpdate.resume();
        return;
    }

    // 如果游戏是初始状态或结束状态，重新初始化游戏参数
    if (flag == 0 || flag == -1) {
        flag = 1;                               // 设置游戏状态为开始
        level = 1;                              // 设置等级为1
        score = 0;                              // 设置分数为0
        miss = 0;                               // 设置错过数量为0
        itemSpeed = 3;                          // 设置物品速度

        charNum = 60;                           // 设置字符数量
        charNumAppeared = 0;                    // 设置已出现字符数量为0
        timeDown = 60000 / (40 + 20 * level);   // 设置倒计时时间
        itemSpeed = 3 * timeDown;               // 设置物品速度

        // 清空已生成的字符物品列表
        qDeleteAll(charItemList);
        charItemList.clear();
    }
    // 如果游戏处于暂停状态，重新开始游戏
    else if (flag == 2) {
        flag = 1;                               // 设置游戏状态为开始
        qDeleteAll(charItemList);               // 清空已生成的字符物品列表
        charItemList.clear();
    }

    // 启动倒计时和更新计时器
    timerDown.start(timeDown);
    timerUpdate.start(timeUpdate);

    // 隐藏等级显示
    levelItem.hide();

    // 更新分数显示
    updateScore();

    // 显示提示信息
    tipItem.setText("Press Space to pause or resume. Press ESC to quit.");
    tipItem.setPos(10, size.height() - tipItem.boundingRect().height() * 1.25);
    tipItem.show();
}

void GameScene::gameLevelUp()
{
    if (flag != 0 && flag != 1)
        return;

    timerDown.stop();
    timerUpdate.stop();

    flag = 2;
    // 升级，提高难度
    level++;
    // 每关落字母数，1级60个，2级80个，3级100个，以此类推
    charNum = 40 + 20 * level;
    charNumAppeared = 0;
    // 落字母时间间隔
    timeDown = 60000 / charNum;
    // 落下速度
    itemSpeed = 3 * timeDown;

    levelItem.setText(QString("Level  %1").arg(level));
    levelItem.setPos((size.width() - levelItem.boundingRect().width())*0.5,
                     (size.height() - levelItem.boundingRect().height())*0.5);
    levelItem.show();

    tipItem.setText("Press ENTER to start, or ESC to quit.");
    tipItem.setPos(levelItem.x() + (levelItem.boundingRect().width() - tipItem.boundingRect().width()) * 0.5,
                   levelItem.y() + levelItem.boundingRect().height() + tipItem.boundingRect().height() * 0.25);
    tipItem.show();
}

void GameScene::gameOver()
{
    // 停止倒计时和更新计时器
    timerDown.stop();
    timerUpdate.stop();

    // 设置游戏状态为结束
    flag = -1;

    // 显示游戏结束信息和分数
    levelItem.setText(QString("Game Over! As a true man , Your score: %1").arg(score));
    levelItem.setPos((size.width() - levelItem.boundingRect().width()) * 0.5,
                     (size.height() - levelItem.boundingRect().height()) * 0.5);
    levelItem.show();

    // 显示重新开始和退出游戏提示信息
    tipItem.setText("Press ENTER to restart, or ESC to quit.");
    tipItem.setPos(levelItem.x() + (levelItem.boundingRect().width() - tipItem.boundingRect().width()) * 0.5,
                   levelItem.y() + levelItem.boundingRect().height() + tipItem.boundingRect().height() * 0.25);
    tipItem.show();
}

void GameScene::gamePause()
{
    timerDown.pause();
    timerUpdate.pause();
}

void GameScene::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()) {
    // 处理回车键或者Enter键，开始游戏
    case Qt::Key_Return:
    case Qt::Key_Enter:
        gameStart();
        break;

        // 处理空格键，暂停或者恢复游戏
    case Qt::Key_Space:
        if (flag != 1) // 如果游戏未开始，直接返回
            break;
        if (timerUpdate.isActive()) // 如果计时器正在运行，暂停游戏；否则，开始游戏
            gamePause();
        else
            gameStart();
        break;

        // 处理ESC键，退出游戏
    case Qt::Key_Escape:
        QApplication::quit();
        break;

        // 处理其他按键
    default:
        if (timerUpdate.isActive()) { // 如果游戏正在进行
            auto i = charItemList.begin();
            QSoundEffect *soundEffect = new QSoundEffect;
            while (i != charItemList.end()) {
                if (keyEvent->key() == (*i)->getKey()) { // 检查按下的键是否与当前字符匹配
                    delete *i; // 删除匹配的字符
                    charItemList.erase(i); // 从列表中移除字符
                    score++; // 增加分数
                    // 获取场景的右上角位置
                    QPoint rightTopPos = QPoint(this->width()/2 - 1, this->height()/2-1);
                    // 显示爆炸特效
                    showExplosionEffect(rightTopPos);
                    //彩蛋
                    soundEffect->setSource(QUrl("qrc:/audio/gin.wav"));
                    soundEffect->setVolume(1.0);
                    soundEffect->play();
                    updateScore(); // 更新分数显示
                    keyEvent->accept(); // 接受按键事件
                    return;
                }
                else
                    i++;
            }
        }
    }
}

void GameScene::addNewCharItem()
{
    // 检查是否已经出现了足够数量的字符，如果是则停止倒计时
    if (charNumAppeared >= charNum)
        timerDown.stop();
    else {
        // 创建新的字符项并设置其位置
        CharItem *item = new CharItem;
        int x = rand() % (size.width() - item->getSize()) + item->getSize() / 2;
        item->setPos(x, 0);
        charItemList.append(item);
        addItem(item);
        charNumAppeared++;
    }
}

void GameScene::updateScore()
{
    // 更新分数显示
    scoreItem.setText(QString("level : %1   score: %2    miss: %3").arg(level).arg(score).arg(miss));
}

void GameScene::advance()
{
    // 调用父类的advance()函数
    QGraphicsScene::advance();

    // 检查字符项是否超出了屏幕范围，如果是则删除并增加miss计数
    auto i = charItemList.begin();
    QSoundEffect *soundEffect = new QSoundEffect;
    while (i != charItemList.end()) {
        if ((*i)->pos().ry() >= height()) {
            delete *i;
            charItemList.erase(i++);
            miss++;

            //彩蛋
            soundEffect->setSource(QUrl("qrc:/audio/niganma.wav"));
            soundEffect->setVolume(1.0);
            soundEffect->play();


            updateScore();
        }
        else
            i++;
    }

    // 如果miss次数达到10次，游戏结束
    if (miss >= 10) {
        gameOver();
        return;
    }

    // 如果倒计时停止且字符项为空，则过关
    if (!timerDown.isActive() && charItemList.isEmpty()) {
        gameLevelUp();
    }
}
