/// Aline Hommerding Amorim & Mayra Camargo Cademartori
/// 00301570 & 00301639

/* Jogo SAVE SHREK onde o jogador principal é uma princesa que deve resgatar o ogro do castelo onde está sendo mantido e guardado por diversos agentes.
A princesa deve buscar no mínimo três chaves pelo mapa para abrir as portas do castelo, mas elas só serão reveladas por proximidade. O jogador tem
três vidas no total e deve tomar cuidado com os agentes que, também por proximidade, "matam" e subtraem uma vida sua. Os agentes não podem matar na
SAFEZONE, uma área 3x3 onde a princesa começa o jogo. Se as vidas do jogador chegarem a zero, o jogo acaba. Se o jogador adquirir três chaves e se
dirigir ao castelo do ogro, ele vence. Seu nome então é colocado no ranking e sua pontuação calculada de acordo com número de chaves coletadas, vidas
restantes e tempo de jogo. */

#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#define LIMX 80
#define LIMY 24
#define NUM_SEGMENTOS 5
#define NUM_PAREDES_MAX 50
#define NUM_PAREDES_FACIL 5
#define NUM_PAREDES_DIFICIL 10
#define NUM_RANK 10
#define NUM_GUARDAS_MAX 5
#define NUM_GUARDAS_MIN 3
#define NUM_CHAVES_MAX 5
#define NUM_CHAVES_MIN 3
#define FALSE 0
#define TRUE 1
#define SAFEX 4
#define SAFEY 12
#define NUM_CASTELO 9
#define DELAY_GUARDAS 30
#define MAX_NOME 30
#define MAX_AUX 50
#define DISTANCIA_CHAVES 4
#define DISTANCIA_GUARDAS 2
#define VIDAS 3
#define SAIR -1
#define X_MIN_CHAVES 2
#define X_MAX_CHAVES 77
#define Y_MIN_CHAVES 2
#define Y_MAX_CHAVES 22
#define X_MIN_GUARDAS 10
#define X_MAX_GUARDAS 68
#define Y_MIN_GUARDAS 5
#define Y_MAX_GUARDAS 13
#define BASE_PONTOS 850
#define VALOR_SEG 10
#define VALOR_CHAVE 50
#define VALOR_VIDA 20
#define QUANT_VIDAS 3
#define CURSOR_X 36
#define CURSOR_Y 13


typedef enum
{
    NULO,
    CIMA,
    BAIXO,
    DIREITA,
    ESQUERDA,
    ESQUERDA_CIMA,
    DIREITA_CIMA,
    ESQUERDA_BAIXO,
    DIREITA_BAIXO

} DIRECOES;

typedef enum
{
    MENU,
    FACIL,
    DIFICIL,
    CASUAL,
    CARREGAR,
    RECORDES,
    ENTER,
    ESC,
    PAUSA,
    TEMPORARIO,
    PERMANENTE

} OPCOES;

typedef struct
{
    int x;
    int y;
}
COORDENADA;

typedef struct
{
    COORDENADA posicao;
    int status; // 1 se foi pega, 0 se não foi pega
    int descoberta; // 1 se sim, 0 se não
}
CHAVE;

typedef struct
{
    char nome[MAX_NOME];
    COORDENADA posicao;
    int chaves;
    int vidas;
    int vitoria; //1 se o jogador ganhou, inicializado com 0
    float pontos;
}
JOGADOR;

typedef struct
{
    COORDENADA posicao;
}
GUARDA;

typedef struct
{
    COORDENADA posicao;
    int castelo; // controle pra excluir o castelo, se o jogador possuir 3 chaves
} OGRO;

typedef struct
{
    JOGADOR jogador;
    GUARDA guardas[NUM_GUARDAS_MAX];
    int num_guardas;
    CHAVE chaves[NUM_CHAVES_MAX];
    int num_chaves;
    COORDENADA paredes[NUM_PAREDES_MAX];
    int num_paredes;
    int segundos;
} JOGO;

typedef struct
{
    char nome[MAX_NOME];
    int pontos;
} RANKING;

typedef struct
{
    int inicial, fim;
    int segundos, pausa;
    float delay1, delay2;

} TEMPO;

///funções que printam
void apaga_castelo(COORDENADA castelo[]);
void desenha_cenario();
void desenha_cenario2();
void desenha_menu();
void desenha_paredes (COORDENADA paredes[], int num_paredes_total);
void desenha_castelo(OGRO shrek, COORDENADA castelo[]);
void mensagem_derrota();
void mensagem_vitoria();
void desenha_placar(JOGADOR jogador, int mododejogo);
void desenha_tempo(TEMPO tempo);
void desenha_jogador(JOGADOR jogador);
void apaga_elemento(COORDENADA coordenada);
void desenha_tudo(JOGADOR jogador, OGRO shrek, COORDENADA castelo[], COORDENADA paredes[],int num_paredes_total, GUARDA guardas[], int num_guardas, int mododejogo);
void bota_cursor(COORDENADA *curs, int soma_y);
void animacao_jogador(JOGADOR *jogador, int soma_x, int soma_y);
void desenha_pausa();

///funções que lidam com paredes
void gera_paredes(COORDENADA paredes[], int num_paredes_total);
void gera_resto_paredes(COORDENADA paredes[], int cont, int soma_x, int soma_y);
int testa_paredes (int x, int y, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek);
int testa_zonas_guarda(int x, int y, COORDENADA castelo[]);

///funções que lidam com guardas
void gera_guardas(GUARDA guardas[], COORDENADA paredes[], COORDENADA castelo[], int num_guardas, int num_paredes_total);
void desenha_um_guarda (COORDENADA coordenada, COLORS cor, char caractere);
void desenha_guardas(GUARDA *guardas, int num_guardas);
void testa_guardas(GUARDA guardas[], JOGADOR *jogador, int num_guardas, int mododejogo);
void movimenta_um_guarda (GUARDA *guarda, int direcao, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves);
void movimenta_guardas (GUARDA guardas[], int num_guardas, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves);
int testa_chaves_guarda(int x, int y, CHAVE chaves[], int num_chaves);
void aux_movimenta_guarda(GUARDA *guarda, COORDENADA paredes[], int num_paredes_total,COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves,int guarda_x, int guarda_y, int aux_guarda_x, int aux_guarda_y);
int guarda_dentro_coordenada(GUARDA guardas[], COORDENADA coordenadas[], int cont, int parada);
void controla_tempo_guardas(TEMPO *tempo, GUARDA guardas[], int *num_guardas, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[],OGRO *shrek,CHAVE chaves[], int *num_chaves);

///funções que lidam com chaves
void gera_chaves(CHAVE chaves[], COORDENADA paredes[], COORDENADA castelo[], int num_chaves, int num_paredes_total);
void testa_chaves_jogador(CHAVE chaves[], JOGADOR *jogador, int num_chaves);
void pega_chaves(CHAVE chaves[], JOGADOR *jogador, int num_chaves, int mododejogo);
int chave_dentro_coordenada(CHAVE chaves[], COORDENADA coordenadas[], int cont, int parada);

///funções que lidam com ranking
int gera_score(TEMPO tempo, JOGADOR jogador);
void troca (int *a, int *b);
int salva_ranking_a(char nome[], int pontos);
int salva_ranking_arranjo_a(RANKING ranking[], FILE *ranking_arq);
void ordena_ranking_a(RANKING *ranking, int fim);
void exibe_ranking_a();
void desenha_ranking();
void escolhe_opcao_ranking();


