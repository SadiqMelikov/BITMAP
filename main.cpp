#include <bits/stdc++.h>
#include "BMP.h"
using namespace std;

double degtorad(double deg)
{
    return (deg*M_PI/180);
}

vector<uint8_t> reversedata(vector<uint8_t> data,uint32_t channels,uint32_t w,uint32_t h)
{
    vector<uint8_t> reverseddata;
    reverseddata.resize(channels*w*h);
    if (channels==3)
    {
        for (int i=0;i<h;i++)
        {
            for (int j=0;j<w;j++)
            {
                reverseddata[channels*(i*w+j)+2]=data[channels*((h-(i+1))*w+j)];
                reverseddata[channels*(i*w+j)+1]=data[channels*((h-(i+1))*w+j)+1];
                reverseddata[channels*(i*w+j)]=data[channels*((h-(i+1))*w+j)+2];
            }
        }
    }
    if (channels==4)
    {
        for (int i=0;i<h;i++)
        {
            for (int j=0;j<w;j++)
            {
                reverseddata[channels*(i*w+j)+3]=data[channels*((h-(i+1))*w+j)];
                reverseddata[channels*(i*w+j)+2]=data[channels*((h-(i+1))*w+j)+1];
                reverseddata[channels*(i*w+j)+1]=data[channels*((h-(i+1))*w+j)+2];
                reverseddata[channels*(i*w+j)]=data[channels*((h-(i+1))*w+j)+3];
            }
        }
    }
    return reverseddata;
}

struct Point
{
    double x,y;
    Point() {}
    Point(double x,double y): x(x),y(y) {}
};

Point rotatearound(Point center,double alpha,Point p)
{
    double s=sin(alpha);
    double c=cos(alpha);
    p.x-=center.x;
    p.x-=center.y;
    double xnew=p.x*c-p.y*s;
    double ynew=p.x*s+p.y*c;
    p.x=xnew+center.x;
    p.y=ynew+center.y;
    return p;
}

struct Color
{
    uint8_t r,g,b,a;
    Color()
    {
        r=0;
        g=0;
        b=0;
        a=false;
    }
    Color(uint8_t r,uint8_t g,uint8_t b,uint8_t a=0): r(r),g(g),b(b),a(a) {}
};

int main()
{
    BMP bmp1("image.bmp");
    int channels=bmp1.bmp_infoheader.bitcount/8;
    int w1=bmp1.bmp_infoheader.width,h1=bmp1.bmp_infoheader.height;
    Point center((double)w1/2,(double)h1/2);
    double alpha;
    cin>>alpha;
    alpha=degtorad(alpha);
    Point bef[4],aft[4];
    bef[0]=Point(0,0);
    bef[1]=Point(w1,0);
    bef[2]=Point(0,h1);
    bef[3]=Point(w1,h1);
    for (int i=0;i<4;i++) aft[i]=rotatearound(center,alpha,bef[i]);
    int w2=0,h2=0;
    for (int i=0;i<4;i++)
    {
        double dw,dh;
        for (int j=0;j<4;j++)
        {
            dw=abs(aft[i].x-aft[j].x);
            dh=abs(aft[i].y-aft[j].y);
            if (dw>(int)dw) dw++;
            if (dh>(int)dh) dh++;
            w2=max((double)w2,dw);
            h2=max((double)h2,dh);
        }
    }
    double xmin=INT_MAX,ymin=INT_MAX;
    for (int i=0;i<4;i++)
    {
        xmin=min(xmin,aft[i].x);
        ymin=min(ymin,aft[i].y);
    }
    BMP bmp2(w2,h2,false);
    vector<uint8_t> data1=reversedata(bmp1.data,channels,w1,h1),data2;
    data2.resize(channels*w2*h2);
    vector<bool> data_empty(w2*h2);
    for (int i=0;i<h2;i++)
    {
        for (int j=0;j<w2;j++)
        {
            data2[channels*(i*w2+j)]=0;
            data2[channels*(i*w2+j)+1]=0;
            data2[channels*(i*w2+j)+2]=0;
            if (channels==4)
                data2[channels*(i*w2+j)+3]=0;
            data_empty[i*w2+j]=true;
        }
    }
    for (int i=0;i<h1;i++)
    {
        for (int j=0;j<w1;j++)
        {
            Point pt=rotatearound(center,alpha,Point(j,i));
            pt.x-=xmin;
            pt.y-=ymin;
            int pos=(int)pt.y*w2+(int)pt.x;
            if (pos<w2*h2)
            {
                data2[channels*pos]=data1[channels*(i*w1+j)];
                data2[channels*pos+1]=data1[channels*(i*w1+j)+1];
                data2[channels*pos+2]=data1[channels*(i*w1+j)+2];
                if (channels==4)
                    data2[channels*pos+3]=data1[channels*(i*w1+j)+3];
                data_empty[pos]=false;
            }
        }
    }
    for (int i=1;i<h2-1;i++)
    {
        for (int j=1;j<w2-1;j++)
        {
            if (data2[channels*(i*w2+j)]==0 && data2[channels*(i*w2+j)+1]==0 && data2[channels*(i*w2+j)+2]==0 && data_empty[i*w2+j])
            {
                Color c[4];
                if (channels==4)
                {
                    c[0]=Color(data2[channels*((i-1)*w2+j)],data2[channels*((i-1)*w2+j)+1],data2[channels*((i-1)*w2+j)+2],data2[channels*((i-1)*w2+j)+3]);
                    c[1]=Color(data2[channels*(i*w2+j+1)],data2[channels*(i*w2+j+1)+1],data2[channels*(i*w2+j+1)+2],data2[channels*((i-1)*w2+j)+3]);
                    c[2]=Color(data2[channels*((i+1)*w2+j)],data2[channels*((i+1)*w2+j)+1],data2[channels*((i+1)*w2+j)+2],data2[channels*((i-1)*w2+j)+3]);
                    c[3]=Color(data2[channels*(i*w2+j-1)],data2[channels*(i*w2+j-1)+1],data2[channels*(i*w2+j-1)+2],data2[channels*((i-1)*w2+j)+3]);
                }
                else
                {
                    c[0]=Color(data2[channels*((i-1)*w2+j)],data2[channels*((i-1)*w2+j)+1],data2[channels*((i-1)*w2+j)+2]);
                    c[1]=Color(data2[channels*(i*w2+j+1)],data2[channels*(i*w2+j+1)+1],data2[channels*(i*w2+j+1)+2]);
                    c[2]=Color(data2[channels*((i+1)*w2+j)],data2[channels*((i+1)*w2+j)+1],data2[channels*((i+1)*w2+j)+2]);
                    c[3]=Color(data2[channels*(i*w2+j-1)],data2[channels*(i*w2+j-1)+1],data2[channels*(i*w2+j-1)+2]);
                }
                int r=0,g=0,b=0,a=0;
                for (int k=0;k<4;k++)
                {
                    r+=c[k].r;
                    g+=c[k].g;
                    b+=c[k].b;
                    if (channels==4)
                        a+=c[k].a;
                }
                r/=4;
                g/=4;
                b/=4;
                a/=4;
                data2[channels*(i*w2+j)]=r;
                data2[channels*(i*w2+j)+1]=g;
                data2[channels*(i*w2+j)+2]=b;
                if (channels==4)
                    data2[channels*(i*w2+j)+3]=a;
            }
        }
    }
    data2=reversedata(data2,channels,w2,h2);
    bmp2.data=data2;
    bmp2.write("result.bmp");
    return 0;
}
