#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <list>
#include <iterator>
#include <openssl/md5.h>
#include <math.h>
#include <mpi.h>
#include <time.h>


using namespace std;

char character[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
    , 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
map<char, int> dictionary;

int get_index(char ch){
	int i;
	for (i = 0; i< 26; i++){
		if (character[i] == ch){
			return i;
		}
	}
}

void convert_pass(int pass, int len, char conv_pass[]){
	int i = len - 1;
	int j;
	int point;
	for (j =0 ; j < len ; j++){
		conv_pass[j] = 'a';
	}
//	printf("len %d \n",strlen(conv_pass));
	while(1){
		if (pass >0){
			point = pass % 26;
			conv_pass[i] = character[point];
			i--;
			pass = pass / 26;
		}else{
			break;
		}
	}
//	printf("KQ : %s len %d \n", conv_pass, strlen(conv_pass));
}

int con_pass(char pass[], int len){
	int i;
	int num = 0;
	for (i = 0; i< len;i++){
		num += pow(26,len-i-1) * (get_index(pass[i]));
	}
//	printf("NUM : %d\n", num);
	return num;
}

void gen_pass(int num_pass, int len,char conv_pass[]){
	char* new_pass = NULL;
	num_pass ++;
//	printf("gen %d\n", num_pass);
	convert_pass(num_pass, len,conv_pass);
}

void encode_password_MD5(const char *string, char *mdString){
    unsigned char digest[16];
    // char mdString[33];

    // encode password into MD5
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, string, strlen(string));
    MD5_Final(digest, &ctx);
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    
    // return mdString;
}


void find_pass(int begin, int end, char *mdString, int len, int rank){
	char pas[len];
	char temp_mdString[33];
	while(1){
		convert_pass(begin, len, pas);
		encode_password_MD5(pas, temp_mdString);
		if(!strcmp(temp_mdString, mdString)){
			printf("pass is %s %d\n", pas, rank);
			break;
		}else if(begin == end){
			break;
		}else{
			begin ++;
		}
	}
	return;
}

void split_dat(char *mdString, int number_process, int len_password, int rank){
	clock_t t1,t2;
    t1 = clock();
	int begin, end;
    int total_password = pow(26, len_password);
    int part = total_password / number_process;
    begin = part * (rank);
    if (rank != number_process - 1){
    	end = begin + part -1;
	}else{
		end = total_password -1;
	}
	printf("%d %d begin end \n", begin, end);
	find_pass(begin, end, mdString, len_password, rank);
	t2=clock();
    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    printf("Time required = %f second in rank %d \n", seconds, rank);
}

int main(int argc, char* argv[])
{
	int size, rank;
	char hostname[50];
	MPI_Init(&argc, &argv);
	
    char mdString[33];
    int len_password;
    if(argc != 3) return -1;
    sscanf(argv[1], "%s", mdString);
    sscanf(argv[2], "%d", &len_password);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    gethostname(hostname, 50);
    printf("My rank is %d , %s \n", rank, hostname);
    split_dat(mdString, size, len_password, rank);
    MPI_Finalize();
    return 0;
}