///funcões de execução do jogo
void executa_jogo(JOGADOR *jogador, OGRO *shrek, COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], CHAVE chaves[],int *num_chaves, int num_paredes_total, int *num_paredes, int *num_guardas, int *mododejogo, TEMPO *tempo, int *tecla);
void jogo_facil(int *num_paredes, int *num_paredes_total, int *num_guardas, int *num_chaves, COORDENADA paredes[],GUARDA guardas[], COORDENADA castelo[],int *num_partidas, CHAVE chaves[]);
void jogo_dificil(int *num_paredes, int *num_paredes_total, int *num_guardas, int *num_chaves, COORDENADA paredes[],GUARDA guardas[], COORDENADA castelo[],int *num_partidas, CHAVE chaves[]);
void menu(int *num_paredes, int *num_guardas, int *num_chaves, int *num_paredes_total,int *mododejogo, int *num_partidas, JOGADOR *jogador, OGRO *shrek, CHAVE chaves[],COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], TEMPO *tempo);
void escolhe_modo(COORDENADA *curs,int *num_paredes, int *num_guardas, int *num_chaves, int *num_paredes_total,int *mododejogo, int *num_partidas,JOGADOR *jogador, OGRO *shrek, CHAVE chaves[],COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], TEMPO *tempo);
void testa_ogro(JOGADOR *jogador, OGRO *shrek, COORDENADA castelo[]);
void testa_vitoria(JOGADOR *jogador, TEMPO tempo);
void pausar(int *tecla, JOGADOR *jogador, GUARDA guardas[], int *num_guardas, CHAVE chaves[],int *num_chaves, COORDENADA paredes[], int *num_paredes, OGRO shrek, COORDENADA castelo[], TEMPO *tempo, int mododejogo);
void salva_jogo(JOGADOR jogador, GUARDA guardas[], int num_guardas, CHAVE chaves[],int num_chaves, COORDENADA paredes[], int num_paredes, int modo, TEMPO tempo);
int carrega_jogo (JOGADOR *jogador, GUARDA *guardas, int *num_guardas, CHAVE *chaves,int *num_chaves, COORDENADA *paredes, int *num_paredes, int modo, TEMPO *tempo);
int procura_nome_arq(FILE *jogo, JOGO *buffer_jogo, int *posicao_encontrados);
void escolhe_jogo_carregado(FILE *jogo, int encontrados, int posicao_encontrados[]);
void iguala_valores_num(int num_chaves_origem, int *num_chaves_destino, int num_paredes_origem, int *num_paredes_destino,int num_guardas_origem, int *num_guardas_destino, int tempo_origem, int *tempo_destino);
void iguala_chaves(int num_chaves, CHAVE *chaves_origem, CHAVE *chaves_destino);
void iguala_chaves_salva_jogo(int num_chaves, CHAVE *chaves_origem, CHAVE *chaves_destino);
void iguala_guardas(int num_guardas, GUARDA *guardas_origem, GUARDA *guardas_destino);
void iguala_paredes(int num_paredes, COORDENADA *paredes_origem, COORDENADA *paredes_destino);
void iguala_jogadores(JOGADOR jogador_origem, JOGADOR *jogador_destino);

///outras
int retorna_tecla();
void pega_nome(JOGADOR *jogador);
void movimenta_jogador(JOGADOR *jogador, int *tecla, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek);
void inicializa_tudo(int *mododejogo, JOGADOR *jogador, OGRO *shrek, int *num_chaves, CHAVE chaves[]);
void movimenta_tudo(int *tecla, JOGADOR *jogador, COORDENADA paredes[], int num_paredes_total, int *num_paredes, COORDENADA castelo[], OGRO *shrek,int *num_guardas, GUARDA guardas[], int *num_chaves, CHAVE chaves[], TEMPO *tempo, int mododejogo);


int main()
{
    JOGADOR fiona;
    OGRO shrek= {LIMX-1, SAFEY, 0};
    GUARDA guardas[NUM_GUARDAS_MAX];
    CHAVE chaves[NUM_CHAVES_MAX]= {13,13,0,0,26,26,0,0,40,40,0,0};
    COORDENADA castelo[NUM_CASTELO]= {79,10,79,14,78,10,78,14,77,10,77,11,77,12,77,13,77,14};
    COORDENADA paredes[NUM_PAREDES_MAX];
    TEMPO tempo={0};
    int tecla,num_paredes,num_paredes_total,num_guardas,num_chaves=3;
    int num_partidas=0, ranking[NUM_RANK],contador;
    int mododejogo=MENU;

    do
    {

        menu(&num_paredes, &num_guardas, &num_chaves, &num_paredes_total,
             &mododejogo, &num_partidas, &fiona, &shrek, chaves, paredes, castelo, guardas, &tempo); //inicializa variáveis, exibe o menu e seleciona modo de jogo

        executa_jogo(&fiona, &shrek, paredes, castelo, guardas, chaves,
                     &num_chaves, num_paredes_total, &num_paredes, &num_guardas, &mododejogo, &tempo, &tecla);
    }
    while (mododejogo!=SAIR);//se pressionar ESC no menu o programa acaba

}

void apaga_castelo(COORDENADA castelo[]) //apaga o castelo
{
    int j;
    for (j=0; j< NUM_CASTELO; j++)
        apaga_elemento(castelo[j]);
}

void desenha_cenario() //desenha as paredes dos limites do jogo
{
    int i, j;

    textbackground(CYAN);

    for(i = 0; i < LIMX; i++) //laço para desenhar os limites horizontais
    {
        gotoxy(i + 1, 1);
        printf("!");
        gotoxy(i + 1, LIMY);
        printf("!");
    }
    for(j = 0; j < LIMY; j++) //laço para desenhar os limites verticais
    {
        gotoxy(1, j + 1);
        printf("!");
        gotoxy(LIMX, j + 1);
        printf("!");
    }
}

void desenha_cenario2() //desenha as paredes dos limites para o menu/etc
{
    int i, j;

    textbackground(CYAN);

    for(i = 0; i < LIMX; i++) //laço para desenhar os limites horizontais
    {
        gotoxy(i + 1, 1);
        printf("@");
        gotoxy(i + 1, LIMY);
        printf("@");
    }
    for(j = 0; j < LIMY; j++) //laço para desenhar os limites verticais
    {
        gotoxy(1, j + 1);
        printf("@");
        gotoxy(LIMX, j + 1);
        printf("@");
    }
}

void desenha_menu() //desenha o menu inicial do jogo
{
    clrscr();
    desenha_cenario2();

    textbackground(CYAN); //desenha o título do jogo (SAVE SHREK TM)
    gotoxy((LIMX/5)+2, (LIMY/4)-2);
    printf(" ###  ##  #  # ###   ### #  # ###  ### #  # TM");
    gotoxy((LIMX/5)+2, (LIMY/4)-1);
    printf(" #   #  # #  # #     #   #  # #  # #   # #    ");
    gotoxy((LIMX/5)+2, (LIMY/4));
    printf(" ### #### #  # ##    ### #### ###  ##  ##     ");
    gotoxy((LIMX/5)+2, (LIMY/4)+1);
    printf("   # #  # #  # #       # #  # #  # #   # #    ");
    gotoxy((LIMX/5)+2, (LIMY/4)+2);
    printf(" ### #  #  ##  ###   ### #  # #  # ### #  #   ");


    textbackground(BLACK); //desenha as opções de jogo
    gotoxy(CURSOR_X, CURSOR_Y);
    printf("1. Facil ");
    gotoxy(CURSOR_X, CURSOR_Y+2);
    printf("2. Dificil ");
    gotoxy(CURSOR_X, CURSOR_Y+4);
    printf("3. Casual ");
    gotoxy(CURSOR_X, CURSOR_Y+6);
    printf("4. Carregar ");
    gotoxy(CURSOR_X, CURSOR_Y+8);
    printf("5. Ranking ");

    gotoxy(30, (LIMY/2)-1);
    printf("M O D O  D E  J O G O : ");

}

void desenha_paredes (COORDENADA paredes[], int num_paredes_total) //desenha as paredes internas do jogo
{
    int i;
    textbackground(CYAN);
    for(i=0; i < num_paredes_total; i++) //laço que depende do número total de "quadrados" de paredes (num_paredes * NUM_SEGMENTOS)
    {
        putchxy(paredes[i].x, paredes[i].y, '!');
        textbackground(CYAN);
    }
}

void desenha_castelo(OGRO shrek, COORDENADA castelo[]) //desenha o castelo e o ogro, de acordo com as coordenadas recebidas para ambos
{
    int i;
    for(i=0; i < NUM_CASTELO; i++) //laço para desenho do castelo, dependendo de uma constante com o número de elementos do arranjo do castelo
    {
        textbackground(CYAN);
        putchxy(castelo[i].x, castelo[i].y, '!');
    }
    gotoxy(shrek.posicao.x, shrek.posicao.y); //desenho do ogro
    textbackground(GREEN);
    printf(" ");
}

