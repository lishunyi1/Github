//#include<stdlib.h>
//#include<mpi.h>
//#include<stdio.h>
//
//int main (int argc, char *argv[])
//{
//	int rank,value,size,i;
//	MPI_Init(&argc,&argv);
//	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//	MPI_Comm_size(MPI_COMM_WORLD,&size);
//	int buf[size];
//	if(0==rank)
//	{
//		for(i=0;i<size;i++)
//		{
//			buf[i]=(i+1)*10;
//		}
//	}
//	MPI_Scatter(buf,1,MPI_INT,&value,1,MPI_INT,0,MPI_COMM_WORLD);
//	printf("Process %d got value %d\n",rank,value);
//	
//	MPI_Finalize();
//	return 0;
//}


//7gather
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>

int main (int argc, char *argv[])
{
	int rank,size,value,i;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int buf[size];
	value=rank*10;
	MPI_Gather(&value,1,MPI_INT,buf,1,MPI_INT,0,MPI_COMM_WORLD);
	for(i=0;i<size;i++)
	{
		printf("Process %d got value buf[%d] = %d\n",rank,i,buf[i]);
	}
	MPI_Finalize();
	return 0;
}



//8reduce
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>
#include<time.h>
int main (int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	int size,myrank,i;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	double ain[size],aout[size];
	int ind[size];
	struct{
		double val;
		int rank;
	}in[size],out[size];
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	srand((unsigned int)(myrank+time(NULL)));
	for(i=0;i<size;i++) ain[i]=(1.0*rand())/RAND_MAX;
	for(i=0;i<size;i++){
		in[i].val=ain[i];
		in[i].rank=myrank;
	}
	for(i=0;i<size;i++) printf("myrank:%d,%.3f\n",myrank,ain[i]);
	MPI_Reduce(in,out,size,MPI_DOUBLE_INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
	if(myrank==0)
	{
		printf("reduce result:\n");
		for(i=0;i<size;i++){
			aout[i]=out[i].val;
			ind[i]=out[i].rank;
			printf("Max value in %d column is %.3f from rank %d\n",i,aout[i],ind[i]);
		}		
	}
	MPI_Finalize();
	return 0;
}


//9allgather




//10allreduce
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>
int main (int argc, char *argv[])
{
	int rank,size,value;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int buf;
	value=rank*10;
	MPI_Allreduce(&value,&buf,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	printf("Process %d got value buf = %d\n",rank,buf);
	MPI_Finalize();
	return 0;
}



//11scan
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>

int main (int argc, char *argv[])
{
	int rank,size,value;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int buf;
	value=rank*10;
	MPI_Scan(&value,&buf,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	printf("Process %d got value buf = %d\n",rank,buf);
	MPI_Finalize();
	return 0;
}



//12alltoall
#include<stdlib.h>
#include<mpi.h>
#include<stdio.h>

int main (int argc, char *argv[])
{
	int rank,size,i;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int buf[size];
	int val[size];
	
	for(i=0;i<size;i++)
	{
		buf[i]=rank*size+i;
	}
	
	MPI_Alltoall(buf,1,MPI_INT,val,1,MPI_INT,MPI_COMM_WORLD);
	for(i=0;i<size;i++)
		printf("Process %d got value buf[%d] = %d\n",rank,i,val[i]);
	MPI_Finalize();
	return 0;
}




//13Barrier
#include<mpi.h>
#include<stdio.h>

int main (int argc, char *argv[])
{
	int rank,size,value;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	do{
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank==0){
			fprintf(stderr,"\nPlease give new value=");
			scanf("%d",&value);
			fprintf(stderr,"%d read < -from (%d)\n",rank,value);
			if(size>1){
				MPI_Send(&value,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
				fprintf(stderr,"%d send (%d) to -> %d \n",rank,value,rank+1);
			}
		}
		else{
			MPI_Recv(&value,1,MPI_INT,rank-1,0,MPI_COMM_WORLD,&status);
			fprintf(stderr,"%d receive (%d) <- from %d \n",rank,value,rank-1);
			if(rank<size-1){
				MPI_Send(&value,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
				fprintf(stderr,"%d send (%d) to -> %d \n",rank,value,rank+1);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}while(value>0);
	MPI_Finalize();
	return 0;
}

