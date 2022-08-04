#ifndef SIMPLESIMULATION_H
#define SIMPLESIMULATION_H

#include <QMainWindow>

#include "utility.hpp"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SimpleSimulation; }
QT_END_NAMESPACE

class SimpleSimulation : public QMainWindow
{
    Q_OBJECT

public:
    SimpleSimulation(QWidget *parent = nullptr);
    ~SimpleSimulation();

    void Update();
    void addTemperaturePlot();
    void addHumidityPlot();
    void storeDataRoutine();
    void plotDataRoutine();
    void showLCDRoutine();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SimpleSimulation *ui;
    QTimer *timer;
    bool is_running;
    int64_t dt;

    CadItCoding::DataAggregation dataHum;
    CadItCoding::DataAggregation dataTem;

    QSharedPointer<QCPDataContainer<QCPGraphData>> temp_min;
    QSharedPointer<QCPDataContainer<QCPGraphData>> temp_max;
    QSharedPointer<QCPDataContainer<QCPGraphData>> temp_avg;
    QSharedPointer<QCPDataContainer<QCPGraphData>> temp_med;

    QSharedPointer<QCPDataContainer<QCPGraphData>> hum_min;
    QSharedPointer<QCPDataContainer<QCPGraphData>> hum_max;
    QSharedPointer<QCPDataContainer<QCPGraphData>> hum_avg;
    QSharedPointer<QCPDataContainer<QCPGraphData>> hum_med;
};
#endif // SIMPLESIMULATION_H
