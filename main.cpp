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
    // ��װȫ���¼�������
//  EventFilter eventFilter;
//  monitor.installEventFilter(&eventFilter);

//  viewfinder.initMonitor(&monitor);
  SettingBar settingBar;
settingBar1 = &settingBar;
  // ����ͼ�˾�֪ͨ����������ͼƬ
  app.connect(&viewfinder, &Viewfinder::screenShotOk, &monitor,
              &Monitor::setScreenShot,Qt::QueuedConnection);
  // �ı䷽������ʱ֪ͨѡ�����ı�Ŵ���
  app.connect(&settingBar, &SettingBar::changeMagnification, &viewfinder,
              &Viewfinder::changeMagnification);

  app.connect(&viewfinder, &Viewfinder::windowSizeChanged, &monitor, &Monitor::changeWindowSize);

  // ���settingBar���ر��˾�֪ͨViewfinderֹͣ����
  app.connect(&app, &QApplication::lastWindowClosed, &viewfinder,
              &Viewfinder::exitProgram);
  // ���յ�Viewfinder�Ŀ����˳��źź󣬹رմ��ڣ��������
  app.connect(&viewfinder, &Viewfinder::exitOk, &app, &QApplication::quit);


  viewfinder.show();
  settingBar.show();

  return app.exec();
}
