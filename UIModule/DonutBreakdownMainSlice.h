#ifndef DonutBreakdownMainSlice_H
#define DonutBreakdownMainSlice_H

#include <QPieSlice>
#include <QPieSeries>


class DonutBreakdownMainSlice : public QPieSlice
{
    Q_OBJECT
public:    
    DonutBreakdownMainSlice(QPieSeries *breakdownSeries, QObject *parent);
    void updateLabel();
    void setName(QString name)
    {
        m_name = name;
    }
    QString name()
    {
        return m_name;
    }
    
    QPieSeries* breakdownSeries()
    {
        return m_breakdownSeries;
    }
private:
    QPieSeries* m_breakdownSeries;
    QString m_name{"test"};
};


#endif //DonutBreakdownMainSlice_H
