#ifndef MAINWINDOW
#define MAINWINDOW

#include <qapplication.h>
#include <QMainWindow>
#include <QWidget>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <QMenuBar>
#include <QInputDialog>


#include "mainactoins.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:
    void GUI();
    void Buttons();
    void addMenu();

protected slots:
void slot_pb_pause();
void slot_pb_stop();
void slot_pb_start();
void slot_outResult();
void slot_workEnded();
void slot_dialogSetSpeed();

signals:
void processOn(int);
int getCountDetals(QChar);
QString getProcessTime(QChar, int);
QString getResults();
void setSpeed(int);


private:
    QWidget* m_widget;
    QGridLayout* m_layout;
    QPushButton* m_pb_start;
    QPushButton* m_pb_pause;
    QPushButton* m_pb_stop;
    QLineEdit* m_le_input;
    QLabel* m_lb_input;
    QLabel* m_lb_countDetals;

    QStringList m_outStat;
    QLabel* outCountDetals;
    QLabel* outPerfomance;
    QLabel* outAllTimeProcess;

    MainActions* m_actions;

    bool pauseEnabled;

};

#endif // MAINWINDOW

