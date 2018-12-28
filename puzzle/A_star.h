#ifndef A_STAR_H
#define A_STAR_H



#include <vector>
#include <QString>

using namespace std;

typedef struct{
    QString value;
    QString lenDescription;

}DYNAMICSTR;

typedef struct {
    vector<DYNAMICSTR> path;
    unsigned int estimateCost;
    unsigned int knownCost;
    DYNAMICSTR key;
}NODE;

vector<DYNAMICSTR> A_star(DYNAMICSTR startStatus,unsigned int len,unsigned int N);
unsigned int computeManhattanDistance(DYNAMICSTR startStatus,unsigned int N,unsigned int len);
DYNAMICSTR exchange(DYNAMICSTR str,unsigned int x,unsigned int y,unsigned int len);
bool isInList(DYNAMICSTR str,vector<NODE> list);
vector<DYNAMICSTR> findNeighbors(DYNAMICSTR status, unsigned int len,unsigned int N);
void nodeDeepCopy(NODE* dst,NODE* src);
void dynamicStrDeepCopy(DYNAMICSTR* dst,DYNAMICSTR* src);
unsigned int readDynamicStrValue(DYNAMICSTR status,unsigned int readPos);
#endif // A_STAR_H
