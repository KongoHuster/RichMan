/**
 * @brief diff two file
 * @authors lb
 * @date 2018-10-29
 * @version debug 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int diff(char *src_path, char *des_path){
    char string1[1024]={0};
    char string2[1024]={0};

    FILE *src_file = fopen(src_path, "r");
    FILE *des_file = fopen(des_path, "r");

    int line = 1;

    while (!feof(src_file) && !feof(des_file)){
        memset(string1, 0, 1024);
        memset(string2, 0, 1024);
        fgets(string1, 1024, src_file);
        fgets(string2, 1024, des_file);
        int i = 0;
        for(i = 0;(string1[i] != '\0' && string1[i] != '\n') && (string2[i] != '\0' && string2[i] != '\n'); i++){
            if(string1[i] == string2[i]){
                continue;
            }
            fclose(src_file);
            fclose(des_file);
            return line;
        }
        if((string1[i] != '\0' && string1[i] != '\n') || (string2[i] != '\0' && string2[i] != '\n')){
            fclose(src_file);
            fclose(des_file);
            return line;
        }
        ++line;
    }
    if(!feof(des_file) || !feof(src_file)){
        fclose(src_file);
        fclose(des_file);
        return line;
    }
    fclose(src_file);
    fclose(des_file);
    return 0;

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
/**
 * @param argc 2
 * @param argv : src_filename1 des_filename
 * @return 0
 */
int main(int argc,char *argv[]) {
    if(argv[1] == NULL || argv[2] == NULL){
        return 0;
    }
    //puts(argv[1]);
    //puts(argv[2]);
    //printf("\n\n");
    int ret = diff(argv[1], argv[2]);
    if(0 == ret){
        printf("通过该文件测试\n");
        return ret;
    }
    printf("\033[1;31m第%d行未通过测试\n",ret);
    printf("\033[0m");
    return ret;
}
#pragma clang diagnostic pop