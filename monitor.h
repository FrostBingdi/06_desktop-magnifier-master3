#pragma once
#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <viewfinder.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Monitor; }
QT_END_NAMESPACE

class Monitor : public QDialog
{
  Q_OBJECT

public:
  Monitor(QWidget *parent = nullptr);
  ~Monitor();

public slots:
  // ���¼������Ŵ���ͼƬ
  void setScreenShot(const QPixmap screenShot);
  void updateWindow() {
      update();
  }
  void changeWindowSize(int width,int height);
protected:
  // �������ػ��¼�
  void paintEvent(QPaintEvent *event) override;
  // ��ס���⴦�ƶ�����
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  bool eventFilter(QObject *, QEvent *);
  // ESC�����˳�
  void keyPressEvent(QKeyEvent *event) override;

private:
  Ui::Monitor *ui;
  // ������Ļ��ͼ
  QPixmap mScreenShot;
  // �������ƶ��¼���صı���
  // ������¼����Ƿ���ѡ������
  bool mOnSelector{false};
  // ������¼ѡ�����ĳ�ʼλ��
  QPoint mSelectorStartPos{this->frameGeometry().topLeft()};
  QPoint mMouseStartPoint;
    QPainter painter;
};
