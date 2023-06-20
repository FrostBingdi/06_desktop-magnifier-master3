#include "settingbar.h"
#include "viewfinder.h"
#include "monitor.h"
#include <QApplication>

extern Viewfinder * viewfinder1;
bool isHide = false;
SettingBar * settingBar1;
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Viewfinder viewfinder;
    viewfinder1 = &viewfinder;
  Monitor monitor;
    // 安装全局事件过滤器
//  EventFilter eventFilter;
//  monitor.installEventFilter(&eventFilter);

//  viewfinder.initMonitor(&monitor);
  SettingBar settingBar;
settingBar1 = &settingBar;
  // 截完图了就通知监视器更新图片
  app.connect(&viewfinder, &Viewfinder::screenShotOk, &monitor,
              &Monitor::setScreenShot,Qt::QueuedConnection);
  // 改变方法倍数时通知选择器改变放大倍数
  app.connect(&settingBar, &SettingBar::changeMagnification, &viewfinder,
              &Viewfinder::changeMagnification);

  app.connect(&viewfinder, &Viewfinder::windowSizeChanged, &monitor, &Monitor::changeWindowSize);

  // 如果settingBar被关闭了就通知Viewfinder停止任务
  app.connect(&app, &QApplication::lastWindowClosed, &viewfinder,
              &Viewfinder::exitProgram);
  // 当收到Viewfinder的可以退出信号后，关闭窗口，程序结束
  app.connect(&viewfinder, &Viewfinder::exitOk, &app, &QApplication::quit);


  viewfinder.show();
  settingBar.show();

  return app.exec();
}
