#include <iostream>
#include <cstdlib>

using namespace std;

int main(){
	int status;
	status = system("g++-7 change_background.cpp -o change_background");
	status = system("./change_background -t 3 -p '/media/ghost/Windows8_OS/Users/Ghost/Pictures/Wallpaper'");

}