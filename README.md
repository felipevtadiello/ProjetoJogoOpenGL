# Jogo Gráfico em OpenGL

## Descrição do Projeto

Este projeto é um jogo simples em 2D desenvolvido em C++ utilizando as bibliotecas *OpenGL, **GLEW* e *GLFW*. O objetivo é controlar o personagem, movendo-o para a esquerda e para a direita e evitando os obstáculos que surgem periodicamente.

O cenário do jogo é composto por vários elementos gráficos, como árvores, uma casa, um sol, nuvens e uma cerca. Cada elemento foi desenhado utilizando primitivas gráficas básicas, como triângulos, quadrados e círculos.

### Controles do Jogo

- *Mover para a Esquerda*: Pressione a tecla A.
- *Mover para a Direita*: Pressione a tecla D.
- *Pular*: Pressione a tecla Espaço.

O objetivo é evitar colisões com os obstáculos (tiros) que surgem da direita para a esquerda. Caso ocorra uma colisão, o personagem voltará para a posição inicial.

## Elementos do Cenário

- *Sol*: Um círculo laranja localizado no canto superior esquerdo.
- *Nuvens*: Desenhadas com três círculos sobrepostos para formar uma aparência de aglomerado, distribuídas na parte superior do cenário.
- *Casa*: Composta por um telhado triangular vermelho e um corpo amarelo com uma janela e uma porta.
- *Árvores*: Diferentes tipos de árvores são representadas, algumas com copa circular, outras com copa triangular e até um pinheiro de múltiplas camadas.
- *Cerca*: Desenhada na parte inferior do cenário, composta por pilares verticais e linhas horizontais para dar a aparência de uma cerca de madeira.
- *Jogador*: Representado por um quadrado cinza (corpo) e um círculo bege (cabeça) que pode se mover para os lados e pular.




## Explicação das Funções

O projeto utiliza as seguintes funções principais para desenhar e controlar o jogo:

1. *Funções de Desenho*:
   - drawTriangle(), drawSquare(), drawCircle(), drawDiamond(): São responsáveis por desenhar as primitivas gráficas que compõem o cenário.

2. *Elementos do Jogo*:
   - drawSun(), drawHouse(), drawGround(), drawTrees(), drawFence(), drawCloud(): Funções que combinam as primitivas gráficas para criar elementos visuais.

3. *Movimentação do Jogador*:
   - handleInput(): Verifica as entradas do usuário para controlar o movimento do personagem.
   - drawPlayer(): Desenha o jogador na tela.

4. *Obstáculos e Colisões*:
   - spawnProjectiles(), updateProjectiles(): Geram e movimentam os obstáculos (tiros).
   - checkCollision(), resetPlayerPosition(): Verificam se o jogador colidiu com um obstáculo e, caso positivo, reinicia sua posição.

5. *Loop Principal do Jogo*:
   - O jogo roda em um loop contínuo (while (!glfwWindowShouldClose(window))) até que o jogador feche a janela. A cada iteração, o jogo verifica a entrada do jogador, atualiza o estado dos obstáculos, verifica colisões, e finalmente renderiza o cenário.


## Instruções para Executar o Projeto

### Requisitos

Para executar este projeto, você precisará dos seguintes componentes:

- *Compilador C++* (por exemplo, g++).
- *Bibliotecas GLEW e GLFW* instaladas no sistema.
- *Sistema com suporte para OpenGL*.

### Passos para Compilar e Executar

1. **Clone ou Baixe o Código-Fonte**: Faça o download deste repositório para o seu computador.
2. **Instale as Dependências**: Instale as bibliotecas necessárias (GLEW e GLFW).
3. **Compile o Código**.
4. **Execute o Jogo**.
 
