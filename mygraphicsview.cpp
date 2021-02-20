#include "mygraphicsview.h"
#include <QDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
    ,mState(S_None)
    ,mType(T_None)
{
    mSizeDefault = QSize(500,400);
    mSizeChange = QSize(mSizeDefault);
}

MyGraphicsView::~MyGraphicsView()
{

}

void MyGraphicsView::wheelEvent(QWheelEvent * event)
{
    if (event->delta() > 0){
        zoomIn();
    }
    else{
        zoomOut();
    }
}

void MyGraphicsView::scaleImage(float factor)
{
    //无场景则不缩放
    if(!this->scene())
        return;
    //累计放缩因子
    mScaleFactor *= factor;
    //构造放缩矩阵
    QMatrix matrix;
    matrix.scale(mScaleFactor, mScaleFactor);
    //QGraphicsView执行放缩
    this->setMatrix(matrix);
//    this->scale(mScaleFactor,mScaleFactor);
    mSizeChange = mSizeChange*factor;
//    qDebug() << "mSize:" << mSizeChange;
}

void MyGraphicsView::zoomIn()
{
    scaleImage(1.2f);
}

void MyGraphicsView::zoomOut()
{
    scaleImage(0.8f);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        if(mState == S_Begin){
            mStartPoint = event->pos();
            //将QGraphicsView中的坐标转换为QGraphicsScene中的坐标
            mStartPoint = QPointF(mapToScene(mStartPoint)).toPoint();
            emit drawImage(mStartPoint,mStartPoint,mType,mState);
            mState = S_Moving;

        }
        else if(mState == S_Moving){
            mEndPoint = event->pos();
            //将QGraphicsView中的坐标转换为QGraphicsScene中的坐标
            mEndPoint = QPointF(mapToScene(mEndPoint)).toPoint();
            //将鼠标点击的坐标信息发给图表显示
            switch (mType) {
            case T_Point:
                emit drawImage(mStartPoint,mEndPoint,mType,mState);
                break;
            case T_Line:
                mState = S_End;
                emit drawImage(mStartPoint,mEndPoint,mType,mState);
                break;
            case T_Rect:
                mState = S_End;
                emit drawImage(mStartPoint,mEndPoint,mType,mState);
                break;
            default:
                break;
            }
        }
    }
    else if(event->button() == Qt::RightButton){

    }
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDoubleClick = true;
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (!isDoubleClick)
        {
            mEndPoint = event->pos();
        }
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!isDoubleClick)
        {
            mEndPoint = event->pos();
            isDrawing = false;
        }
    }
}

