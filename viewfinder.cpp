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
    // ��ȡ����Ļ
    QScreen *screen = QGuiApplication::primaryScreen();

    // ��ȡ��Ļ�Ĵ�С��λ����Ϣ
    QRect screenRect = screen->geometry();

    // ���㴰������Ļ�м��λ��
    int windowX = screenRect.x() + (screenRect.width() - width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - height()) / 2;

    // ���ô��ڵ�λ��
    move(windowX, windowY);

    //    this->lower();

    //    // ���߳̿���������ˢ�¼�����
    //    QFuture<void> future = QtConcurrent::run([&] {
    //        forever {
    //            if (this->mExit) {
    //                emit exitOk();
    //                break;
    //            }
    //            // ��ȡѡ�������Ͻǵ�ȫ��λ��
    //            QPoint selectorTopLeft = this->frameGeometry().topLeft();
    //            int selectorX = selectorTopLeft.x();
    //            int selectorY = selectorTopLeft.y();
    //            // ����ѡ������λ�ý���
    ////            QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
    ////                        0, selectorX, selectorY, this->width(), this->height());

    //            // ��ȡҪ��ȡ�Ĵ��ھ��
    //            HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
    //            if (windowHandle == nullptr)
    //            {
    //                qDebug() << "Failed to find the window.";
    //            }

    //             QPixmap screenShot = grabWindowByHWND(windowHandle);

    //             QRect rect(selectorX,selectorY,this->width(),this->height());

    //             QPixmap screenShot1 = cropPixmap(screenShot,rect);

    //            // �����ź�֪ͨ��������ͼƬ���õ���������
    //            emit this->screenShotOk(screenShot1);
    //        }
    //    });

    // ���߳̿���������ˢ�¼�����
    //    QThreadPool::globalInstance()->setMaxThreadCount(200);
    //    QThreadPool::globalInstance()->start(this);

    // ���߳̿���������ˢ�¼�����

    // ��ȡѡ�������Ͻǵ�ȫ��λ��
    QPoint selectorTopLeft = this->frameGeometry().topLeft();
    selectorX = selectorTopLeft.x();
    selectorY = selectorTopLeft.y();

    timerID = startTimer(1000); // ������һ����ʱ������¼��ID

    for(int i = 1; i <= 15; i++){
        QFuture<void> future = QtConcurrent::run([&] {
            forever {
                if (this->mExit) {
                    emit exitOk();
                    break;
                }



                // ����ѡ������λ�ý���
                // QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
                //            0, selectorX, selectorY, this->width(), this->height());

                // ��ȡҪ��ȡ�Ĵ��ھ��
                //        HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");

                mutex1.lock();
                QPixmap screenShot;
                if(windowHandle != NULL){
                    QRect rect(selectorX, selectorY, this->width(), this->height());
                    screenShot = cropPixmap(grabWindowByHWND(windowHandle), rect);
                }
                mutex1.unlock();
                mutex2.lock();
                // �����ź�֪ͨ��������ͼƬ���õ���������
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
//        // ��ȡѡ�������Ͻǵ�ȫ��λ��
//        QPoint selectorTopLeft = this->frameGeometry().topLeft();
//        int selectorX = selectorTopLeft.x();
//        int selectorY = selectorTopLeft.y();
//        // ����ѡ������λ�ý���
//        // QPixmap screenShot = QGuiApplication::primaryScreen()->grabWindow(
//        //            0, selectorX, selectorY, this->width(), this->height());

//        // ��ȡҪ��ȡ�Ĵ��ھ��
//        //        HWND windowHandle = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
//        //        if (windowHandle == nullptr)
//        //        {
//        //            qDebug() << "Failed to find the window.";
//        //        }

//        QPixmap screenShot = grabWindowByHWND(windowHandle);

//        QRect rect(selectorX, selectorY, this->width(), this->height());

//        QPixmap screenShot1 = cropPixmap(screenShot, rect);

//        // �����ź�֪ͨ��������ͼƬ���õ���������
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

    // ��ȡ����Ļ
    QScreen *screen = QGuiApplication::primaryScreen();

    // ��ȡ��Ļ�Ĵ�С��λ����Ϣ
    QRect screenRect = screen->geometry();

    // ���㴰������Ļ�м��λ��
    int windowX = screenRect.x() + (screenRect.width() - width()) / 2;
    int windowY = screenRect.y() + (screenRect.height() - height()) / 2;

    // ���ô��ڵ�λ��
    move(windowX, windowY);

    // ��ȡѡ�������Ͻǵ�ȫ��λ��
    QPoint selectorTopLeft = this->frameGeometry().topLeft();
    selectorX = selectorTopLeft.x();
    selectorY = selectorTopLeft.y();
}

void Viewfinder::exitProgram() { this->mExit = true; }

//����ȡ�����ƶ�����
void Viewfinder::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // ��������ڴ�����Ϊ��
        this->mOnSelector = true;
        // ��¼���ڵĳ�ʼλ��
        this->mSelectorStartPos = this->frameGeometry().topLeft();
        // ��¼������ʼ��
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
    painter.drawPixmap(0, 0, pic);//����ͼ��
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
    // ��ȡ X11 ���ھ��
    return widget->winId();
#endif
}

QPixmap Viewfinder::cropPixmap(const QPixmap &sourcePixmap, const QRect &rect)
{
    if (rect.isNull() || sourcePixmap.isNull()) {
        return QPixmap();
    }

    // ����Ŀ����δ�С�Ŀհ� QPixmap
    QPixmap croppedPixmap(rect.size());
    croppedPixmap.fill(Qt::transparent);

    // ��Ŀ�� QPixmap �ϻ��ƽ�ȡ������
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

