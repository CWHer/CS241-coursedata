#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"
#include "mythread.hpp"
#include "utils.h"

// store data
//  need func 'movetoThread' from baseclass
class DataBase : public QObject {
    Q_OBJECT
  public:
    QString folder_path;
    QFileInfoList file_list;
    void parseFolder();
    void loadData();
    void initData();
    // grid_id, start_time, end_time
    //  return number of tours during [start_time, end_time)
    int startCount(int, int, int);
    int endCount(int, int, int);
    // fee average
    double feeCount(int, int, int);
    // time average (seconds)
    double timeCount(int, int, int);

  public:
    // name like 201611xx
    //  date + abs_path
    vector<QString> file_info[DAY_NUM];

    // day x tour_pre_day
    vector<Tour> tours[DAY_NUM];

    // grid pos
    vector<Grid> grids;
    vector<double> lat, lng;

    // !below sorted by start time
    // start tour in each grid
    vector<Tour> start_tour[GRID_NUM];
    // prefix sum
    vector<double> time_sum[GRID_NUM];
    vector<double> fee_sum[GRID_NUM];
    // !below sorted by end time
    // end tour in each grid
    vector<Tour> end_tour[GRID_NUM];

  private:
    int searchGridID(const Pos &);
    // std::function<bool(const T &, const T &)> func
    template <class T> int lower_bound(const vector<T> &, bool (*func)(const T &, const T &), T);
    // file number
    int total_file;

  signals:
    void setValue(double value);
    void loadDone();
};

#endif // DATABASE_H
