class IRC
{
  public:
  static const int samplesize=10;
  int b[samplesize*2];
  static const int s=10;
  int g[s]={0,0,0,0,0,0,0,0,0,0};
  int c=0;

  int v=0;

  int cValue=0;
  
  IRC(int samples[]);

  void addvalue(int vr);

  void showConverted();
  
};

