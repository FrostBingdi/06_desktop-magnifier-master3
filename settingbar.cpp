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
                // ִ�� Esc ���Ĳ���
                qDebug() << 1;
            }
            else if (key == VK_RETURN)
            {
                // ִ�лس����Ĳ���
                qDebug() << 2;
            }
            else if (key == VK_F1)
            {
                // ִ�� F1 ���Ĳ���
                qDebug() << 4;
                isHide = !isHide;
                if(settingBar1->ui->pushbutton_on_off->text() == QString::fromLocal8Bit("�رտ���")){
                    QString str = QString::fromLocal8Bit("��������");
                    settingBar1->ui->pushbutton_on_off->setText(str);
                    monitor->hide();
                    viewfinder1->hide();
                }
                else{
                    QString str = QString::fromLocal8Bit("�رտ���");
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
                // ִ�пո���Ĳ���
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
    this->setAttribute(Qt::WA_DeleteOnClose);//�ڴ��ڹرյ�ʱ���ͷŵ�
    this->move(
                (QGuiApplication::primaryScreen()->geometry().width() - this->width()) / 2,
                0);
    setFocusPolicy(Qt::StrongFocus);

    // ��װȫ���¼�������
    QCoreApplication::instance()->installEventFilter(this);
    // ��װ���̹���
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);

}

SettingBar::~SettingBar()
{

    // ж�ؼ��̹���
    UnhookWindowsHookEx(keyboardHook);
    viewfinder1->close();
    monitor->close();
    delete ui;
}

void SettingBar::keyPressEvent(QKeyEvent *event)
{
    // ���������¼�
    int key = event->key();

    // ���ݰ��µİ���������Ӧ����
    if (key == Qt::Key_Escape) {
        // ������µ��� Esc ������ִ���ض�����
        qDebug() << 1;
    } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        // ������µ��ǻس�������ִ���ض�����
        qDebug() << 2;
    }

    // ���¼����ݸ����ദ����ѡ��
    QDialog::keyPressEvent(event);
}

void SettingBar::keyReleaseEvent(QKeyEvent *event)
{
    // �����ͷ��¼�
    int key = event->key();

    // �����ͷŵİ���������Ӧ����
    if (key == Qt::Key_Space) {
        // ����ͷŵ��ǿո������ִ���ض�����
        qDebug() << 2;
    }

    // ���¼����ݸ����ദ����ѡ��
    QDialog::keyReleaseEvent(event);
}

bool SettingBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        // ������̰����¼�
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // ���ݰ��»��ͷŵİ���ִ����Ӧ����
        if (event->type() == QEvent::KeyPress)
        {
            int key = keyEvent->key();
            if (key == Qt::Key_Escape)
            {
                // ִ�� Esc ���Ĳ���

            }
            else if (key == Qt::Key_Enter || key == Qt::Key_Return)
            {
                // ִ�лس����Ĳ���

            }
        }
        else if (event->type() == QEvent::KeyRelease)
        {
            int key = keyEvent->key();
            if (key == Qt::Key_Space)
            {
                // ִ�пո���Ĳ���

            }
        }

        return true; // ���� true ��ʾ�¼��Ѵ���
    }

    return QDialog::eventFilter(obj, event); // �����¼�Ĭ�ϴ���
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
    if(ui->pushbutton_on_off->text() == QString::fromLocal8Bit("�رտ���")){
        QString str = QString::fromLocal8Bit("��������");
        ui->pushbutton_on_off->setText(str);
        monitor->hide();
        viewfinder1->hide();
    }
    else{
        QString str = QString::fromLocal8Bit("�رտ���");
        ui->pushbutton_on_off->setText(str);
        viewfinder1->show();
        monitor->hide();
    }
}
