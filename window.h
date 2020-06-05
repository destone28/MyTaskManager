/*
 * This is a simple task manager that allows user to start
 * new tasks choosing from available types of them.
 * User can choose type, interval and add some type-related
 * informations.
 *
 * The code includes some lists and map population to easily
 * allow the development of future functions such as listing,
 * start and stop or other management of tasks.
 *
 * Simple GUI and SystemTrayMenu are included in this version.
 *
 * Qt version: 5.9.9
 *
 * System requirement: Windows XP, 7, 8.1, 10
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QDir>

#ifndef QT_NO_SYSTEMTRAYICON

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

struct SimpleTask {
    QString type;
    QString task_related_content;
    int duration;
    qint64 id;
};

class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void changeLineEditLabel();
    void dispatchTask();
    void messageClicked();
    void showMessage();
    void startTask();

private:
    void createIconGroupBox();
    void createActions();
    void createTrayIcon();
    void checkFile(QString);
    void printText(QString);
    int secondsFromGuiInput();

    QAction *createNewAction;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QComboBox *task_type_combobox;
    QComboBox *time_unit_combobox;

    QGroupBox *window_group_box;

    QLabel *task_type_label;
    QLabel *interval_label;
    QLabel *task_related_details_label;

    QLineEdit *task_related_details_lineedit;

    QList<SimpleTask> *tasks_list;
    QList<QTimer*> timers_list;

    QMap<qint64,int> *taskids_timerid_map;

    QMenu *trayIconMenu;
    QMenu *listMenu;
    QMenu *singleTaskMenu;

    QPushButton *start_task_button;

    QSpinBox *time_interval_spinbox;

    QString current_line_edit_detail;

    QSystemTrayIcon *trayIcon;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
