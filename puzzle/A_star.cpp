#include "A_star.h"
#define INF 999999999

//#include <iostream>
//#include <queue>

//bool ascendSort(NODE a,NODE b){
//    return (a.estimateCost + a.knownCost < b.estimateCost +b.knownCost);
//}
//template<typename T> vector<T> vectorPush(vector<T> vec, T TObject){
//    vec.push_back(TObject);
//    sort(vec.begin(),vec.end(),ascendSort);
//    return vec;
//}

//template<typename T> vector<T> vectorErase(vector<T> vec, unsigned int pos){
//    vec.erase(vec.begin()+pos);
//    sort(vec.begin(),vec.end(),ascendSort);
//    return vec;
//}

vector<DYNAMICSTR> A_star(DYNAMICSTR startStatus,unsigned int len,unsigned int N){

    DYNAMICSTR targetStatus;
   // vector<QString> path;

    //target status
    for(unsigned int i=0; i < len;i++){
            targetStatus.value += QString::number(i);
            targetStatus.lenDescription += QString::number(QString::number(i).length());
    }

    vector<NODE> openList;
    vector<NODE> closeList;

    NODE startPoint;
    startPoint.path.push_back(startStatus);
    startPoint.estimateCost = INF;
    startPoint.knownCost = 0 ;
    //startPoint.key = startStatus;
    dynamicStrDeepCopy(&startPoint.key,&startStatus);

    openList.push_back(startPoint);
    //openList = vectorPush(openList,startPoint);

    //unsigned int cur=0;
    //unsigned int times=0;

    do{
        //times++;
        //find the node with min F value in openlist
        int FValue = INF,tmp;
        unsigned int cur=0;
        for(unsigned int i=0; i < openList.size();i++){
             tmp = openList[i].estimateCost+openList[i].knownCost;
             if(FValue > tmp){
                 FValue = tmp;
                 cur = i;
             }
        }
        //cout<<"serch time:"<<times<<" openList size:"<<openList.size()<<" closeList size:"<<closeList.size()<<
        //   " Path size:"<<openList[cur].path.size()<<endl;

        //remove current node and put it in closed list
        closeList.push_back(openList[cur]);

        NODE curNode;
        nodeDeepCopy(&curNode,&openList[cur]);
        //memcpy(&curNode, &openList[cur], sizeof(NODE));//deep copy就用这么一句就搞定了，呵呵
        openList.erase(openList.begin()+cur);
        //openList = vectorErase(openList,cur);
        //cout<<"openList size erased:"<<openList.size()<<endl;

        //find neighbor
        vector<DYNAMICSTR> neighborStr = findNeighbors(curNode.key,len, N);

        for(unsigned int i=0; i < neighborStr.size();i++){
            if(isInList(neighborStr[i],closeList).flag)
                continue;
            RETURNFLAG flag = isInList(neighborStr[i],openList);
            if(flag.flag){

                //current known path is short, so change previous path
                if(openList[flag.pos].knownCost > curNode.knownCost +1){
                    openList[flag.pos].path.clear();
                    openList[flag.pos].path.assign(curNode.path.begin(), curNode.path.end());
                    openList[flag.pos].path.push_back(neighborStr[i]);
                    openList[flag.pos].estimateCost = computeManhattanDistance(openList[flag.pos].key,N,len);
                    openList[flag.pos].knownCost = curNode.knownCost + 1;

                }
                break;
            }
            else{
                NODE neighborNode;
                //neighborNode.key = neighborStr[i];
                dynamicStrDeepCopy(&neighborNode.key,&neighborStr[i]);
                neighborNode.path.clear();
                neighborNode.path.assign(curNode.path.begin(), curNode.path.end());//将v2赋值给v1 但会清除掉v1以前的内容
                neighborNode.path.push_back(neighborStr[i]);
                neighborNode.estimateCost = computeManhattanDistance(neighborStr[i],N,len);
                neighborNode.knownCost = curNode.knownCost + 1 ;
                openList.push_back(neighborNode);
                //openList = vectorPush(openList,neighborNode);

            }

        }

    }while(!isInList(targetStatus,openList).flag);
    unsigned int i;
    for(i=0; i < openList.size();i++){
        if(targetStatus==openList[i].key)
            break;
    }
   // targetStatus==openList[i].key;

    return openList[i].path;
}

