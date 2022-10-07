#include<iostream>
using namespace std;

void convert_string(int number,char* int_str){
int i=0;
while(number!=0){
  int_str[i]=(char) (number % 10);
  number = number/10;
  i++;
}
return;}


int main(){
  char *int_str;
int s=23;
convert_string(23,int_str);
cout<<int_str;
    return 0;}
