#ifndef DonutBreakDownChart_H
#define DonutBreakDownChart_H

#include <QtCharts>
#include <QPieSeries>

class DonutBreakdownChart : public QChart
{
    Q_OBJECT
public:

    DonutBreakdownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags);
    void addBreakdownSeries(QPieSeries *breakdownSeries, QColor color);
    void recalculateAngles();
    void updateLegendMarkers();
    QPieSeries* mainSeries()
    {
        return m_mainSeries;
    }
private:
    QPieSeries* m_mainSeries;
};


#endif //DonutBreakDownChart_H
