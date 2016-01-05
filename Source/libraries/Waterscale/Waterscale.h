class Waterscale
{
  public:
  Waterscale( ){}

  int accelX;
  int accelY;

  int accel[5][2];
  int count;

  void Show();
  void Update();
  
  void displayTilt( int tiltX, int  tiltY);
  private:

  unsigned char convertToBit( int in );
 
  void putNumbertoDisplay(unsigned char charNumber [3]);

};