void mensagem_derrota() //desenha a mensagem de derrota
{
    clrscr();
    desenha_cenario2();
    gotoxy((LIMX/3)+9, (LIMY/2)-2);
    textbackground(CYAN);
    printf("M O R R E U :c");
    textbackground(BLACK);
    Sleep(80);
    getchar();
    fflush(stdin);

}

void mensagem_vitoria() //desenha a mensagem de vitória
{
    textbackground(BLACK);
    clrscr();
    gotoxy((LIMX/3)+6, (LIMY/2)-8);
    textbackground(CYAN);
    printf("@@@ P A R A B E N S @@@");
    textbackground(BLACK);

}

void desenha_placar(JOGADOR jogador, int mododejogo) //desenha o placar do jogo
{
    char dificuldade[MAX_NOME]= {"Facil"};

    switch(mododejogo) //dependendo do modo de jogo, muda o valor da string dificuldade
    {
    case FACIL: strcpy(dificuldade,"Facil"); break;
    case DIFICIL: strcpy(dificuldade,"Dificil"); break;
    case CASUAL: strcpy(dificuldade,"Casual"); break;
    }
    gotoxy(1, LIMY + 1);
    textbackground(BLACK);
    //utilizando os valores do jogador e a dificuldade, imprime o placar
    printf(" Princesa %s | Vidas: %i | Chaves: %i | Dificuldade: %s", jogador.nome, jogador.vidas, jogador.chaves, dificuldade);
}

void desenha_tempo(TEMPO tempo) //imprime o tempo recebido (número de segundos passados desde o início do jogo)
{
    gotoxy(LIMX-3, LIMY + 1);
    textbackground(BLACK);
    printf( "%.2d", tempo.segundos);
}

void desenha_jogador(JOGADOR jogador) //desenha o jogador na cor verde, nas coordenadas da estrutura
{
    gotoxy(jogador.posicao.x, jogador.posicao.y);
    textbackground(GREEN);
    printf(" ");
}

void apaga_elemento(COORDENADA coordenada) //"apaga" uma coordenada recebida, imprimindo um espaço com a cor preta
{
    gotoxy(coordenada.x, coordenada.y);
    textbackground(BLACK);
    printf(" ");
}

void desenha_tudo(JOGADOR jogador, OGRO shrek, COORDENADA castelo[], COORDENADA paredes[],int num_paredes_total, GUARDA guardas[], int num_guardas, int mododejogo)
// função auxiliar que desenha todos os elementos necessários para o início do jogo:
// o castelo, o ogro, o cenário, as paredes internas, o jogador, os guardas e o placar
{
    desenha_castelo(shrek, castelo);
    desenha_cenario();
    desenha_paredes(paredes, num_paredes_total);
    desenha_jogador(jogador);
    desenha_guardas(guardas, num_guardas);
    desenha_placar(jogador, mododejogo);
}

void bota_cursor(COORDENADA *curs, int soma_y) //imprime o "cursor" '>' nas escolhas do menu
{
    char cursor = '>';
    apaga_elemento(*curs); //apaga o local anterior do cursor
    curs->y+=soma_y; //indica valor a modificar a ordenada do cursor. Se '>' vai para baixo a funcao é chamada com soma_y positiva, se vai para cima é negativa
    putchxy(curs->x,curs->y,cursor); //imprime o cursor no local novo
}

void animacao_jogador(JOGADOR *jogador, int soma_x, int soma_y) //auxiliar de movimenta_jogador
{
    apaga_elemento (jogador->posicao); //apaga a posição anterior do jogador
    // altera a posicão do jogador de acordo com os valores recebidos para x e y
    jogador->posicao.x+=soma_x;//soma_x indica valor a ser somado a abscissa do jogador. Se ele vai para direita a funcao é chamada com soma_x positiva, se vai para esquerda é negativa
    jogador->posicao.y+=soma_y;//soma_y indica valor a ser somado a do jogador. Se ele vai para baixo a funcao é chamada com soma_y positiva, se vai para cima é negativa
    desenha_jogador(*jogador); //desenha o jogador na nova posição
}

void desenha_pausa() //desenha o menu de pausa
{
    clrscr();
    desenha_cenario2();
    gotoxy(34, 7);
    textbackground(CYAN);
    printf("@@@ P A U S A @@@");
    textbackground(BLACK);
    gotoxy(31, 13);
    printf("  Sim, continuar o jogo");
    gotoxy(31, 15);
    printf("  Nao, sair e salvar");
    gotoxy(31, 17);
    printf("  Nao, sair e nao salvar");
    gotoxy(34, 10);
    printf("Deseja continuar? ");//aviso
}



void gera_paredes(COORDENADA paredes[], int num_paredes_total) //gera coordenadas aleatórias para as paredes do jogo
{
    int i,j;
    int direcao;
    srand((unsigned)time(NULL)); //usa comando de tempo da biblioteca time para gerar valores aleatórios

    for(i=0; i < num_paredes_total; i += NUM_SEGMENTOS)
    {
        paredes[i].x = NUM_SEGMENTOS + (rand() % (LIMX - NUM_SEGMENTOS*3)); //gera os valores de x
        paredes[i].y = NUM_SEGMENTOS + (rand() % (LIMY - NUM_SEGMENTOS*2)); //gera os valores de y

        direcao = 1 + (rand() % 4); //gera a direção do segmento de paredes

        switch(direcao) //de acordo com a direção, gera o resto das paredes para cima, baixo, esquerda ou direita.
        {
        case CIMA: gera_resto_paredes(paredes,i,0,-1); break;

        case BAIXO: gera_resto_paredes(paredes,i,0,+1); break;

        case ESQUERDA: gera_resto_paredes(paredes,i,-1,0); break;

        case DIREITA: gera_resto_paredes(paredes,i,1,0); break;
        }
    }
}

void gera_resto_paredes(COORDENADA paredes[], int cont, int soma_x, int soma_y)
//a partir de uma "semente" da parede e 2 valores a serem somados a x e y de acordo com a direção que ela deve crescer,
//gera o resto das paredes
{
    int j;

    for(j=1; j<NUM_SEGMENTOS; j++)
    {
        paredes[cont + j].x = paredes[cont + j -1].x + soma_x;
        paredes[cont + j].y = paredes[cont + j -1].y + soma_y;
    }
}

int testa_paredes (int x, int y, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek)
//recebe a posição futura do jogador após o movimento e retorna um booleano que "permite" ou não que o movimento seja feito
//testa condição do jogador em relação às coordenadas das paredes e castelo a fim de não transpô-las
{
    int i, j;
    float distanciax, distanciay;
    for (i=0; i<num_paredes_total; i+=NUM_SEGMENTOS) //testa as "sementes" das paredes
    {
        distanciax = x-paredes[i].x;
        distanciay = y-paredes[i].y;
        if (abs(distanciax)<(NUM_SEGMENTOS+1)&&abs(distanciay)<(NUM_SEGMENTOS+1)) //se a "semente" da parede estiver a até 6 quadradinhos de distância, testa o resto
            for (j=i; j<i+NUM_SEGMENTOS; j++)
                if (x==paredes[j].x && y==paredes[j].y)
                    return FALSE; //se a coordenada futura do jogador for igual a de uma das paredes internas, retorna falso
    }

    if (x<2 || x>LIMX-1 || y<2 || y>LIMY-1) //se a coordenada futura do jogador for igual a de uma das paredes dos limites, retorna falso
        return FALSE;

    for (i=0; i<NUM_CASTELO; i++) //se a coordenada futura do jogador for igual a de uma das paredes do castelo, retorna falso
        if (x==castelo[i].x && y==castelo[i].y && shrek->castelo == FALSE) //enquanto o jogador não possuir as 3 chaves, o castelo permanece inacessível
            return FALSE;

    return TRUE; //caso não ocorra nenhum dos casos acima, retorna true e permite que o movimento seja feito
}

