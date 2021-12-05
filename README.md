# Programacao Concorrente 2021.1
Exercícios feitos na disciplina de Programação Concorrente em 2021.1

Programação concorrente é um paradigma de programação para a construção de programas de computador que fazem uso da execução simultânea de várias tarefas computacionais interativas, que podem ser implementadas como programas separados ou como um conjunto de threads criadas por um único programa. Essas tarefas podem ser executadas por um único processador, vários processadores em um único equipamento ou processadores distribuídos por uma rede. 

Compilação e execução dos programas: 

```gcc -pthread programa.c -o programa``` 

```./programa```

# Links 

[Manual de Programação com Pthreads](https://hpc-tutorials.llnl.gov/posix/)

# Estudos dirigidos

<details>
    <summary>Estudo Dirigido 2 - Problema dos leitores e escritores</summary>
    
Este problema é uma abstração do acesso à base de dados, onde não existe o perigo em termos diversos processos lendo concorrentemente (leitores), mas escrevendo ou mudando os dados (escritores) deve ser feito sob exclusão mútua para garantir consistência.
Leitores e escritores é uma família de modelos de controle de concorrência em que leitores (entes que não alteram conteúdo) pode acessar concorrentemente os recursos (por exemplo, um banco de dados) e escritores (entes que alteram conteúdo) requerem acesso exclusivo.

- [Solução](/EstudosDirigidos/estudoDirigido2.c)

</details>

<details>
    <summary>Estudo Dirigido 3 - Problema dos macacos </summary>

Suponha que há macacos em dois morros A e B, ligados por uma ponte de corda. De tempos em tempos os macacos decidem passar para o outro morro à procura de comida. Esta passagem é feita através da ponte de corda. Mais de um macaco pode atravessar a ponte ao mesmo tempo, mas isso só é possível se eles estiverem indo na mesma direção. Implemente um programa que faça o controle da passagem de macacos pela ponte usando locks.

DESAFIO: Após testar o programa acima, crie agora uma nova verão do programa adicionando dois gorilas, um em cada morro. Como os gorilas são muito pesados, eles só poderão atravessar a ponte sozinhos.

- [Solução](/EstudosDirigidos/estudoDirigido3.c)

</details>

<details>
    <summary>Estudo Dirigido 4 e 6 - Problema dos produtores e consumidores </summary>

O problema do produtor e consumidor (também conhecido como o problema do buffer limitado), consiste em um conjunto de processos que compartilham um mesmo buffer. Os processos chamados produtores põem informação no buffer. Os processos chamados consumidores retiram informação deste buffer. Quando o buffer estiver cheio, os produtores devem aguardar por posições disponíveis para inserir os dados produzidos. Da mesma forma, quando o buffer estiver vazio os consumidores devem aguardar por dados para serem consumidos.

- [Solução usando variáveis de condição](/EstudosDirigidos/estudoDirigido4.c)

- [Solução usando semáforo](/EstudosDirigidos/estudoDirigido6.c)

</details>

<details>
    <summary>Estudo dirigido 5 - Problema dos Canibais </summary>

Uma tribo de N canibais selvagens está jantando os missionários capturados. Eles ficam ao redor de um grande caldeirão contendo X porções de ensopado de missionário cozido. Quando um selvagem tem fome, ele se serve no caldeirão (pega 1 porção), a menos que esteja vazio (neste caso, o canibal deve aguardar/dormir até que o **cozinheiro** forneça mais porções). Quando um canibal pegar a última porção, o mesmo deve acordar o cozinheiro para que ele prepare mais X porções de ensopado e encha o caldeirão. Após encher a tigela, o cozinheiro acorda os canibais selvagens que estavam esperando o ensopado ficar pronto e volta a dormir.

- [Solução](/EstudosDirigidos/estudoDirigido5.c)

</details>

<details>
    <summary>Estudo dirigido 7 - Problema do barbeiro dorminhoco </summary>

Um outro problema clássico de IPC acontece em uma barbearia. Na barbearia há um barbeiro, uma cadeira de barbeiro e N cadeiras para eventuais clientes esperarem a vez. Quando não há clientes, o barbeiro senta-se e cai no sono (fica bloqueado). Quando chega um cliente, ele precisa acordar o barbeiro. Se outros clientes chegarem enquanto o barbeiro estiver cortando o cabelo de um cliente, eles se sentarão (se houver cadeiras vazias) ou sairão da barbearia (se todas as cadeiras estiverem ocupadas). O problema é programar o barbeiro e os M clientes sem cair em condições de disputa. Esse problema é semelhante a situações com várias filas, como uma mesa de atendimento de telemarketing com diversos atendentes e com um sistema computadorizado de chamadas em espera, atendendo a um número limitado de chamadas que chegam.

- [Solução](/EstudosDirigidos/estudoDirigido7.c)

</details>

<details>
    <summary>Estudo dirigido 8 - Problema do jantar dos filósofos</summary>

O Jantar dos filósofos foi proposto por Dijkstra em 1965 como um problema de sincronização. A partir de então todos os algoritmos propostos como soluções de sincronização acabaram sendo relacionados ou testados contra o problema do Jantar dos filósofos.

Cinco filósofos estão sentados em uma mesa redonda para jantar. Cada filósofo tem um prato com espaguete à sua frente. Cada prato possui um garfo para pegar o espaguete. O espaguete está muito escorregadio e, para que um filósofo consiga comer, será necessário utilizar dois garfos. 

Lembre-se que é apenas uma analogia. Nesse sentido, cada filósofo alterna entre duas tarefas: comer ou pensar. Quando um filósofo fica com fome, ele tenta pegar os garfos à sua esquerda e à sua direita; um de cada vez, independente da ordem. Caso ele consiga pegar dois garfos, ele come durante um determinado tempo e depois recoloca os garfos na mesa. Em seguida ele volta a pensar.

- [Solução](/EstudosDirigidos/estudoDirigido8.c)

</details>

</details>


<details>
    <summary>Estudos dirigidos 9 e 10 - Problema do pombo-correio</summary>

Um pombo correio leva cartas entre os sites A e B, mas só quando o número de cartas acumuladas em sua mochila chegar a 20. Inicialmente, o pombo fica em A, esperando que existam 20 cartas para carregar, e dormindo enquanto não houver. Quando as cartas chegam a 20, o pombo deve levar todas as 20 cartas de A para B, e em seguida voltar para A, repetindo este ciclo. As cartas são escritas por uma quantidade qualquer de usuários. Cada usuário, quando tem uma carta pronta, coloca sua carta na mochila do pombo e volta a escrever uma nova carta. Caso o pombo tenha partido, ele deve esperar o seu retorno para colar a carta na mochila.

- [Solução usando semáforos](/EstudosDirigidos/estudoDirigido9.c)
- [Solução usando variáveis de condição](/EstudosDirigidos/estudoDirigido10.c)

</details>

<details>
    <summary>Estudo dirigido 11 - Problema da soma dos elementos de uma matriz</summary>

Utilizando como base o código para multiplicação de matrizes, faça um programa para somar os valores dos elementos de uma matriz utilizando várias thread

- [Solução](/EstudosDirigidos/estudoDirigido11.c)

</details>


# Outros Problemas

- [Pista F1](/OutrosProblemas/pistaF1.c)

- [Multiplicação de Matrizes (Barreiras)](/OutrosProblemas/matriz_multiplicacao.c)

- [ABCD (Barreiras)](/OutrosProblemas/ABCD.c)
