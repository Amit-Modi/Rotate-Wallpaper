#include <iostream>	//int, char, dirent
#include <unistd.h>	//getcwd
#include <stdlib.h>	//system, NULL, EXIT_FAILURE, srand, rand
#include <dirent.h>	//DIR, opendir, closedir, readdir
#include <regex>	//regex_t, regcomp, regexec
#include <vector>	//vector
#include <atomic>

using namespace std;

int main(int argc, char **argv){
	char path[1024]	;
	string command,str;
	vector<string> imgList;
	int status, nimgList, temp;
	DIR *d;
	struct dirent *dir;
	atomic_bool keyIsPressed(false);

	// get current working directory
	if (getcwd(path, sizeof(path)) == NULL){
		cout<<"getcwd() error\n";
		exit(1);
	}

	// get list of images in path directory
	if (d = opendir(path)){
		while((dir = readdir(d)) != NULL){
			// checkif dir is image or not
			if (dir->d_type == DT_REG){
				if (regex_match(dir->d_name,regex(".*\.JPG"))){
					cout<<"match "<<dir->d_name<<endl;
					imgList.push_back(dir->d_name);
				}
			}
		}
		closedir(d);
	}

	command="gsettings set org.gnome.desktop.background picture-uri ";
	command.append(path);

	while(true){
		// randomize list
		nimgList=imgList.size();
		srand(nimgList);
		for (int i=0;i<nimgList;i++){
			temp=i+rand()%(nimgList-i);
			str=imgList[temp];
			imgList[temp]=imgList[i];
			imgList[i]=str;
			str = command + "/" + str;
			cout<<"executing "<<str<<endl;
			status = system(str.c_str());
			sleep(10);
		}
	}

	return 0;

}