int testa_zonas_guarda(int x, int y, COORDENADA castelo[])
//testa a posição futura dos guardas após um movimento, e "permite" ou não que ele seja realizado.
//desse modo, impede que os guardas se movam no castelo e em uma "zona segura" onde o jogador aparece quando inicia o jogo ou morre.
{
    int i;
    for (i=0; i<NUM_CASTELO; i++) //testa as coordenadas das paredes do castelo
    {
        if(abs(x-castelo[i].x)<=DISTANCIA_GUARDAS && abs(y-castelo[i].y)<=DISTANCIA_GUARDAS)
            return FALSE; //retorna falso se alguma coordenada estiver a menos de 2 quadradinhos de distância do guarda
    }
    if(abs(y-SAFEY)<=DISTANCIA_GUARDAS && abs(x-SAFEX)<=DISTANCIA_GUARDAS+1)
        return FALSE; //retorna falso se o guarda estiver a até 3 quadrados da coordenada central da safe zone
    else
        return TRUE; //caso não ocorra nenhum dos casos acima, retorna true
}



void gera_guardas(GUARDA guardas[], COORDENADA paredes[], COORDENADA castelo[], int num_guardas, int num_paredes_total) //gera coordenadas aleatórias para os guardas do jogo
{
    int i=0, j;
    srand((unsigned)time(NULL));

    do
    {
        guardas[i].posicao.x  = X_MIN_GUARDAS + (rand() % X_MAX_GUARDAS);
        guardas[i].posicao.y = Y_MIN_GUARDAS + (rand() % Y_MAX_GUARDAS);

        if(guarda_dentro_coordenada(guardas,castelo,i,NUM_CASTELO) || guarda_dentro_coordenada(guardas,paredes,i,num_paredes_total))
            i--; //se o guarda tiver sido gerado dentro do castelo ou das paredes internas, executa o laço de novo para obter valores diferentes
        i++;
    }
    while(i < num_guardas);
}

void desenha_um_guarda (COORDENADA coordenada, COLORS cor, char caractere) //desenha um guarda em uma dada coordenada
{
    gotoxy(coordenada.x, coordenada.y);
    textbackground(cor);
    printf("%c", caractere);
}

void desenha_guardas(GUARDA *guardas, int num_guardas) //desenha todos os guardas do jogo
{
    int i;
    for (i=0; i<num_guardas; i++)
        desenha_um_guarda(guardas[i].posicao, MAGENTA, 'G');
}

void testa_guardas(GUARDA guardas[], JOGADOR *jogador, int num_guardas, int mododejogo)
//testa condição do jogador em relação às coordenadas dos guardas.
{
    int i;
    for (i=0; i<num_guardas; i++)
    {
        if(abs(jogador->posicao.x - guardas[i].posicao.x) <= DISTANCIA_GUARDAS &&
           abs(jogador->posicao.y - guardas[i].posicao.y) <= DISTANCIA_GUARDAS)
        {
                //se o jogador se encontra ao alcance dos guardas, "mata" o jogador, subtraindo uma vida e desenhando-o novamente na coordenada de início
                apaga_elemento(jogador->posicao);
                jogador->posicao.x= SAFEX;
                jogador->posicao.y= SAFEY;
                desenha_jogador(*jogador);
                jogador->vidas=jogador->vidas-1;
                desenha_placar(*jogador, mododejogo);
        }
    }
}

void movimenta_um_guarda (GUARDA *guarda, int direcao, COORDENADA paredes[], int num_paredes_total,COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves)
//movimenta um dado guarda em uma direção gerada anteriormente
{
    switch(direcao) //de acordo com a direção, altera os valores a serem somados a x/y
    {
    case CIMA: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves, 0,-1, 0,-1); break;

    case BAIXO: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves, 0, 1, 0, 1); break;

    case DIREITA: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves, 1, 0, 1, 0); break;

    case ESQUERDA: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves,-1, 0,-1, 0); break;

    case ESQUERDA_CIMA: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves,-1,-1,-1,-1); break;

    case DIREITA_CIMA: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves, 1,-1, 1,-1); break;

    case ESQUERDA_BAIXO: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves,-1, 1,-1, 1); break;

    case DIREITA_BAIXO: aux_movimenta_guarda(guarda, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves, 1, 1, 1, 1); break;
    }
}

void movimenta_guardas (GUARDA guardas[], int num_guardas, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves)
//função com o laço que movimenta todos os guardas do jogo.
{
    int i, direcao;
    srand((unsigned)time(NULL));

    for (i=0; i<num_guardas; i++)
    {
        direcao = (rand() % 8); //gera uma direção aleatória para o guarda se mover.
        movimenta_um_guarda (&guardas[i], direcao, paredes, num_paredes_total, castelo, shrek, chaves, num_chaves);
    }
}

int testa_chaves_guarda(int x, int y, CHAVE chaves[], int num_chaves)
//testa condição dos guardas em relação às coordenadas das chaves a fim de não se sobreporem
{
    int i;
    for (i=0; i<num_chaves; i++)
        if (x==chaves[i].posicao.x && y==chaves[i].posicao.y && chaves[i].descoberta==TRUE)
            return FALSE; //se o guarda estiver se sobrepondo a uma das chaves, retorna false

    return TRUE; //caso contrário retorna true

}

//função auxiliar de movimenta_um_guarda. aux_guarda_x e aux_guarda_y sao somadas as posicoes dos guardas para testar se suas proximas posicoes sao validas
void aux_movimenta_guarda(GUARDA *guarda, COORDENADA paredes[], int num_paredes_total,COORDENADA castelo[], OGRO *shrek, CHAVE chaves[], int num_chaves,int soma_x, int soma_y, int aux_guarda_x, int aux_guarda_y)
{
    if(testa_paredes(guarda->posicao.x+aux_guarda_x, guarda->posicao.y+aux_guarda_y, paredes, num_paredes_total, castelo, shrek) &&
            testa_chaves_guarda(guarda->posicao.x+aux_guarda_x, guarda->posicao.y+aux_guarda_y, chaves, num_chaves) &&
            testa_zonas_guarda(guarda->posicao.x+aux_guarda_x, guarda->posicao.y+aux_guarda_y, castelo))
    {
        //se o movimento for possível (não ultrapassa nenhuma parede, chave ou a safe zone) movimenta o guarda
        apaga_elemento(guarda->posicao);
        guarda->posicao.x+=soma_x;//soma_x indica valor a modificar abscissa do guarda. Se ele vai para direita a funcao é chamada com soma_x positiva, se vai para esquerda é negativa
        guarda->posicao.y+=soma_y;//soma_y indica valor a modificar ordenada do guarda. Se ele vai para baixo a funcao é chamada com soma_y positiva, se vai para cima é negativa
        desenha_um_guarda(guarda->posicao, MAGENTA, 'G');
    }

}

int guarda_dentro_coordenada(GUARDA guardas[], COORDENADA coordenadas[], int cont, int parada)
//testa se os guardas se encontram dentro de um dado arranjo de coordenadas.
{
    int j;
    for(j=0; j<parada; j++)
        if(guardas[cont].posicao.x==coordenadas[j].x  && guardas[cont].posicao.y==coordenadas[j].y)
            return TRUE;

    return FALSE;
}

void controla_tempo_guardas(TEMPO *tempo, GUARDA guardas[], int *num_guardas, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[],OGRO *shrek,CHAVE chaves[], int *num_chaves)
//controla o movimento dos guardas de acordo com o tempo, para garantir que exista um "delay" entre um movimento e outro
{
    tempo->fim = clock();
    tempo->segundos = ((int)(tempo->fim - tempo->inicial)/CLOCKS_PER_SEC)+ tempo->pausa;
    tempo->delay2=clock();

    if ((tempo->delay2-tempo->delay1)>DELAY_GUARDAS) //controle da movimentação dos guardas
    {
        tempo->delay1=clock();
        movimenta_guardas (guardas, *num_guardas, paredes,num_paredes_total, castelo, shrek, chaves, *num_chaves);
        desenha_tempo(*tempo);
    }
}



