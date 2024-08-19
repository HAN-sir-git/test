#include "calculateboard_dxf.h"
#include "qdebug.h"

QPolygonF calcuBoard(const PointData &data)
{
    QPolygonF board;
    QPointF min(data.X, data.Y);
    QPointF max(data.X, data.Y);
    board.push_back(min);
    board.push_back(max);
    return board;
}

QPolygonF calcuBoard(const LineData &data)
{
    QPolygonF board;
    QPointF min( std::min(data.startpointX ,data.endpointX), std::min(data.startpointY, data.endpointY));
    QPointF max( std::max(data.startpointX ,data.endpointX), std::max(data.startpointY, data.endpointY));
    board.push_back(min);
    board.push_back(max);
    return board;
}

QPolygonF calculatorBoard(const vector<void *> &list)
{
    QPolygonF board;
    double min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    for(int i=0; i<list.size(); i++)
    {
        int flag = *(int *)list[i];
        board.clear();
        switch(flag)
        {
            case 0:
            {
                PointData *point = static_cast<PointData*>(list[i]);
                board << calcuBoard(*point);
                break;
            }
            case 1:
            {
                LineData *line = static_cast< LineData*>(list[i]);
                board << calcuBoard(*line);
                break;
            }
            default:
                qDebug() <<"图形图元不能处理";
                break;
        }

        if(board.size() == 2)
        {
            if(i == 0)
            {
                min_x = board[0].x();
                min_y = board[0].y();
                max_x = board[1].x();
                max_y = board[1].y();
            }
            min_x = std::min(board[0].x(), min_x);
            min_y = std::min(board[0].y(), min_y);
            max_x = std::max(board[1].x(), max_x);
            max_y = std::max(board[1].y(), max_y);
        }
        else
        {
            qDebug()<<"计算边界出现错误！";
        }
    }
    board.clear();
    board.push_back(QPointF(min_x, min_y));//解析出的dxf图形集中最小的x,y坐标
    board.push_back(QPointF(max_x, max_y));//解析出的dxf图形集中最大的x,y坐标
    return board;
}
