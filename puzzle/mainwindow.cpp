#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QPainter>
#include <QKeyEvent>
#include "QMessageBox"
#include <QImageWriter>
#include <math.h>
#include <QInputDialog>
#include "A_star.h"

#include <QTime>


#define FULL_W 180*4
#define FULL_H 160*4
//#define SMALL_W 180
//#define SMALL_H 140
#define PHOTO_X 20
#define PHOTO_Y 80


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);

    //creat timer
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(seconds()));
    sourceImage = NULL;

}


void MainWindow::seconds(){
    tim++;
    ui->label_5->setText(QString::number(tim));
}


void MainWindow::cutImage(){

    for(unsigned int i = 0;i < totalPatch; i++){
        QImage temp = sourceImage->scaled(SMALL_W*N,SMALL_H*M);
        pImage[i] = temp.copy((i%N)*SMALL_W,(i/N)*SMALL_H,SMALL_W,SMALL_H);
        plbImage[i]->setPixmap(QPixmap::fromImage(pImage[i]));
        pCompare[i/N][i%N] = i;

    }
    QPixmap tep(":/blank.jpg");
    plbImage[totalPatch-1]->setPixmap(tep);
}


void MainWindow::moveImage(){
    for(unsigned int i=0; i < totalPatch; i++){
        int index = pCompare[i/N][i%N];
        plbImage[index]->move(PHOTO_X+i%N*SMALL_W,PHOTO_Y+i/N*SMALL_H);
    }

}

void MainWindow::random(){
    for(unsigned int i=0;i<(qrand()%totalPatch * 10);i++){

        int firstRand = qrand()%totalPatch;
        int secondRand;
        int temp;
        while(true){
            secondRand = qrand()%totalPatch;
            if(secondRand != firstRand)
                break;
        }
        temp = pCompare[firstRand/N][firstRand%N];
        pCompare[firstRand/N][firstRand%N] = pCompare[secondRand/N][secondRand%N];
        pCompare[secondRand/N][secondRand%N] = temp;

    }
    moveImage();
}



void MainWindow::keyPressEvent(QKeyEvent *event){
    if(timer->isActive()==false){
        return;
    }
    unsigned int row = 0;
    unsigned int col = 0;
    int tmp;
    for(unsigned int j=0;j < totalPatch; j++){
        if( pCompare[j/N][j%N] == totalPatch-1){
            row = j/N;
            col = j%N;
            break;
        }
    }
    switch(event->key()){

        //up
        case Qt::Key_W:{
            if(row < M-1){
                tmp = pCompare[row][col];
                pCompare[row][col] = pCompare[row+1][col];
                pCompare[row+1][col]=tmp;
            }

        }break;
        //down
        case Qt::Key_S:{
            if(row > 0){
                tmp = pCompare[row][col];
                pCompare[row][col] = pCompare[row-1][col];
                pCompare[row-1][col]=tmp;
            }

        }break;
        //left
        case Qt::Key_A:{
            if(col < N-1){
                tmp = pCompare[row][col];
                pCompare[row][col]=pCompare[row][col+1];
                pCompare[row][col+1]=tmp;
            }

        }break;
        //right
        case Qt::Key_D:{
            if(col > 0){
                tmp=pCompare[row][col];
                pCompare[row][col]=pCompare[row][col-1];
                pCompare[row][col-1] = tmp;
            }

        }break;

        //replay
        case Qt::Key_R:{
            if(sourceImage == NULL)
                return;
            tim = 0;
            ui->label_5->setText(QString::number(tim));
            if(timer->isActive()){
                timer->stop();
            }
            timer->start(1000);
            plbImage[totalPatch-1]->setPixmap(QPixmap(":/blank.jpg"));
            random();

        }break;

        //quit
        case Qt::Key_P:{
            this->close();

        }break;

        //demonstrate the game
        case Qt::Key_F:{
            //QString status,lenDescription;
            DYNAMICSTR status;
            for(unsigned int i= 0; i < totalPatch; i++){
                status.value += QString::number(pCompare[i/N][i%N]);
                status.lenDescription += QString::number(QString::number(pCompare[i/N][i%N]).length());
            }
            vector<DYNAMICSTR> path;
            A_star(path,status,totalPatch,N);
//            vector<QString> path;
//            path.push_back("1302");
//            path.push_back("3102");
//            path.push_back("0132");
//            path.push_back("0123");
            for(unsigned int i= 0; i < path.size(); i++){
                for(unsigned int j =0; j< totalPatch;j++){
                    pCompare[j/N][j%N] = readDynamicStrValue(path[i],j);
                }
                moveImage();
                delay(300);
            }
        }break;

        //save current image
        case Qt::Key_G:{

            QImage *saveImg = new QImage(N*SMALL_W,M*SMALL_H,QImage::Format_RGB32);
            QPainter painter(saveImg);
            for(unsigned int i=0; i<totalPatch;i++){
                if(pCompare[i/N][i%N] == totalPatch-1)
                    painter.drawPixmap(i%N*SMALL_W,i/N*SMALL_H,SMALL_W,SMALL_H,
                                       QPixmap(":/blank.jpg"));
                else
                    painter.drawPixmap(i%N*SMALL_W,i/N*SMALL_H,SMALL_W,SMALL_H,
                                   QPixmap::fromImage(pImage[pCompare[i/N][i%N]]));
            }


            QImageWriter writer("../puzzle_process.jpeg", "jpeg");//save image path and format
            writer.setQuality(100);
            if (writer.canWrite()){
                // 写入图片至文件AuthorLogo.jpeg
                writer.write(*saveImg);
                QMessageBox msgBox;
                  msgBox.setText("The image has been saved!");
                  msgBox.exec();
            }
            //delete saveImg;

        }break;

        //save current process
        case Qt::Key_I:{

            if(saveTimes < MAXSAVEPROCESS){
                bool isOK;
                QString savename = QInputDialog::getText(NULL, "Input Dialog",
                                                   "Please input your save name",
                                                   QLineEdit::Normal,
                                                   "your path",
                                                   &isOK);
                saveArray[saveTimes].saveName = savename;
                for(unsigned  int i=0; i < totalPatch;i++){
                    saveArray[saveTimes].array[i] = pCompare[i/N][i%N];
                }
                saveTimes += 1;
            }else{
                QMessageBox msgBox;
                  msgBox.setText("have save max progress!");
                  msgBox.exec();

            }


        }break;

        //load saved process
        case Qt::Key_O:{
            if(saveTimes==0){
                QMessageBox msgBox;
                  msgBox.setText("There is no saved progress!");
                  msgBox.exec();
            }else{
                bool isOK;
                QString serchname = QInputDialog::getText(NULL, "Input Dialog",
                                                   "Please input your serch name",
                                                   QLineEdit::Normal,
                                                   "serch name",
                                                   &isOK);
                for(unsigned int i = 0;i<saveTimes;i++){
                    if(saveArray[saveTimes-1].saveName == serchname){
                        for(unsigned int i = 0;i<totalPatch;i++)
                            pCompare[i/N][i%N] = saveArray[saveTimes-1].array[i];
                        moveImage();
                        break;
                    }

                }
            }

        }break;
    }
    moveImage();

    bool isFinish = true;

    for(unsigned int row=0;row<M;row++){
        for(unsigned int colum=0;colum<N;colum++){
            if(pCompare[row][colum] != row*N+colum){
                isFinish = false;
                break;
            }
        }
        if(isFinish == false){
            break;
        }

    }
    if(isFinish == true){
        plbImage[totalPatch-1]->setPixmap(QPixmap::fromImage(pImage[totalPatch-1]));
        timer->stop();

        QMessageBox msgBox;
          msgBox.setText("You are so great!");
          msgBox.exec();
          this->close();

    }

}

