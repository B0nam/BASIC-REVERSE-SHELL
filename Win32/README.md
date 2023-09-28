# REVERSE SHELL SIMPLES PARA DISPOSITIVOS WINDOWS
Aqui está o código comentado de uma reverse shell para sistemas operacionais windows.

## COMO UTILIZAR?
Para utilizar, basta editar as informacoes presentes no cabeçalho do código inserindo o ip e a porta do servidor que recebera o acesso remoto. Após editar estas informacoes, compile o código e execute o excutavel na máquina alvo.

## INFORMACOES ADICIONAIS PARA A COMPILACAO
ATANÇÃO: Para que o código seja compilado corretamente é necessário utilizar o parametro -lWs2_32 para que o compilador vincule a bibliotca Ws2_32.lib ao programa.

```bash
gcc .\main.c -o main.exe -lWs2_32