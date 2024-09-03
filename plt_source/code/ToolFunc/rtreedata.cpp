#include "rtreedata.h"

bool RTreeData::clearSLine()
{
    sLines_.RemoveAll();
    return true;
}

bool RTreeData::clearVLine()
{
    vLines_.RemoveAll();
    return true;
}

bool RTreeData::clearHLine()
{
    hLines_.RemoveAll();
    return true;
}

bool RTreeData::clearLine()
{
    sLines_.RemoveAll();
    vLines_.RemoveAll();
    hLines_.RemoveAll();
    return true;
}

bool RTreeData::clearText()
{
    texts_.RemoveAll();
    return true;
}

bool RTreeData::clearCircle()
{
    circles_.RemoveAll();
    return true;
}

bool RTreeData::clearSBackIndex()
{
    sBackIndex_.clear();
    return true;
}

bool RTreeData::clearHBackIndex()
{
    hBackIndex_.clear();
    return true;
}

bool RTreeData::clearVBackIndex()
{
    vBackIndex_.clear();
    return true;
}

bool RTreeData::clearBackIndex()
{
    sBackIndex_.clear();
    hBackIndex_.clear();
    vBackIndex_.clear();
    return true;
}

bool RTreeData::clearTextBackIndex()
{
    textBackIndex_.clear();
    return true;
}

bool RTreeData::clearCircleBackIndex()
{
    circleBackIndex_.clear();
    return true;
}

