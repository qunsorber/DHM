#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtDataVisualization>

#include <Qtcharts>


#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "TransDHMDll.h"
#include <string>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QImage>
#include <QThread>
#include <QMutex>
#include "paramset.h"
#include "mygraphicsscene.h"
#include "mygraphicsview.h"

#pragma comment(lib,"D:\\QtProjects\\20210120\\camera3d\\TransDHMDll.lib")

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

QT_CHARTS_USE_NAMESPACE

using namespace QtDataVisualization;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
signals:
    void captureFinish();

private slots:
    void initHologramData();
    void initPhaseData();
    void displayHologram();
    void displayPhase();
    void display2Dphase();
    void init3Ddata();
    void display3Dphase();
    void getRectData();
    void getLineData();
    void initChart();
    void onDisplayChart(QPoint beginPos,QPoint nextPos,int type,int state);
    void cleanAllDisplay();
//    void zoomIn();
//    void zoomOut();
//    bool eventFilter(QObject* watched, QEvent* event);
    void getParam();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
    cv::Mat srcImg,grayImg;
    QTimer *timerHologram;
    QTimer *timerPhase;
    QTimer *timerChart;
    //摄像头
//    CInstantCamera camera;
//    CGrabResultPtr ptrGrabResult;
    unsigned char *pImageBuffer;
    //相位计算
    unsigned char *hologram;
    double *phasedata;
    unsigned char *phase42ddisplay;
    std::deque<double*> deqPhase;
    QMutex mtx;
    int inWidth;
    int inHeight;
    int outWidth;
    int outHeight;
    float m_max,m_min;
    MyGraphicsScene *myHoloScene;
    //相位显示
    MyGraphicsScene *myPhaseScene;
    //3D图表
private:
    QWidget *graphContariner;
    Q3DSurface *graph3D;
    QSurface3DSeries *seriers;
    QSurfaceDataProxy *proxy;
    int mGraphNum=0;
    QSize mSizeDefault;//默认的图片缩放尺寸
    QSize mSizeChange;//变化的图片缩放尺寸
    QRect mSelectRect;
    QRect mCropRect;
    bool mCalibration;
    int mRectClick = -1;
    //2D折线
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QLine mLine;
    int mLineClick = 0;//二维折线初始化的控制变量
    int mLineNum = 0;
    int mRectNum = 0;
    QPoint pStart;
    QPoint pEnd;
    //放大缩小
    float mScaleFactor = 1.0;
    //画线画框
//    bool mouseIsdown = false;
//    QGraphicsLineItem *lineItem;
//    QGraphicsRectItem *rectItem;
    //参数设置
    ParamSet *pSet;

public:

    void initGraph3D();
    void createColorBar(float,float);
    void systemCalibration();
    QPoint mapToImage(QPoint pSrc);//将场景Scene中的坐标转换到场景中图像的索引坐标
//    void scaleImage(float factor);
public:
//    void mousePressEvent(QMouseEvent *ev);

};

#endif // MAINWINDOW_H
