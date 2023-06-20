#pragma once

#include "viewfinder.h"
#include <QDialog>
#include <QScreen>
namespace Ui {
class SettingBar;
}

class SettingBar : public QDialog
{
  Q_OBJECT

public:
  explicit SettingBar(QWidget *parent = nullptr);
  ~SettingBar();

signals:
  void changeMagnification(bool zoomIn);

public:
  Ui::SettingBar *ui;
private:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);
  void closeEvent(QCloseEvent *event);
private slots:
  void on_ZoomIn_clicked();

  void on_ZoomOut_clicked();
  void on_pushbutton_on_off_clicked();
};
