# :white_check_mark: Minishell(project 2021)

## Описание
В одно время все программисты согласились с тем, что общение с компьютером с использованием переключателей 1/0 серьезно раздражает.
Вполне логично, что им пришла в голову идея общаться с компьютером с помощью интерактивных строк команд на языке, несколько близком к английскому.
Таким образом, основная задача этого проекта - написать свою собственную мини-оболочку на C.

## Немного теории  
### :page_facing_up: Fork
**The Fork system call** используется для создания нового процесса, называемого дочерним процессом, который выполняется одновременно с процессом, вызывающим fork () (родительский процесс). После создания нового дочернего процесса оба процесса выполнят следующую инструкцию после системного вызова fork (). Дочерний процесс использует тот же компьютер (счетчик программ), те же регистры процессора и те же открытые файлы, которые используются в родительском процессе. 

Он не принимает параметры и возвращает целочисленное значение. Различные значения, возвращаемые функцией fork (), показаны ниже. 

**Negative value:** не удалось создать дочерний процесс.
**Zero:** вернулся к вновь созданному дочернему процессу.
**A positive value** возвращается родителю. Это значение содержит идентификатор процесса вновь созданного дочернего процесса.
  
![](https://github.com/markveligod/minishell/raw/master/img/1.jpg)  
  
Предсказать результат следующей программы:
```
#include <stdio.h>  
#include <sys/types.h>  
#include <unistd.h>  
  
int main(void)  
{  
// make two processes that work the same way  
// program after this instruction  
    fork();  
    printf("Hello world!\n");  
    return (0);  
}  
```
Output:
```
Hello world!  
Hello world!  
```
The number of times `'hello'` is printed is equal to the number of processes created. The total number of processes = 2^n, where n is the number of fork system calls. So here n = 3, 2^3 = 8  
Let's put some placemark names for three rows:  
```
fork ();   // Line 1  
fork ();   // Line 2  
fork ();   // Line 3  
  
       L1       // There will be 1 child process  
    /     \     // created by line 1.  
  L2      L2    // There will be 2 child processes  
 /  \    /  \   //  created by line 2  
L3  L3  L3  L3  // There will be 4 child processes  
                // created by line 3  
```
Thus, there are a total of eight processes (new child processes and one original process).  
If we want to represent the relationships between processes in a tree hierarchy, this is as follows:  
Main process: P0  
Processes created by the 1st fork: **P1**  
Processes created by the 2nd fork: **P2, P3**  
Processes created by the 3rd fork: **P4, P5, P6, P7**
```
             P0  
         /   |   \  
       P1    P4   P2  
      /  \          \  
    P3    P6         P5  
   /  
 P7  
```
We predict the output of the next program:  
```
#include <stdio.h>  
#include <sys/types.h>  
#include <unistd.h>  
  
void forkexample()  
{  
    // child process, because the return value is zero  
    if (fork() == 0)  
        printf("Hello from Child!\n");  
    // parent process, because the return value is non-zero.  
    else  
        printf("Hello from Parent!\n");  
}  
int main()  
{  
    forkexample();  
    return (0);  
}  
```
Output:
```
1.  
Hello from Child!  
Hello from Parent!  
     (or)  
2.  
Hello from Parent!  
Hello from Child!  
```
The code above creates a child process. fork () returns 0 in the child process and a positive integer in the parent process.
There are two possible outputs here, because the parent process and the child process work simultaneously. So we don't know if the OS will first pass control to the parent process or to the child process.  
  
**Important:** the parent process and child process run the same program, but this does not mean that they are identical. The OS allocates different data and States for these two processes, and the control flow for these processes may be different.  
See the following example:

```
#include <stdio.h>  
#include <sys/types.h>  
#include <unistd.h>  
  
void forkexample()  
{  
    int x = 1;  
    if (fork() == 0)  
        printf("Child has x = %d\n", ++x);  
    else  
        printf("Parent has x = %d\n", --x);  
}  
  
int main()  
{  
    forkexample();  
    return (0);  
}  
```
Output:
```
Parent has x = 0  
Child has x = 2  
     (or)  
Child has x = 2  
Parent has x = 0  
```
Here, changing a global variable in one process does not affect the other two processes, because the data / state of the two processes is different. As well as the parent and child work simultaneously, so there are two possible outputs.  
  
### :page_facing_up: Execve()
**The execve() system call function** is used to execute a binary executable or a script.  
The function returns nothing on success and -1 on error.  
The first parameter must be the path of a binary executable or a script.  
The second must be an array of pointers on a character (char *myArray[]), and the last pointer must be set to NULL.  
The third parameter must be an environment.  
  
Example:
```
#include        <stdio.h>  
#include        <stdlib.h>  
#include        <unistd.h>  
#include        <string.h>  
#include        <errno.h>  
  
int     main()  
{  
  int e;  
  char *envp[] = { NULL };  
  char *argv[] = { "/bin/ls", "-l", NULL };  
  
  e = execve("/bin/ls", argv, envp);  
  if (e == -1)  
      fprintf(stderr, "Error: %s\n", strerror(errno));  
  return (0);  
}  
```
Output:
```
drwxr-xr-x  2 badprog tutorial  4096 March  6 19:03 .  
drwxr-xr-x 39 badprog tutorial  4096 March  6 18:51 ..  
-rwxr-xr-x  1 badprog tutorial  7031 March  6 19:03 execve-me  
-rw-r--r--  1 badprog tutorial   385 March  6 19:03 main.c  
```

### :page_facing_up: pipe
Conceptually, a pipe is a connection between two processes, so that the standard output of one process becomes the standard input of the other process. In the UNIX operating system, pipe are useful for communicating between related processes (inter-process communication).  
  
* The pipe is a one-way link, i.e. We can use the channel so that one process writes to the channel and the other writes from the channel. It opens a channel that is an area of main memory that is treated as a" virtual file".
* The pipe can be used by the creation process, as well as by all its child processes for reading and writing. One process can write to this "virtual file" or channel, and another related process can read from it.
* If a process tries to read before something is written to the feed, the process is paused until something is written.
The pipe system call finds the first two available positions in the open file table of the process and distributes them at the ends of the pipe for reading and writing.  
![](https://github.com/markveligod/minishell/raw/master/img/2.jpg)  
  
The syntax in C:
```
int pipe(int fds[2]);  
  
Parameters :  
fd[0] will be the fd(file descriptor) for the read end of pipe.  
fd[1] will be the fd for the write end of pipe.  
Returns : 0 on Success.  
-1 on error.  
```
Pipes behave like FIFO ("first in,first out"), pipes behave like a queue data structure . The read and write size should not be the same here. We can write 512 bytes at a time, but we can only read 1 byte at a time in the pipe.  

```
#include <stdio.h>  
#include <unistd.h>  
#define MSGSIZE 16  
char* msg1 = "hello, world #1";  
char* msg2 = "hello, world #2";  
char* msg3 = "hello, world #3";  
  
int main()  
{  
    char inbuf[MSGSIZE];  
    int p[2], i;  
    if (pipe(p) < 0)  
        exit(1);  
    write(p[1], msg1, MSGSIZE);  
    write(p[1], msg2, MSGSIZE);  
    write(p[1], msg3, MSGSIZE);  
    for (i = 0; i < 3; i++) {  
        read(p[0], inbuf, MSGSIZE);  
        printf("% s\n", inbuf);  
    }  
    return (0);  
}
```
Output:
```
hello, world #1  
hello, world #2  
hello, world #3  
```
  
When we use a fork in any process, the file descriptors remain open for the child process as well as for the parent process. If we call fork after creating the pipe, then parent and child can communicate through the pipe.  
  
![](https://github.com/markveligod/minishell/raw/master/img/3.jpg)  

```
#include <stdio.h>  
#include <unistd.h>  
#define MSGSIZE 16  
  
char* msg1 = "hello, world #1";  
char* msg2 = "hello, world #2";  
char* msg3 = "hello, world #3";  
  
int main()  
{  
    char inbuf[MSGSIZE];  
    int p[2], pid, nbytes;  
    if (pipe(p) < 0)  
        exit(1);  
    if ((pid = fork()) > 0) {  
        write(p[1], msg1, MSGSIZE);  
        write(p[1], msg2, MSGSIZE);  
        write(p[1], msg3, MSGSIZE);  
        wait(NULL);  
    }  
    else {  
        while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)  
            printf("% s\n", inbuf);  
        if (nbytes != 0)  
            exit(2);  
        printf("Finished reading\n");  
    }  
    return (0);  
}
```
Output:
```
hello world, #1  
hello world, #2  
hello world, #3  
(hangs)         //program does not terminate but hangs  
```
Here, In this code, after the read / write is complete, the parent and child blocks instead of completing the process, and so the program freezes. This is because the read system call gets as much data as it requests, or as much data as the pipe has, whichever is smaller.

* If the pipe is empty and we call the read system call, then Reads on the pipe will return EOF (the return value is 0) if none of the processes have the end of the write open.
* If some other process has a channel open for writing, the read is blocked while waiting for new data, so the output of this code freezes, because here the write terminates the parent process, and the child process is not closed.  


## Useful links:
[«Шелл» на С: пишем командную оболочку для Unix](https://tproger.ru/translations/unix-shell-in-c/)  
[Tutorial - Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)  
[Изучаем процессы в Linux](https://habr.com/ru/post/423049/)  
[Создание процессов с помощью вызова fork().](https://www.opennet.ru/docs/RUS/linux_parallel/node7.html)  
[Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)  
[Making your own Linux Shell in C](https://www.geeksforgeeks.org/making-linux-shell-c/)  
[Чтение содержимого директорий diropen, readdir, closedir](https://firststeps.ru/linux/r.php?20)  
[Несколько подробностей о функции main](https://habr.com/ru/post/128111/)  
[Коды ошибок для errno](https://gist.github.com/greggyNapalm/2413028)  
[Fork() execve and pipe](https://yanniss.github.io/k24/set005.pdf)

## Other

**Authors:**  
*[Mark Veligod](https://github.com/markveligod)*  
*[Polina Baranovskaya](https://github.com/polinariabar)*  
