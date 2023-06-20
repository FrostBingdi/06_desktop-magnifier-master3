#include "settingbar.h"
#include "ui_settingbar.h"
#include <QDebug>
#include <QCoreApplication>
#include <QEvent>

#include "settingbar.h"
#include "ui_settingbar.h"
#include <QDebug>
#include <Windows.h>

extern Monitor * monitor;
extern Viewfinder * viewfinder1;
extern bool isHide;
extern SettingBar * settingBar1;
HHOOK keyboardHook;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            int key = kbStruct->vkCode;

            if (key == VK_ESCAPE)
            {
                // 执行 Esc 键的操作
                qDebug() << 1;
            }
            else if (key == VK_RETURN)
            {
                // 执行回车键的操作
                qDebug() << 2;
            }
            else if (key == VK_F1)
            {
                // 执行 F1 键的操作
                qDebug() << 4;
                isHide = !isHide;
                if(settingBar1->ui->pushbutton_on_off->text() == QString::fromLocal8Bit("关闭开镜")){
                    QString str = QString::fromLocal8Bit("开启开镜");
                    settingBar1->ui->pushbutton_on_off->setText(str);
                    monitor->hide();
                    viewfinder1->hide();
                }
                else{
                    QString str = QString::fromLocal8Bit("关闭开镜");
                    settingBar1->ui->pushbutton_on_off->setText(str);
                    viewfinder1->show();
                    monitor->hide();
                }
            }
        }
        else if (wParam == WM_KEYUP)
        {
            KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            int key = kbStruct->vkCode;

            if (key == VK_SPACE)
            {
                // 执行空格键的操作
                qDebug() << 3;
            }
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}



SettingBar::SettingBar(QWidget *parent) : QDialog(parent), ui(new Ui::SettingBar)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint |
                         Qt::WindowTitleHint);
    this->setAttribute(Qt::WA_QuitOnClose);
    this->setAttribute(Qt::WA_DeleteOnClose);//内存在关闭的时候释放掉
    this->move(
                (QGuiApplication::primaryScreen()->geometry().width() - this->width()) / 2,
                0);
    setFocusPolicy(Qt::StrongFocus);

    // 安装全局事件过滤器
    QCoreApplication::instance()->installEventFilter(this);
    // 安装键盘钩子
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);

}

SettingBar::~SettingBar()
{

    // 卸载键盘钩子
    UnhookWindowsHookEx(keyboardHook);
    viewfinder1->close();
    monitor->close();
    delete ui;
}

void SettingBar::keyPressEvent(QKeyEvent *event)
{
    // 按键按下事件
    int key = event->key();

    // 根据按下的按键进行相应处理
    if (key == Qt::Key_Escape) {
        // 如果按下的是 Esc 键，则执行特定操作
        qDebug() << 1;
    } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        // 如果按下的是回车键，则执行特定操作
        qDebug() << 2;
    }

    // 将事件传递给父类处理（可选）
    QDialog::keyPressEvent(event);
}

void SettingBar::keyReleaseEvent(QKeyEvent *event)
{
    // 按键释放事件
    int key = event->key();

    // 根据释放的按键进行相应处理
    if (key == Qt::Key_Space) {
        // 如果释放的是空格键，则执行特定操作
        qDebug() << 2;
    }

    // 将事件传递给父类处理（可选）
    QDialog::keyReleaseEvent(event);
}

bool SettingBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        // 处理键盘按键事件
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // 根据按下或释放的按键执行相应操作
        if (event->type() == QEvent::KeyPress)
        {
            int key = keyEvent->key();
            if (key == Qt::Key_Escape)
            {
                // 执行 Esc 键的操作

            }
            else if (key == Qt::Key_Enter || key == Qt::Key_Return)
            {
                // 执行回车键的操作

            }
        }
        else if (event->type() == QEvent::KeyRelease)
        {
            int key = keyEvent->key();
            if (key == Qt::Key_Space)
            {
                // 执行空格键的操作

            }
        }

        return true; // 返回 true 表示事件已处理
    }

    return QDialog::eventFilter(obj, event); // 其他事件默认处理
}

void SettingBar::on_ZoomIn_clicked()
{
    emit this->changeMagnification(true);
}

void SettingBar::on_ZoomOut_clicked()
{
    emit this->changeMagnification(false);
}

void SettingBar::closeEvent(QCloseEvent *event)
{

    QCoreApplication::quit();
    viewfinder1->close();
    monitor->close();
    event->accept();
}

void SettingBar::on_pushbutton_on_off_clicked()
{
    isHide = !isHide;
    if(ui->pushbutton_on_off->text() == QString::fromLocal8Bit("关闭开镜")){
        QString str = QString::fromLocal8Bit("开启开镜");
        ui->pushbutton_on_off->setText(str);
        monitor->hide();
        viewfinder1->hide();
    }
    else{
        QString str = QString::fromLocal8Bit("关闭开镜");
        ui->pushbutton_on_off->setText(str);
        viewfinder1->show();
        monitor->hide();
    }
}
