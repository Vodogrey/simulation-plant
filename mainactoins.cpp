#include "mainactoins.h"


MainActions::MainActions(QObject *parent) : QObject(parent)
{
    generator1 = new GenerateDetals(15);
    generator2 = new GenerateDetals(6);
    generator3 = new GenerateDetals(10);

    m_generators << generator1;
    m_generators << generator2;
    m_generators << generator3;

    queue = new CountDetals();

    stage1 = new ConnectDetals(15,3, *queue);
    stage2 = new ConnectDetals(18, 3, *queue);
    stage3 = new ConnectDetals(5, 2, *queue);
    stage4 = new ConnectDetals(10, 2, *queue);
    stage5 = new ConnectDetals(15, 4, *queue);
    stage6 = new ConnectDetals(20, 5, *queue);

    m_connections << stage1;
    m_connections << stage2;
    m_connections << stage3;
    m_connections << stage4;
    m_connections << stage5;
    m_connections << stage6;

    m_processSpeed = 500;
}

MainActions::~MainActions()
{
    delete generator1;
    delete generator2;
    delete generator3;
}

void MainActions::Process(int countDetals)
{
    GenerateDetals* tempGen;
    QChar tempDetalType = 'A';

    for(int countGen = 0; countGen < m_generators.size(); countGen++) {
        tempGen = m_generators.at(countGen);
        tempGen->setCountDetals(countDetals);
    }
    bool crutch = true;

    while((isCanWork() && !m_stop) || crutch)
    {
        if(!isCanWork())
            crutch = false;

        for(int countGen = 0; countGen < m_generators.size(); countGen++) {
            tempGen = m_generators.at(countGen);
            tempDetalType = 'A' + countGen;
            if(tempGen->isCanGenerate())
                if(tempGen->isTime()) {
                    queue->add(tempGen->GetRequest(tempDetalType));
                }
        }

        for(int i = 1; i < 7;i++) {
            stage(i);
        }

        emit updateStats();
        queue->addTime();
        QTimer::singleShot(m_processSpeed,&loop,SLOT(quit()));
        loop.exec();
    }
    if(!isCanWork()) {
        emit workEnded();
    }
}




bool MainActions::isAvaliable(QChar detal, int count)
{
    Request rec;
    rec.type = detal;
    int num = 0;

    for( int pos = 0 ; pos < queue->getSize(); ++pos) {
        if (queue->at(pos)->type == rec.type) {
            num++;
        }
    }

    if(num >= count) {
        return true;
    }
    return false;
}


void MainActions::stage(int stageNum)
{
    switch (stageNum) {
    case 1:
        if(stage1->isEnded()) {
            queue->add(stage1->GetRequest('d'));
            stage1->setEnd(false);
            stage1->setFree(true);
        }
        if(stage1->isFree()) {
            if(isAvaliable('A',1) && isAvaliable('B', 2)) {
                stage1->setFree(false);
                stage1->connecting("ABB");
            }
        }
        break;
    case 2:
        if(stage2->isEnded()) {
            queue->add(stage2->GetRequest('e'));
            stage2->setEnd(false);
            stage2->setFree(true);
        }
        if(stage2->isFree()) {
            if(isAvaliable('A') && isAvaliable('B') && isAvaliable('C')) {
                stage2->setFree(false);
                stage2->connecting("ABC");
            }
        }
        break;
    case 3:
        if(stage3->isEnded()) {
            queue->add(stage3->GetRequest('D'));
            queue->add(stage3->GetRequest('E'));
            stage3->setEnd(false);
            stage3->setFree(true);
        }
        if(stage3->isFree()) {
            if(isAvaliable('d') && isAvaliable('e')) {
                stage3->setFree(false);
                stage3->connecting("de");
            }
        }
        break;
    case 4:
        if(stage4->isEnded()) {
            queue->add(stage4->GetRequest('F'));
            stage4->setEnd(false);
            stage4->setFree(true);
        }
        if(stage4->isFree()) {
            if(isAvaliable('D') && isAvaliable('B')) {
                stage4->setFree(false);
                stage4->connecting("DB");
            }
        }
        break;
    case 5:
        if(stage5->isEnded()) {
            queue->add(stage5->GetRequest('G'));
            stage5->setEnd(false);
            stage5->setFree(true);
        }
        if(stage5->isFree()) {
            if(isAvaliable('F') && isAvaliable('E')) {
                stage5->setFree(false);
                stage5->connecting("FE");

            }
        }
        break;
    case 6:
        if(stage6->isEnded()) {
            queue->add(stage6->GetRequest('H'));
            m_resultsInSystem << queue->at(queue->getSize()-1)->systemTime;
            m_resultsInConnections <<queue->at(queue->getSize()-1)->procTime;
            stage6->setEnd(false);
            stage6->setFree(true);
        }
        if(stage6->isFree()) {
            if(isAvaliable('G') && isAvaliable('B') && isAvaliable('C',2)) {
                stage6->setFree(false);
                stage6->connecting("GBCC");
            }
        }
        break;
    default:
        break;
    }
}

