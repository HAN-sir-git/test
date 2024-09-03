#ifndef RTREEDATA_H
#define RTREEDATA_H
#include "RTree.h"
#include <QRectF>

using RType = RTree<int, double, 2, double>;

class RTreeData
{
public:
    bool clearSLine();
    bool clearVLine();
    bool clearHLine();
    bool clearLine();
    bool clearText();
    bool clearCircle();

    bool clearSBackIndex();
    bool clearHBackIndex();
    bool clearVBackIndex();
    bool clearBackIndex();
    bool clearTextBackIndex();
    bool clearCircleBackIndex();


    bool insertSLines(const double min[], const double max[], const int id);
    bool insertHLines(const double min[], const double max[], const int id);
    bool insertVLines(const double min[], const double max[], const int id);
    bool insertTexts(const double min[], const double max[], const int id);
    bool insertCircles(const double min[], const double max[], const int id);

    bool insertSLinesQt(const QRectF& rect, const int id);
    bool insertHLinesQt(const QRectF& rect, const int id);
    bool insertVLinesQt(const QRectF& rect, const int id);
    bool insertTextsQt(const QRectF& rect, const int id);
    bool insertCirclesQt(const QRectF& rect, const int id);


    bool pushBackSBackIndex(int idx);
    bool pushBackHBackIndex(int idx);
    bool pushBackVBackIndex(int idx);
    bool pushBackTextBackIndex(int idx);
    bool pushBackCircleBackIndex(int idx);

    const RType& sLines() const;
    const RType& hLines() const;
    const RType& vLines() const;
    const RType& texts() const;
    const RType& circles() const;

    const std::vector<int>& getSlashIndex() const;
    const std::vector<int>& getHorizontalIndex() const;
    const std::vector<int>& getVerticalIndex() const;
    const std::vector<int>& getTextIndex() const;
    const std::vector<int>& getCircleIndex() const;

    // 通用版本
    void getSlashLineIdx(std::vector<int>& slashLineIdxVec, const double min[2], const double max[2]);
    void getHorizontalLineIdx(std::vector<int>& hLineIdxVec, const double min[2], const double max[2]);
    void getVerticalLineIdx(std::vector<int>& vLineIdxVec, const double min[2], const double max[2]);
    void getLineIdx(std::vector<int>& lineIdxVec, const double min[2], const double max[2]);
    void getTextIdx(std::vector<int>& textIdxVec, const double min[2], const double max[2]);
    void getCircleIdx(std::vector<int>& circleIdxVec, const double min[2], const double max[2]);

    // 适合QT调用
    void getSlashLineIdxQt(std::vector<int>& slashLineIdxVec, const QRectF& rect);
    void getHorizontalLineIdxQt(std::vector<int>& hLineIdxVec, const QRectF& rect);
    void getVerticalLineIdxQt(std::vector<int>& vLineIdxVec, const QRectF& rect);
    void getLineIdxQt(std::vector<int>& lineIdxVec, const QRectF& rect);
    void getTextIdxQt(std::vector<int>& textIdxVec, const QRectF& rect);
    void getCircleIdxQt(std::vector<int>& circleIdxVec, const QRectF& rect);

    RTreeData& operator=(const RTreeData& rtree);
private:
    RType sLines_;
    RType vLines_;
    RType hLines_;
    std::vector<int> sBackIndex_;
    std::vector<int> hBackIndex_;
    std::vector<int> vBackIndex_;

    RType texts_;
    std::vector<int> textBackIndex_;

    RType circles_;
    std::vector<int> circleBackIndex_;


    // test var
    QRectF box;
};

#endif // RTREEDATA_H
