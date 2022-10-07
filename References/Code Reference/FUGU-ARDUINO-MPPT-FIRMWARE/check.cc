#include<iostream>
using namespace std;

int decimal_extractor(float value) {
    int result =0;
  result = (value < 0) ?  ((int)(value * (-100))) % 100 :  ((int)(value * 100)) % 100;
  return result;
}
int main(){
    float value = 1+decimal_extractor(1.12)*0.01;
    cout<<value<<endl;
    return 0;}
