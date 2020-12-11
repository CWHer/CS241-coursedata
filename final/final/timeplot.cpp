#include "timeplot.h"

TimePlot::TimePlot(const int w, const int h)
    : WIDTH(w)
    , HEIGHT(h) {}

void TimePlot::setupLayouts() {
    QFont font = QFont("consolas", 10);
    main_widget = new QWidget();
    auto outer_layout = new QVBoxLayout(main_widget);

    // selection part begin
    auto selection_part = new QVBoxLayout();

    // top begin
    auto top_layout = new QHBoxLayout();
    auto spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //  place select
    auto place_label = new QLabel();
    place_label->setText("Place");
    place_label->setFont(font);
    place_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //    place_label->setMinimumWidth(20);
    top_layout->addWidget(place_label);
    place_combo = new QComboBox();
    place_combo->setFont(font);
    //**********************
    // add a dialog to ask grid id
    place_combo->addItem(QString("Chengdu"));
    place_combo->addItem(QString("seperate areas"));
    top_layout->addWidget(place_combo);
    top_layout->addItem(spacer);

    //  date select
    auto date_label = new QLabel();
    date_label->setText("Date");
    date_label->setFont(font);
    date_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //    date_label->setMinimumWidth(
    top_layout->addWidget(date_label);
    date_combo = new QComboBox();
    date_combo->setFont(font);
    for (auto i = 1; i <= DAY_NUM; ++i)
        date_combo->addItem("11-" + QString::number(i));
    top_layout->addWidget(date_combo);
    top_layout->addItem(spacer);

    //  start time select
    auto start_time_label = new QLabel();
    start_time_label->setText("Start Time");
    start_time_label->setFont(font);
    start_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(start_time_label);
    start_combo = new QComboBox();
    start_combo->setFont(font);
    for (auto i = 0; i < 23; ++i)
        start_combo->addItem(QString::number(i));
    top_layout->addWidget(start_combo);
    top_layout->addItem(spacer);

    //  end time select
    auto end_time_label = new QLabel();
    end_time_label->setText("End Time");
    end_time_label->setFont(font);
    end_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(end_time_label);
    end_combo = new QComboBox();
    end_combo->setFont(font);
    for (auto i = 1; i <= 24; ++i)
        end_combo->addItem(QString::number(i));
    top_layout->addWidget(end_combo);
    top_layout->addItem(spacer);

    selection_part->addLayout(top_layout);
    // top end

    //  mid begin
    auto mid_layout = new QHBoxLayout();

    // step select
    auto step_label = new QLabel();
    step_label->setText("Step(min)");
    step_label->setFont(font);
    step_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(step_label);
    step_combo = new QComboBox();
    step_combo->setFont(font);
    for (auto i = 30; i <= 180; i += 30)
        step_combo->addItem(QString::number(i));
    mid_layout->addWidget(step_combo);
    mid_layout->addItem(spacer);

    //  type select
    auto type_label = new QLabel();
    type_label->setText("Plot Type");
    type_label->setFont(font);
    type_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(type_label);
    type_combo = new QComboBox();
    type_combo->setFont(font);
    type_combo->addItem("Line Chart");
    type_combo->addItem("Pie Chart");
    mid_layout->addWidget(type_combo);
    mid_layout->addItem(spacer);

    //  progress bar
    progress_bar = new QProgressBar();
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 4);
    mid_layout->addWidget(progress_bar);
    auto min_spacer = new QSpacerItem(40, 20);
    mid_layout->addSpacerItem(min_spacer);

    //  plot button
    plot_button = new QPushButton();
    plot_button->setText("Plot!");
    plot_button->setFont(font);
    plot_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(plot_button);

    selection_part->addLayout(mid_layout);
    // mid end

    outer_layout->addLayout(selection_part);
    // selection part end

    // plot area begin
    plot_area = new QChartView();
    plot_area->setMinimumSize(WIDTH, HEIGHT / 4 * 3);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    outer_layout->addWidget(plot_area);
    // plot area end
}

void TimePlot::setupConnects() {
    // button -> plotMap
    connect(plot_button, &QPushButton::clicked, this, &TimePlot::plotMap);
}

void TimePlot::plotMap() {
    // get settings

    //********************
    // todo: grid selection
    grid_id.clear(), grid_id.push_back(0);
    //********************
    QRegExp expr("11-(\\d+)");
    expr.indexIn(date_combo->currentText());
    day = expr.cap(1).toInt();
    start_hour = start_combo->currentText().toInt();
    end_hour = end_combo->currentText().toInt();
    step_min = step_combo->currentText().toInt();
    // using another thread
    if (type_combo->currentText() == "Line Chart") {
        auto painter = new myThread();
        connect(painter, &myThread::funcStart, this, &TimePlot::plotLineMap);
        this->moveToThread(painter);
        db->moveToThread(painter);
        painter->start();
    } else {
        auto painter = new myThread();
        connect(painter, &myThread::funcStart, this, &TimePlot::plotPieMap);
        this->moveToThread(painter);
        db->moveToThread(painter);
        painter->start();
    }
}

void TimePlot::plotLineMap() {
    auto line = new QLineSeries();
    for (auto i = start_hour * 60; i <= end_hour * 60; i += step_min) {
        // transform time to Unix time stamp
        //  hh:mm
        QString time_day = (i / 60 < 10 ? "0" : "") + QString::number(i / 60) + ":";
        time_day += (i / 60 < 10 ? "0" : "") + QString::number(i % 60);
        QString time_str = QString::number(day) + " Dec 2016 " + time_day;
        QDateTime time_now = QLocale(QLocale::Chinese, QLocale::China).toDateTime(time_str, "dd MMM yyyy hh:mm");
        time_now.setTimeSpec(Qt::UTC);
        auto start_time = time_now.toTime_t();
        auto end_time = start_time + step_min * 60;
        int num = 0;
        for (auto &id : grid_id)
            num += db->startCount(id, start_time, end_time);
        line->append(i, num);
    }
    auto chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(line);
    chart->createDefaultAxes();
    plot_area->setChart(chart);
    emit plotDone(this);
}

void TimePlot::plotPieMap() {
    // get settings

    emit plotDone(this);
}