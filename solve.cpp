#include "solve.h"

solve::solve(QObject *parent)
    : QObject{parent}
{}

QString solve::num_calculate(const QString& str)
{
    return QString::fromStdString(getResult(str.toStdString()));
}

bool solve::isResult()
{
    return m_isResult;
}

void solve::setIsResult(bool val)
{
    m_isResult = val;
}
