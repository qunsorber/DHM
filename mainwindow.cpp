#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //文件操作相关
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
    //全息图的显示，巴斯相机
    ui->actionbasler->setCheckable(true);
    ui->actionbasler->setChecked(false);
    connect(ui->actionbasler,&QAction::toggled,this,&MainWindow::initHologramData);
    //视频文件
    ui->actionVideo->setCheckable(true);
    ui->actionVideo->setChecked(false);
    connect(ui->actionVideo,&QAction::toggled,this,&MainWindow::initHologramData);
    //图片文件
    ui->actionPicture->setCheckable(true);
//    ui->actionPicture->setChecked(false);
    connect(ui->actionPicture,&QAction::triggered,this,&MainWindow::initHologramData);
    //系统校正
    mCalibration = false;
    connect(ui->actionCalib,&QAction::triggered,this,&MainWindow::systemCalibration);
    //相位计算及2D平面显示
    connect(ui->actionPhaseCompute,&QAction::triggered,this,&MainWindow::initPhaseData);
    connect(ui->action2Dpreview,&QAction::triggered,this,&MainWindow::display2Dphase);
    //相位局部3D显示
    connect(ui->actionRect,&QAction::triggered,this,&MainWindow::getRectData);
    connect(ui->action3Dpreview,&QAction::triggered,this,&MainWindow::init3Ddata);
    //二维折线显示
    connect(ui->actionLine,&QAction::triggered,this,&MainWindow::getLineData);

    mSizeDefault = QSize(500,400);
    mSizeChange = QSize(mSizeDefault);
    mCropRect.setTopLeft(QPoint(0,0));
    mCropRect.setBottomRight(QPoint(3840-1,2160-1));
    mLine = QLine(QPoint(0,0),QPoint(0,0));
    chart = new QChart();
    series =new QLineSeries();
    axisX = new QValueAxis;
    axisY = new QValueAxis;

    //工具栏
    QAction *a1 = new QAction(QIcon(":/images/clear.png"),QObject::tr("清除"));
    QAction *a2 = new QAction(QIcon(":/images/control.png"),QObject::tr("暂停"));
    QAction *a3 = new QAction(QIcon(":/images/continue.png"),QObject::tr("继续"));
    QAction *a4 = new QAction(QIcon(":/images/zoomin.png"),QObject::tr("放大"));
    QAction *a5 = new QAction(QIcon(":/images/zoomout.png"),QObject::tr("缩小"));
    ui->mainToolBar->addAction(a1);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(a2);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(a3);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(a4);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(a5);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //工具栏中的功能实现
    connect(a1,&QAction::triggered,this,&MainWindow::cleanAllDisplay);
    connect(a4,&QAction::triggered,this,&MainWindow::zoomIn);
    connect(a5,&QAction::triggered,this,&MainWindow::zoomOut);

    //全息图，相位图实现滚轴放大缩小
    //对QGraphcisView控件注册事件响应
    ui->gViewCompute->installEventFilter(this);
    //使能QGraphcisView控件的鼠标跟踪
    ui->gViewCompute->setMouseTracking(true);

    //相位场景对象
//    myPhaseScene = new QGraphicsScene();
    lineItem = new QGraphicsLineItem();
    rectItem = new QGraphicsRectItem();
    //参数设置
    connect(ui->actionParm,&QAction::triggered,this,&MainWindow::getParam);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete chart;
//    delete seriers;
    delete axisX;
    delete axisY;
//    delete myPhaseScene;
//    delete lineItem;
}

