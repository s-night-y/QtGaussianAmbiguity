#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <math.h>
double gaussFunc1D(int x);
double* getKernal(int r,double* weight,int radius=2);
void gauss(QImage& img,int radius=1);
int edge(int x, int i, int w);
//计算 高斯的 sigma值
double caculateSigma(int ksize){
    return 0.3*((ksize-1)*0.5-1)+0.8;
}

double sigma = caculateSigma(8);

int main(int argc, char *argv[])
{

    int r=30;//取值半径范围
    QString argF="%1_%2.png";
    QImage *img=new QImage("eb32.png");//打开图片
    gauss(*img,r);//高斯模糊
    img->save(argF.arg(0).arg(r));//保存
    delete img;//删除
    return 0;
}
int edge(int x, int i, int w)
{
    // x为中心位置, i为偏移量,w为宽度
    int inx = x+i;
    if(inx<0 || inx>=w)
        return x-i;
    return inx;
}
//根据sigma 计算权重值
double gaussFunc1D(int x)
{
    double A = 1/(sigma*sqrt(2*3.141592653));
    double index = -1.0*(x*x)/(2*sigma*sigma);

    return A*exp(index);
}
//获取对应范围内的 比例值
double* getKernal(int r,double* weight,int radius)
{

    qDebug()<<sigma<<Q_FUNC_INFO<<__LINE__;
    double sum=0;

    // 获取权值空间weight[]
    for(int i=0; i<2*r+1;i++)
    {
        weight[i] = gaussFunc1D(i-r);
        sum+=weight[i];
    }
    // 归一化
    for(int i=0; i<2*r+1;i++)
    {
        weight[i]/=sum;
    }
    return weight;
}

void gauss(QImage& img,int radius){

    QImage temp=img;
    QImage temptwo=img;
    int r=30;
    double *weight= new double[2*r+1]{};//={};
    getKernal(r,weight,radius);//[3]={0.0947416*2,0.118318*2,0.0947416*2    };

    // 在横向进行一次相加
    for(int y=0;y<img.height();y++)
    {
        for(int x=0;x<img.width();x++)
        {
            double red=0,green=0,blue=0;
            for(int i=-r; i<=r; i++)
            {
                // 边界处理后的对应的权值矩阵实际值
                int inx = edge(x,i,img.width());

                QColor rgb = img.pixelColor(inx,y);
                red+=rgb.red()*weight[r+i];
                green+=rgb.green()*weight[r+i];
                blue+=rgb.blue()*weight[r+i];
            }
            temp.setPixel(x,y,qRgb(red,green,blue));
        }
    }
    // 在纵方向对第一次的结果重新进行一次
    for(int y=0;y<img.height();y++)
    {
        for(int x=0;x<img.width();x++)
        {
            double red=0,green=0,blue=0;
            for(int i=-r; i<=r; i++)
            {
                int iny = edge(y,i,img.height());

                QColor rgb = temp.pixelColor(x,iny);
                red+=rgb.red()*weight[r+i];
                green+=rgb.green()*weight[r+i];
                blue+=rgb.blue()*weight[r+i];
            }

            temptwo.setPixel(x,y,qRgb(red,green,blue));
        }
    }

    img=temptwo;
	delete [] weight;

}
