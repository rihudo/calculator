#include "solve.h"
#include <QDebug>

solve::solve(QObject *parent)
    : QObject{parent},
    keys_map{
               {0x28, "("},
          {0x29, ")"},
          {0x2e, "."},
          {0x2f, "/"},
          {0x2b, "+"},
          {0x2d, "-"},
          {0x2a, "x"},
          {88, "x"},
          {0x01000003, "DEL"},
          {0x1000004, "="}

    }
{}

QString solve::num_calculate(const QString& str)
{
    return QString::fromStdString(getResult(str.toStdString()));
}

QString solve::get_mapping_str(int num)
{
    if (keys_map.count(num))
    {
        return keys_map[num];
    }
    return "";
}

bool solve::isResult()
{
    return m_isResult;
}

void solve::setIsResult(bool val)
{
    m_isResult = val;
}
