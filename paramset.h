#ifndef PARAMSET_H
#define PARAMSET_H

#include <QDialog>
#include <QString>

namespace Ui {
class ParamSet;
}

class ParamSet : public QDialog
{
    Q_OBJECT

public:
    explicit ParamSet(QWidget *parent = 0);
    ~ParamSet();

private slots:
    void okSet();

private:
    Ui::ParamSet *ui;
    float mLambda;
    QString mType;
    float mRefraIndex;
    float mThickness;
    float mDistance;
    float mScale;
    bool mTest;
    bool mInverse;
    QString mSampleName;

public:
    float lamda() const;
    QString type() const;
    float refaction() const;
    float thickness() const;
    float distance() const;
    float scale() const;
    bool test() const;
    bool inverse() const;
    QString sampleName() const;

};

#endif // PARAMSET_H
