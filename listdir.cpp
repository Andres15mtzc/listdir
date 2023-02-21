#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void error(const char *s);

int main(int argc, char *argv[]) {
	DIR *dir;
	ofstream fout;
	struct dirent *ent;
	char *strDir = argv[1];
	
	dir = opendir (strDir);
	
	if (dir == NULL){
		perror ("No puedo abrir el directorio");
		exit(EXIT_FAILURE);
	}	
	
	ofstream file("dir.json");
	fout.open("dir.json");
	
	fout << "{\"files\":[\n";
	
	ent = readdir (dir);
	
	ostringstream line;
	bool cond = true;
	do {
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ) {
			char *archivo = ent->d_name;
			ostringstream ss;
			ss << strDir << "\\" << archivo;
			string path = ss.str();
			FILE *fich;
			long ftam;
			
			fich = fopen(path.c_str(), "r");
			if (fich) {
				fseek(fich, 0L, SEEK_END);
				ftam = ftell(fich);
				fclose(fich);
				if (cond){
					line << "   {\"name\":\"" << archivo << "\",\"size\":" << ftam << "}";
					cond = false;
				}
				else {
					line << ",\n   {\"name\":\"" << archivo << "\",\"size\":" << ftam << "}";
				}
			}
			else if (cond){
				line << "   {\"name\":\"" << archivo << "\",\"size\":\"No Info.\"}";
				cond = false;
			}
			else {
				line << ",\n   {\"name\":\"" << archivo << "\",\"size\":\"No Info.\"}";
			}
		}
		ent = readdir (dir);
	} while (ent != NULL);
	fout << line.str() << "]}";
	closedir (dir);
	fout.close();
	
	return EXIT_SUCCESS;
}
