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

// vari�veis do jogador (personagem)
float playerX = -0.8f;  // posi��o X inicial do jogador
float playerY = -0.5f;  // posi��o Y inicial do jogador (ch�o)
float playerWidth = 0.1f, playerHeight = 0.1f; // dimens�es do jogador
float initialPlayerX = -0.8f;  // posi��o inicial do jogador, que � usada para resetar ap�s colis�o
bool isJumping = false; // estado de pulo (se o jogador est� pulando ou n�o)
float jumpSpeed = 0.02f; // velocidade inicial do pulo
float gravity = 0.001f;  // gravidade que puxa o jogador para baixo
float verticalVelocity = 0.0f;  // velocidade vertical atual do jogador (usada para calcular pulo e queda)
bool gameOver = false; // indicador de fim de jogo
float playerSpeed = 0.02f;  // velocidade de movimenta��o horizontal do jogador

// vari�veis dos tiros - obst�culos
std::vector<float> projectiles;  // vetor que armazena a posi��o X dos tiros (obst�culos)
float projectileSpeed = 0.03f;  // velocidade com que os tiros se movem da direita para a esquerda
float spawnTimer = 0.0f; // temporizador para controlar o intervalo entre tiros gerados
float spawnDelay = 2.0f; // tempo de atraso entre o surgimento dos tiros em segundos

// fun��o para desenhar um tri�ngulo
void drawTriangle(float x, float y, float size, float red, float green, float blue, float alpha) {
    // define a cor do tri�ngulo e desenha utilizando v�rtices 
    glColor4f(red, green, blue, alpha);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size / 2, y + size);
    glEnd();
}