//Load Image
void MainWindow::on_loadImgBtn_clicked(){

    QString tmpname = QFileDialog::getOpenFileName(this,"select pictures","../","Images(*.png *.jpg *.bmp)");

    if(tmpname.isEmpty()){
        return;
    }
    strFileName = tmpname;
    if(sourceImage!=NULL){
        delete sourceImage;
        sourceImage = NULL;
    }
    sourceImage = new QImage(strFileName);
    QImage temp = sourceImage->scaled(ui->displayLabel->width(),ui->displayLabel->height());
    ui->displayLabel->setPixmap(QPixmap::fromImage(temp));
    //cutImage();
    //random();


}


//set difficulty
void MainWindow::on_Start_clicked(){

    if(sourceImage == NULL)
        return;

    saveTimes = 0;

    if(!dFlag){

        M = ui->lineEditM->text().toInt();
        N = ui->lineEditN->text().toInt();
        assert(M < MAXROW && N < MAXROW);

        totalPatch = M * N;

        SMALL_W = floor(FULL_W/N);
        SMALL_H = floor(FULL_H/M);
        for(unsigned int i = 0 ; i < totalPatch ; i++){
            plbImage[i] = new QLabel(this);
            plbImage[i]->setGeometry(0,0,SMALL_W,SMALL_H);
            plbImage[i]->move(PHOTO_X+SMALL_W*(i%N),PHOTO_Y+SMALL_H*(i/N));
            plbImage[i]->setFrameShape(QFrame::Box);
          //  plbImage[i]->setPixmap(QPixmap(":/blank.jpg"));
            pCompare[i/N][i%N]=i;
            plbImage[i]->show();
        }
        dFlag = true;

        cutImage();
        random();

        //check whether can be solved
        bool solvable = MainWindow::isSolvable();

        if(!solvable){
          //  QMessageBox::Apply(NULL,"not solvable","==========");
            QMessageBox msgBox;
            msgBox.setWindowTitle("Solvalable test");
            msgBox.setText("Whether generate a new one");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            if(msgBox.exec() == QMessageBox::Yes){
              // do something
                while(!MainWindow::isSolvable()){
                    random();
                }
                QMessageBox msgBox;
                  msgBox.setText("The game can be solved.");
                  msgBox.exec();

            }else {
              // do something else
            }
        }
        else{
            QMessageBox msgBox;
              msgBox.setText("The game can be solved.");
              msgBox.exec();
        }

        tim = 0;
        ui->label_5->setText(QString::number(0));
        if(timer->isActive()){
            timer->stop();
        }
        timer->start(1000);
    }
    else{
        QMessageBox::warning(this, tr("Warning!"),
               tr("Please restart the app and set difficulty!"),
               QMessageBox::Cancel,
               QMessageBox::Yes);
    }

}

bool MainWindow::isSolvable()
{
    //target flag
    int flag = (0+(totalPatch-1)/N+(totalPatch-1)%N)%2;
    //get Reverse order number
    int inv = 0;
    for (unsigned int i = 1; i < totalPatch; i++){
        for (unsigned int j = 0; j < i; j++){
            if (pCompare[i/N][i%N] < pCompare[j/N][j%N]){
                inv++;
            }
        }
    }

    int row = 0;
    int col = 0;
    for(unsigned int j=0;j < totalPatch; j++){
        if( pCompare[j/N][j%N] == totalPatch-1){
            row = j/N;
            col = j%N;
            break;
        }
    }
    int Curflag = inv + row + col;
    return Curflag % 2 == flag;
}


void MainWindow::delay(unsigned int n){
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}






