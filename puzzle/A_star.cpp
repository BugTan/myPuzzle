#include "A_star.h"
#define INF 999999999999

QString A_star(QString startStatus,unsigned int len,unsigned int N){

    QString targetStatus;
    QString path;

    //target status
    for(unsigned int i=0; i < len;i++){
        targetStatus += QString::number(i);
    }

    vector<NODE> openList;
    vector<NODE> closeList;

    NODE startPoint;
    startPoint.father = NULL;
    startPoint.estimateCost = computeManhattanDistance(startStatus,N,len);
    startPoint.knownCost = 0 ;
    startPoint.key = startStatus;

    openList.push_back(startPoint);

    unsigned int currentMinCost = startPoint.knownCost + startPoint.estimateCost;

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
        NODE curNode = openList[cur];
        openList.erase(openList.begin()+cur);

        //find neighbor
        vector<QString> neighborStr = findNeighbors(openList[cur].key,len, N);

        for(unsigned int i=0; i < neighborStr.size();i++){

            if(!isInList(neighborStr[i],openList)){
                NODE neighborNode;
                neighborNode.key = neighborStr[i];
                neighborNode.path.push_back(curNode);
                neighborNode.estimateCost = computeManhattanDistance(neighborStr[i],N,len);
                neighborNode.knownCost = curNode.knownCost + 1 ;
                openList.push_back(neighborNode);
            }
            if(isInList(neighborStr[i],openList)){
                for(unsigned int inx=0; inx < openList.size();inx++){
                    if(neighborStr[inx]==openList[inx].key)
                        break;
                }
                if(openList[inx].knownCost+ openList[inx].estimateCost < currentMinCost){
                    openList[inx].path

                }
            }

        }




    }while();



    return path;
}

unsigned int computeManhattanDistance(QString startStatus,unsigned int N,unsigned int len){

    unsigned int estimateCost = 0;
    for(unsigned int i = 0;int(i) < startStatus.length();i++){
        if(startStatus == QString::number(len - 1) )
            continue;
        estimateCost += labs((startStatus.mid(i,1)).toInt()/N - i/N) +
                labs(startStatus.mid(i,1).toInt()%N - i%N);

    }
    return 5*estimateCost;

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
        neighbors.push_back(exchange(status,row+*N+col+1,pos));
    }

    else if(row == 0 && col == 0){
        //right
        neighbors.push_back(exchange(status,row+*N+col+1,pos));

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
        neighbors.push_back(exchange(status,row+*N+col+1,pos));
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
        neighbors.push_back(exchange(status,row+*N+col+1,pos));
    }
    else if(row == len/N -1 && col > 0 && col < N-1){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //left
        neighbors.push_back(exchange(status,row*N+col-1,pos));
        //right
        neighbors.push_back(exchange(status,row+*N+col+1,pos));
    }
    else if(row > 0 && row < len/N -1 && col ==0){
        //up
        neighbors.push_back(exchange(status,(row-1)*N+col,pos));
        //down
        neighbors.push_back(exchange(status,(row+1)*N+col,pos));
        //right
        neighbors.push_back(exchange(status,row+*N+col+1,pos));
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
    str.mid(x,1) = str.mid(y,1);
    str.mid(pos,1) = tmp;

    return str;

}

bool isInList(QString str,vector<NODE> list){

    for(unsigned int i=0; i < list.size();i++){
        if(str==list[i].key)
            break;
    }
    if(str.isEmpty())
        return false;
    else
        return true;

}