int MainActions::slot_getCountDetals(QChar detal)
{
    Request rec;
    rec.type = detal;
    int num = 0;

    for( int pos = 0 ; pos < queue->getSize(); ++pos) {
        if (queue->at(pos)->type == rec.type) {
            num++;
        }
    }
    return num;
}

QString MainActions::slot_getProcessTime(QChar type, int number)
{
    QString time;
    if(type == 'G') {
        if(!m_generators[number]->isCanGenerate()) {
            time = "Нет поступлений";
            return time;
        }
        else
        {
            time = QString("%1 минут").arg(m_generators[number]->getProcessTime());
            return time;
        }
    }

    if(type == 'C') {
        if(m_connections[number]->getConnectTime() == 0) {
            time = "Не изготавляется";
            return time;
        }
        else {
            time = QString("будет готова через: %1 минут").arg(m_connections[number]->getConnectTime());
            return time;
        }
    }

    return "-1";
}

QString MainActions::slot_getResult()
{
    if(!m_resultsInConnections.isEmpty()) {
    QString time;
    int averageInSystem = 0;
    int averageInProc = 0;
    for(int i = 0; i < m_resultsInSystem.size(); i++) {
        averageInSystem += m_resultsInSystem.at(i);
        averageInProc += m_resultsInConnections.at(i);
    }
    averageInSystem /= m_resultsInConnections.size();
    averageInProc /= m_resultsInConnections.size();

    time = QString("Деталь изготовлялась %1").arg(m_resultsInSystem[m_resultsInSystem.size()-1]) +
           QString("\nИз них обрабатывалась %1").arg(m_resultsInConnections[m_resultsInConnections.size()-1]) +
            QString("\nВ среднем эти значения равны %1 и %2 соответственно").arg(averageInSystem).arg(averageInProc);
    return time;
    }
    return "";
}

bool MainActions::isCanWork()
{

    for(int i = 0;i < m_generators.size();i++)
    {
        if(m_generators.at(i)->isCanGenerate())
            return true;
    }

    for(int i = 0; i < m_connections.size(); i++) {
        if(m_connections[i]->getConnectTime() != 0)
            return true;
    }
    return false;
}

void MainActions::clear()
{
    m_resultsInConnections.clear();
    m_resultsInSystem.clear();
    queue->clear();
    for(int i = 0; i<m_generators.size(); i++)
        m_generators.at(i)->clear();
    for(int i = 0;i < m_connections.size();i++)
        m_connections.at(i)->clear();
}

void MainActions::setStop(bool stop)
{
    m_stop = stop;
}

void MainActions::setSpeed(int speed)
{
 loop.quit();
 m_processSpeed = speed;
}
