#include "monitor.h"
#include "ui_monitor.h"
#include <QtWidgets>
#include <Windows.h>

HHOOK mouseHook;

Monitor * monitor;
Viewfinder * viewfinder1;
extern bool isHide;
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_RBUTTONDOWN)
    {
        // 鼠标右键按下事件
        qDebug() << 4;
        if(isHide == false)
        {
            if(monitor->isHidden()){

                monitor->show();
                viewfinder1->hide();

            }
            else{
                monitor->hide();
                viewfinder1->show();

            }
        }
        else{
            monitor->hide();
            viewfinder1->hide();
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

Monitor::Monitor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Monitor)
{
    ui->setupUi(this);

    monitor = this;

    this->move(0, 0);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground, true);
    // 设置窗口属性使其对鼠标事件透明
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setAttribute(Qt::WA_DeleteOnClose);
    installEventFilter(this);

    //    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    //    LONG extendedStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    //    SetWindowLong(hwnd, GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);//表示窗口是透明的，即允许鼠标事件透过窗口传递给下方的窗口。
    HWND hwnd = nullptr;
    LONG extendedStyle = 0;

    bool success = false;

    while (!success) {
        hwnd = reinterpret_cast<HWND>(this->winId());
        extendedStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        if (SetWindowLong(hwnd, GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT) != 0) {
            success = true;
        } else {
            // 等待一段时间再尝试重新执行
            QThread::msleep(100);
        }
    }

    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 获取屏幕的大小和位置信息
    QRect screenRect = screen->geometry();

    // 计算窗口在屏幕中间的位置
    int windowX = screenRect.x() + (screenRect.width() - width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - height()) / 2;

    // 设置窗口的位置
    move(windowX, windowY);

    do{
        // 安装鼠标钩子
        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);
    }while(mouseHook == NULL);


}

Monitor::~Monitor()
{
    // 卸载鼠标钩子
    UnhookWindowsHookEx(mouseHook);
    delete ui;
}

void Monitor::setScreenShot(const QPixmap screenShot)
{
    this->mScreenShot = screenShot;

    update();
}

void Monitor::changeWindowSize(int width, int height)
{
    this->resize(width*2,height*2);
    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 获取屏幕的大小和位置信息
    QRect screenRect = screen->geometry();

    // 计算窗口在屏幕中间的位置
    int windowX = screenRect.x() + (screenRect.width() - this->width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - this->height()) / 2;

    // 设置窗口的位置
    move(windowX, windowY);
}


void Monitor::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(rect(), QColor(0, 0, 0, 100));

    painter.drawPixmap(this->rect(), this->mScreenShot);
    static QPixmap pixmap(":/Image/selector.png");
    painter.drawPixmap(this->rect(), pixmap);

    event->accept();
}

// 处理监视器移动问题
void Monitor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->mOnSelector = true;
        this->mSelectorStartPos = this->frameGeometry().topLeft();
        this->mMouseStartPoint = event->globalPos();
    }
}

void Monitor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && this->mOnSelector) {
        QPoint delta = event->globalPos() - this->mMouseStartPoint;
        this->move(this->mSelectorStartPos + delta);
    }
}

void Monitor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->mOnSelector = false;
    }
}

bool Monitor::eventFilter(QObject * obj, QEvent * event)
{
    Q_UNUSED(obj);

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        // 处理鼠标事件
        if (mouseEvent->button() == Qt::RightButton){
            if(this->isHidden()){
                this->show();
            }
            else{
                this->hide();
            }
        }
    }

    // 返回 false，将事件继续传递给原始的事件接收者
    return QObject::eventFilter(obj, event);
}


void Monitor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        event->ignore();
    }
}