bool RTreeData::insertSLines(const double min[], const double max[], const int id)
{
    sLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertHLines(const double min[], const double max[], const int id)
{
    hLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertVLines(const double min[], const double max[], const int id)
{
    vLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertTexts(const double min[], const double max[], const int id)
{
    texts_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertCircles(const double min[], const double max[], const int id)
{
    circles_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertSLinesQt(const QRectF &rect, const int id)
{
    box = box.united(rect);
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    sLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertHLinesQt(const QRectF &rect, const int id)
{
    box = box.united(rect);
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    hLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertVLinesQt(const QRectF &rect, const int id)
{
    box = box.united(rect);
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    vLines_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertTextsQt(const QRectF &rect, const int id)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    texts_.Insert(min, max, id);
    return true;
}

bool RTreeData::insertCirclesQt(const QRectF &rect, const int id)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    circles_.Insert(min, max, id);
    return true;
}

bool RTreeData::pushBackSBackIndex(int idx)
{
    sBackIndex_.push_back(idx);
    return true;
}

bool RTreeData::pushBackHBackIndex(int idx)
{
    hBackIndex_.push_back(idx);
    return true;
}

bool RTreeData::pushBackVBackIndex(int idx)
{
    vBackIndex_.push_back(idx);
    return true;
}

bool RTreeData::pushBackTextBackIndex(int idx)
{
    textBackIndex_.push_back(idx);
    return true;
}

bool RTreeData::pushBackCircleBackIndex(int idx)
{
    circleBackIndex_.push_back(idx);
    return true;
}

const RType& RTreeData::sLines() const
{
    return sLines_;
}

const RType& RTreeData::hLines() const
{
    return hLines_;
}

const RType& RTreeData::vLines() const
{
    return vLines_;
}

const RType &RTreeData::texts() const
{
    return texts_;
}

const RType &RTreeData::circles() const
{
    return circles_;
}

const std::vector<int>& RTreeData::getSlashIndex() const
{
    return sBackIndex_;
}

const std::vector<int>& RTreeData::getHorizontalIndex() const
{
    return hBackIndex_;
}

const std::vector<int>& RTreeData::getVerticalIndex() const
{
    return vBackIndex_;
}

const std::vector<int> &RTreeData::getTextIndex() const
{
    return textBackIndex_;
}

const std::vector<int> &RTreeData::getCircleIndex() const
{
    return circleBackIndex_;
}

void RTreeData::getSlashLineIdx(std::vector<int>& slashLineIdxVec, const double min[2], const double max[2])
{
    sLines_.Search(min, max, [this](const int idx) { return pushBackSBackIndex(idx); });
    slashLineIdxVec.insert(slashLineIdxVec.end(), sBackIndex_.begin(), sBackIndex_.end());
    sBackIndex_.clear(); // clean results
}

void RTreeData::getHorizontalLineIdx(std::vector<int>& hLineIdxVec, const double min[2], const double max[2])
{
    hLines_.Search(min, max, [this](const int idx) { return pushBackHBackIndex(idx); });
    hLineIdxVec.insert(hLineIdxVec.end(), hBackIndex_.begin(), hBackIndex_.end());
    hBackIndex_.clear(); // clean results
}

void RTreeData::getVerticalLineIdx(std::vector<int>& vLineIdxVec, const double min[2], const double max[2])
{
    vLines_.Search(min, max, [this](const int idx) { return pushBackVBackIndex(idx); });
    vLineIdxVec.insert(vLineIdxVec.end(), vBackIndex_.begin(), vBackIndex_.end());
    vBackIndex_.clear(); // clean results
}

void RTreeData::getLineIdx(std::vector<int>& lineIdxVec, const double min[2], const double max[2])
{
    getSlashLineIdx(lineIdxVec, min, max);
    getHorizontalLineIdx(lineIdxVec, min, max);
    getVerticalLineIdx(lineIdxVec, min, max);
}

void RTreeData::getTextIdx(std::vector<int> &textIdxVec, const double min[], const double max[])
{
    texts_.Search(min, max, [this](const int idx) { return pushBackTextBackIndex(idx); });
    textIdxVec.insert(textIdxVec.end(), textBackIndex_.begin(), textBackIndex_.end());
    textBackIndex_.clear(); // clean results
}

void RTreeData::getCircleIdx(std::vector<int> &circleIdxVec, const double min[], const double max[])
{
    circles_.Search(min, max, [this](const int idx) { return pushBackCircleBackIndex(idx); });
    circleIdxVec.insert(circleIdxVec.end(), circleBackIndex_.begin(), circleBackIndex_.end());
    circleBackIndex_.clear(); // clean results
}

void RTreeData::getSlashLineIdxQt(std::vector<int>& slashLineIdxVec, const QRectF& rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getSlashLineIdx(slashLineIdxVec, min, max);
}

void RTreeData::getHorizontalLineIdxQt(std::vector<int>& hLineIdxVec, const QRectF& rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getHorizontalLineIdx(hLineIdxVec, min, max);
}

void RTreeData::getVerticalLineIdxQt(std::vector<int>& vLineIdxVec, const QRectF& rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getVerticalLineIdx(vLineIdxVec, min, max);
}

void RTreeData::getLineIdxQt(std::vector<int>& lineIdxVec, const QRectF& rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getLineIdx(lineIdxVec, min, max);
}

void RTreeData::getTextIdxQt(std::vector<int> &textIdxVec, const QRectF &rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getTextIdx(textIdxVec, min, max);
}

void RTreeData::getCircleIdxQt(std::vector<int> &circleIdxVec, const QRectF &rect)
{
    double min[2] = { rect.left(),  rect.top()};
    double max[2] = { rect.right(), rect.bottom() };
    getCircleIdx(circleIdxVec, min, max);
}

RTreeData& RTreeData::operator=(const RTreeData& rtree)
{
    sLines_ = rtree.sLines_;
    hLines_ = rtree.hLines_;
    vLines_ = rtree.vLines_;
    sBackIndex_ = rtree.sBackIndex_;
    hBackIndex_ = rtree.hBackIndex_;
    vBackIndex_ = rtree.vBackIndex_;

    texts_ = rtree.texts_;
    textBackIndex_ = rtree.textBackIndex_;

    circles_ = rtree.circles_;
    circleBackIndex_ = rtree.circleBackIndex_;

    box = rtree.box;

    return *this;
}