void MainWindow::initHologramData()
{
    if(ui->actionbasler->isChecked()){
//        PylonInitialize();
//        try
//        {
//            camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());
//            cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
//            camera.StartGrabbing( GrabStrategy_LatestImageOnly);
//            timerHologram = new QTimer;
//            timerHologram->setInterval(200);
//            connect(timerHologram,&QTimer::timeout,this,&MainWindow::displayHologram);
//            timerHologram->start();
//        }
//        catch (const GenericException &e)
//        {
//            // Error handling.
//            cerr << "An exception occurred." << endl
//                 << e.GetDescription() << endl;
//        }
    }
    else if(ui->actionVideo->isChecked()){
        cap.open("D:\\QtProjects\\imageData\\test1024.avi");
        timerHologram = new QTimer;
        timerHologram->setInterval(500);
        connect(timerHologram,&QTimer::timeout,this,&MainWindow::displayHologram);
        timerHologram->start();
    }
    else if(ui->actionPicture->isChecked()){
        QString file = QFileDialog::getOpenFileName(this,
                                                    "打开全息图片文件",
                                                    "D:\\QtProjects\\imageData",
                                                    "Images files (*bmp *jpg *png)");
//        cap.open("D:\\QtProjects\\imageData\\181.bmp");
        if(file.isEmpty()) return;
        cap.open(file.toStdString());
        displayHologram();
    }
}

void MainWindow::displayHologram()
{
    if(ui->actionbasler->isChecked()){
//        try
//        {
//            if(camera.IsGrabbing()){
//                camera.RetrieveResult(5000,ptrGrabResult,TimeoutHandling_ThrowException);
//                if(ptrGrabResult->GrabSucceeded()){
//                    //cout << ptrGrabResult->GetWidth() << "--" <<ptrGrabResult->GetHeight()<< endl;
//                    QImage Qtemp = QImage((unsigned char*)ptrGrabResult->GetBuffer(),
//                                          ptrGrabResult->GetWidth(),
//                                          ptrGrabResult->GetHeight(),
//                                          QImage::Format_Indexed8);
//                    //Qtemp = Qtemp.scaled(ui->gViewCamera->width(),ui->gViewCamera->height(),Qt::IgnoreAspectRatio);
//                    QGraphicsScene *myScene = new QGraphicsScene();
//                    //QSize size = ui->gViewCamera->size();
//                    myScene->addPixmap(QPixmap::fromImage(Qtemp).scaled(mSizeDefault));
//                    ui->gViewCamera->setScene(myScene);
//                }
//                else{
//                    qDebug() << "grab failed!";
//                }
//            }
//            else{
//                qDebug() << "no grab!";
//            }

//        }
//        catch (const GenericException &e)
//        {
//            // Error handling.
//            cerr << "An exception occurred." << endl
//                 << e.GetDescription() << endl;
//        }
    }
    //PylonTerminate();
    if(cap.isOpened()){
        cap.read(srcImg);
        if(!srcImg.empty()){
            cvtColor(srcImg,grayImg,CV_BGR2GRAY);
            QImage Qtemp = QImage((unsigned char*)grayImg.data,
                                  grayImg.cols,grayImg.rows,
                                  QImage::Format_Indexed8);
            QGraphicsScene *myScene = new QGraphicsScene();
            myScene->addPixmap(QPixmap::fromImage(Qtemp).scaled(mSizeDefault));
            ui->gViewCamera->setScene(myScene);
            //QThread::msleep(2000);
        }
    }
}

void MainWindow::systemCalibration()
{
//    std::string filename ="D:\\QtProjects\\imageData\\180.bmp";
    QString file = QFileDialog::getOpenFileName(this,
                                                "打开图片文件",
                                                "D:\\QtProjects\\imageData",
                                                "Images files (*bmp *jpg *png)");
    if(file.isEmpty()) return;
    std::string filename = file.toStdString();
    InitTransDHM(&filename[0]);
    inWidth = grayImg.cols;
    inHeight = grayImg.rows;
    outWidth = inWidth;
    outHeight = inHeight;
    SetParameter(outWidth,outHeight);
    SystemCalib(&filename[0]);
    QMessageBox::information(NULL,"注意","完成校正!",QMessageBox::Yes);
    mCalibration = true;
}

