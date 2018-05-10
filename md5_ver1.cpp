#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/md5.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#define LENCH 26

using namespace std;

char character[LENCH] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
    , 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

// get index of character in distionary
int get_index(char ch){
	int i;
	for (i = 0; i< LENCH; i++){
		if (character[i] == ch){
			return i;
		}
	}
}

// convert pass in type number to string
void convert_pass_num2str(int pass, int len, char conv_pass[]){
	int i = len - 1;
	int j;
	int point;
	for (j =0 ; j < len ; j++){
		conv_pass[j] = 'a';
	}
	conv_pass[len] = '\0';
	while(1){
		if (pass >0){
			point = pass % LENCH;
			conv_pass[i] = character[point];
			i--;
			pass = pass / LENCH;
		}else{
			break;
		}
	}
}

// generate next password 
void gen_pass(int num_pass, int len,char conv_pass[]){
	char* new_pass = NULL;
	num_pass ++;
	convert_pass_num2str(num_pass, len,conv_pass);
}

// encode password to MD5 hash
void encode_password_MD5(const char *string, char *mdString){
    unsigned char digest[16];

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, string, strlen(string));
    MD5_Final(digest, &ctx);
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
}

// decryt password in a node
void decrypt_pass(int begin, int end, char *mdString, int len, int rank){
	char pas[len];
	char temp_mdString[33];
	while(1){
		convert_pass_num2str(begin, len, pas);
		encode_password_MD5(pas, temp_mdString);
		if(!strcmp(temp_mdString, mdString)){
			printf("pass is %s in rank %d\n", pas, rank);
			break;
		}else if(begin == end){
			break;
		}else{
			begin ++;
		}
	}
	return;
}

// split job with rank of node
void split_job_data(char *mdString, int number_process, int len_password, int rank){
	clock_t t1,t2;
    t1 = clock();
	int begin, end;
    int total_password = pow(LENCH, len_password);
    int part = total_password / number_process;
    begin = part * (rank);
    if (rank != number_process - 1){
    	end = begin + part -1;
	}else{
		end = total_password -1;
	}
	decrypt_pass(begin, end, mdString, len_password, rank);
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
    
    // get arg to variable
    if(argc != 3) return -1;
    sscanf(argv[1], "%s", mdString);
    sscanf(argv[2], "%d", &len_password);
    
    // get rank and size of MPI
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	gethostname(hostname, 50);
	
	printf("My rank is %d , %s \n", rank, hostname);
	
	// brute force to find password by parallel compute in MPI
	split_job_data(mdString, size, len_password, rank);
	
	MPI_Finalize();
	return 0;
}

// mpic++ md5.cpp -o md5  -lcrypto
// mpirun -hostfile hostfile -np 2 ./md5 10b30316838d2c01e7cb6350178ba812 5



