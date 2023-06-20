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
  // 更新监视器放大后的图片
  void setScreenShot(const QPixmap screenShot);
  void updateWindow() {
      update();
  }
  void changeWindowSize(int width,int height);
protected:
  // 监视器重绘事件
  void paintEvent(QPaintEvent *event) override;
  // 按住任意处移动窗口
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  bool eventFilter(QObject *, QEvent *);
  // ESC不让退出
  void keyPressEvent(QKeyEvent *event) override;

private:
  Ui::Monitor *ui;
  // 保存屏幕截图
  QPixmap mScreenShot;
  // 以下是移动事件相关的变量
  // 用来记录鼠标是否在选择器上
  bool mOnSelector{false};
  // 用来记录选择器的初始位置
  QPoint mSelectorStartPos{this->frameGeometry().topLeft()};
  QPoint mMouseStartPoint;
    QPainter painter;
};