void gera_chaves(CHAVE chaves[], COORDENADA paredes[], COORDENADA castelo[], int num_chaves, int num_paredes_total)//gera coordenadas aleatórias para as chaves do jogo
//gera coordenadas aleatórias para as chaves do jogo
{
    int i=0;
    srand((unsigned)time(NULL));

    do
    {
        chaves[i].posicao.x = X_MIN_CHAVES + (rand() % X_MAX_CHAVES);
        chaves[i].posicao.y = Y_MIN_CHAVES + (rand() % Y_MAX_CHAVES);

        if(chave_dentro_coordenada(chaves,castelo,i,NUM_CASTELO) || chave_dentro_coordenada(chaves,paredes,i,num_paredes_total))
            i--; //se a chave se encontra dentro do castelo ou de uma parede interna, diminui o contador para gerá-la novamente em uma coordenada diferente

        i++;
    }
    while(i<num_chaves);
}

void testa_chaves_jogador(CHAVE chaves[], JOGADOR *jogador, int num_chaves)
//testa condição do jogador em relação às coordenadas das chaves a fim de printá-las na tela
{
    int i;
    for (i=0; i<num_chaves; i++)
        if(abs(jogador->posicao.x - chaves[i].posicao.x) <= DISTANCIA_CHAVES)
            if(abs(jogador->posicao.y - chaves[i].posicao.y) <= DISTANCIA_CHAVES)
                if(chaves[i].status == FALSE && chaves[i].descoberta == FALSE)
                {
                    //se o jogador se encontra em a uma distância mínima da chave e ela não foi pega/descoberta anteriormente, printa ela
                    gotoxy(chaves[i].posicao.x, chaves[i].posicao.y);
                    textbackground(YELLOW);
                    printf(" ");
                    chaves[i].descoberta = TRUE; //controle para printar apenas uma vez
                }
}

void pega_chaves(CHAVE chaves[], JOGADOR *jogador, int num_chaves, int mododejogo)//testa condição do jogador em relação às coordenadas das chaves
{
    int i;
    for (i=0; i<num_chaves; i++)
        if(jogador->posicao.x == chaves[i].posicao.x && jogador->posicao.y == chaves[i].posicao.y)
            if(chaves[i].status == FALSE) //controle para pegar a chave apenas uma vez
            {
                chaves[i].status = TRUE; //determina que a chave foi pega
                chaves[i].descoberta = FALSE; //determina que a chave não está mais printada
                jogador->chaves++;
                desenha_placar(*jogador, mododejogo);
            }
}

int chave_dentro_coordenada(CHAVE chaves[], COORDENADA coordenadas[], int cont, int parada)
//verifica se um arranjo de chaves se encontra dentro de um dado arranjo de coordenadas
{
    int j;
    for(j=0; j<parada; j++)
        if(chaves[cont].posicao.x==coordenadas[j].x  && chaves[cont].posicao.y==coordenadas[j].y)
            return TRUE;

    return FALSE;
}




int gera_score (TEMPO tempo, JOGADOR jogador)
//calcula o score do jogador a partir do tempo que levou para terminar a partida, levando também em conta o número de chaves pegas e
//o número de vidas restantes
{
    int score;

    score= (BASE_PONTOS-(VALOR_SEG*tempo.segundos)+(VALOR_CHAVE*(jogador.chaves)-(VALOR_VIDA*(QUANT_VIDAS-jogador.vidas))));

    return score;
}

void troca (int *a, int *b) //troca dois valores
{
    int aux;
    aux=*a;
    *a=*b;
    *b=aux;
}

int salva_ranking_a(char nome[], int pontos) //salva o ranking "atualizado" (com um novo nome e um novo número de pontos) em um arq. de texto
{
    RANKING ranking[NUM_RANK];
    char aux[NUM_RANK];
    FILE *ranking_arq;
    int i=0, j=0, fim;
    int auxInt;
    char auxString[MAX_AUX];

    if ((ranking_arq = fopen("ranking.csv", "a+")) == NULL) //abre o arquivo
        return 0;

    fim = salva_ranking_arranjo_a(ranking, ranking_arq); //passa todos os valores do arquivo para um arranjo, e coloca em "fim" o número de elementos desse arranjo + 1.
    strcpy(ranking[fim].nome, nome);
    ranking[fim].pontos=pontos;
    ordena_ranking_a(ranking, fim); //ordena o arranjo, agora incluindo o novo jogador
    fclose(ranking_arq); //fecha o arquivo
    ranking_arq = fopen("ranking.csv", "w+"); //abre o arquivo para escrita

    for(j=0; j<=fim && j<NUM_RANK-1; j++) //enquanto não chegar ao fim do arranjo (e não atingir 10 valores), escreve o valor do ranking no arquivo
    {
        fprintf(ranking_arq, "%s;%i;\n", ranking[j].nome, ranking[j].pontos);
        if(ferror(ranking_arq))
        {
            printf("Erro na escrita");
            return 0;
        }
    }

    fclose(ranking_arq); //fecha o arquivo
    return 1;
}

int salva_ranking_arranjo_a(RANKING ranking[], FILE *ranking_arq) //salva os valores de ranking de um arquivo texto em um arranjo
{
    int i=0;
    char buffer_ranking[MAX_NOME];

    while (!feof(ranking_arq)) //enquanto não chegar ao fim do arquivo
    {
        if ((fgets(buffer_ranking, sizeof(buffer_ranking), ranking_arq)) != NULL)
        {
            //copia os valores para o arranjo
            strcpy(ranking[i].nome, strtok(buffer_ranking, ";"));
            ranking[i].pontos=atoi(strtok(NULL, ";"));
            i++;
        }
    }
    return i; //retorna o número de linhas desse arranjo + 1
}

void ordena_ranking_a(RANKING *ranking, int fim) //ordena o arranjo de rankings pelo método bubblesort
{
    int i, j;
    int auxiliar;
    char auxiliar_nome[MAX_NOME];

    for (i=0; i<fim; i++)
    {
        for (j=0; j<fim-i; j++)
        {
            if (ranking[j].pontos<ranking[j+1].pontos)
            {
                auxiliar=ranking[j].pontos;
                ranking[j].pontos=ranking[j+1].pontos;
                ranking[j+1].pontos=auxiliar;
                strcpy(auxiliar_nome, ranking[j].nome);
                strcpy(ranking[j].nome, ranking[j+1].nome);
                strcpy(ranking[j+1].nome, auxiliar_nome);
            }
        }
    }
}

void exibe_ranking_a() //exibe o ranking guardado em arquivo texto
{
    int i=0, pontos;
    char buffer_ranking[MAX_NOME], nome[MAX_NOME];
    FILE *ranking_arq;
    desenha_ranking();

    if( !(ranking_arq = fopen("ranking.csv", "r+"))) //abre o arquivo para leitura e testa se está vazio
    {
        gotoxy((LIMX/3)+10, (LIMY/2)-2+i);
        printf("Arquivo Vazio!");
    }

    else
    {
        while(!feof(ranking_arq)) //enquanto não chegar ao fim
        {
            if((fgets(buffer_ranking, sizeof(buffer_ranking), ranking_arq)) != NULL)
            {
                //imprime os valores na tela
                strcpy(nome, strtok(buffer_ranking, ";"));
                pontos=atoi(strtok(NULL, ";"));
                gotoxy(35, 9+i);
                printf("%.2i pontos - %s\n", pontos, nome);
            }
            i++;
        }
    }
    fclose(ranking_arq); //fecha o arquivo
    escolhe_opcao_ranking();
}

void desenha_ranking()
{
    //desenha o menu principal do ranking
    desenha_cenario2();
    gotoxy(CURSOR_X-1, CURSOR_Y-6);
    textbackground(CYAN);
    printf(" R A N K I N G :\n");
    textbackground(BLACK);
    gotoxy(CURSOR_X, CURSOR_Y+7);
    printf("Voltar ao menu");
    gotoxy(CURSOR_X, CURSOR_Y+9);
    printf("Apagar ranking");
}

void escolhe_opcao_ranking() //escolhe uma opção no menu do ranking
{
    char setinha;
    COORDENADA curs={34,20};

    do
    {
        bota_cursor(&curs, 0);
        if(kbhit())
        {
            setinha = retorna_tecla();
            if (setinha==CIMA && curs.y-2 >=20)    // move a seta do menu para cima
                bota_cursor(&curs, -2);

            if(setinha==BAIXO && curs.y+2 <=22) // move a seta do menu para baixo
                bota_cursor(&curs, +2);

            if(setinha == ENTER && curs.y == 22)
                remove("ranking.csv");
        }
        Sleep(40);
    }
    while(setinha != ENTER); //enquanto o usuário não der enter
}



