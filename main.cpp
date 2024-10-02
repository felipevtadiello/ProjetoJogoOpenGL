#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

// configs da janela
const int windowWidth = 800;  // largura da janela
const int windowHeight = 600; // altura da janela

// variáveis do jogador (personagem)
float playerX = -0.8f;  // posição X inicial do jogador
float playerY = -0.5f;  // posição Y inicial do jogador (chão)
float playerWidth = 0.1f, playerHeight = 0.1f; // dimensões do jogador
float initialPlayerX = -0.8f;  // posição inicial do jogador, que é usada para resetar após colisão
bool isJumping = false; // estado de pulo (se o jogador está pulando ou não)
float jumpSpeed = 0.02f; // velocidade inicial do pulo
float gravity = 0.001f;  // gravidade que puxa o jogador para baixo
float verticalVelocity = 0.0f;  // velocidade vertical atual do jogador (usada para calcular pulo e queda)
bool gameOver = false; // indicador de fim de jogo
float playerSpeed = 0.02f;  // velocidade de movimentação horizontal do jogador

// variáveis dos tiros - obstáculos
std::vector<float> projectiles;  // vetor que armazena a posição X dos tiros (obstáculos)
float projectileSpeed = 0.03f;  // velocidade com que os tiros se movem da direita para a esquerda
float spawnTimer = 0.0f; // temporizador para controlar o intervalo entre tiros gerados
float spawnDelay = 2.0f; // tempo de atraso entre o surgimento dos tiros em segundos

// função para desenhar um triângulo
void drawTriangle(float x, float y, float size, float red, float green, float blue, float alpha) {
    // define a cor do triângulo e desenha utilizando vértices 
    glColor4f(red, green, blue, alpha);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size / 2, y + size);
    glEnd();
}

