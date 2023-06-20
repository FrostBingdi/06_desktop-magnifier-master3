#include "viewfinder.h"
#include "ui_viewfinder.h"
#include <Windows.h>
#include "monitor.h"
#include "ui_monitor.h"
#include <QThreadPool>

//HWND windowHandle;

Viewfinder::Viewfinder(QWidget *parent)
    : QDialog(parent), ui(new Ui::Viewfinder) {
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setAttribute(Qt::WA_DeleteOnClose);
    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 获取屏幕的大小和位置信息
    QRect screenRect = screen->geometry();

    // 计算窗口在屏幕中间的位置
    int windowX = screenRect.x() + (screenRect.width() - width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - height()) / 2;

    // 设置窗口的位置
    move(windowX, windowY);

    //    this->lower();

    //    // 多线程开启，不断刷新监视器
    //    QFuture<void> future = QtConcurrent::run([&] {
    //        forever {
    //            if (this->mExit) {
    //                emit exitOk();
    //                break;
    //            }
    //            // 获取选择器左上角的全局位置
    //            QPoint selectorTopLeft = this->frameGeometry().topLeft();
    //            int selectorX = selectorTopLeft.x();
    //            int selectorY = selectorTopLeft.y();
    //            // 根据选择器的位置截屏
    ////            QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
    ////                        0, selectorX, selectorY, this->width(), this->height());

    //            // 获取要截取的窗口句柄
    //            HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
    //            if (windowHandle == nullptr)
    //            {
    //                qDebug() << "Failed to find the window.";
    //            }

    //             QPixmap screenShot = grabWindowByHWND(windowHandle);

    //             QRect rect(selectorX,selectorY,this->width(),this->height());

    //             QPixmap screenShot1 = cropPixmap(screenShot,rect);

    //            // 发出信号通知监视器把图片设置到监视器上
    //            emit this->screenShotOk(screenShot1);
    //        }
    //    });

    // 多线程开启，不断刷新监视器
    //    QThreadPool::globalInstance()->setMaxThreadCount(200);
    //    QThreadPool::globalInstance()->start(this);

    // 多线程开启，不断刷新监视器

    // 获取选择器左上角的全局位置
    QPoint selectorTopLeft = this->frameGeometry().topLeft();
    selectorX = selectorTopLeft.x();
    selectorY = selectorTopLeft.y();

    timerID = startTimer(1000); // 启动第一个定时器并记录其ID

    for(int i = 1; i <= 10; i++){
        QFuture<void> future = QtConcurrent::run([&] {
            forever {
                if (this->mExit) {
                    emit exitOk();
                    break;
                }



                // 根据选择器的位置截屏
                // QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
                //            0, selectorX, selectorY, this->width(), this->height());

                // 获取要截取的窗口句柄
                //        HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");

                mutex1.lock();
                QPixmap screenShot;
                if(windowHandle != NULL){
                    QRect rect(selectorX, selectorY, this->width(), this->height());
                    screenShot = cropPixmap(grabWindowByHWND(windowHandle), rect);
                }
                mutex1.unlock();
                mutex2.lock();
                // 发出信号通知监视器把图片设置到监视器上
                if(!screenShot.isNull()){
                    emit this->screenShotOk(screenShot);
                }
                mutex2.unlock();

            }
        });
    }

}





//void Viewfinder::run() {
//    forever {
//        if (this->mExit) {
//            emit exitOk();
//            break;
//        }
//        // 获取选择器左上角的全局位置
//        QPoint selectorTopLeft = this->frameGeometry().topLeft();
//        int selectorX = selectorTopLeft.x();
//        int selectorY = selectorTopLeft.y();
//        // 根据选择器的位置截屏
//        // QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
//        //            0, selectorX, selectorY, this->width(), this->height());

//        // 获取要截取的窗口句柄
//        //        HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
//        //        if (windowHandle == nullptr)
//        //        {
//        //            qDebug() << "Failed to find the window.";
//        //        }

//        QPixmap screenShot = grabWindowByHWND(windowHandle);

//        QRect rect(selectorX, selectorY, this->width(), this->height());

//        QPixmap screenShot1 = cropPixmap(screenShot, rect);

//        // 发出信号通知监视器把图片设置到监视器上
//        emit this->screenShotOk(screenShot1);
//    }
//}


Viewfinder::~Viewfinder() { delete ui; }

void Viewfinder::changeMagnification(bool zoomIn) {
    if (zoomIn) {
        this->resize(this->width() / 2, this->height() / 2);
    } else {
        this->resize(this->width() * 2, this->height() * 2);
    }

    emit windowSizeChanged(this->width(),this->height());

    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 获取屏幕的大小和位置信息
    QRect screenRect = screen->geometry();

    // 计算窗口在屏幕中间的位置
    int windowX = screenRect.x() + (screenRect.width() - width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - height()) / 2;

    // 设置窗口的位置
    move(windowX, windowY);

    // 获取选择器左上角的全局位置
    QPoint selectorTopLeft = this->frameGeometry().topLeft();
    selectorX = selectorTopLeft.x();
    selectorY = selectorTopLeft.y();
}

