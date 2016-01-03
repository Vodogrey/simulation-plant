#include "generatedetals.h"

GenerateDetals::GenerateDetals(int time)
{
setCentralTime(time);
setCalcTime(0);
}

bool GenerateDetals::isTime()
{
     qDebug() << "!TIME" << m_waitTime;
    if(--m_waitTime <= 0) {
        CalcTime();
        return true;
    }
    return false;
}

void GenerateDetals::setCalcTime(int time)
{
  m_waitTime = time;
}

//ниже нет бред коняшек

void GenerateDetals::CalcTime()
{
        if(m_waitTime <= 0) {
            int dispersion = qrand() % ((getCentralTime()/3) * 2) + 1 - getCentralTime()/3;
            m_waitTime = getCentralTime() + dispersion;
            qDebug() << "generator time" << m_waitTime << "central time" << getCentralTime() << "dispersion" << dispersion;
        }
}

//нужен тип детали

Request* GenerateDetals::GetRequest(char typeDetal)
{
    Request* request = new Request;
    request->procTime=0;
    request->systemTime=0;
    request->type=typeDetal; // тип детали
    //qDebug() << "type" << request->type;
    CalcTime();
    return request;
}

int GenerateDetals::getCentralTime()
{
   return m_centralTime;
}

void GenerateDetals::setCentralTime(int time)
{
    m_centralTime = time;
}
