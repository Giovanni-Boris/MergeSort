#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define LENGTH 10
#define NUM_THREADS 3
//threads por tamaño

int arr[LENGTH];
void combine_array( int , int );
void printMatriz(int arr[],int,int);
void* merge_sort(void*a);
typedef struct sizes {
    int left;
    int right;
} SIZES;


//divide en posiciones
void* merge_sort(void *a) {
  SIZES *p = (SIZES *)a;
  SIZES t1, t2;
  int mid = (p->left+p->right)/2;
  pthread_t tid1, tid2;
  int retu;

  t1.left = p->left;
  t1.right = mid;

  t2.left = mid+1;
  t2.right = p->right;

  if (p->left >= p->right)
    return ;
  //Primer thread
  pthread_create(&tid1, NULL, merge_sort, &t1);
  //creating second thread
  pthread_create(&tid2, NULL, merge_sort, &t2);
  //joining the threads
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  combine_array(p->left, p->right);
  pthread_exit(NULL);

}
void combine_array( int left, int right) {
  int i = 0;
  int j = 0;
  int middle = (left+right)/2;
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
  SIZES n;
  n.left = 0;
  n.right = LENGTH -1;
  pthread_t tid;
  printMatriz(arr,0,9);

  pthread_create(&tid, NULL, merge_sort, &n);

  pthread_join(tid, NULL);

  printf("Resultado: \n");
  printMatriz(arr,0,9);

  return 0;
}
