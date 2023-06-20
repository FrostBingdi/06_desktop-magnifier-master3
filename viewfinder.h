#pragma once
#include "monitor.h"
#include "ui_monitor.h"
#include <QAtomicInt>
#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtConcurrent/QtConcurrent>
#include <QScreen>
#include <QGuiApplication>
#include <QThreadPool>
#include "viewfinder.h"
#include "ui_viewfinder.h"
#include <Windows.h>
#include "monitor.h"
#include "ui_monitor.h"
#include <QThreadPool>
#include <QDialog>
#include <QRunnable>

namespace Ui {
class Viewfinder;
}

class Viewfinder : public QDialog
{
    Q_OBJECT

public:
    explicit Viewfinder(QWidget *parent = nullptr);
    ~Viewfinder();

signals:
    // ����ͼ��
    void screenShotOk(QPixmap screenShot);
    // ���߳��˳����
    void exitOk();

    void windowSizeChanged(int width,int height);
public slots:
    // �ı�Ŵ���
    void changeMagnification(bool zoomIn);
    // �ӵ��˳���֪ͨ������һ��
    void exitProgram();

protected:
    // ��ס���⴦�ƶ�����
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // ESC�����˳�
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event);
public:
    void paintEvent(QPaintEvent *event);
//    void initMonitor(Monitor *monitor2);
//    void run()Q_DECL_OVERRIDE;
    int getSelectorX() const;
    void setSelectorX(int value);

    int getSelectorY() const;
    void setSelectorY(int value);

private:
    Ui::Viewfinder *ui;
    // �����������߳�Ҫ�˳���
    QAtomicInt mExit{false};
    // �������ƶ��¼���صı���
    // ������¼����Ƿ���ѡ������
    bool mOnSelector{false};
    // ������¼ѡ�����ĳ�ʼλ��
    QPoint mSelectorStartPos{this->frameGeometry().topLeft()};
    QPoint mMouseStartPoint;
    QPixmap grabWindowByHWND(HWND hwnd);
//    Monitor * monitor1;
    WId getWindowHandle(QWidget *widget);
    QPixmap cropPixmap(const QPixmap& sourcePixmap, const QRect& rect);
    WId wid;
    QMutex mutex1;
    QMutex mutex2;
    QMutex mutex3;
    int selectorX;
    int selectorY;
    HWND windowHandle;
    int timerID;
};
