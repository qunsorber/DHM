#include "paramset.h"
#include "ui_paramset.h"

ParamSet::ParamSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamSet)
{
    ui->setupUi(this);
    connect(ui->btnOk,&QPushButton::clicked,this,&ParamSet::okSet);
    connect(ui->btnCancel,&QPushButton::clicked,this,&ParamSet::close);
    //默认设置test
    ui->radioRelief->setChecked(true);
    ui->radioTestYes->setChecked(true);
    ui->radioReverseNo->setChecked(true);
    ui->spinBoxLamda->setValue(532);
    ui->spinBoxThick->setValue(2);
    ui->spinBoxSpace->setValue(2);
    ui->spinBoxScale->setValue(4);
    ui->lineRefraction->setText("1.5");
    ui->lineEditName->setText("SampleName");
}

ParamSet::~ParamSet()
{
    delete ui;
}

void ParamSet::okSet()
{
    mLambda = ui->spinBoxLamda->value();
    mType = ui->radioRelief->isChecked() ? QString("r") : QString("n");
    mRefraIndex = ui->lineRefraction->text().toFloat();
    mThickness = ui->spinBoxThick->value();
    mDistance = ui->spinBoxSpace->value();
    mScale = ui->spinBoxScale->value();
    mTest = ui->radioTestYes->isChecked() ? true:false;
    mInverse = ui->radioReverseYes->isChecked() ? true:false;
    mSampleName = ui->lineEditName->text();
    accept();
}

float ParamSet::lamda() const
{
    return mLambda/1000;
}

QString ParamSet::type() const
{
    return mType;
}

float ParamSet::refaction() const
{
    return mThickness;
}

float ParamSet::thickness() const
{
    return mThickness;
}

float ParamSet::distance() const
{
    return mDistance;
}

float ParamSet::scale() const
{
    return mScale;
}

bool ParamSet::test() const
{
    return mTest;
}

bool ParamSet::inverse() const
{
    return mInverse;
}

QString ParamSet::sampleName() const
{
    return mSampleName;
}
