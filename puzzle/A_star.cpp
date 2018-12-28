#include "A_star.h"
#define INF 999999999

vector<QString> A_star(QString startStatus,unsigned int len,unsigned int N){

    QString targetStatus;
   // vector<QString> path;

    //target status
    for(unsigned int i=0; i < len;i++){
        targetStatus += QString::number(i);
    }

    vector<NODE> openList;
    vector<NODE> closeList;

    NODE startPoint;
    startPoint.path.push_back(startStatus);
    startPoint.estimateCost = INF;
    startPoint.knownCost = 0 ;
    startPoint.key = startStatus;

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
        vector<QString> neighborStr = findNeighbors(curNode.key,len, N);

        for(unsigned int i=0; i < neighborStr.size();i++){
            if(isInList(neighborStr[i],closeList))
                continue;

            if(!isInList(neighborStr[i],openList)){
                NODE neighborNode;
                neighborNode.key = neighborStr[i];
                neighborNode.path.clear();
                neighborNode.path.assign(curNode.path.begin(), curNode.path.end());//将v2赋值给v1 但会清除掉v1以前的内容
                neighborNode.path.push_back(neighborStr[i]);
                neighborNode.estimateCost = computeManhattanDistance(neighborStr[i],N,len);
                neighborNode.knownCost = curNode.knownCost + 1 ;
                openList.push_back(neighborNode);
            }
            if(isInList(neighborStr[i],openList)){
                unsigned int inx;
                for(inx=0; inx < openList.size();inx++){
                    if(neighborStr[i]==openList[inx].key)
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

        }

    }while(!isInList(targetStatus,openList));
    unsigned int i;
    for(i=0; i < openList.size();i++){
        if(targetStatus==openList[i].key)
            break;
    }

    return openList[i].path;
}

unsigned int computeManhattanDistance(QString startStatus,unsigned int N,unsigned int len){

    unsigned int estimateCost = 0;
    for(unsigned int i = 0;int(i) < startStatus.length();i++){
        if(startStatus.mid(i,1) == QString::number(len - 1) )
            continue;
        estimateCost += abs(int(startStatus.mid(i,1).toInt()/N - i/N))
                +abs(int(startStatus.mid(i,1).toInt()%N - i%N));

    }
    return 2*estimateCost;

}

vector<QString> findNeighbors(QString status, unsigned int len,unsigned int N){
    vector<QString> neighbors;

    unsigned int pos = status.indexOf(QString::number(len-1));
    unsigned int row = pos/N;
    unsigned int col = pos%N;

    if(row > 0 && row < len/N -1 && col > 0 && col < N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));
    }

    else if(row == 0 && col == 0){
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));

        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));

    }

    else if(row == 0 && col == N-1){
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));

        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));

    }

    else if(row == len/N -1 && col == 0){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));
    }
    else if(row == len/N -1 && col == N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
    }


    else if(row == 0 && col > 0 && col < N-1){
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));
    }
    else if(row == len/N -1 && col > 0 && col < N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));
    }
    else if(row > 0 && row < len/N -1 && col ==0){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));
        //right
        neighbors.push_back(exchange(status,row*N+col+1,pos));
    }
    else if(row > 0 && row < len/N -1 && col == N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));
    }

    return neighbors;


}

QString exchange(QString str,unsigned int x,unsigned int y){
    QString tmp;

    tmp = str.mid(x,1);
    str.replace(x,1,str.mid(y,1));
    str.replace(y,1,tmp);
    str.mid(y,1) = tmp;

    return str;

}

bool isInList(QString str,vector<NODE> list){
    bool flag = false;
    for(unsigned int i=0; i < list.size();i++){
        if(str==list[i].key){
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
    for(unsigned int i= 0; i < src->path.size();i++){
        dst->path.push_back(src->path[i]);

    }
}

