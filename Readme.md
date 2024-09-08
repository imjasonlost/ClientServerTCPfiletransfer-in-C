
2)Problems I have been ran 
   i)Signalling ii)InterProcess-Zombie formation (Child & Parent) iii)Not able to copy exact of file Content 4)Saving each client by unique name
Solved 1)signal handler function ii)Used sigaction struct in proper form iii)Used read & write function for file descriptor iv)In order to save file used process ID which was unique
3)Still not able to terminate conncetion after 10 second. Still need to implement select() for timeout
4)BSD socket wikipedia link & geeks for geeks

Some extra Hints how my code will work
1)You have to define path from which you want to read files and send to server
 example-In my case- #define Path_Dir "E:\\Client\\"  Only I can send file from client folder.
2)I need to define directory parameter at server side by passing a extra parameter. 
  In my case- <D:\Save> parameterr. So all file going to be saved in Save folder of Director D:
3)All my saved file name will be by child process ID of client. If client is not sending anything then there will be empty .txt file. 
4)Still I need to implement 10 second timeout 
