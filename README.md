# mpi-crack-md5
hosfile : file contains total nodes's name in rocks-cluster

## build file :
- mpic++ md5_ver1.cpp -o md5_v1 -lcrypto
- mpic++ md5_ver2.cpp -o md5_v2 -lcrypto

## run file :
- md5_v1 :
```
- mpirun -np <number task> [-hostfile] [hostfile] ./md5_v1 [MD5 code password] [lenght of pass]
VD : mpirun -np 5 -hostfile hostfile ./md5 bb7d4b236b564cf1ec27aa891331e0af 4

(bb7d4b236b564cf1ec27aa891331e0af is MD5 code of password : 'tung')
```

- md5_v2 :
```
- mpirun -np <number task> [-hostfile] [hostfile] ./md5_v2 [MD5 code password] [lenght of pass]
VD : mpirun -np 5 -hostfile hostfile ./md5 bb7d4b236b564cf1ec27aa891331e0af 4

(bb7d4b236b564cf1ec27aa891331e0af is MD5 code of password : 'tung')
```
