  //کد جمع دو ماتریس 
 /*
#include<iostream>
int main(){
    int a[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int m[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int l[4][4];
   
   
      for(int o=0;o<4;o++){
        for(int p=0;p<4;p++){
       
       l[o][p]=(a[o][p]+m[o][p]);

        }
        }

for(int u=0;u<4;u++){
        for(int r=0;r<4;r++){
            std::cout<<l[u][r]<<"  ";
            
        }
        
        std::cout<<"\n"<<std::endl;
        }
}
*/
//کد تفریق دو ماتریس
/*
#include<iostream>
int main(){
    int a[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int m[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int l[4][4];
   
   
      for(int o=0;o<4;o++){
        for(int p=0;p<4;p++){
       
       l[o][p]=(a[o][p]- m[o][p]);

        }
        }

for(int u=0;u<4;u++){
        for(int r=0;r<4;r++){
            std::cout<<l[u][r]<<"  ";
            
        }
        
        std::cout<<"\n"<<std::endl;
        }
}
*/
//کد ضرب دو ماتریس 

#include<iostream>
int main(){
    int a[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int m[4][4]={{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
    int l[4][4];
   
   
      for(int o=0;o<4;o++){
        for(int p=0;p<4;p++){
          l[o][p]=0;
       for(int q=0;q<4;q++){
       l[o][p]+=a[o][q]*m[q][p];

        }
        }
      }

for(int u=0;u<4;u++){
        for(int r=0;r<4;r++){
            std::cout<<l[u][r]<<"  ";
            
        }
        
        std::cout<<"\n"<<std::endl;
        }
}