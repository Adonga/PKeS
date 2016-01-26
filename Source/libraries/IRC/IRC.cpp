#include "IRC.h"


  IRC::IRC(int samples[])
  {
    
    for(int i=0;i<samplesize;i++)
    {
        b[2*i]=samples[i];
        b[2*i +1]=(25*i)+75;  
    }
    
  };

  void IRC::addvalue(int vr)
  {
    g[(c++)%s]=vr;
    if(c>2*s)c-=s;
    v=0;
    
    for(int i=0;i<s;++i)
    {
      v+=g[i];  
    }
    v=v/((c<s)?c:s);

    int k=0;
    if(v>b[0])
    {
     k=-10;   
    }
    else
    {
      for(int i=0;i<samplesize-1;++i)
      {
        if(b[i*2]>=v && b[(i+1)*2]<=v)
        {
          int m=b[i*2]-b[(i+1)*2];
          int h,j;
          h=v-b[(i+1)*2];
          j=b[i*2]-v;

          k=j*(b[(i+1)*2 +1])+h*(b[i*2 +1]);
          k=k/m;

          break;
        }
      }  
    }
    if(!k)k=999;
    cValue=k;
  }

  void IRC::showConverted(Display *dis)
  {
    
    dis->showSmallNumber(cValue,One);
  }
  
