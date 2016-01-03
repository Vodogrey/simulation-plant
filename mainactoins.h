#ifndef MAINACTOINS_H
#define MAINACTOINS_H

#include <QObject>

#include "generatedetals.h"
#include "countdetals.h"
#include "connect.h"

class MainActoins : public QObject
{
    Q_OBJECT
public:
    explicit MainActoins(QObject *parent = 0);
    bool stage(char stage);


signals:

public slots:
    void Process();

private:
    bool canConnect;
    GenerateDetals* generator;
    GenerateDetals* generator2;
    GenerateDetals* generator3;

    CountDetals* queue;
};

#endif // MAINACTOINS_H
