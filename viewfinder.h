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
    // 截完图了
    void screenShotOk(QPixmap screenShot);
    // 子线程退出完毕
    void exitOk();

    void windowSizeChanged(int width,int height);
public slots:
    // 改变放大倍数
    void changeMagnification(bool zoomIn);
    // 接到退出的通知，处理一下
    void exitProgram();

protected:
    // 按住任意处移动窗口
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // ESC不让退出
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
    // 用来告诉子线程要退出了
    QAtomicInt mExit{false};
    // 以下是移动事件相关的变量
    // 用来记录鼠标是否在选择器上
    bool mOnSelector{false};
    // 用来记录选择器的初始位置
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
