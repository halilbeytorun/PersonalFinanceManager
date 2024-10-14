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
    void removeBreakdownSeries(const std::string& name);
    void recalculateAngles();
    void updateLegendMarkers();
    QPieSeries* mainSeries()
    {
        return m_mainSeries;
    }
    QPieSeries* unusedMoneySeries()
    {
        return m_unusedMoneySeries;
    }
    /// @brief Ownership does not belong to this class  
    void setUnusedMoneySeries(QPieSeries* unusedMoneySeries)
    {
        m_unusedMoneySeries = unusedMoneySeries;
    }  
private:
    QPieSeries* m_mainSeries;
    QPieSeries* m_unusedMoneySeries;
};


#endif //DonutBreakDownChart_H
