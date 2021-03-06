#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <math.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080 
#define LEVELS 13
#define SHIFT 3
#define FILTERBUFLEN 28
char *password[12] = {"entendido\n","itba\n","M4GFKZ289aku\n","fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n","K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n","chin_chu_lan_cha\n","gdb_rules\n","normal\n"};
const int run_me __attribute__((section(".RUN_ME")));
char *stringDes4 = "too_easy";
char *filterbuff = "La respuesta es K5n2UFfpFMUN";


void generateNormalNumbers(){
    int i;
    int m = 1000;
        for ( i = 0; i < m; i += 2 )
        {
            double x,y,rsq,f;
            do {
                x = 4.0 * rand() / (double)RAND_MAX - 1.0;
                y = 4.0 * rand() / (double)RAND_MAX - 1.0;
                rsq = x * x + y * y;
            }while( rsq >= 1. || rsq == 0. );
            f = sqrt( -2.0 * log(rsq) / rsq );
            printf("%f ", x * f);
            printf("%f ", y * f);
        }
        putchar('\n');
}

void quineVerify(){
    int num;
    num = system("gcc -o quine quine.c");
    if ( num == 0)
        printf("¡Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde.\n");
    else {
        printf("\nENTER para reintentar.\n");
        return;
    }
    system("./quine > quine.txt");
    num = system("diff quine.c quine.txt");
    if ( num == 0)
        printf("La respuesta es: %s\n", password[9]);
    else
        printf("diff encontró diferencias\n");
    system("rm quine.txt");
}
void gdbme() {
    if (getpid() == 0x12345678)
        puts("La respuesta es gdb_rules\n");
}

void printRandChar(int len) {
    char c;
    while (len > 0) {
        c = (rand() % 125) + 32;
        fprintf(stderr, "%c",c);
        len--;
    }
    fflush(stderr);
}

void filterDes() {
    int i = 0;
    int trashlen;
    srand(time(NULL));
    while (i < FILTERBUFLEN) {
        trashlen = (rand() % 8) + 1 ;
        printRandChar(trashlen);
        printf("%c",filterbuff[i]);
        fflush(stdout);
        i++;
    }
    printf("\n");
}

void desafio(int desafio) {
    system("clear");
    printf("------------- DESAFIO -------------\n");
    switch(desafio) {
        case 0: printf("Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\nPara continuar entre: entendido\\n.\n");break;
        case 1: printf("The Wire S1E5 \n5295 888 6288\n");break;
        case 2: printf("https://ibb.co/tc0Hb6w\n");break;
        case 3: write(13, "La respuesta es fk3wfLCm3QvS\n\0\0\0", 32);printf("EBADF...\n\n");perror("write");break; 
        case 4: printf("respuesta = strings:49\n");break;
        case 5: printf(".data ? .bss .comment .shstrtab .symtab .strtab\n");break;
        case 6: printf("Filter error\n");filterDes();break;
        case 7: printf("¿?\n\n\033[40m \033[30m La respuesta es BUmyYq5XxXGt \033[0m\033[0m\n");break;
        case 8: printf("Latexme\nSi\n\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\ny =\n");break;
        case 9: printf("quine\n");quineVerify();break;
        case 10: printf("b gdbme y encontrá el valor mágico\nENTER para reintentar.\n");gdbme();break;
        case 11: printf("Me conoces\n\n");generateNormalNumbers();break;
        case 12: printf("Felicitaciones, finalizaron el juego.\n");
    }
    
}

int main(int argc, char const *argv[]) 
{ 
    int currentDes = 0;
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    


    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    

    while (currentDes < LEVELS - 1) {
        desafio(currentDes);
        fflush(stdout);
        valread = read( new_socket , buffer, 1024);
        buffer[valread] = 0;
        if (strcmp(buffer,password[currentDes]) == 0)
            currentDes++;
        else {
            if (buffer[0] == 0)
                break;
            printf("Respuesta incorrecta: %s\n", buffer);
            sleep(3);
        }
    }
    desafio(currentDes);
    return 0; 
} 