// Fun��o para desenhar um quadrado
void drawSquare(float x, float y, float width, float height, float red, float green, float blue, float alpha) {
    // define a cor do quadrado e desenha utilizando v�rtices
    glColor4f(red, green, blue, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// fun��o para desenhar um c�rculo
void drawCircle(float cx, float cy, float radius, int num_segments, float red, float green, float blue) {
    // define a cor do c�rculo e desenha utilizando um loop para criar v�rtices ao longo do contorno
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

// fun��o para desenhar um losango
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

// desenhar uma nuvem composta por tr�s c�rculos sobrepostos
void drawCloud(float cx, float cy, float radius) {
    // tr�s c�rculos brancos sobrepostos para criar o efeito de nuvem
    drawCircle(cx, cy, radius, 20, 1.0f, 1.0f, 1.0f); // c�rculo principal
    drawCircle(cx - radius * 0.6f, cy, radius * 0.8f, 20, 1.0f, 1.0f, 1.0f); // c�rculo � esquerda
    drawCircle(cx + radius * 0.6f, cy, radius * 0.8f, 20, 1.0f, 1.0f, 1.0f); // c�rculo � direita
}

// fun��o para desenhar o sol no canto superior esquerdo
void drawSun() {
    drawCircle(-0.7f, 0.7f, 0.2f, 30, 1.0f, 0.5f, 0.0f);  // Sol laranja
}

// fun��o para desenhar uma cerca
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

// fun��o para desenhar uma casa
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

// fun��o para desenhar o ch�o e a grama
void drawGround() {
    // ch�o marrom
    drawSquare(-1.0f, -1.0f, 2.0f, 0.3f, 0.6f, 0.3f, 0.0f, 1.0f);
    // grama verde acima do ch�o marrom
    drawSquare(-1.0f, -0.7f, 2.0f, 0.2f, 0.0f, 0.6f, 0.0f, 1.0f);
}

// fun��o para desenhar �rvores no cen�rio
void drawTrees() {
    // primeira �rvore - tronco fino e copa circular
    drawSquare(-0.8f, -0.5f, 0.05f, 0.2f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawCircle(-0.775f, -0.25f, 0.15f, 30, 0.0f, 0.8f, 0.0f);

    // segunda �rvore - tronco fino, copa circular e um losango no topo
    drawSquare(-0.4f, -0.5f, 0.05f, 0.2f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawCircle(-0.375f, -0.25f, 0.15f, 30, 0.0f, 0.8f, 0.0f);
    drawDiamond(-0.375f, -0.05f, 0.1f, 0.0f, 0.8f, 0.0f, 1.0f);

    // terceira �rvore - tronco fino e copa triangular
    drawSquare(0.83f, -0.7f, 0.05f, 1.0f, 0.6f, 0.3f, 0.0f, 1.0f);
    drawTriangle(0.7f, 0.2f, 0.3f, 0.0f, 0.8f, 0.0f, 1.0f);

    // quarta �rvore - pinheiro com tr�s camadas triangulares
    drawSquare(-0.025f, -0.7f, 0.05f, 0.6f, 0.6f, 0.3f, 0.0f, 1.0f); // tronco
    drawTriangle(-0.15f, -0.2f, 0.3f, 0.0f, 0.5f, 0.0f, 1.0f);  // primeira camada
    drawTriangle(-0.13f, 0.0f, 0.25f, 0.0f, 0.6f, 0.0f, 1.0f);  // segunda camada
    drawTriangle(-0.11f, 0.15f, 0.2f, 0.0f, 0.7f, 0.0f, 1.0f);  // terceira camada
}

// desenhar o jogador na posi��o atual
void drawPlayer(float x, float y, float width, float height) {
    // corpo do jogador
    drawSquare(x, y, width, height, 0.5f, 0.5f, 0.5f, 1.0f);
    // cabe�a do jogador
    drawCircle(x + width / 2, y + height + 0.05f, width / 2, 20, 1.0f, 0.8f, 0.6f);
}

// fun��o para lidar com o input do jogador, como movimenta��o e pulo
void handleInput(GLFWwindow* window) {
    // movimenta��o para a esquerda tecla a
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerX -= playerSpeed; // decrementa a posi��o X
        if (playerX < -1.0f) {  // limite da borda esquerda da tela
            playerX = -1.0f; // garante que o jogador n�o ultrapasse a borda
        }
    }

    // Movimenta��o para a direita tecla d
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerX += playerSpeed; // incrementa a posi��o X
        if (playerX + playerWidth > 1.0f) { // limite da borda direita
            playerX = 1.0f - playerWidth; // garante que o jogador n�o ultrapasse a borda
        }
    }

    // pular = espa�o
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        isJumping = true; // seta o estado do jogador como pulando
        verticalVelocity = jumpSpeed; // inicializa a velocidade vertical do pulo
    }

    // atualizar o movimento vertical se estiver pulando
    if (isJumping) {
        playerY += verticalVelocity; // atualiza a posi��o Y com base na velocidade vertical
        verticalVelocity -= gravity; // aplica a gravidade para reduzir a velocidade vertical

        // checa se o jogador voltou ao ch�o
        if (playerY <= -0.5f) {
            playerY = -0.5f; // posiciona o jogador no ch�o
            isJumping = false; // o jogador n�o est� mais pulando
        }
    }
}

// fun��o para gerar tiros periodicamente
void spawnProjectiles() {
    spawnTimer += 0.01f; // incrementa o temporizador
    if (spawnTimer >= spawnDelay) { // ve se o temporizador ultrapassou o atraso necess�rio
        projectiles.push_back(1.0f); // adiciona um novo tiro na posi��o X = 1.0 (fora da tela, a direita)
        spawnTimer = 0.0f; // reseta o temporizador
    }
}

// fun��o para atualizar a posi��o dos tiros (obst�culos)
void updateProjectiles() {
    // move cada tiro para a esquerda
    for (auto& projectile : projectiles) {
        projectile -= projectileSpeed; // decrementa a posi��o X de cada tiro
    }

    // remove os tiros que sa�ram do limite esquerdo da tela
    if (!projectiles.empty() && projectiles.front() < -1.2f) {
        projectiles.erase(projectiles.begin()); // remove o primeiro elemento que est� fora da tela
    }
}

// fun��o para verificar colis�o entre o jogador e os tiros 
bool checkCollision() {
    for (const auto& projectile : projectiles) {
        // verifica se h� sobreposi��o entre o jogador e um tiro
        if (playerX + playerWidth > projectile && playerX < projectile + 0.1f && playerY < -0.4f) {
            return true; // colis�o detectada
        }
    }
    return false; // caso nao tenha colis�o detectada
}

// fun��o para resetar a posi��o do jogador ap�s colis�o
void resetPlayerPosition() {
    std::cout << "Colis�o detectada! Jogador voltando � posi��o inicial.\n";
    playerX = initialPlayerX; // redefine a posi��o do jogador
}

// fun��o de renderiza��o para desenhar todos os elementos na tela
void render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // desenha o c�u azul como fundo
    glClear(GL_COLOR_BUFFER_BIT); // limpa o buffer de cor

    // desenhar elementos do cen�rio
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

    // desenha os tiros (obst�culos)
    for (const auto& projectile : projectiles) {
        drawSquare(projectile, -0.4f, 0.1f, 0.02f, 1.0f, 0.0f, 0.0f, 1.0f);  // define a cor vermelha e expessura fina
    }

    glfwSwapBuffers(glfwGetCurrentContext()); // troca os buffers para renderizar o frame
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // inicializa o gerador de n�meros aleat�rios

    // inicializa��o do GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return -1;
    }

    // cria��o da janela GLFW
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "JOGO", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // define o contexto atual da janela

    // inicializa��o do GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        return -1;
    }

    // configura��o do OpenGL para habilitar a transpar�ncia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, windowWidth, windowHeight); // define a �rea de visualiza��o
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // define a proje��o ortogr�fica

    // onfigura��es de taxa de atualiza��o 
    const float FPS = 60.0f;
    const float frameDelay = 1.0f / FPS;
    float previousTime = glfwGetTime();

    // loop principal
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - previousTime;

        // controle de FPS para garantir que a l�gica e renderiza��o sejam consistentes
        if (elapsedTime >= frameDelay && !gameOver) {
            previousTime = currentTime;

            // gerencia o input do jogador e atualiza o estado do jogo
            handleInput(window);
            spawnProjectiles();
            updateProjectiles();

            // verifica colis�es e reseta posi��o se necess�rio
            if (checkCollision()) {
                resetPlayerPosition();
            }

            // renderiza todos os elementos na tela
            render();
            glfwPollEvents(); // processa eventos como input do usu�rio
        }
    }

    glfwTerminate();
    return 0;
}