#ifndef A_STAR_H
#define A_STAR_H



#include <vector>
#include <QString>

using namespace std;

typedef struct {
    vector<QString> path;
    unsigned int estimateCost;
    unsigned int knownCost;
    QString key;



}NODE;

vector<QString> A_star(QString startStatus,unsigned int len,unsigned int N);
unsigned int computeManhattanDistance(QString startStatus,unsigned int N,unsigned int len);
QString exchange(QString str,unsigned int x,unsigned int y);
bool isInList(QString str,vector<NODE> list);
vector<QString> findNeighbors(QString status, unsigned int len,unsigned int N);
void nodeDeepCopy(NODE* dst,NODE* src);

#endif // A_STAR_H
