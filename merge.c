#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define LENGTH 10
#define NUM_THREADS 3
//threads por tamaño
const int NUMEROS_POR_THREAD = LENGTH / NUM_THREADS;
const int RESIDUO = LENGTH % NUM_THREADS;
int arr[LENGTH];

void merge_sort(int,int);
void combine_array( int, int , int );
void* thread_function(void* );
void combine_pthreads( int , int );
void printMatriz(int arr[],int,int);
void *thread_function(void* arg){
  int thread_id = (int)arg;
  printf("id %d\n", thread_id);
  //inicio
  int left = thread_id * (NUMEROS_POR_THREAD);
  //fin
  int right = (thread_id + 1) * (NUMEROS_POR_THREAD) - 1;
  //puede haber residuos no siempre es exacto
  if (thread_id == NUM_THREADS - 1) {
    right += RESIDUO;
  }
  if (left < right) {
    printf("Thread: %d left: %d right: %d\n",thread_id,left,right);
    merge_sort(left, right);
    printMatriz(arr,left,right);
  }
  return NULL;
}
//divide en posiciones
void merge_sort(int left, int right) {
  if (left < right) {
    int middle = left + (right - left) / 2;
    merge_sort(left, middle);
    merge_sort(middle + 1, right);
    combine_array(left, middle, right);
  }
}
void combine_array( int left, int middle, int right) {
  int i = 0;
  int j = 0;
  int left_length = middle - left + 1;
  int right_length = right - middle;
  int left_array[left_length];
  int right_array[right_length];

  //Copiar valores del array izquierdo
  for (int i = 0; i < left_length; i ++) {
    left_array[i] = arr[left + i];
  }

  //Copiar valores del array derecho 
  for (int j = 0; j < right_length; j ++) {
    right_array[j] = arr[middle + 1 + j];
  }
  int k = 0;
  i = 0;
  j = 0;
  //copiar los valores en orden ascendente 
  while (i < left_length && j < right_length) {
    if (left_array[i] <= right_array[j]) {
      arr[left + k] = left_array[i];
      i ++;
    } else {
      arr[left + k] = right_array[j];
      j ++;
    }
    k ++;
  }

 //valores restantes a la matriz  
  while (i < left_length) {
    arr[left + k] = left_array[i];
    k ++;
    i ++;
  }
  while (j < right_length) {
    arr[left + k] = right_array[j];
    k ++;
    j ++;
  }
}
//combina los threads
void combine_pthreads( int threads, int aggregation) {
  for(int i = 0; i < threads; i = i + 2) {
    int left = i * (NUMEROS_POR_THREAD * aggregation);
    int right = ((i + 2) * NUMEROS_POR_THREAD * aggregation) - 1;
    int middle = left + (right - left) / 2;
    if (right >= LENGTH) {
      right = LENGTH - 1;
    }
    combine_array(left, middle, right);
  }
  if (threads / 2 >= 1) {
    combine_pthreads(threads / 2, aggregation * 2);
  }
}
void printMatriz(int arr[],int left, int right){
  for(int i=left; i <= right; i++){
    printf(" %d ",arr[i]);
  }
  printf("\n");
}
int main() {
  srand(time(NULL));
  //datos de prueba  
  for (int i = 0; i < LENGTH; i ++) {
    arr[i] = rand() % 9; 
  }
  printMatriz(arr,0,9); 
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i ++) {
    pthread_create(&threads[i], NULL, thread_function, (void *)i);

  }
  for(long i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  combine_pthreads(NUM_THREADS, 1);
  printf("Resultado: \n");
  printMatriz(arr,0,9);

  return 0;
}
