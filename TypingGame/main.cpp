#include <QtWidgets>
#include "gameview.h"
#include "gamescene.h"
#include "charitem.h"
#include <QSoundEffect>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // 创建 QSoundEffect 对象
    QSoundEffect soundEffect;

    // 设置音频文件路径
    soundEffect.setSource(QUrl("qrc:/audio/bgm.wav"));
    soundEffect.setVolume(1.0);
    // 设置循环播放
    soundEffect.setLoopCount(QSoundEffect::Infinite);
    soundEffect.play();


    GameScene scene;
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    GameView view(&scene);
    view.show();

    return app.exec();
}
