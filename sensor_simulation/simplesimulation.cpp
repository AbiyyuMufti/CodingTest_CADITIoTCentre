#include "simplesimulation.h"
#include "./ui_simplesimulation.h"

#include <string>
#include <iostream>
#include <random>
#include <chrono>


const double temperature_lower = 10;
const double temperature_upper = 40;
const double humidity_lower = 20;
const double humidity_upper = 70;


SimpleSimulation::SimpleSimulation(QWidget *parent): QMainWindow(parent), ui(new Ui::SimpleSimulation)
{
    ui->setupUi(this);
    is_running = false;

    // Start Timer with callback of 2 second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SimpleSimulation::Update));
    timer->start(2000);

    srand(QDateTime::currentDateTime().toTime_t());
    addTemperaturePlot();
    addHumidityPlot();
    ui->TemperatureMin->setDecMode();
    ui->TemperatureMax->setDecMode();
    ui->TemperatureAverage->setDecMode();
    ui->TemperatureMedian->setDecMode();

    ui->HumidityMin->setDecMode();
    ui->HumidityMax->setDecMode();
    ui->HumidityAverage->setDecMode();
    ui->HumidityMedian->setDecMode();
}

SimpleSimulation::~SimpleSimulation()
{
    Json::Value temp = dataHum.makeJSON("Temperature");
    Json::Value humd = dataTem.makeJSON("Humidity");
    Json::Value endpoint;
    endpoint.append(temp);
    endpoint.append(humd);
    CadItCoding::WriteJson("../result/random_aggregation.json", endpoint);
    std::cout << endpoint.toStyledString() << std::endl;
    std::cout << " -------------------------------------- " << std::endl;

    delete ui;
}


void SimpleSimulation::addTemperaturePlot()
{
    ui->TemperaturePlot->yAxis->setRange(temperature_lower, temperature_upper);
	ui->TemperaturePlot->axisRect()->setupFullAxesBox();
    ui->TemperaturePlot->xAxis->setLabel("timestamp");
	ui->TemperaturePlot->yAxis->setLabel("temperature (°C)");
    ui->TemperaturePlot->legend->setVisible(true);

    QPen minPen, maxPen, avgPen, medPen;

    minPen.setColor(QColor(0, 0, 255));
    minPen.setWidthF(1);
    maxPen.setColor(QColor(255, 0, 0));
    maxPen.setWidthF(1);
    avgPen.setColor(QColor(0, 255, 0));
    avgPen.setWidthF(1);
    medPen.setColor(QColor(255, 0, 255));
    medPen.setWidthF(1);

    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->TemperaturePlot->graph(0)->setName(QString("Min"));
    ui->TemperaturePlot->graph(0)->setPen(minPen);

    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->TemperaturePlot->graph(1)->setName(QString("Max"));
    ui->TemperaturePlot->graph(1)->setPen(maxPen);
    
    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->TemperaturePlot->graph(2)->setName(QString("Average"));
    ui->TemperaturePlot->graph(2)->setPen(avgPen);

    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(3)->setLineStyle(QCPGraph::lsLine);
    ui->TemperaturePlot->graph(3)->setName(QString("Median"));
    ui->TemperaturePlot->graph(3)->setPen(medPen);
    

	temp_min.reset(new QCPDataContainer<QCPGraphData>);
    temp_max.reset(new QCPDataContainer<QCPGraphData>);
    temp_avg.reset(new QCPDataContainer<QCPGraphData>);
    temp_med.reset(new QCPDataContainer<QCPGraphData>);
}

void SimpleSimulation::addHumidityPlot()
{
    ui->HumidityPlot->yAxis->setRange(humidity_lower, humidity_upper);
	ui->HumidityPlot->axisRect()->setupFullAxesBox();
    ui->HumidityPlot->xAxis->setLabel("timestamp");
	ui->HumidityPlot->yAxis->setLabel("humidity (%)");
    ui->HumidityPlot->legend->setVisible(true);

    QPen minPen, maxPen, avgPen, medPen;
    minPen.setColor(QColor(0, 0, 255));
    minPen.setWidthF(1);
    maxPen.setColor(QColor(255, 0, 0));
    maxPen.setWidthF(1);
    avgPen.setColor(QColor(0, 255, 0));
    avgPen.setWidthF(1);
    medPen.setColor(QColor(255, 0, 255));
    medPen.setWidthF(1);

    ui->HumidityPlot->addGraph();
    ui->HumidityPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->HumidityPlot->graph(0)->setName(QString("Min"));
    ui->HumidityPlot->graph(0)->setPen(minPen);

    ui->HumidityPlot->addGraph();
    ui->HumidityPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->HumidityPlot->graph(1)->setName(QString("Max"));
    ui->HumidityPlot->graph(1)->setPen(maxPen);
    
    ui->HumidityPlot->addGraph();
    ui->HumidityPlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->HumidityPlot->graph(2)->setName(QString("Average"));
    ui->HumidityPlot->graph(2)->setPen(avgPen);

    ui->HumidityPlot->addGraph();
    ui->HumidityPlot->graph(3)->setLineStyle(QCPGraph::lsLine);
    ui->HumidityPlot->graph(3)->setName(QString("Median"));
    ui->HumidityPlot->graph(3)->setPen(medPen);
    

	hum_min.reset(new QCPDataContainer<QCPGraphData>);
    hum_max.reset(new QCPDataContainer<QCPGraphData>);
    hum_avg.reset(new QCPDataContainer<QCPGraphData>);
    hum_med.reset(new QCPDataContainer<QCPGraphData>);

}