void Viewfinder::exitProgram() { this->mExit = true; }

//处理取景框移动问题
void Viewfinder::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 设置鼠标在窗口上为真
        this->mOnSelector = true;
        // 记录窗口的初始位置
        this->mSelectorStartPos = this->frameGeometry().topLeft();
        // 记录鼠标的起始点
        this->mMouseStartPoint = event->globalPos();
    }
}

void Viewfinder::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pic;
    painter.setRenderHint(QPainter::Antialiasing);
    pic.load(":/Image/selector.png");
    pic = pic.scaled(this->width(),this->height());
    painter.drawPixmap(0, 0, pic);//绘制图像
}

//void Viewfinder::initMonitor(Monitor * monitor2)
//{
//    this->monitor1 = monitor2;
//    this->wid = monitor2->winId();
//}

QPixmap Viewfinder::grabWindowByHWND(HWND hwnd)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "Failed to get the primary screen.";
        return QPixmap();
    }


    HDC hdc = GetDC(hwnd);
    if (!hdc) {
        qWarning() << "Failed to get the device context.";
        return QPixmap();
    }

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible device context
    HDC compatibleDC = CreateCompatibleDC(hdc);
    if (!compatibleDC) {
        ReleaseDC(hwnd, hdc);
        qWarning() << "Failed to create a compatible device context.";
        return QPixmap();
    }

    // Create a compatible bitmap
    HBITMAP compatibleBitmap = CreateCompatibleBitmap(hdc, width, height);
    if (!compatibleBitmap) {
        DeleteDC(compatibleDC);
        ReleaseDC(hwnd, hdc);
        qWarning() << "Failed to create a compatible bitmap.";
        return QPixmap();
    }

    // Select the compatible bitmap into the compatible device context
    HBITMAP oldBitmap = (HBITMAP)SelectObject(compatibleDC, compatibleBitmap);

    // Copy the window contents to the compatible device context
    BOOL result = PrintWindow(hwnd, compatibleDC, PW_CLIENTONLY);
    if (!result) {
        qWarning() << "Failed to copy window contents.";
        SelectObject(compatibleDC, oldBitmap);
        DeleteObject(compatibleBitmap);
        DeleteDC(compatibleDC);
        ReleaseDC(hwnd, hdc);
        return QPixmap();
    }

    // Create a QImage from the compatible bitmap
    BITMAPINFO bitmapInfo;
    memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -height; // Use negative height to ensure correct orientation
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32; // Assuming 32-bit bitmap
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    QImage image(width, height, QImage::Format_ARGB32);
    GetDIBits(compatibleDC, compatibleBitmap, 0, height, image.bits(), &bitmapInfo, DIB_RGB_COLORS);

    // Create a QPixmap from the QImage
    QPixmap pixmap = QPixmap::fromImage(image);
    if (pixmap.isNull()) {
        qWarning() << "Failed to create a QPixmap from the image.";
        SelectObject(compatibleDC, oldBitmap);
        DeleteObject(compatibleBitmap);
        DeleteDC(compatibleDC);
        ReleaseDC(hwnd, hdc);
        return QPixmap();
    }

    // Clean up resources
    SelectObject(compatibleDC, oldBitmap);
    DeleteObject(compatibleBitmap);
    DeleteDC(compatibleDC);
    ReleaseDC(hwnd, hdc);

    return pixmap;
}

WId Viewfinder::getWindowHandle(QWidget *widget)
{
#if defined(Q_OS_WIN)
    return widget->winId();
#elif defined(Q_OS_MACOS)
    return widget->windowHandle()->winId();
#elif defined(Q_OS_LINUX)
    // 获取 X11 窗口句柄
    return widget->winId();
#endif
}

QPixmap Viewfinder::cropPixmap(const QPixmap &sourcePixmap, const QRect &rect)
{
    if (rect.isNull() || sourcePixmap.isNull()) {
        return QPixmap();
    }

    // 创建目标矩形大小的空白 QPixmap
    QPixmap croppedPixmap(rect.size());
    croppedPixmap.fill(Qt::transparent);

    // 在目标 QPixmap 上绘制截取的区域
    QPainter painter(&croppedPixmap);
    painter.drawPixmap(0, 0, sourcePixmap, rect.x(), rect.y(), rect.width(), rect.height());
    painter.end();

    return croppedPixmap;
}

int Viewfinder::getSelectorY() const
{
    return selectorY;
}

void Viewfinder::setSelectorY(int value)
{
    selectorY = value;
}

int Viewfinder::getSelectorX() const
{
    return selectorX;
}

void Viewfinder::setSelectorX(int value)
{
    selectorX = value;
}


void Viewfinder::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton and this->mOnSelector) {
        QPoint delta = event->globalPos() - this->mMouseStartPoint;
        this->move(this->mSelectorStartPos + delta);
    }
}

void Viewfinder::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        this->mOnSelector = false;
    }
}

void Viewfinder::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        event->ignore();
    }
}

void Viewfinder::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerID) {
        windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
        if(windowHandle != NULL) killTimer(timerID);
    }

}

