#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define N 10000
#define MAX 1000
#define FOR(i, n, k) for(int i = 0; i < n; i += k)

int arr[N];
sem_t mutex;

typedef struct PARAMETER{
    int left;
    int right;
}PARAMETER;

void generateData(){
    FOR(i, N, 1)
        arr[i] = (rand() % MAX) + 1;
}

void printArray(int left, int right){
    printf("Array: ");
    FOR(i, right-left+1, 1)
        printf("%d ", arr[i+left]);
    printf("\n");
}

void mergeArray(int left, int middle, int right){
    int length = right + 1 - left;
    int auxiliar[length];
    int x = left, y = middle + 1;

    sem_wait(&mutex);

    // passing values
    FOR(i, length, 1){
        if(x < middle+1 && y < right+1){
            if(arr[x] <= arr[y])
                auxiliar[i] = arr[x], x++;
            else
                auxiliar[i] = arr[y], y++;
        }else if(x < middle+1){
            auxiliar[i] = arr[x];
            x++;
        }else{
            auxiliar[i] = arr[y];
            y++;
        }
    }

    // coping values
    FOR(i, length, 1)
        arr[i + left] = auxiliar[i];

    sem_post(&mutex);
}

void* mergeSort(void* args){
    PARAMETER p = *((PARAMETER*)args);
    int left = p.left;
    int right = p.right;
    int middle = (right + left) / 2;

    if(left < right){
        pthread_t thread1, thread2;
        PARAMETER p1, p2;

        // first
        p1.left = left, p1.right = middle;
        pthread_create(&thread1, NULL, mergeSort, &p1);
        //second
        p2.left = middle + 1, p2.right = right;
        pthread_create(&thread2, NULL, mergeSort, &p2);

        // join
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        mergeArray(left, middle, right);
    }
    pthread_exit(NULL);
}

void sort(){
    PARAMETER param;
    param.left = 0;
    param.right = N - 1;
    pthread_t thread;
    sem_init(&mutex, 0, 1);

    // call
    pthread_create(&thread, NULL, mergeSort, &param);
    pthread_join(thread, NULL);
}

void validateSort(){
    FOR(i, N-1, 1){
        if(arr[i] > arr[i+1]){
            printf("Unsorted array\n");
            exit(1);
        }
    }
}

int main(){
    clock_t begin, end;
    float time_spent = 0.0;
    srand(time(NULL));
    generateData();

    begin = clock();
    //printArray(0, N-1);
    sort();
    //printArray(0, N-1);
    end = clock();

    time_spent += ((float)end - (float)begin) / CLOCKS_PER_SEC;
    validateSort();
    printf("Time: %.5f\n", time_spent);
    return 0;
}
