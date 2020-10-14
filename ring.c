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
  int eleicao = 0;

  if ( my_rank == 0 ){
    vetor[0] = 0 ;
    printf("Pid: %d Message: %d\n", my_rank, tela);
      for(int i = 1; i<proc_n;i++) vetor[i] = 1;
    MPI_Send(&tela, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Send(&vetor[0], proc_n, MPI_INT, 1, 1, MPI_COMM_WORLD);
  }else{
    if(my_rank == proc_n-1){
      MPI_Recv(&tela, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&vetor[0], proc_n, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, &status);
      tela++;
      MPI_Send(&eleicao, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
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
      MPI_Recv(&eleicao, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);

      int falhou = 0;
      printf("Alguém falhou?(n processo): \n");
      scanf("%i", &falhou);

      int voltou = 0;
      printf("Alguém voltou?(n processo): \n");
      scanf("%i", &voltou);
      printf("----------------------------------\n");
      vetor[falhou] = 0;
      if(voltou != 0){
        vetor[voltou] = 1;
      }

      int eleicao = 0;
      if(falhou == cordenator) eleicao = 1;

      int k = my_rank + 1;
      int prox = 0;
      for(k; k<proc_n; k++){
        if(vetor[k] == 1){
          prox = k;
          break;
        } 
      }

      if(eleicao == 0){
        MPI_Send(&eleicao, 1, MPI_INT, prox, 2, MPI_COMM_WORLD);
        MPI_Send(&cordenator, 1, MPI_INT, prox, 3, MPI_COMM_WORLD);
        MPI_Send(&vetor[0], proc_n, MPI_INT, prox, 1, MPI_COMM_WORLD);
      }else{
        for(int i = proc_n-1; i>0; i--){
          if(vetor[i] == 1){
            cordenator = i;
            break;
          }
        }
        MPI_Send(&eleicao, 1, MPI_INT, prox, 2, MPI_COMM_WORLD);
        MPI_Send(&cordenator, 1, MPI_INT, prox, 3, MPI_COMM_WORLD);
        MPI_Send(&vetor[0], proc_n, MPI_INT, prox, 1, MPI_COMM_WORLD);
      }

    }
    else{
      MPI_Recv(&eleicao, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
      if(eleicao == 0){
        MPI_Recv(&cordenator, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&vetor[0], proc_n, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        int k = my_rank + 1;
        int prox = 0;
        for(k; k<proc_n; k++){
          if(vetor[k] == 1){
            prox = k;
            break;
          } 
        }
        if(vetor[my_rank] == 1){
          if(cordenator == my_rank){
            printf("Pid: %d Message: %d v: %d (ELEITO)\n", my_rank, tela, vetor[my_rank]);
          }else{
            printf("Pid: %d Message: %d v: %d\n", my_rank, tela, vetor[my_rank]);
          }
          
        }
        if(prox == 0){
          MPI_Send(&eleicao, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }else{
          MPI_Send(&eleicao, 1, MPI_INT, prox, 2, MPI_COMM_WORLD);
          MPI_Send(&cordenator, 1, MPI_INT, prox, 3, MPI_COMM_WORLD);
          MPI_Send(&vetor[0], proc_n, MPI_INT, prox, 1, MPI_COMM_WORLD);
        }
      }else{
        MPI_Recv(&cordenator, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&vetor[0], proc_n, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        int k = my_rank + 1;
        int prox = 0;
        for(k; k<proc_n; k++){
          if(vetor[k] == 1){
            prox = k;
            break;
          } 
        }
        if(vetor[my_rank] == 1){
          printf("Pid: %d Candidato\n", my_rank);
        }
        if(prox == 0){
          MPI_Send(&eleicao, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }else{
          MPI_Send(&eleicao, 1, MPI_INT, prox, 2, MPI_COMM_WORLD);
          MPI_Send(&cordenator, 1, MPI_INT, prox, 3, MPI_COMM_WORLD);
          MPI_Send(&vetor[0], proc_n, MPI_INT, prox, 1, MPI_COMM_WORLD);
        }
      }

    }
  }
  MPI_Finalize();
}