void executa_jogo(JOGADOR *jogador, OGRO *shrek, COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], CHAVE chaves[],int *num_chaves, int num_paredes_total, int *num_paredes, int *num_guardas, int *mododejogo, TEMPO *tempo, int *tecla)
//função principal de execução do jogo
{
    if(*mododejogo>=FACIL && *mododejogo<=CARREGAR) // não inicializa jogo se a opção for ranking
    {
        desenha_tudo(*jogador, *shrek, castelo,paredes,num_paredes_total, guardas, *num_guardas, *mododejogo);

        tempo->inicial = clock();
        tempo->delay1 = tempo->inicial;
        *tecla = TRUE; // controle de entradas do teclado, se o jogador sair pela pausa vira MENU

        do //laço de movimento
        {
            movimenta_tudo(tecla, jogador, paredes, num_paredes_total, num_paredes,
            castelo,shrek,num_guardas, guardas, num_chaves, chaves, tempo, *mododejogo);
        }
        while(jogador->vidas!=FALSE  && jogador->vitoria!=TRUE && *tecla != MENU); // jogo termina quando o jogador vence, morre ou sai

        fflush(stdin);
        tempo->pausa=FALSE;
        testa_vitoria(jogador, *tempo); //testa se o jogador venceu ou não o jogo
        clrscr();
    }
}

void jogo_facil(int *num_paredes, int *num_paredes_total, int *num_guardas, int *num_chaves, COORDENADA paredes[],GUARDA guardas[], COORDENADA castelo[],int *num_partidas, CHAVE chaves[])
//inicializa os valores de paredes, guardas e chaves para um jogo no modo fácil
{
    *num_paredes=NUM_PAREDES_FACIL;
    *num_paredes_total= (*num_paredes)*NUM_SEGMENTOS;
    *num_guardas=NUM_GUARDAS_MIN;
    *num_chaves=NUM_CHAVES_MAX;
    gera_paredes(paredes, *num_paredes_total);
    gera_guardas(guardas, paredes, castelo, *num_guardas, *num_paredes_total);
    gera_chaves(chaves, paredes, castelo, *num_chaves, *num_paredes_total);
    (*num_partidas)++;
}


void jogo_dificil(int *num_paredes, int *num_paredes_total, int *num_guardas, int *num_chaves, COORDENADA paredes[], GUARDA guardas[], COORDENADA castelo[],int *num_partidas, CHAVE chaves[])
//inicializa os valores de paredes, guardas e chaves para um jogo no modo difícil
{
    *num_paredes=NUM_PAREDES_DIFICIL;
    *num_paredes_total= (*num_paredes)*NUM_SEGMENTOS;
    *num_guardas=NUM_GUARDAS_MAX;
    *num_chaves=NUM_CHAVES_MIN;
    gera_paredes(paredes, *num_paredes_total);
    gera_guardas(guardas, paredes, castelo, *num_guardas, *num_paredes_total);
    gera_chaves(chaves, paredes, castelo, *num_chaves, *num_paredes_total);
    (*num_partidas)++;
}

void menu(int *num_paredes, int *num_guardas, int *num_chaves, int *num_paredes_total,int *mododejogo, int *num_partidas, JOGADOR *jogador, OGRO *shrek, CHAVE chaves[],COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], TEMPO *tempo)
//função do menu principal do jogo, desenha o menu e permite que o usuário escolha um modo de jogo
{
    int tecla=TRUE;
    COORDENADA curs= {CURSOR_X-2,CURSOR_Y};

    desenha_menu();
    inicializa_tudo(mododejogo,jogador,shrek, num_chaves, chaves);

    do
    {
        bota_cursor(&curs, 0);

        if(kbhit())
        {
            tecla = retorna_tecla();  // armazena a tecla pressionada

            if (tecla==CIMA && curs.y-2 >=CURSOR_Y)    // move a seta do menu para cima
                bota_cursor(&curs, -2);

            if(tecla==BAIXO && curs.y+2 <=CURSOR_Y+8) // move a seta do menu para baixo
                bota_cursor(&curs, +2);

            if(tecla==ESC)
                *mododejogo=SAIR;

            if(tecla==ENTER)
                escolhe_modo(&curs,num_paredes,num_guardas,num_chaves,num_paredes_total,mododejogo,num_partidas,jogador,shrek,chaves,paredes,castelo,guardas,tempo);
        }
        Sleep(40);
    }
    while(*mododejogo==RECORDES);
    fflush(stdin);
    clrscr();
}

//escolhe o modo de jogo de acordo com o valor da ordenada do cursor depois da tecla ENTER ter sido pressionada
void escolhe_modo(COORDENADA *curs,int *num_paredes, int *num_guardas, int *num_chaves, int *num_paredes_total,int *mododejogo, int *num_partidas,
                  JOGADOR *jogador, OGRO *shrek, CHAVE chaves[],COORDENADA paredes[], COORDENADA castelo[], GUARDA guardas[], TEMPO *tempo)
{
    switch(curs->y)
    {
    case CURSOR_Y: *mododejogo=FACIL;

        pega_nome(jogador);
        jogo_facil(num_paredes, num_paredes_total, num_guardas, num_chaves, paredes, guardas, castelo,num_partidas,chaves);
        break;

    case CURSOR_Y+2: *mododejogo=DIFICIL;

        pega_nome(jogador);
        jogo_dificil(num_paredes, num_paredes_total, num_guardas, num_chaves, paredes, guardas, castelo,num_partidas,chaves);
        break;

    case CURSOR_Y+4: *mododejogo=CASUAL;

        jogo_facil(num_paredes, num_paredes_total, num_guardas, num_chaves, paredes, guardas, castelo,num_partidas,chaves);
        break;

    case CURSOR_Y+6: *mododejogo=CARREGAR;

        if(carrega_jogo(jogador, guardas, num_guardas, chaves, num_chaves, paredes, num_paredes, PERMANENTE, tempo)) //se carregamento nao funciona volta ao menu
            *num_paredes_total= (*num_paredes)*NUM_SEGMENTOS;

        else
            *mododejogo = MENU;
        break;

    case CURSOR_Y+8: *mododejogo=RECORDES;

        clrscr();
        exibe_ranking_a();
        *mododejogo=MENU; //depois de qualquer tecla pressionada no ranking, retorna ao menu
        clrscr();
        break;
    }
}


void testa_ogro(JOGADOR *jogador, OGRO *shrek, COORDENADA castelo[])
//testa condição do jogador em relação às coordenadas do ogro a fim de terminar o jogo caso o jogador possua as 3 chaves

{
    int i,j;
    for (i=0; i< NUM_CASTELO; i++)
    {
        if(abs(jogador->posicao.x - castelo[i].x) <= 1 && abs(jogador->posicao.y - castelo[i].y) <= 1)
        {
            if(jogador->chaves >= NUM_CHAVES_MIN && shrek->castelo == FALSE)
            {
                apaga_castelo(castelo);
                shrek->castelo = TRUE;//apaga castelo quando jogador está perto e com as 3 chaves, deixa de ser uma parede válida
            }
        }
    }
    if(abs(jogador->posicao.x - shrek->posicao.x) <= 1 && abs(jogador->posicao.y - shrek->posicao.y) <= 1)
        jogador->vitoria = TRUE;
}


void testa_vitoria(JOGADOR *jogador, TEMPO tempo) //executa outras funções dependendo da vitória ou derrota do jogador na partida
{
    if(jogador->vitoria) //se o jogador venceu, gera o seu score, atualiza o ranking e exibe-o
    {
        jogador->pontos=gera_score(tempo, *jogador);
        salva_ranking_a(jogador->nome, jogador->pontos);
        mensagem_vitoria();
        exibe_ranking_a();
    }
    if(!jogador->vidas) //se o jogador perdeu, exibe uma mensagem de derrota na tela
        mensagem_derrota();
}

