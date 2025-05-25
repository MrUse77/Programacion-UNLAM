/*
 * #include <pthread.h>
 * #include <stdio.h>
 * pthread_mutex_t mutex;
 * int shared_data =0;
 * void *increment_data(void *arg) {    pthread_mutex_lock(&mutex);
 * shared_data++;
 * printf("Thread incremented data to %d\n", shared_data);
 * pthread_mutex_unlock(&mutex);
 * return NULL;
 * }
 * int main() {
 * pthread_t thread1, thread2;
 * pthread_mutex_init(&mutex, NULL);
 * pthread_create(&thread1, NULL, increment_data, NULL);
 * pthread_create(&thread2, NULL, increment_data, NULL);
 * pthread_join(thread1,NULL);
 * pthread_join(thread2, NULL);
 * pthread_mutex_destroy(&mutex);
 * printf("Final data value: %d\n", shared_data);
 * return 0;}
 */
