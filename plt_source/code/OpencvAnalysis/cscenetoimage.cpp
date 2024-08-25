#include "cscenetoimage.h"
#include <QPainter>
#include <QFileDialog>
#include <qmath.h>
#include <QGraphicsView>
CScenetoImage::CScenetoImage(QObject *parent)
{

}

void CScenetoImage::saveSceneToImage(QGraphicsScene *scene, const QString &filePath) {
    // 创建一个QImage对象，大小与场景相同
    QRectF sceneRect = scene->sceneRect();
    QSize imageSize(sceneRect.width(), sceneRect.height());
    QImage* image = new QImage(imageSize, QImage::Format_Grayscale8);
    image->fill(Qt::white); // 可选：设置背景色

    // 创建一个QPainter对象用于绘制
    QPainter painter(image);

    // 将QGraphicsScene内容渲染到QImage
    scene->render(&painter);

    // 保存图片到文件
    image->save(filePath);
}

void CScenetoImage::saveSceneExample(QGraphicsScene *scene, QWidget *parentWidget) {
    QString filePath = QFileDialog::getSaveFileName(parentWidget, "Save Image", "", "Images (*.jpg)");
    if (!filePath.isEmpty()) {
        saveSceneToImage(scene, filePath);
    }
}

void CScenetoImage::exportSceneInChunks(QGraphicsScene *scene, const QString &baseFilePath, const QSize &chunkSize) {
    QRectF sceneRect = scene->sceneRect();
    int chunksX = qCeil(sceneRect.width() / chunkSize.width());
    int chunksY = qCeil(sceneRect.height() / chunkSize.height());
    QRect sschunkRect(
        0,
        0,
        chunkSize.width(),
        chunkSize.height()
        );


    for (int x = 0; x < chunksX; ++x) {
        for (int y = 0; y < chunksY; ++y) {
            QRect chunkRect(
                x * chunkSize.width(),
                y * chunkSize.height(),
                chunkSize.width(),
                chunkSize.height()
                );

            // 创建QImage对象
            QSize imageSize(chunkSize);

            QImage* image = new QImage(imageSize, QImage::Format_Grayscale8);
            image->fill(Qt::white); // 可选：设置背景色

            // 创建一个QPainter对象用于绘制
            QPainter painter(image);


            // 渲染场景的一部分到QImage
            QGraphicsView view(scene);
            view.setSceneRect(chunkRect);
            view.render(&painter, sschunkRect, chunkRect);

            // 保存每个分块的图像
            QString filePath = baseFilePath + QString("_%1_%2.png").arg(x).arg(y);
            image->save(filePath);

        }
    }
}

void CScenetoImage::exportSceneExample(QGraphicsScene *scene, QWidget *parentWidget) {
    QString baseFilePath = QFileDialog::getSaveFileName(parentWidget, "Save Image Base Path", "", "Images (*.png)");
    if (baseFilePath.isEmpty()) return;

    QSize chunkSize(5000, 5000); // 定义每个分块的大小

    exportSceneInChunks(scene, baseFilePath, chunkSize);
}
