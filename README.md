Nome dos participantes:
- Diana Naranjo

Características do sistema:
* CPU: Intel Core i5
* SO: Ubuntu 14.04 (64 bit)

Instruções em geral:
- Fazer um merge dos arquivos e pastas da pasta src do EP com a pasta src do paquete pbrt.
- Dentro da pasta src, realizar a compilação com o arquivo Makefile (cmd: make), se precisar fazer uma limpeza (cmd: make clean).
- Criar um symbolic link do executable pbrt (cmd: sudo ln -s /pbrt/directory/src/bin/pbrt /usr/local/bin/pbrt).
- Dentro da pasta files(pbrt-grammar) executar cada um dos arquivos pbrt (cmd: pbrt nome_arquivo.pbrt), se precisar o tempo execute o comando time (cmd: time pbrt nome_arquivo.pbrt).

Notas:
* Para realizar a compilação foi necessario instalar bison e flex.
* Dentro da pasta files(pbrt-grammar)/lsystem encontram-se as grammaticas usadas e alguns arquivos auxiliares para o material dos shapes; igualmente dentro da pasta files(pbrt-grammar)/textures encontram-se as texturas usadas.

Tempo de execução:
lsystem.pbrt: 2m58.483s (rendering: 17.2s)
lsystem2.pbrt: 0m26.479s (rendering: 5.3s)
lsystem3.pbrt: 0m14.371s (rendering: 3.1s)
lsystem4.pbrt: 0m13.606s (rendering: 4.0s)
lsystem5.pbrt: 0m13.402s (rendering: 3.6s)
lsystem6.pbrt: 0m22.664s (rendering: 5.8s)
lsystem7.pbrt: 4m43.632s (rendering: 54.6s)

