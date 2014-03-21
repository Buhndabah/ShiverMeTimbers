#ifndef DUMMYGRID__H
#define DUMMYGRID__H

class DummyGrid {
public:
  static DummyGrid& getInstance();

  int getW() const {return w;}
  int getH() const {return h;}
  int getTileWidth() const {return tileWidth;}

private:
  DummyGrid();
  DummyGrid(const DummyGrid&);
  DummyGrid& operator=(const DummyGrid&);

  int w,h;
  float tileWidth;
};

#endif
