#include <UIModule/DonutBreakdownChart.h>
#include <UIModule/DonutBreakdownMainSlice.h>
#include <QGraphicsItem>

#include <exception>

DonutBreakdownChart::DonutBreakdownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags), m_unusedMoneySeries(nullptr)
{
    // create the series for main center pie
    m_mainSeries = new QPieSeries;
    m_mainSeries->setName("MAINSERIES_UNUSED_NAME");
    m_mainSeries->setPieSize(0.7);
    QChart::addSeries(m_mainSeries);
}

void DonutBreakdownChart::addBreakdownSeries(QPieSeries *breakdownSeries, QColor color)
{
    const QFont font("Arial", 8);

    // add breakdown series as a slice to center pie
    auto mainSlice = new DonutBreakdownMainSlice(breakdownSeries, this->parent());
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue(breakdownSeries->sum());
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush(color);
    mainSlice->setLabelVisible();
    mainSlice->setLabelColor(Qt::white);
    mainSlice->setLabelPosition(QPieSlice::LabelInsideNormal);
    mainSlice->setLabelFont(font);

    // position and customize the breakdown series
    breakdownSeries->setPieSize(0.8);
    breakdownSeries->setHoleSize(0.7);
    breakdownSeries->setLabelsVisible();
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        color = color.lighter(115);
        slice->setBrush(color);
        slice->setLabelFont(font);
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    recalculateAngles();

    // update customize legend markers
    updateLegendMarkers();
}


void DonutBreakdownChart::recalculateAngles()
{
    qreal angle = 0;
    const auto slices = m_mainSeries->slices();
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<DonutBreakdownMainSlice *>(slice)->breakdownSeries();
        breakdownSeries->setPieStartAngle(angle);
        angle += slice->percentage() * 360.0; // full pie is 360.0
        breakdownSeries->setPieEndAngle(angle);
    }
}

void DonutBreakdownChart::updateLegendMarkers()
{
    // go through all markers
    const auto allseries = series();
    for (QAbstractSeries *series : allseries) {
        const auto markers = legend()->markers(series);
        for (QLegendMarker *marker : markers) {
            auto pieMarker = qobject_cast<QPieLegendMarker *>(marker);
            if (series == m_mainSeries || series == m_unusedMoneySeries) {
                // hide markers from main series
                pieMarker->setVisible(false);
            } else {
                // modify markers from breakdown series
                pieMarker->setLabel(QString("%1: %2")
                                    .arg(pieMarker->slice()->label())
                                    .arg(pieMarker->slice()->value(), 0, 'f', 2));
                pieMarker->setFont(QFont("Arial", 8));
            }
        }
    }
}

void DonutBreakdownChart::removeBreakdownSeries(const std::string& name)
{   
    if(name == m_unusedMoneySeries->name().toStdString()) // can't remove unusedMoneySeries
        return;

    auto iterSlices = m_mainSeries->slices().begin();
    while(iterSlices != m_mainSeries->slices().end())
    {
        if(static_cast<DonutBreakdownMainSlice*>(*iterSlices)->name().toStdString() == name)
        {
            break;
        }
        iterSlices++;
    }
    if(iterSlices == m_mainSeries->slices().end())
    {
        throw std::domain_error{"Slice Not Found"};
    }
    
    auto delete_target = static_cast<DonutBreakdownMainSlice*>((*iterSlices))->breakdownSeries();

    // Slice is removed first to keep iterSlice valid, if series is deleted first, iterator can be invalid!
    bool return_value = m_mainSeries->remove(*iterSlices);
    QChart::removeSeries(delete_target);

    if(false == return_value)
        throw std::domain_error{"Cannot delete Slice"};
    recalculateAngles();
    updateLegendMarkers();
}

