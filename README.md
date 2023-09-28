# Basic Reverse Shell
Uma reverse shell básica escrita em linguagem C/C++.

## O que é uma reverse shell
Uma reverse shell é a uma shell remota, onde o alvo se conecta ao atacante. Revese Shells são comunmente utilizadas durate a etapa de exploração para se obter um acesso remoto a máquina alvo.

## Por que uma conexão reversa e não uma conexão comum?
A reverse shell é uma eficiente forma de burlar restrições de rede, onde as conexões de rede recebidas são filtradas. Como no reverse shell é a maquina alvo que se conecta ao atacante, o código obtem vantagens sobre portas de saída comuns, como 80, 443, 8080 e etc...

### Como funciona?
Para que se obtenha uma reverse shell são necessários três aspectos:
 - Execução de codigos na máquina alvo
 - Um Listener na maquina atacante. (Para obter a conexão remota)
 - Execução da Reverse Shell na máquina alvo.