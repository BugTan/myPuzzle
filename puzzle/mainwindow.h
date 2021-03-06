#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QLabel>
#include<QTimer>
#include <QString>
#include <QMainWindow>
#define MAXROW 9
#define MAXSAVEPROCESS 10

namespace Ui{
    class MainWindow;
}

typedef struct{
    QString saveName;
    unsigned int array[MAXROW*MAXROW];
}SaveArray;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //鼠标事件
    void keyPressEvent(QKeyEvent *event);
    //随机数
    void random();
    //切割图片
    void cutImage();
    //图片移动
    void moveImage();
private slots:
    //new
    void on_loadImgBtn_clicked();

    //timer
    void seconds();

    void on_Start_clicked();
    //whether can solved
    bool isSolvable();

    void delay(unsigned int n);
private:
    Ui::MainWindow *ui;
    //image path
    QString strFileName;
    //new image
    QImage* sourceImage;

    //九宫格的Image
    QImage pImage[MAXROW*MAXROW];
    //pCompare[i][j] 表示第i行j列存放的Label
    //unsigned int* pCompare[5][3];
    unsigned int pCompare[MAXROW][MAXROW];
    QLabel* plbImage[MAXROW*MAXROW];
    //定时器
    QTimer* timer;
    //用于显示时间
    int tim;
    //set difficulty
    unsigned int M,N;
    unsigned int totalPatch;

    unsigned int SMALL_W,SMALL_H;

    bool dFlag = false;

    //is solvable flag
    //int flag;
    //宫格Label
    //QLabel* plbImage;

    SaveArray saveArray[MAXSAVEPROCESS];

    unsigned int saveTimes;
};



#endif // MAINWINDOW_H
