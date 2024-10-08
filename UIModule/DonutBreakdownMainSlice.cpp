#include <UIModule/DonutBreakdownMainSlice.h>


DonutBreakdownMainSlice::DonutBreakdownMainSlice(QPieSeries *breakdownSeries, QObject *parent)
    : QPieSlice(parent),
      m_breakdownSeries(breakdownSeries)
{
    connect(this, &DonutBreakdownMainSlice::percentageChanged,
            this, &DonutBreakdownMainSlice::updateLabel);
}

// Understand these shit
void DonutBreakdownMainSlice::updateLabel()
{
    setLabel(QString("%1 %2%").arg(m_name).arg(percentage() * 100, 0, 'f', 2));
}