void pausar(int *tecla, JOGADOR *jogador, GUARDA guardas[], int *num_guardas, CHAVE chaves[],int *num_chaves, COORDENADA paredes[], int *num_paredes, OGRO shrek, COORDENADA castelo[], TEMPO *tempo, int mododejogo)
{
    char resposta;
    int i,comando;
    COORDENADA curs= {CURSOR_X-5,CURSOR_Y};
    desenha_pausa();

    do
    {
        bota_cursor(&curs, 0);
        if(kbhit())
        {
            comando = retorna_tecla();  // armazena a tecla pressionada

            if (comando==CIMA && curs.y-2 >=CURSOR_Y)    // move a seta do menu para cima
                bota_cursor(&curs, -2);

            if(comando==BAIXO && curs.y+2 <=CURSOR_Y+4) // move a seta do menu para baixo
                bota_cursor(&curs, +2);

            if(comando==ENTER)   // se pressionou ENTER
            {
                switch(curs.y)
                {
                case CURSOR_Y:///CONTINUAR
                // se o jogador deseja continuar, o aviso é sobreposto ao carregar do jogo

                    salva_jogo(*jogador, guardas, *num_guardas, chaves, *num_chaves, paredes, *num_paredes, TEMPORARIO, *tempo);
                    *tecla = TRUE;
                    clrscr();
                    tempo->inicial=clock(); //zera os segundos, os segundos totais do jogador sao corrigidos somando o valor de "tempo->pausa" na executa_jogo
                    carrega_jogo(jogador, guardas, num_guardas, chaves, num_chaves, paredes, num_paredes, TEMPORARIO, tempo);
                    desenha_tudo(*jogador, shrek, castelo, paredes,(*num_paredes)*NUM_SEGMENTOS,guardas,*num_guardas, mododejogo);
                    break;

                case CURSOR_Y+2:///SAIR E SALVAR

                    salva_jogo (*jogador, guardas, *num_guardas, chaves, *num_chaves, paredes, *num_paredes, PERMANENTE, *tempo);
                    *tecla = MENU;
                    break;

                case CURSOR_Y+4:///SAIR E NAO SALVAR
                    *tecla = MENU;
                    break;
                }
            }
        }
        Sleep(40);
    }
    while(comando!=ENTER);
}


void salva_jogo(JOGADOR jogador, GUARDA guardas[], int num_guardas, CHAVE chaves[],int num_chaves, COORDENADA paredes[], int num_paredes, int modo, TEMPO tempo) //salva o jogo em arquivo binário
{
    FILE *jogo;
    JOGO buffer_jogo;
    int i;

    if(modo==TEMPORARIO) //se o modo for temporário (salva somente para a pausa), abre com o nome temporario.bin
        jogo = fopen("temporario.bin","w+b");
    else //caso contrário, o save é "permanente" e é adicionado no arquivo jogos.bin
        jogo = fopen("jogos.bin","a+b");

    if(ferror(jogo))
        printf("Erro na criacao!");

    else
    {
        //se não ocorreu nenhum erro na abertura, passa todos os valores necessários para a execução do jogo para um buffer
        iguala_valores_num(num_chaves, &(buffer_jogo.num_chaves), num_paredes, &(buffer_jogo.num_paredes),
        num_guardas, &(buffer_jogo.num_guardas), tempo.segundos, &(buffer_jogo.segundos));
        iguala_chaves_salva_jogo(num_chaves, chaves, buffer_jogo.chaves);
        iguala_guardas(num_guardas, guardas, buffer_jogo.guardas);
        iguala_paredes(num_paredes, paredes, buffer_jogo.paredes);
        iguala_jogadores(jogador, &(buffer_jogo.jogador));
    }

    if(!(fwrite(&buffer_jogo,sizeof(JOGO),1,jogo))) //escreve o buffer no arquivo aberto
        printf("erro na escrita");

    fclose(jogo); //fecha o arquivo
}


int carrega_jogo (JOGADOR *jogador, GUARDA *guardas, int *num_guardas, CHAVE *chaves,int *num_chaves, COORDENADA *paredes, int *num_paredes, int modo, TEMPO *tempo)
//carrega o jogo previamente salvo em arquivo binário
{
    FILE *jogo;
    JOGO buffer_jogo;
    char nome[MAX_NOME];
    int encontrados=0, contador=0, posicao_encontrados[MAX_NOME], escolhido;
    int i;

    if(modo==TEMPORARIO) //se é jogo pausado, abre o arquivo temporário e lê o jogo (que será o único no arquivo)
    {
        if(!(jogo = fopen("temporario.bin","rb")))
            printf("Erro na abertura!");
        if (!(fread(&buffer_jogo, sizeof(JOGO), 1, jogo)))
            printf("Erro na leitura!");
    }
    else //se o jogo tiver sido salvo permanentemente, abre o arquivo jogos.bin
    {
        if(!(jogo = fopen("jogos.bin","rb")))
        {
            printf(" 4. Sem jogos!");
            getchar();
            return 0;
        }

        encontrados = procura_nome_arq(jogo, &buffer_jogo, posicao_encontrados);
        //procura um dado nome nos jogos do arquivo e coloca em "encontrados" o número de jogos encontrados com esse nome

        if (!encontrados)
        {
            gotoxy(LIMX/4,LIMY/2);
            printf("Nao foi encontrado nenhum jogo com esse nome");
            getchar();
            return 0;
        }

        if (encontrados>1) //se houver mais de um jogo com o mesmo nome, dá a opção de escolher o jogo ao usuário
            escolhe_jogo_carregado(jogo, encontrados, posicao_encontrados);

        else
            fseek(jogo, (posicao_encontrados[0]-1)*sizeof(JOGO), SEEK_SET); //se houver somente um encontrado, usa fseek para colocar a posição corrente desejada

        if(!(fread(&buffer_jogo,sizeof(JOGO),1,jogo)))
            printf("Erro na leitura");
    }

    if (!ferror(jogo)) //se não ocorreu nenhum erro, copia todos os valores do buffer para as variáveis do jogo
    {
        iguala_valores_num(buffer_jogo.num_chaves, num_chaves, buffer_jogo.num_paredes,
        num_paredes,buffer_jogo.num_guardas, num_guardas, buffer_jogo.segundos, &(tempo->pausa));
        iguala_chaves(*num_chaves, buffer_jogo.chaves, chaves);
        iguala_guardas(*num_guardas, buffer_jogo.guardas, guardas);
        iguala_paredes(*num_paredes, buffer_jogo.paredes, paredes);
        iguala_jogadores(buffer_jogo.jogador, jogador);
    }
    else

        printf ("\nErro!");

    fclose(jogo);
    return 1; //se tudo ocorreu corretamente retorna 1
}

int procura_nome_arq(FILE *jogo, JOGO *buffer_jogo, int *posicao_encontrados) //procura um dado nome no arquivo de jogos
{

    char nome[MAX_NOME];
    int contador=0, encontrados=0;

    clrscr();
    desenha_cenario2();
    gotoxy(LIMX/3, (LIMY/2)-2);
    textbackground(BLACK);
    printf("N O M E  D O  J O G A D O R : ");
    gets(nome); //recebe o nome a ser procurado

    while (!feof(jogo))
    {
        if(fread(buffer_jogo,sizeof(JOGO),1,jogo) == 1) //lê o jogo
            if(!(strcmp(nome, buffer_jogo->jogador.nome)))
            {
                //se o jogo possuir o nome procurado, adiciona-o ao arranjo de posições de encontrados, e adiciona um ao número de encontrados
                posicao_encontrados[encontrados]=contador+1;
                encontrados++;
            }
        contador++;
    }
    return encontrados; //retorna o número de jogos encontrados com o nome procurado
}

void escolhe_jogo_carregado(FILE *jogo, int encontrados, int posicao_encontrados[]) //permite que o jogador escolha entre dois ou mais jogos com o mesmo nome
{
    int contador=0, escolhido=0;

    gotoxy((LIMX/3)-3,(LIMY/2)+1);
    printf("Foram encontrados %i jogos com esse nome", encontrados);

    for (contador=0; contador<encontrados; contador++)
    {
        //imprime as posições (mais um) dos encontrados
        gotoxy(39,15+contador);
        printf("Jogo %i", posicao_encontrados[contador]);
    }

    gotoxy(29,16+contador);
    printf("Qual jogo deseja carregar? ");
    scanf("%i", &escolhido); //determina o escolhido pelo jogador
    escolhido--; //diminui um para ter a posição correta
    rewind(jogo); //reinicia o arquivo
    fseek(jogo, escolhido*sizeof(JOGO), SEEK_SET); //e coloca a posição corrente no lugar desejado
}

