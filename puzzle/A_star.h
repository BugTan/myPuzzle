#ifndef A_STAR_H
#define A_STAR_H



#include <vector>
#include <QString>

using namespace std;


typedef struct DynamicStr{
    QString value;
    QString lenDescription;

    bool operator == (const DynamicStr &a) const{
        //按score由小到大排列，如果要由大到小排列，使用“>”即可；
        bool flag = true;
        for(int i = 0 ; i<lenDescription.length();i++){

            unsigned int startA = 0;
            unsigned int start = 0;
            for(int j = 0; j < i;j++){
                start +=  lenDescription.mid(j,1).toInt();
                startA += a.lenDescription.mid(j,1).toInt();
            }

            if(a.value.mid(startA,a.lenDescription.mid(i,1).toInt()) !=
                    value.mid(start,lenDescription.mid(i,1).toInt())){
                flag = false;
                break;
            }

        }
        return flag;
    }
}DYNAMICSTR;

typedef struct Flag{
    bool flag;
    unsigned int pos;




}RETURNFLAG;

typedef struct Node{
    vector<DYNAMICSTR> path;
    unsigned int estimateCost;
    unsigned int knownCost;
    DYNAMICSTR key;

}NODE;

vector<DYNAMICSTR> A_star(DYNAMICSTR startStatus,unsigned int len,unsigned int N);
unsigned int computeManhattanDistance(DYNAMICSTR startStatus,unsigned int N,unsigned int len);
DYNAMICSTR exchange(DYNAMICSTR str,unsigned int x,unsigned int y,unsigned int len);
RETURNFLAG isInList(DYNAMICSTR str,vector<NODE> list);
vector<DYNAMICSTR> findNeighbors(DYNAMICSTR status, unsigned int len,unsigned int N);
void nodeDeepCopy(NODE* dst,NODE* src);
void dynamicStrDeepCopy(DYNAMICSTR* dst,DYNAMICSTR* src);
unsigned int readDynamicStrValue(DYNAMICSTR status,unsigned int readPos);


#endif // A_STAR_H
