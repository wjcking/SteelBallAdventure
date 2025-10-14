Assembly <Time>
Assembly <Team>
include <Role>
include <Map>

namespace ke(th sh zh)

class Hezk namespace ke
{  
  abstract Hezk* create();
  abstract Role& listen();
}

class Role : virtual Hezk;
{
  Color Boundary();
  string& Advertise();
  abstract void mesh();
}

class Map : abstract Hezk;
{
  Color Boundary();
  Color Ground();
  string& Advertise();
  mesh();
}

internal Matric : public Role, public Map; 
{
  virtual void mesh();
  virtual Hezk* create() override;
  virtual Role& listen() override;
  Color Boundary();
  Color Ground();
  string& Advertise();
}
