#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_SIZE 1024
static char receive[BUFFER_SIZE];

int main(){
   int ret, fd;
   char stringToSend[BUFFER_SIZE];
   printf("Testing mydevice...\n");
   fd = open("/dev/MyCharDev", O_RDWR);
   if (fd < 0){
      perror("Failed to open mydevice.\n");
      return -1;
   }
   printf("Type a string:\n");
   scanf("%[^\n]%*c", stringToSend);//%*c ignore the trailing character
   printf("Sending message: [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend));
   if (ret < 0){
      perror("Failed to write to mydevice.\n");
      return -1;
   }

   printf("Press ENTER to continue.\n");
   getchar();

   ret = read(fd, receive, BUFFER_SIZE);
   if (ret < 0){
      perror("Failed to read from mydevice.\n");
      return -1;
   }
   printf("Receiving message: [%s]\n", receive);
   printf("Test Complete.\n");
   return 0;
}