void SimpleSimulation::on_pushButton_clicked()
{
    std::string text = ui->pushButton->text().toStdString();
    if (text == "Start")
    {
        ui->pushButton->setText("Stop");
        is_running = true;
        auto p1 = std::chrono::system_clock::now();
        dt = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count() % 1000;
        ui->TemperaturePlot->xAxis->setRange(dt-2, dt + 10);
        ui->TemperaturePlot->graph(0)->rescaleAxes(true);

        ui->HumidityPlot->xAxis->setRange(dt-2, dt + 10);
        ui->HumidityPlot->graph(0)->rescaleAxes(true);        
    }
    else
    {
        ui->pushButton->setText("Start");
        is_running = false;
    }    
}

static double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void SimpleSimulation::storeDataRoutine()
{
    // generate random value for temperature
    double temp_random = fRand(temperature_lower + 5, temperature_upper - 5);
    // generate random value for humidity
    double humid_random = fRand(humidity_lower, humidity_upper);

    auto p1 = std::chrono::system_clock::now();
    // only show 3 digit of timestamp
    dt = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count() % 1000;
    
    // std::cout << dt << " temperature: " << temp_random << "\thumidity: " << humid_random << std::endl;
    dataTem.update(temp_random);
    dataHum.update(humid_random);

    QCPGraphData dataMinTem(dt, dataTem.getMin());
    QCPGraphData dataMaxTem(dt, dataTem.getMax());
    QCPGraphData dataAvgTem(dt, dataTem.getAverage());
    QCPGraphData dataMedTem(dt, dataTem.getMedian());
    temp_min->add(dataMinTem);
    temp_max->add(dataMaxTem);
    temp_avg->add(dataAvgTem);
    temp_med->add(dataMedTem);

    QCPGraphData dataMinHum(dt, dataHum.getMin());
    QCPGraphData dataMaxHum(dt, dataHum.getMax());
    QCPGraphData dataAvgHum(dt, dataHum.getAverage());
    QCPGraphData dataMedHum(dt, dataHum.getMedian());
    hum_min->add(dataMinHum);
    hum_max->add(dataMaxHum);
    hum_avg->add(dataAvgHum);
    hum_med->add(dataMedHum);
}

void SimpleSimulation::plotDataRoutine()
{
    ui->TemperaturePlot->graph(0)->setData(temp_min);
    ui->TemperaturePlot->graph(0)->rescaleAxes(true);
    ui->TemperaturePlot->graph(1)->setData(temp_max);
    ui->TemperaturePlot->graph(1)->rescaleAxes(true);
    ui->TemperaturePlot->graph(2)->setData(temp_avg);
    ui->TemperaturePlot->graph(2)->rescaleAxes(true);
    ui->TemperaturePlot->graph(3)->setData(temp_med);
    ui->TemperaturePlot->graph(3)->rescaleAxes(true);
    ui->TemperaturePlot->replot();
    ui->TemperaturePlot->update();

    ui->HumidityPlot->graph(0)->setData(hum_min);
    ui->HumidityPlot->graph(0)->rescaleAxes(true);
    ui->HumidityPlot->graph(1)->setData(hum_max);
    ui->HumidityPlot->graph(1)->rescaleAxes(true);
    ui->HumidityPlot->graph(2)->setData(hum_avg);
    ui->HumidityPlot->graph(2)->rescaleAxes(true);
    ui->HumidityPlot->graph(3)->setData(hum_med);
    ui->HumidityPlot->graph(3)->rescaleAxes(true);
    ui->HumidityPlot->replot();
    ui->HumidityPlot->update();
}

void SimpleSimulation::showLCDRoutine()
{
    
    ui->TemperatureMin->display(dataTem.getMin());
    ui->TemperatureMax->display(dataTem.getMax());
    ui->TemperatureAverage->display(dataTem.getAverage());
    ui->TemperatureMedian->display(dataTem.getMedian());
    ui->HumidityMin->display(dataHum.getMin());
    ui->HumidityMax->display(dataHum.getMax());
    ui->HumidityAverage->display(dataHum.getAverage());
    ui->HumidityMedian->display(dataHum.getMedian());
}

void SimpleSimulation::Update()
{
    if (is_running)
    {
        storeDataRoutine();
        plotDataRoutine();
        showLCDRoutine();
    }
}