void iguala_valores_num(int num_chaves_origem, int *num_chaves_destino, int num_paredes_origem, int *num_paredes_destino,
                        int num_guardas_origem, int *num_guardas_destino, int tempo_origem, int *tempo_destino)
{
    //iguala valores de número de chaves, guardas, paredes e tempo.
    *num_chaves_destino=num_chaves_origem;
    *num_paredes_destino=num_paredes_origem;
    *num_guardas_destino=num_guardas_origem;
    *tempo_destino=tempo_origem;
}

void iguala_chaves(int num_chaves, CHAVE *chaves_origem, CHAVE *chaves_destino)
{
    //iguala dois arranjos de chaves dados
    int i;

    for (i=0; i<num_chaves; i++)
    {
        chaves_destino[i].posicao.x = chaves_origem[i].posicao.x;
        chaves_destino[i].posicao.y = chaves_origem[i].posicao.y;
        chaves_destino[i].status = chaves_origem[i].status;
        chaves_destino[i].descoberta = chaves_origem[i].descoberta;
    }
}

void iguala_chaves_salva_jogo(int num_chaves, CHAVE *chaves_origem, CHAVE *chaves_destino)
{
    //iguala as chaves na função salva jogo
    //é diferente da outra função de igualar chaves pois leva em consideração se a chave está impressa/descoberta/pega
    int i;

    for (i=0; i<num_chaves; i++)
    {
        chaves_destino[i].posicao.x = chaves_origem[i].posicao.x;
        chaves_destino[i].posicao.y = chaves_origem[i].posicao.y;
        chaves_destino[i].status = chaves_origem[i].status;
        chaves_destino[i].descoberta = chaves_origem[i].descoberta;
        if(chaves_origem[i].status == FALSE && chaves_origem[i].descoberta ==TRUE)
            chaves_destino[i].descoberta=FALSE;
        else
            chaves_destino[i].descoberta=chaves_origem[i].descoberta;
    }
}

void iguala_guardas(int num_guardas, GUARDA *guardas_origem, GUARDA *guardas_destino)
{
    //iguala dois arranjos de guardas
    int i;

    for (i=0; i<num_guardas; i++)
    {
        guardas_destino[i].posicao.x = guardas_origem[i].posicao.x;
        guardas_destino[i].posicao.y = guardas_origem[i].posicao.y;
    }
}

void iguala_paredes(int num_paredes, COORDENADA *paredes_origem, COORDENADA *paredes_destino)
{
    //iguala dois arranjos de paredes
    int i;

    for (i=0; i<num_paredes*NUM_SEGMENTOS; i++)
    {
        paredes_destino[i].x = paredes_origem[i].x;
        paredes_destino[i].y = paredes_origem[i].y;
    }
}

void iguala_jogadores(JOGADOR jogador_origem, JOGADOR *jogador_destino)
{
    //iguala todos os valores de dois jogadores
    strcpy (jogador_destino->nome, jogador_origem.nome);

    jogador_destino->chaves = jogador_origem.chaves;
    jogador_destino->pontos = jogador_origem.pontos;

    jogador_destino->posicao.x = jogador_origem.posicao.x;
    jogador_destino->posicao.y = jogador_origem.posicao.y;

    jogador_destino->vidas = jogador_origem.vidas;
    jogador_destino->vitoria = jogador_origem.vitoria;
}



int retorna_tecla()//retorna um comando de acordo com a tecla pressionada
{
    int tecla;
    tecla = getch();
    switch(tecla)
    {
    case 72: return CIMA; break; //setinha cima
    case 80: return BAIXO; break; //setinha baixo
    case 77: return DIREITA; break; //setinha direita
    case 75: return ESQUERDA; break; //setinha esquerda
    case 13: return ENTER; break; //ENTER
    case 27: return ESC; break; //ESC
    case 112: return PAUSA; break; //letra 'p'
    }
}

void pega_nome(JOGADOR *jogador) //pega o nome do jogador
{
    clrscr();
    textbackground(BLACK);
    desenha_cenario2();
    gotoxy((LIMX/4)+12, (LIMY/2)-4);
    printf(" N O V O  J O G O ");
    gotoxy(LIMX/4, LIMY/2);
    textbackground(BLACK);
    printf("      N O M E  D O  J O G A D O R : ");
    gets(jogador->nome);
    fflush(stdin);
    clrscr();
}

void movimenta_jogador(JOGADOR *jogador, int *tecla, COORDENADA paredes[], int num_paredes_total, COORDENADA castelo[], OGRO *shrek)
{
    //movimenta o jogador de acordo com as teclas pressionadas pelo usuário

    switch(*tecla)
    {
    case CIMA:
        //se o jogador não estiver se movimentando em direção a uma paredes, movimenta-o
        if(testa_paredes(jogador->posicao.x, jogador->posicao.y-1, paredes, num_paredes_total, castelo, shrek))
            animacao_jogador(jogador,0,-1);
        break;

    case BAIXO:
        if(testa_paredes(jogador->posicao.x, jogador->posicao.y+1, paredes, num_paredes_total, castelo, shrek))
            animacao_jogador(jogador,0,1);
        break;

    case DIREITA:
        if(testa_paredes(jogador->posicao.x+1, jogador->posicao.y, paredes, num_paredes_total, castelo, shrek))
            animacao_jogador(jogador,1,0);
        break;

    case ESQUERDA:
        if(testa_paredes(jogador->posicao.x-1, jogador->posicao.y, paredes, num_paredes_total, castelo, shrek))
            animacao_jogador(jogador,-1,0);
        break;
    }

}

void inicializa_tudo(int *mododejogo, JOGADOR *jogador, OGRO *shrek, int *num_chaves, CHAVE chaves[])
{
    //inicializa todos os valores necessários para a execução do jogo que não tenham que ser gerados aleatoriamente como guardas, chaves e paredes

    int contador;

    *mododejogo=RECORDES;
    jogador->vidas=VIDAS;
    jogador->chaves=FALSE;
    jogador->posicao.x=SAFEX;
    jogador->posicao.y=SAFEY;
    jogador->vitoria=FALSE;
    strcpy(jogador->nome,"Fiona");
    shrek->castelo=FALSE;
    for(contador=0; contador < *num_chaves; contador++)
    {
        chaves[contador].status = FALSE;
        chaves[contador].descoberta = FALSE;
    }
}

void movimenta_tudo(int *tecla, JOGADOR *jogador, COORDENADA paredes[], int num_paredes_total, int *num_paredes, COORDENADA castelo[], OGRO *shrek,int *num_guardas, GUARDA guardas[], int *num_chaves, CHAVE chaves[], TEMPO *tempo, int mododejogo)
{
    //realiza todos os movimentos do jogo

    if(kbhit())
    {
        *tecla = retorna_tecla();
        movimenta_jogador(jogador,tecla,paredes,num_paredes_total,castelo,shrek);
    }

    if(*tecla != ESC && *tecla != PAUSA)
    {
        //faz os testes necessários para a execução do jogo
        testa_guardas(guardas, jogador, *num_guardas, mododejogo);
        testa_chaves_jogador(chaves, jogador, *num_chaves);
        pega_chaves(chaves, jogador, *num_chaves, mododejogo);
        testa_ogro(jogador, shrek, castelo); // se o jogador tiver pelo menos 3 chaves o shrek é resgatado
        controla_tempo_guardas(tempo,guardas,num_guardas,paredes,num_paredes_total,castelo,shrek,chaves,num_chaves);
    }
    else
        //se o usuário clicar ESC ou P, exibe a tela de pausa
        pausar(tecla, jogador, guardas, num_guardas, chaves, num_chaves, paredes, num_paredes, *shrek, castelo, tempo, mododejogo);
}
