#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

class MyGraphicsView: public  QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = 0);
    ~MyGraphicsView();
public:
    enum Type {
        T_None,
        T_Point,
        T_Line,
        T_Circle,
        T_Ellipse,
        T_Rect,
        T_RoundRect,
        T_Polygon,
        T_Bezier
    };
    enum State {
        S_None,
        S_Begin,
        S_Moving,
        S_Press,
        S_End
    };
    void setState(State state){mState = state;}
    void setType(Type type){mType = type;}
public:
    void scaleImage(float);
    void zoomIn();
    void zoomOut();

private:
    float mScaleFactor = 1.0f;
    QSize mSizeDefault;//默认的图片缩放尺寸
    QSize mSizeChange;//变化的图片缩放尺寸

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

signals:
    void drawImage(QPoint beginPos,QPoint nextPos,int type,int state);

private:
    Type mType;
    State mState;
    bool isDrawing;
    bool isDoubleClick;
    QRect mRectDraw;
    QPoint mStartPoint;
    QPoint mMovePoint;
    QPoint mEndPoint;
};

#endif // MYGRAPHICSVIEW_H
