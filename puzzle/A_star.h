#ifndef A_STAR_H
#define A_STAR_H



#include <vector>
#include <QString>

using namespace std;

typedef struct Node{
    Node* father;
    unsigned int estimateCost;
    unsigned int knownCost;
    QString key;

}NODE;

QString A_star(QString startStatus,unsigned int len,unsigned int N);
unsigned int computeManhattanDistance(QString startStatus,unsigned int N,unsigned int len);
QString exchange(QString str,unsigned int x,unsigned int y);
bool isInList(QString str,vector<NODE> list);

#endif // A_STAR_H
