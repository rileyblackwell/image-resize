#include <iostream>
#include <cassert>
using namespace std;

 
struct Matrix{
  int width = 1;
  string name = "table";
};

int main(){
  Matrix mat();
  const char* type = typeid(mat).name();
  const char* type2 = typeid(Matrix).name(); 
  cout << type << endl << type2 << endl;
  return 0;
}

 