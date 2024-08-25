#ifndef CSCENETOIMAGE_H
#define CSCENETOIMAGE_H

#include <QObject>
#include <QGraphicsScene>

class CScenetoImage: public QObject
{
    Q_OBJECT
public:
    CScenetoImage(QObject* parent = nullptr);
    ~CScenetoImage(){};

    void saveSceneToImage(QGraphicsScene *scene, const QString &filePath);

    // 导出为一张图片（需要注意导出图片的大小）
    void saveSceneExample(QGraphicsScene *scene, QWidget *parentWidget);

    void exportSceneInChunks(QGraphicsScene *scene, const QString &baseFilePath, const QSize &chunkSize);

    // 导出位多张切割后的图片
    void exportSceneExample(QGraphicsScene *scene, QWidget *parentWidget);



};

#endif // CSCENETOIMAGE_H