void MainWindow::initPhaseData()
{
//    if(ui->actionbasler->isChecked() && ptrGrabResult->GrabSucceeded()){
//        hologram = new unsigned char[inWidth*inHeight];
//        phasedata = new double[outWidth*outHeight];

//        timerPhase = new QTimer;
//        timerPhase->setInterval(500);
//        connect(timerPhase,&QTimer::timeout,this,&MainWindow::displayPhase,Qt::UniqueConnection);
//        timerPhase->start();
//    }
    if(!mCalibration){
        qDebug() << "请先校正!";
        QMessageBox::StandardButton result =  QMessageBox::warning(NULL,"注意","请先校正!",
                                              QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        if (result == QMessageBox::Yes)
            return;
    }
    hologram = new unsigned char[inWidth*inHeight];
    phasedata = new double[outWidth*outHeight];
    if(!grayImg.empty()){
        if(ui->actionVideo->isChecked()){
            timerPhase = new QTimer;
            timerPhase->setInterval(500);
            connect(timerPhase,&QTimer::timeout,this,&MainWindow::displayPhase,Qt::UniqueConnection);
            timerPhase->start();
        }
        else if(ui->actionPicture->isChecked()){
            displayPhase();
        }
    }
}

void MainWindow::displayPhase()
{
//    if(ui->actionbasler->isChecked())
//        memcpy(hologram,ptrGrabResult->GetBuffer(),inWidth*inHeight);
//    else
        hologram  = grayImg.data;
    HoloPHase(hologram,phasedata);
    //获得相位数组的最大值最小值，转换为0-255区间
    phase42ddisplay = new unsigned char[outWidth*outHeight];
    double min=0,max=0;
    for(int i=0;i < outWidth*outHeight;i++){
        double cur = phasedata[i];
        if (min > cur) min = cur;
        if (max < cur) max = cur;
    }
    for(int i=0;i < outWidth*outHeight;i++){
        double cur = phasedata[i];
        int temp = 255*(cur-min)/(max-min);
        phase42ddisplay[i] = temp;
    }
    m_max = max;
    m_min = min;
//    Mat phaseSave(outHeight,outWidth,CV_8UC1);
//    phaseSave.data = phase4display;
//    imwrite("D:\\QtProjects\\imageData\\phasesave.bmp",phaseSave);

    QImage Qtemp = QImage(phase42ddisplay,outWidth,outHeight,QImage::Format_Indexed8);
//    Qtemp = Qtemp.scaled(ui->gViewCompute->width(),ui->gViewCompute->height(),Qt::IgnoreAspectRatio);
    QGraphicsScene *myScene = new QGraphicsScene();
//    myPhaseScene->addPixmap(QPixmap::fromImage(Qtemp).scaled(mSizeDefault));
    myScene->addPixmap(QPixmap::fromImage(Qtemp).scaled(mSizeDefault));
    ui->gViewCompute->setScene(myScene);
    if(mRectNum)
        myScene->addItem(rectItem);
    if(mLineNum)
        myScene->addItem(lineItem);
//    delete hologram;
//    delete phasedata;
//    delete phase4display;
}

void MainWindow::display2Dphase()
{
    if(!mCalibration){
        QMessageBox::warning(NULL,"注意","请先校正!",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        return;
    }
    if(ui->actionVideo->isChecked() || ui->actionVideo->isChecked())
        connect(timerPhase,&QTimer::timeout,this,&MainWindow::displayPhase,Qt::UniqueConnection);
    else if(ui->actionPicture->isChecked())
        displayPhase();
}

void MainWindow::initGraph3D()
{
    graph3D = new Q3DSurface();
    graphContariner = QWidget::createWindowContainer(graph3D);
    //创建坐标轴
    QValue3DAxis *axisX = new QValue3DAxis;
    axisX->setTitle("Axis X");
    axisX->setTitleVisible(true);
    axisX->setRange(0,outWidth);
    graph3D->setAxisX(axisX);

    QValue3DAxis *axisY = new QValue3DAxis;
    axisY->setTitle("Axis Y");
    axisY->setTitleVisible(true);
//    axisY->setRange(-11,11);
    axisY->setAutoAdjustRange(true);
    graph3D->setAxisY(axisY);

    QValue3DAxis *axisZ=new QValue3DAxis;
    axisZ->setTitle("Axis Z");
    axisZ->setTitleVisible(true);
    axisZ->setRange(0,outHeight);
//    axisZ->setAutoAdjustRange(true);
    graph3D->setAxisZ(axisZ);

    //创建数据代理
    proxy = new QSurfaceDataProxy();
    seriers = new QSurface3DSeries(proxy);
    seriers->setItemLabelFormat("(@xLabel @yLabel @zLabel)");
    seriers->setMeshSmooth(true);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

    seriers->setDrawMode(QSurface3DSeries::DrawSurface);
    graph3D->addSeries(seriers);
//    seriers->setBaseColor(Qt::red);
    QLinearGradient gr;
    gr.setColorAt(1.0f,Qt::black);
    gr.setColorAt(0.8f,Qt::darkGreen);
    gr.setColorAt(0.6f,Qt::green);
    gr.setColorAt(0.4f,Qt::yellow);
    gr.setColorAt(0.2f,Qt::red);
    gr.setColorAt(0.0f,Qt::darkRed);
    seriers->setBaseGradient(gr);
    seriers->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

}

void MainWindow::init3Ddata()
{
//    disconnect(timerPhase,&QTimer::timeout,this,&MainWindow::displayPhase);
    if(mRectClick==-1){
        QMessageBox::warning(NULL,"注意","请先选择区域!",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
        return;
    }
    initGraph3D();
    mGraphNum++;
    ui->verticalLayout->addWidget(graphContariner);
    if(ui->actionVideo->isChecked())
        connect(timerPhase,&QTimer::timeout,this,&MainWindow::display3Dphase);
    else if(ui->actionPicture->isChecked())
        display3Dphase();

}

void MainWindow::display3Dphase()
{
    //创建相位数据
//    int N=outWidth;//-10:0.5:10, N个数据点
    QSurfaceDataArray *dataArray = new QSurfaceDataArray; //数组
//    dataArray->reserve(N);
    float z=0;
    for (int i =0 ; i <outHeight; i++){
        //跳过不在所选矩形区域中的行
        if(i < mCropRect.topLeft().y() || i > mCropRect.bottomRight().y())
            continue;
//        QSurfaceDataRow *newRow = new QSurfaceDataRow(outWidth);
//        if(mRectClick==2) newRow= new QSurfaceDataRow(mCropRect.width());
//        QSurfaceDataRow *newRow = new QSurfaceDataRow(outWidth); //一行的数据
        QSurfaceDataRow *newRow = new QSurfaceDataRow(mCropRect.width()); //一行的数据
        int index=0;
        for (int j = 0; j <outWidth; j++){
            //跳过不在所选矩形区域中的列
            if (j < mCropRect.topLeft().x() || j > mCropRect.bottomRight().x())
                continue;
            z = phasedata[i*outWidth + j];
            (*newRow)[index++].setPosition(QVector3D(j,z,outHeight-i));
        }
        *dataArray << newRow;
        qDebug() << "newRow size:" << newRow->size();
    }
    proxy->resetArray(dataArray);
    createColorBar(m_max,m_min);
}

void MainWindow::createColorBar(float max,float min)
{
//    qDebug() << "创建色标";
    int height = 400;
    int width = 100;
    int border = 10;
    QLinearGradient gr(0,0,1,height -2 * border);
    gr.setColorAt(0.0f,Qt::black);
    gr.setColorAt(0.2f,Qt::darkGreen);
    gr.setColorAt(0.4f,Qt::green);
    gr.setColorAt(0.6f,Qt::yellow);
    gr.setColorAt(0.8f,Qt::red);
    gr.setColorAt(1.0f,Qt::darkRed);

    QPixmap pm(width, height);
    pm.fill(Qt::transparent);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(gr));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(border, border, 20, height - 2 * border);
    pmp.setPen(Qt::black);
    int step = (height - 2 * border) / 5;
    for (int i = 0; i < 6; i++) {
        int yPos = i * step + border;
//        pmp.drawLine(border, yPos, 55, yPos);
        int disnum = max - (i * (max-min)/5);
        pmp.drawText(32, yPos + 2, QString::number(disnum));
    }
    ui->labelColorBar->setPixmap(pm);
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton){
        int x = ev->x();
        int y = ev->y();
//        qDebug() <<"x:"<< ui->gViewCompute->x() << "--ex:" << x << "--centerx:"<< centralWidget()->x();
//        qDebug() <<"y:"<< ui->gViewCompute->y() << "--ey:" << y << "--centery:"<< centralWidget()->y();
        //如果不在指定的区域选区域或者选直线，将不保存本次点击坐标，鼠标点击的纵坐标包括标题栏、菜单栏和工具栏的高度，因此需减去
        if (x < ui->gViewCompute->pos().x()
               || x > ui->gViewCompute->pos().x() + ui->gViewCompute->size().width()
               || y - centralWidget()->y() < ui->gViewCompute->pos().y()
               || y - centralWidget()->y() > ui->gViewCompute->pos().y() + ui->gViewCompute->size().height())
            return;
        if (mRectClick==0){
            mSelectRect.setTopLeft(QPoint(x, y - centralWidget()->y()));//保存的坐标以去除菜单栏工具栏后的窗口为原点
            mRectClick++;
//            qDebug () << "x:" << x << "--y:" << y;
        }
        else if(mRectClick==1){
            mSelectRect.setBottomRight(QPoint(x, y - centralWidget()->y()));
            mRectClick++;
//            qDebug () << "x:" << x << "--y:" << y;
            this->setCursor(Qt::ArrowCursor);
            QPoint pDrawStart = toDrawPoint(mSelectRect.topLeft());
            QPoint pDrawEnd = toDrawPoint(mSelectRect.bottomRight());

            QPen pen;
            pen.setWidth(2);
            pen.setColor(Qt::red);
            rectItem = ui->gViewCompute->scene()->addRect(QRect(pDrawStart,pDrawEnd),pen);
            mRectNum++;
            mCropRect.setTopLeft(convPoint(mSelectRect.topLeft()));
            mCropRect.setBottomRight(convPoint(mSelectRect.bottomRight()));
        }
        else if(mLineClick==0){
            pStart.setX(x);
            pStart.setY(y - centralWidget()->y());
            mLineClick++;
        }
        else if(mLineClick==1){
            pEnd.setX(x);
            pEnd.setY(y - centralWidget()->y());
            mLineClick++;
            this->setCursor(Qt::ArrowCursor);
            QPoint pDrawStart = toDrawPoint(pStart);
            QPoint pDrawEnd = toDrawPoint(pEnd);
            QPen pen;
            pen.setWidth(2);
            pen.setColor(Qt::red);
            lineItem = ui->gViewCompute->scene()->addLine(QLine(pDrawStart,pDrawEnd),pen);
            mLineNum++;
            pStart = convPoint(pStart);
            pEnd = convPoint(pEnd);
            if(ui->actionPicture->isChecked())
                displayChart();
            else if(ui->actionVideo->isChecked())
                timerChart->start();
        }
    }
}

void MainWindow::getRectData()
{
    this->setCursor(Qt::CrossCursor);
//    initGraph3D();
    mRectClick = 0;
    if(mRectNum){
        ui->gViewCompute->scene()->removeItem(rectItem);
        mRectNum--;
    }
}

void MainWindow::getLineData()
{
    this->setCursor(Qt::CrossCursor);
    initChart();
    if(mLineClick==-1&&ui->actionVideo->isChecked()){
        timerChart = new QTimer;
        timerChart->setInterval(500);
        connect(timerChart,&QTimer::timeout,this,&MainWindow::displayChart);
    }
    mLineClick = 0;
    if(mLineNum){
        ui->gViewCompute->scene()->removeItem(lineItem);
        mLineNum--;
    }
}

void MainWindow::initChart()
{
    //仅点击显示二维折线图表且第一次选点的时候初始化图表参数
    qDebug() << "mLineClick:" << mLineClick;
    if(mLineClick==-1){
        chart->setTitle(tr("切片显示"));
        //chart->setAcceptHoverEvents(true);
        ui->gViewLine->setChart(chart);
        ui->gViewLine->setRenderHint(QPainter::Antialiasing);

    //    QValueAxis *axisX = new QValueAxis;
    //    axisX->setRange(0, 10); //设置坐标轴范围
        axisX->setLabelFormat("%.1f"); //标签格式
    //    axisX->setTickCount(11); //主分隔个数
    //    axisX->setMinorTickCount(4);
        axisX->setTitleText("line(um)"); //标题
        axisX->setGridLineVisible(true);

    //    QValueAxis *axisY = new QValueAxis;
    //    axisY->setRange(-2, 2);
        axisY->setRange(m_min,m_max);
        axisY->setTitleText("height(nm)");
    //    axisY->setTickCount(5);
        axisY->setLabelFormat("%.2f"); //标签格式
        axisY->setGridLineVisible(true);
    //    axisY->setMinorTickCount(4);


        //另一种实现设置坐标轴的方法,在Qt 5.12.1里编译时不会提示过时
        chart->addAxis(axisX,Qt::AlignBottom); //坐标轴添加到图表，并指定方向
        chart->addAxis(axisY,Qt::AlignLeft);
        chart->addSeries(series);
        series->attachAxis(axisX); //序列 series 附加坐标轴
        series->attachAxis(axisY);
    }
    else{
        chart->setTitle(tr("切片显示"));
        ui->gViewLine->setChart(chart);
        ui->gViewLine->setRenderHint(QPainter::Antialiasing);
        axisY->setRange(m_min,m_max);
        qDebug() << "reload chart";
    }
}

void MainWindow::displayChart()
{
    QPoint p1 = pStart;
    QPoint p2 = pEnd;

//    QPen  pen;
//    pen.setStyle(Qt::DotLine);//Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine
//    pen.setWidth(2);
//    pen.setColor(Qt::red);
//    series0->setPen(pen); //折线序列的线条设置
//    QChart *chart = new QChart();

    int deltx = p2.x()-p1.x();
    int delty = p2.y()-p1.y();
    qreal z=0;
    //curAxis=axisX; //当前坐标轴
    series->clear();
    if(deltx > delty){
        axisX->setRange(p1.x(),p2.x());
        for(int i=p1.x();i<p2.x();i++){
            //index就是找两点构成的直线在每一行的纵坐标是在哪里
            int index = (p1.y() + (i-p1.x())*delty/deltx)*outWidth + i;
            z =phasedata[index];
            series->append(i,z);
        }
    }
    else{
        axisX->setRange(p1.y(),p2.y());
        for(int i=p1.y();i<p2.y();i++){
            //index就是找两点构成的直线在每一行的纵坐标是在哪里
            int index = i*outWidth + p1.x() + (i-p1.y())*deltx/delty;
            z = phasedata[index];
            series->append(i,z);
        }
    }
}

QPoint MainWindow::convPoint(QPoint pSrc)
{
    int x1 = pSrc.x();
    int deltx = ui->gViewCompute->x() + ui->gViewCompute->size().width()/2 - x1;
    //控件的纵坐标不包括菜单栏工具栏等，鼠标点击的纵坐标包括
    int y1 = pSrc.y();
    int delty = ui->gViewCompute->y() + ui->gViewCompute->size().height()/2 - y1;
    int pDestx = outWidth/2 - deltx*outWidth/mSizeChange.width();
    int pDesty = outHeight/2 - delty*outHeight/mSizeChange.height();
    if(pDestx<0) pDestx = 0;
    if(pDestx>=outWidth) pDestx =outWidth-1;
    if(pDesty<0) pDesty = 0;
    if(pDesty>=outHeight) pDesty = outHeight-1;
     qDebug() << "x1:" << pDestx << "--y1:" <<pDesty;
    return QPoint(pDestx,pDesty);
}

QPoint MainWindow::toDrawPoint(QPoint pSrc)
{
    int x1 = pSrc.x();
    int deltx = x1 - ui->gViewCompute->x();
    int y1 = pSrc.y();
    int delty = y1 - ui->gViewCompute->y();
    //如果有滚动条，加上滚动条的位置
//    deltx -=ui->gViewCompute->horizontalScrollBar()->value();
//    delty -=ui->gViewCompute->verticalScrollBar()->value();
    qDebug() << ui->gViewCompute->horizontalScrollBar()->value() <<"><" <<ui->gViewCompute->verticalScrollBar()->value();
    //实际上是找在默认放缩尺度时哪一个点变化来的
    int pDestx = mSizeDefault.width()/2 - (ui->gViewCompute->width()/2 - deltx)*mSizeDefault.width()/mSizeChange.width();
    int pDesty = mSizeDefault.height()/2 - (ui->gViewCompute->height()/2 - delty)*mSizeDefault.height()/mSizeChange.height();
    return QPoint(pDestx,pDesty);
}

void MainWindow::cleanAllDisplay()
{
    QGraphicsScene *myScene = new QGraphicsScene();
    ui->gViewCamera->setScene(myScene);
    ui->gViewCompute->setScene(myScene);
//    ui->gViewLine->setScene(myScene);
//    ui->gViewLine->setParent(NULL);
    //清除2D折线数据
    series->clear();
    QChart *tempChart = new QChart();
    ui->gViewLine->setChart(tempChart);
    //删除3D图表
    if(mGraphNum==1){
        mGraphNum--;
        graphContariner->setParent(NULL);
        ui->verticalLayout->removeWidget(graphContariner);
        //删除色标
        ui->labelColorBar->clear();
    }
    mSizeChange = QSize(mSizeDefault);
    //视口还原
    QMatrix matrix;
    matrix.setMatrix(1,ui->gViewCompute->matrix().m12(),
                     ui->gViewCompute->matrix().m21(),1,
                     ui->gViewCompute->matrix().dx(),
                     ui->gViewCompute->matrix().dy());
    ui->gViewCompute->setMatrix(matrix,false);
}

//放缩图片（例如：放大时factor=1.2f，缩小时factor=0.8f）
void MainWindow::scaleImage(float factor)
{
    //累计放缩因子
    mScaleFactor *= factor;
    //构造放缩矩阵
    QMatrix matrix;
    matrix.scale(mScaleFactor, mScaleFactor);
    //QGraphicsView执行放缩
    ui->gViewCompute->setMatrix(matrix);
    mSizeChange = mSizeChange*factor;
    qDebug() << "mSize:" << mSizeChange;
}

void MainWindow::zoomIn()
{
    scaleImage(1.2f);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8f);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    //如果信号不是来自于QGraphicsView，返回。
    if (watched != ui->gViewCompute)
        return false;

    switch (event->type())
    {
    //按键事件（操作放缩）
    case QEvent::KeyPress:
    {
        QKeyEvent * kEvent = (QKeyEvent*)event;
        //‘+’即键盘中的‘=’，执行放大
        if (kEvent->key() == '=')
        {
            scaleImage(1.2f);
        }
        //‘-’执行缩小
        else if (kEvent->key() == '-')
        {
            scaleImage(0.8f);
        }
    }
    case QEvent::Wheel:
    {
        QWheelEvent *wEvent = (QWheelEvent*)event;
        if(wEvent->delta() > 0)
        {
            zoomIn();
        }
        else
        {
            zoomOut();
        }
    }

    default:
        break;
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::getParam()
{
    pSet = new ParamSet(this);
    if(pSet->exec() == QDialog::Accepted)
    {
        ui->textBrowserParam->clear();
        QString lamda = QString("波长  ") + QString("%1").arg(pSet->lamda()) + QString(" 微米");
        QString type = QString("<br>样本类型 ") + pSet->type();
        QString refaction = QString("<br>样本折射 ") + QString("%1").arg(pSet->refaction());
        QString thickness = QString("<br>样本厚度 ") + QString("%1").arg(pSet->thickness());
        QString space = QString("<br>摄像像素间隔 ") + QString("%1").arg(pSet->distance()) + QString(" 微米");
        QString scale = QString("<br>放大倍数 ") + QString("%1").arg(pSet->scale());
        QString test = QString("<br>程序测试 ")  + (pSet->test()? QString(" y"):QString(" n"));
        QString sampleName = QString("<br>样本名称 ") + pSet->sampleName();
        ui->textBrowserParam->append(lamda);
        ui->textBrowserParam->append(type);
        ui->textBrowserParam->append(refaction);
        ui->textBrowserParam->append(thickness);
        ui->textBrowserParam->append(space);
        ui->textBrowserParam->append(scale);
        ui->textBrowserParam->append(test);
        ui->textBrowserParam->append(sampleName);
    }
    else
    {
        qDebug() << "取消";
    }

}
