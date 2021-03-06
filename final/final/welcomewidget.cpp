#include "welcomewidget.h"

// ***********************
// todo: add background img and beautify
void WelcomeWidget::setupLayouts() {
    QFont font = QFont("consolas", 12);
    this->resize(WIDTH, HEIGHT);
    this->setWindowTitle("Analysis and Visualization");
    this->setFont(font);
    this->setWindowIcon(QIcon("../taxi.svg"));
    QVBoxLayout *outerlayout = new QVBoxLayout(this);

    // top
    QHBoxLayout *top_layout = new QHBoxLayout();
    top_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QLabel *toplabel = new QLabel();
    toplabel->setText("\nVisualize Taxi Data\nSelect data folder and begin");
    toplabel->setAlignment(Qt::AlignHCenter);
    toplabel->setMinimumWidth(WIDTH);
    toplabel->setMinimumHeight(HEIGHT / 3);
    toplabel->setFont(font);
    top_layout->addWidget(toplabel);
    top_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(top_layout);

    // mid
    QHBoxLayout *mid_layout = new QHBoxLayout();
    //    bottomlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 5, QSizePolicy::Expanding, QSizePolicy::Expanding));
    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    progress_bar->setFormat("%p%");
    progress_bar->setMinimumWidth(WIDTH / 5 * 3);
    progress_bar->setMinimumHeight(WIDTH / 12);
    progress_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mid_layout->addWidget(progress_bar);
    //    bottomlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 5, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(mid_layout);

    // bottom
    QHBoxLayout *bottom_layout = new QHBoxLayout();
    bottom_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));

    select_button = new QPushButton();
    select_button->setText("Select");
    select_button->setMinimumWidth(WIDTH / 5);
    select_button->setMinimumHeight(HEIGHT / 6);
    select_button->setFont(font);
    bottom_layout->addWidget(select_button);
    bottom_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));

    load_button = new QPushButton();
    load_button->setText("Load");
    load_button->setMinimumWidth(WIDTH / 5);
    load_button->setMinimumHeight(HEIGHT / 6);
    load_button->setFont(font);
    bottom_layout->addWidget(load_button);

    bottom_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(bottom_layout);
}

void WelcomeWidget::setupConnects() {
    connect(select_button, &QPushButton::clicked, this, &WelcomeWidget::getPath);
    connect(load_button, &QPushButton::clicked, this, &WelcomeWidget::loadData);
}

void WelcomeWidget::getPath() {
    // get folder paht and store in folder_path
    db->folder_path = QFileDialog::getExistingDirectory();
}

// load data to database
//  using another thread
void WelcomeWidget::loadData() {
    db->parseFolder();
    auto loader = new MyThread();
    connect(loader, &MyThread::funcStart, db, &DataBase::loadData);
    connect(db, &DataBase::setValue, this, &WelcomeWidget::setBarValue);
    connect(db, &DataBase::loadDone, this, &WelcomeWidget::loadDone);
    connect(this, &WelcomeWidget::switchWindow, loader, &MyThread::deleteLater);
    db->moveToThread(loader);
    loader->start();
}

void WelcomeWidget::loadDone() {
    // move back and switch window
    db->moveToThread(QThread::currentThread());
    emit switchWindow();
}

void WelcomeWidget::setBarValue(double value) {
    // change loadbar progress
    progress_bar->setValue(value);
}
