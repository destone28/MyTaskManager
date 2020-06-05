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

#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

//main constructor for Window class
Window::Window()
{
    createIconGroupBox();
    tasks_list = new QList<SimpleTask>();
    taskids_timerid_map = new QMap<qint64,int>();

    task_type_label->setMinimumWidth(interval_label->sizeHint().width());

    createActions();
    createTrayIcon();

    connect(start_task_button, &QAbstractButton::clicked, this, &Window::showMessage);
    connect(start_task_button, &QAbstractButton::clicked, this, &Window::startTask);
    connect(task_type_combobox, &QComboBox::currentTextChanged, this, &Window::changeLineEditLabel);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(window_group_box);
    setLayout(mainLayout);

    task_type_combobox->setCurrentIndex(0);
    task_related_details_label->setText("Enter text to print:");
    trayIcon->show();
    setWindowIcon(QIcon("icon.png"));
    setWindowTitle(tr("Ti-Care Task Manager"));
    resize(350, 150);
}

//closeEvent handle close window button
void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Ti-Care Task Manager"),
                                 tr("Ti-Care Task Manager will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

//showMessage emit a notification when new task starts
void Window::showMessage()
{
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(1);
        QString tmp_msg = "\"" + task_type_combobox->currentText()
                + "\" task just started running!";
        trayIcon->showMessage("Task Started", tmp_msg, msgIcon, 1000);
}

//messageClicked inform how to use the app when its notification emitted is clicked
void Window::messageClicked()
{
    QMessageBox::information(0, tr("Ti-Care Task Manager"),
                             tr("To start another task, "
                                "right-click on the app icon, "
                                "then \"Create new task\" "));
}

//changeLineEditLabel switch gui label based on task type
void Window::changeLineEditLabel()
{
    if (task_type_combobox->currentText().compare("Check file presence") == 0)
    {
        current_line_edit_detail = "file path";
    }
    else if(task_type_combobox->currentText().compare("Print text in debug") == 0)
    {
        current_line_edit_detail = "text to print";
    }

    task_related_details_label->setText("Enter " + current_line_edit_detail + ":");
}


//startTask creates new timers and start new tasks
void Window::startTask()
{
    //setting unique id to assign to new task
    qint64 new_task_id = QDateTime::currentMSecsSinceEpoch();

    if (!taskids_timerid_map->keys().contains(new_task_id))
    {
        SimpleTask new_simple_task;
        QString new_task_type = task_type_combobox->currentText();
        QString new_task_related_content = task_related_details_lineedit->text();
        int new_task_duration = secondsFromGuiInput();

        //populating new task with inputs from gui
        new_simple_task.type = new_task_type;
        new_simple_task.task_related_content = new_task_related_content;
        new_simple_task.duration = new_task_duration;
        new_simple_task.id = new_task_id;

        //starting new timer for new task execution
        QTimer *timer = new QTimer();
        connect(timer, &QTimer::timeout, this, &Window::dispatchTask);
        timer->start(new_task_duration*1000);

        //populating tasks list, timers list and task/timer map for future task management implementations
        tasks_list->append(new_simple_task);
        taskids_timerid_map->insert(new_task_id, timer->timerId());
        timers_list.append(timer);

    }

}

//dispatchTask is a slot connected to properly start task related function into just started timer
void Window::dispatchTask()
{
    QTimer *local_timer = qobject_cast<QTimer*>(sender());
    int local_timer_id = local_timer->timerId();
    SimpleTask local_task;
    qint64 local_task_id = 0;

    for (int i=0; i<taskids_timerid_map->size(); i++)
    {
        if (local_timer_id == taskids_timerid_map->values().at(i))
        {
            local_task_id = taskids_timerid_map->keys().at(i);
        }
    }

    for (int j=0; j<tasks_list->size(); j++)
    {
        if (local_task_id == tasks_list->at(j).id)
        {
            local_task = tasks_list->at(j);
        }
    }

    if (local_task.type.compare("Check file presence") == 0)
    {
        checkFile(local_task.task_related_content);
    }
    else if(local_task.type.compare("Print text in debug")==0)
    {
        printText(local_task.task_related_content);
    }


    local_timer->start(local_task.duration*1000);
}

//printText emit a system tray notification containing text passed in argument
void Window::printText(QString text_to_print)
{
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(1);
    trayIcon->showMessage("Notification for entered text",text_to_print, msgIcon, 0);
    qDebug() << text_to_print;
}

//checkFile emit a system tray notification when searched file passed in argument doesn't exists
void Window::checkFile(QString file_to_check_string)
{
    QFile file_to_check(file_to_check_string);
    QString result;

    if (!file_to_check.exists())
    {
        result = "File " + file_to_check_string + " doesn't exists!";
        qDebug() << result;
        QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(1);
        trayIcon->showMessage("Notification for file presence", result, msgIcon, 0);
    }
}

//secondsFromGuiInput takes whatever user time selection from gui and translate it in seconds
int Window::secondsFromGuiInput()
{
    int n = time_interval_spinbox->value();

    if (time_unit_combobox->currentText().compare("minutes") == 0)
    {
        n *= 60;
    }
    else if (time_unit_combobox->currentText().compare("hours") == 0)
    {
        n*= 3600;
    }

    return n;
}

//createIconGroupBox setup main window gui for app
void Window::createIconGroupBox()
{
    window_group_box = new QGroupBox(tr("Create new task"));

    task_type_label = new QLabel("Task type:");

    task_type_combobox = new QComboBox;
    task_type_combobox->addItem("Print text in debug");
    task_type_combobox->addItem("Check file presence");
    current_line_edit_detail = "file path";

    interval_label = new QLabel("Check every");

    time_interval_spinbox = new QSpinBox;
    time_interval_spinbox->setRange(1, 59);
    time_interval_spinbox->setValue(10);

    time_unit_combobox = new QComboBox;
    time_unit_combobox->addItem("seconds");
    time_unit_combobox->addItem("minutes");
    time_unit_combobox->addItem("hours");

    task_related_details_label = new QLabel();
    task_related_details_lineedit = new QLineEdit();

    start_task_button = new QPushButton(tr("Start Task"));
    start_task_button->setDefault(true);

    QGridLayout *window_layout = new QGridLayout;
    window_layout->addWidget(task_type_label, 0, 0);
    window_layout->addWidget(task_type_combobox, 0, 1, 1, 2);
    window_layout->addWidget(interval_label, 1, 0);
    window_layout->addWidget(time_interval_spinbox, 1, 1);
    window_layout->addWidget(time_unit_combobox, 1, 2, 1, 1);
    window_layout->addWidget(task_related_details_label, 2, 0);
    window_layout->addWidget(task_related_details_lineedit, 2, 1, 1, 2);
    window_layout->addWidget(start_task_button, 3, 2);
    window_group_box->setLayout(window_layout);
}

//createActions create actions for system tray menu
void Window::createActions()
{
    createNewAction = new QAction(tr("Create new task"), this);
    connect(createNewAction, &QAction::triggered, this, &QWidget::showNormal);

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

//createTrayIcon populate a menu with actions created in createActions
void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(createNewAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon("icon.png");
    trayIcon->setIcon(icon);
    trayIcon->setContextMenu(trayIconMenu);
}

#endif