unsigned int computeManhattanDistance(DYNAMICSTR startStatus,unsigned int N,unsigned int len){

    unsigned int estimateCost = 0;
    for(unsigned int i = 0;i < len;i++){
        if(readDynamicStrValue(startStatus,i) == len - 1 )
            continue;
        estimateCost += abs(int(readDynamicStrValue(startStatus,i)/N - i/N))
                +abs(int(readDynamicStrValue(startStatus,i)%N - i%N));

    }
    return 5*estimateCost;

}

vector<DYNAMICSTR> findNeighbors(DYNAMICSTR status, unsigned int len,unsigned int N){
    vector<DYNAMICSTR> neighbors;

  //  unsigned int pos = status.indexOf(QString::number(len-1));
    unsigned int i;
    for(i=0; i < len;i++ ){
        if(readDynamicStrValue(status,i) == len-1)
            break;
    }
    unsigned int pos = i;
    unsigned int row = pos/N;
    unsigned int col = pos%N;

    if(row > 0 && row < len/N -1 && col > 0 && col < N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));
    }

    else if(row == 0 && col == 0){
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));

        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));

    }

    else if(row == 0 && col == N-1){
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));

        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));

    }

    else if(row == len/N -1 && col == 0){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));
    }
    else if(row == len/N -1 && col == N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));
    }


    else if(row == 0 && col > 0 && col < N-1){
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));
    }
    else if(row == len/N -1 && col > 0 && col < N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));
    }
    else if(row > 0 && row < len/N -1 && col ==0){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos,len));
    }
    else if(row > 0 && row < len/N -1 && col == N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos,len));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos,len));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos,len));
    }

    return neighbors;


}

DYNAMICSTR exchange(DYNAMICSTR str,unsigned int x,unsigned int y,unsigned int len){
    DYNAMICSTR tmp;

    for(unsigned int i= 0; i < len; i++){
        if(i==x){
            tmp.value += QString::number(readDynamicStrValue(str,y));
            tmp.lenDescription += QString::number(QString::number(readDynamicStrValue(str,y)).length());
        }
        else if(i == y){
            tmp.value += QString::number(readDynamicStrValue(str,x));
            tmp.lenDescription += QString::number(QString::number(readDynamicStrValue(str,x)).length());
        }
        else{
            tmp.value += QString::number(readDynamicStrValue(str,i));
            tmp.lenDescription += QString::number(QString::number(readDynamicStrValue(str,i)).length());
        }
    }

    return tmp;

}

RETURNFLAG isInList(DYNAMICSTR str,vector<NODE> list){
    RETURNFLAG flag;
    flag.flag = false;
    unsigned int i;
    for(i=0; i < list.size();i++){
        if(str==list[i].key){
            flag.flag = true;
            break;
            }
    }
    flag.pos = i;
    return flag;
}


void nodeDeepCopy(NODE* dst,NODE* src){
    dst->estimateCost = src->estimateCost;
    dst->knownCost = src->knownCost;
    dst->key = src->key;
    dst->path.clear();
    for(unsigned int i= 0; i < src->path.size();i++){      
        dst->path.push_back(src->path[i]);
        //dynamicStrDeepCopy(,&src->path[i]);

    }
}

void dynamicStrDeepCopy(DYNAMICSTR* dst,DYNAMICSTR* src){
    dst->value = src->value;
    dst->lenDescription = src->lenDescription;

}


unsigned int readDynamicStrValue(DYNAMICSTR status,unsigned int readPos){

    unsigned int start = 0;

    for(unsigned int i = 0; i < readPos;i++){
        start +=  status.lenDescription.mid(i,1).toInt();
    }

    return status.value.mid(start,status.lenDescription.mid(readPos,1).toInt()).toInt();

}




