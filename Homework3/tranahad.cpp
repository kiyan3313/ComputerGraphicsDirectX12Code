#include<iostream>
int main(){
    
    int x[4][4];
    int y[4][4];
    std::cout<<"enter a number:"<<std::endl;
    for(int u=0;u<4;u++){
        for(int o=0;o<4;o++){
            std::cin>>x[u][o];
        }
    }
    for(int i=0;i<=3;i++){
        for(int j=0;j<=3;j++){
           
           y[i][j]=x[j][i];
    }
    
}
  for(int k=0;k<=3;k++){
        for(int l=0;l<=3;l++){
            std::cout<<y[k][l]<<" ";
        }
        std::cout<<std::endl;
  }
return 0;
}
  