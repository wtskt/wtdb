#include "kv.h"
#include <time.h>
#include <unistd.h>
unsigned int seed = 1;
char* genRandomString(int length) { // 生成length长度的随机字符串
    int flag, i;
    char* string;
//    unsigned int seed = (unsigned) time(NULL);
    srand(seed++);
    if ((string = (char*) malloc(length)) == NULL ) {
        return NULL ;
    }

    for (i = 0; i < length - 1; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                string[i] = 'A' + rand() % 26;
                break;
            case 1:
                string[i] = 'a' + rand() % 26;
                break;
            case 2:
                string[i] = '0' + rand() % 10;
                break;
            default:
                string[i] = 'x';
                break;
        }
    }
    string[length - 1] = '\0';
    return string;
}

int main(int argc, char const *argv[]) {
    char fileName[50] = "test";//?????
    char key[ABUP_JSON_KEY_SIZE] = "10";//??
    char *key1 = NULL;
    char *value1 = NULL;
    char value[ABUP_JSON_KEY_SIZE] = "hello"; //?
    char *temp = NULL;
    int i = 0;

	//???????е?key???????value??
//	key1 = genRandomString(3);
//    key1 = "3oy";
//	printf("----------key1 = %s ------\n", key1);
//	temp = ReadValue("test",key1);
//	if(temp == NULL) {
//		printf("-----------??????????????в鵽????-------------\n");
//	} else {
//		printf("-----------??鵽value?????-------------\n");
//		puts(temp);
//	}

    clock_t start, stop;
    start = clock();
	for(i = 0; i < 100; i++) {
		key1 = genRandomString(8);
		value1 = genRandomString(10);
		WriteKeyValue(fileName,key1,value1);
//		printf("------i = %d-------\n",i);
	}
    stop = clock();
    //计算以秒为单位的运行时间，（结束时间-开始时间）/CLK_TCK
    printf("Fun函数耗费时间为%f秒\n", ((double)(stop - start)) / CLK_TCK);



	//???fileName1?е?key-value
//	RemoveKeyValue(fileName,"3jX");

    return 0;
}