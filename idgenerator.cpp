#include "idgenerator.h"
#include "QDebug"
IDGenerator::IDGenerator()
{
id=0;
}
int IDGenerator::newproduct()
{

    return newid();
}

int IDGenerator::generateID()
{
    static int s_id = 0;
    return ++s_id;
}
int IDGenerator::newid()
{
return 0;
}

void IDGenerator::printdebugid()
{
    qDebug()<<id;
}
