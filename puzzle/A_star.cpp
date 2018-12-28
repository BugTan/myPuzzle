#include "A_star.h"
#define INF 999999999

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

    do{
        //find the node with min F value in openlist
        int FValue = INF,tmp;
        int cur=0;
        for(unsigned int i=0; i < openList.size();i++){
             tmp = openList[i].estimateCost+openList[i].knownCost;
             if(FValue > tmp){
                 FValue = tmp;
                 cur = i;
             }
        }

        //remove current node and put it in closed list
        closeList.push_back(openList[cur]);
        NODE curNode;
        nodeDeepCopy(&curNode,&openList[cur]);
        //memcpy(&curNode, &openList[cur], sizeof(NODE));//deep copy就用这么一句就搞定了，呵呵
        openList.erase(openList.begin()+cur);

        //find neighbor
        vector<DYNAMICSTR> neighborStr = findNeighbors(curNode.key,len, N);

        for(unsigned int i=0; i < neighborStr.size();i++){
            if(isInList(neighborStr[i],closeList))
                continue;
            if(isInList(neighborStr[i],openList)){
                unsigned int inx;
                for(inx=0; inx < openList.size();inx++){
                    if(neighborStr[i].value==openList[inx].key.value)
                        break;
                }

                //current known path is short, so change previous path
                if(openList[inx].knownCost > curNode.knownCost){
                    openList[inx].path.clear();
                    openList[inx].path.assign(curNode.path.begin(), curNode.path.end());
                    openList[inx].path.push_back(neighborStr[i]);
                    openList[inx].estimateCost = computeManhattanDistance(openList[inx].key,N,len);
                    openList[inx].knownCost = curNode.knownCost + 1;

                }
            }
            if(!isInList(neighborStr[i],openList)){
                NODE neighborNode;
                //neighborNode.key = neighborStr[i];
                dynamicStrDeepCopy(&neighborNode.key,&neighborStr[i]);
                neighborNode.path.clear();
                neighborNode.path.assign(curNode.path.begin(), curNode.path.end());//将v2赋值给v1 但会清除掉v1以前的内容
                neighborNode.path.push_back(neighborStr[i]);
                neighborNode.estimateCost = computeManhattanDistance(neighborStr[i],N,len);
                neighborNode.knownCost = curNode.knownCost + 1 ;
                openList.push_back(neighborNode);
            }

        }

    }while(!isInList(targetStatus,openList));
    unsigned int i;
    for(i=0; i < openList.size();i++){
        if(targetStatus.value==openList[i].key.value)
            break;
    }

    return openList[i].path;
}

unsigned int computeManhattanDistance(DYNAMICSTR startStatus,unsigned int N,unsigned int len){

    unsigned int estimateCost = 0;
    for(unsigned int i = 0;i < len;i++){
        if(readDynamicStrValue(startStatus,i) == QString::number(len - 1) )
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
        if(QString::number(readDynamicStrValue(status,i)) == QString::number(len-1))
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

bool isInList(DYNAMICSTR str,vector<NODE> list){
    bool flag = false;
    for(unsigned int i=0; i < list.size();i++){
        if(str.value==list[i].key.value){
            flag = true;
            break;
            }
    }
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
