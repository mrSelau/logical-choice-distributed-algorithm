#include <stdio.h>
#include "mpi.h"

main(int argc, char** argv){
  int my_rank;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int message = 0;       // Buffer para as mensagens                    
  MPI_Status status; // estrutura que guarda o estado de retorno          


  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)
  
  int cordenator = proc_n-1;
  int vetor[proc_n];
  int prox = 0;
  int tela = 0;
  int falha = 0;

  if ( my_rank == 0 ){
    vetor[0] = 0 ;
    printf("Pid: %d Message: %d\n", my_rank, tela);
      for(int i = 1; i<proc_n;i++) vetor[i] = 1;
    MPI_Send(&tela, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Send(&vetor[0], proc_n, MPI_INT, 1, 1, MPI_COMM_WORLD);
  }else{
    if(my_rank == cordenator){
      MPI_Recv(&tela, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&vetor[0], proc_n, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, &status);
      tela++;
      MPI_Send(&falha, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
      printf("Pid: %d Message: %d v: %d\n", my_rank, tela, vetor[my_rank]);
    }else{
      MPI_Recv(&tela, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&vetor[0], proc_n, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, &status);
      tela++;
      MPI_Send(&tela, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      MPI_Send(&vetor[0], proc_n, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
      printf("Pid: %d Message: %d v: %d\n", my_rank, tela, vetor[my_rank]);
      }
    }


  while(1){
    if ( my_rank == 0 ){
      MPI_Recv(&falha, 1, MPI_INT, cordenator, 2, MPI_COMM_WORLD, &status);
      printf("Eleicao: \n");
      scanf("%i", &falha);

      int voltou = 0;
      printf("Alguém voltou?: \n");
      scanf("%i", &voltou);

      if(voltou != 0){
        vetor[voltou] = 1;
      }

      if(falha == 0){
        MPI_Send(&falha, 1, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
        MPI_Send(&vetor[0], proc_n, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
      }else{
        vetor[cordenator] = 0;
        for(int i = proc_n-1; i>0; i--){
          if(vetor[i] == 1){
            cordenator = i;
            break;
          }
        }
        MPI_Send(&falha, 1, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
        MPI_Send(&cordenator, 1, MPI_INT, my_rank+1, 3, MPI_COMM_WORLD);
        MPI_Send(&vetor[0], proc_n, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
      }

    }
    else{
      MPI_Recv(&falha, 1, MPI_INT, my_rank-1, 2, MPI_COMM_WORLD, &status);
      if(falha == 0){
        MPI_Recv(&vetor[0], proc_n, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, &status);
        if(vetor[my_rank] == 1){
          printf("Pid: %d Message: %d v: %d\n", my_rank, tela, vetor[my_rank]);
        }
        if(my_rank == cordenator){
          MPI_Send(&falha, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }else{
          MPI_Send(&falha, 1, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
          MPI_Send(&vetor[0], proc_n, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
        }
      }else{
        MPI_Recv(&cordenator, 1, MPI_INT, my_rank-1, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&vetor[0], proc_n, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, &status);
        if(vetor[my_rank] == 1){
          printf("Pid: %d Candidato\n", my_rank);
        }
        if(my_rank == cordenator){
          MPI_Send(&falha, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }else{
          MPI_Send(&falha, 1, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
          MPI_Send(&cordenator, 1, MPI_INT, my_rank+1, 3, MPI_COMM_WORLD);
          MPI_Send(&vetor[0], proc_n, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
        }
      }

    }
  }
  MPI_Finalize();
}
