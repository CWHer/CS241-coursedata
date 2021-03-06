#ifndef COMMON_H
#define COMMON_H

#include <QAreaSeries>
#include <QChartView>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QDateTimeEdit>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QErrorMessage>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QLocale>
#include <QPieSeries>
#include <QProgressBar>
#include <QPushButton>
#include <QRegExp>
#include <QSplineSeries>
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QValueAxis>
#include <cmath>
#include <limits>
#include <memory>
#include <queue>
#include <utility>
#include <vector>
QT_CHARTS_USE_NAMESPACE
using std::make_pair;
using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::vector;
using Pos = std::pair<double, double>;

const auto DAY_NUM = 15;
struct Tour {
    QString id;
    int start, end;
    Pos src, dst;
    // lng,lat
    double fee;
};
const auto GRID_NUM = 100;
const auto GRID_PER = 10;
struct Grid {
    int id;
    Pos y1, y2, x1, x2;
    // LU,RU,LD,UD
};

#endif // COMMON_H