// Função para desenhar um quadrado
void drawSquare(float x, float y, float width, float height, float red, float green, float blue, float alpha) {
    // define a cor do quadrado e desenha utilizando vértices
    glColor4f(red, green, blue, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// função para desenhar um círculo
void drawCircle(float cx, float cy, float radius, int num_segments, float red, float green, float blue) {
    // define a cor do círculo e desenha utilizando um loop para criar vértices ao longo do contorno
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// função para desenhar um losango
void drawDiamond(float cx, float cy, float size, float red, float green, float blue, float alpha) {
    // desenha um losango 
    glColor4f(red, green, blue, alpha);
    glBegin(GL_QUADS);
    glVertex2f(cx, cy + size);       // topo
    glVertex2f(cx - size, cy);       // esquerda
    glVertex2f(cx, cy - size);       // baixo
    glVertex2f(cx + size, cy);       // direita
    glEnd();
}

// desenhar uma nuvem composta por três círculos sobrepostos
void drawCloud(float cx, float cy, float radius) {
    // três círculos brancos sobrepostos para criar o efeito de nuvem
    drawCircle(cx, cy, radius, 20, 1.0f, 1.0f, 1.0f); // círculo principal
    drawCircle(cx - radius * 0.6f, cy, radius * 0.8f, 20, 1.0f, 1.0f, 1.0f); // círculo à esquerda
    drawCircle(cx + radius * 0.6f, cy, radius * 0.8f, 20, 1.0f, 1.0f, 1.0f); // círculo à direita
}

// função para desenhar o sol no canto superior esquerdo
void drawSun() {
    drawCircle(-0.7f, 0.7f, 0.2f, 30, 1.0f, 0.5f, 0.0f);  // Sol laranja
}

// função para desenhar uma cerca
void drawFence() {
    float fenceY = -0.8f; // altura da cerca
    // desenha os pilares verticais da cerca
    for (float x = -1.0f; x < 1.0f; x += 0.1f) {
        drawSquare(x, fenceY, 0.02f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    // desenha as linhas horizontais da cerca
    for (float y = fenceY; y < fenceY + 0.2f; y += 0.05f) {
        drawSquare(-1.0f, y, 2.0f, 0.01f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

// função para desenhar uma casa
void drawHouse() {
    // telhado da casa
    glBegin(GL_TRIANGLES);
    glColor3f(0.5f, 0.0f, 0.0f); // telhado vermelho
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.6f, 0.2f);
    glVertex2f(1.0f, -0.2f);
    glEnd();

    // corpo da casa - amarelo
    drawSquare(0.2f, -0.7f, 0.8f, 0.5f, 1.0f, 0.8f, 0.2f, 1.0f);

    // janela e porta da casa - vermelho escuro
    drawSquare(0.7f, -0.4f, 0.1f, 0.1f, 0.6f, 0.0f, 0.0f, 1.0f); // janela
    drawSquare(0.3f, -0.7f, 0.1f, 0.4f, 0.6f, 0.0f, 0.0f, 1.0f); // porta
}

// função para desenhar o chão e a grama
void drawGround() {
    // chão marrom
    drawSquare(-1.0f, -1.0f, 2.0f, 0.3f, 0.6f, 0.3f, 0.0f, 1.0f);
    // grama verde acima do chão marrom
    drawSquare(-1.0f, -0.7f, 2.0f, 0.2f, 0.0f, 0.6f, 0.0f, 1.0f);
}

// função para desenhar árvores no cenário
void drawTrees() {
    // primeira árvore - tronco fino e copa circular
    drawSquare(-0.8f, -0.5f, 0.05f, 0.2f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawCircle(-0.775f, -0.25f, 0.15f, 30, 0.0f, 0.8f, 0.0f);

    // segunda árvore - tronco fino, copa circular e um losango no topo
    drawSquare(-0.4f, -0.5f, 0.05f, 0.2f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawCircle(-0.375f, -0.25f, 0.15f, 30, 0.0f, 0.8f, 0.0f);
    drawDiamond(-0.375f, -0.05f, 0.1f, 0.0f, 0.8f, 0.0f, 1.0f);

    // terceira árvore - tronco fino e copa triangular
    drawSquare(0.83f, -0.7f, 0.05f, 1.0f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawTriangle(0.7f, 0.2f, 0.3f, 0.0f, 0.8f, 0.0f, 1.0f);

    // quarta árvore - pinheiro com três camadas triangulares
    drawSquare(-0.025f, -0.7f, 0.05f, 0.6f, 0.6f, 0.3f, 0.0f, 1.0f); // tronco
    drawTriangle(-0.15f, -0.2f, 0.3f, 0.0f, 0.5f, 0.0f, 1.0f);  // primeira camada
    drawTriangle(-0.13f, 0.0f, 0.25f, 0.0f, 0.6f, 0.0f, 1.0f);  // segunda camada
    drawTriangle(-0.11f, 0.15f, 0.2f, 0.0f, 0.7f, 0.0f, 1.0f);  // terceira camada
}

// desenhar o jogador na posição atual
void drawPlayer(float x, float y, float width, float height) {
    // corpo do jogador
    drawSquare(x, y, width, height, 0.5f, 0.5f, 0.5f, 1.0f);
    // cabeça do jogador
    drawCircle(x + width / 2, y + height + 0.05f, width / 2, 20, 1.0f, 0.8f, 0.6f);
}

// função para lidar com o input do jogador, como movimentação e pulo
void handleInput(GLFWwindow* window) {
    // movimentação para a esquerda tecla a
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerX -= playerSpeed; // decrementa a posição X
        if (playerX < -1.0f) {  // limite da borda esquerda da tela
            playerX = -1.0f; // garante que o jogador não ultrapasse a borda
        }
    }

    // Movimentação para a direita tecla d
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerX += playerSpeed; // incrementa a posição X
        if (playerX + playerWidth > 1.0f) { // limite da borda direita
            playerX = 1.0f - playerWidth; // garante que o jogador não ultrapasse a borda
        }
    }

    // pular = espaço
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        isJumping = true; // seta o estado do jogador como pulando
        verticalVelocity = jumpSpeed; // inicializa a velocidade vertical do pulo
    }

    // atualizar o movimento vertical se estiver pulando
    if (isJumping) {
        playerY += verticalVelocity; // atualiza a posição Y com base na velocidade vertical
        verticalVelocity -= gravity; // aplica a gravidade para reduzir a velocidade vertical

        // checa se o jogador voltou ao chão
        if (playerY <= -0.5f) {
            playerY = -0.5f; // posiciona o jogador no chão
            isJumping = false; // o jogador não está mais pulando
        }
    }
}

// função para gerar tiros periodicamente
void spawnProjectiles() {
    spawnTimer += 0.01f; // incrementa o temporizador
    if (spawnTimer >= spawnDelay) { // ve se o temporizador ultrapassou o atraso necessário
        projectiles.push_back(1.0f); // adiciona um novo tiro na posição X = 1.0 (fora da tela, a direita)
        spawnTimer = 0.0f; // reseta o temporizador
    }
}

// função para atualizar a posição dos tiros (obstáculos)
void updateProjectiles() {
    // move cada tiro para a esquerda
    for (auto& projectile : projectiles) {
        projectile -= projectileSpeed; // decrementa a posição X de cada tiro
    }

    // remove os tiros que saíram do limite esquerdo da tela
    if (!projectiles.empty() && projectiles.front() < -1.2f) {
        projectiles.erase(projectiles.begin()); // remove o primeiro elemento que está fora da tela
    }
}

// função para verificar colisão entre o jogador e os tiros 
bool checkCollision() {
    for (const auto& projectile : projectiles) {
        // verifica se há sobreposição entre o jogador e um tiro
        if (playerX + playerWidth > projectile && playerX < projectile + 0.1f && playerY < -0.4f) {
            return true; // colisão detectada
        }
    }
    return false; // caso nao tenha colisão detectada
}

// função para resetar a posição do jogador após colisão
void resetPlayerPosition() {
    std::cout << "Colisão detectada! Jogador voltando à posição inicial.\n";
    playerX = initialPlayerX; // redefine a posição do jogador
}

// função de renderização para desenhar todos os elementos na tela
void render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // desenha o céu azul como fundo
    glClear(GL_COLOR_BUFFER_BIT); // limpa o buffer de cor

    // desenhar elementos do cenário
    drawSun();
    drawHouse();
    drawGround();
    drawTrees();
    drawFence();

    // desenhar nuvens
    drawCloud(0.0f, 0.8f, 0.1f);
    drawCloud(0.75f, 0.75f, 0.15f);
    drawCloud(0.35f, 0.87f, 0.1f);

    // desenhar jogador
    drawPlayer(playerX, playerY, playerWidth, playerHeight);

    // desenha os tiros (obstáculos)
    for (const auto& projectile : projectiles) {
        drawSquare(projectile, -0.4f, 0.1f, 0.02f, 1.0f, 0.0f, 0.0f, 1.0f);  // define a cor vermelha e expessura fina
    }

    glfwSwapBuffers(glfwGetCurrentContext()); // troca os buffers para renderizar o frame
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // inicializa o gerador de números aleatórios

    // inicialização do GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return -1;
    }

    // criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "JOGO", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // define o contexto atual da janela

    // inicialização do GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        return -1;
    }

    // configuração do OpenGL para habilitar a transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, windowWidth, windowHeight); // define a área de visualização
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // define a projeção ortográfica

    // onfigurações de taxa de atualização 
    const float FPS = 60.0f;
    const float frameDelay = 1.0f / FPS;
    float previousTime = glfwGetTime();

    // loop principal
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - previousTime;

        // controle de FPS para garantir que a lógica e renderização sejam consistentes
        if (elapsedTime >= frameDelay && !gameOver) {
            previousTime = currentTime;

            // gerencia o input do jogador e atualiza o estado do jogo
            handleInput(window);
            spawnProjectiles();
            updateProjectiles();

            // verifica colisões e reseta posição se necessário
            if (checkCollision()) {
                resetPlayerPosition();
            }

            // renderiza todos os elementos na tela
            render();
            glfwPollEvents(); // processa eventos como input do usuário
        }
    }

    glfwTerminate();
    return 0;
}