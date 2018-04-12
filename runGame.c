#include<stdlib.h>
int main(){
	system("g++ gameDisplay.cpp -std=c++11 -lGL -lGLU -lglut -o Tunnel_Trouble && ./Tunnel_Trouble && killall paplay && rm Tunnel_Trouble");
	return 0;
}
