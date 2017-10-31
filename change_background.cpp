//My first step towrds greater heights	

#include <iostream>	//int, char, dirent
#include <unistd.h>	//getcwd
#include <stdlib.h>	//system, NULL, EXIT_FAILURE, srand, rand
#include <dirent.h>	//DIR, opendir, closedir, readdir
#include <regex>	//regex_t, regcomp, regexec
#include <vector>	//vector
#include <string>

#define ss pair<string,string>

using namespace std;

vector<ss> get_all_images(string path){
	vector<ss> result,temp;
	DIR *d;
	struct dirent *dir;
	cout<<"path is "<<path<<endl;
	if (d = opendir(path.c_str())){
		while((dir = readdir(d)) != NULL){
			// checkif dir is image or not
			if (dir->d_type == DT_REG){
				if (regex_match(dir->d_name,regex(".*\.(JPG|jpg|png|PNG)"))){
					// cout<<"match "<<dir->d_name<<endl;
					result.push_back(ss(path,dir->d_name));
				}
			}
			else if(dir->d_type == DT_DIR){
				// cout<<"dir->d_name "<<dir->d_name<<endl;
				if (!strcmp(dir->d_name,".") && !strcmp(dir->d_name,"..")){
					temp = get_all_images(path + "/" +dir->d_name);
					result.insert(result.end(),temp.begin(),temp.end());
				}
			}
		}
		closedir(d);
	}
	else{
		cout<<"opendir failure"<<endl;
		exit(1);
	}
	return result;
}

int main(int argc, char **argv){
	string path, changeCommand, copyCommand, deleteCommand, str, PicturesPath, currentWallpaper;
	vector<ss> imgList;
	int status, nimgList, temp, waitTime;
	char tmpchar[1024];
	ss ssPair;

	path = "";
	waitTime = 10;

	for (int i=1; i<argc; i++){

		if(!strcmp(argv[i] ,"-p")){
			i++;
			path.append(argv[i]);
		}
		else if(!strcmp(argv[i] ,"-t")){
			i++;
			// cout<<stoi(argv[i])<<endl;
			waitTime = stoi(argv[i]);
		}
	}

	if (path == ""){
		// get current working directory
		if (getcwd(tmpchar, sizeof(tmpchar)) == NULL){
				cout<<"getcwd() error\n";
			exit(1);
		}
		path.append(tmpchar);
	}
	// cout<<"dslf"<<endl;

	// get list of images in path directory
	imgList=get_all_images(path);

	if (waitTime < 1){
		waitTime =1;
	}

	PicturesPath= "/home/ghost/Pictures/Wallpapers";

	// cout<<imgList.size()<<endl;

	nimgList=imgList.size();
	// cout<<"fsldf "<<nimgList<<endl;

	while(nimgList){
		// randomize list
		srand(nimgList);
		for (int i=0;i<nimgList;i++){
			// cout<<"i "<<i<<endl;
			temp=i+rand()%(nimgList-i);
			// cout<<"temp"<<temp<<endl;
			ssPair=imgList[temp];
			// cout<<"ssPair"<<ssPair.second<<endl;
			imgList[temp]=imgList[i];
			// cout<<"imgList[temp]"<<imgList[temp].second<<endl;
			imgList[i]=ssPair;
			// cout<<"starg"<<endl;
			status = system(("cp '" + ssPair.first + "/" + ssPair.second + "' '" + PicturesPath + "'").c_str());
			// cout<<"status "<<status<<" copying from "<< "cp '" + ssPair.first + "/" + ssPair.second + "' '" + PicturesPath + "'"<<endl;
			
			currentWallpaper = PicturesPath + "/" + ssPair.second;
			status = system(("gsettings set org.gnome.desktop.background picture-uri 'file://" + currentWallpaper + "'").c_str());
			// cout<<"status "<<status<<" changing to "<< "gsettings set org.gnome.desktop.background picture-uri 'file://" + currentWallpaper + "'"<<endl;
			
			// status = system(("rm '" + currentWallpaper + "'").c_str());
			// cout<<"status "<<status<<" removing "<< "rm '" + currentWallpaper + "'"<<endl;
			sleep(waitTime);
		}
	}

	return 0;

}