#include <LiquidCrystal.h>

// ======================================================
// CONFIGURACAO DO LCD
// ======================================================

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 7, 8, 9, 10);

// ======================================================
// PINOS
// ======================================================

const int BTN_CANDIDATO_1 = 5;
const int BTN_CANDIDATO_2 = 4;
const int BTN_CANDIDATO_3 = 3;
const int BTN_CANDIDATO_4 = 2;

const int BUZZER = 6;

// ======================================================
// ESTADOS DA URNA
// ======================================================

enum EstadoUrna {
  BLOQUEADA,
  LIBERADA,
  ENCERRADA
};

EstadoUrna estadoAtual = BLOQUEADA;

// ======================================================
// DADOS DOS CANDIDATOS
// ======================================================

const int TOTAL_CANDIDATOS = 4;

String nomes[TOTAL_CANDIDATOS] = {
  "Candidato 1",
  "Candidato 2",
  "Candidato 3",
  "Candidato 4"
};

int votos[TOTAL_CANDIDATOS] = {
  0, 0, 0, 0
};

// ======================================================
// SETUP
// ======================================================

void setup() {

  // Inicializa comunicacao serial
  Serial.begin(9600);

  // Inicializa LCD 16x2
  lcd.begin(16, 2);

  // Configura botoes com resistor interno pull-up
  pinMode(BTN_CANDIDATO_1, INPUT_PULLUP);
  pinMode(BTN_CANDIDATO_2, INPUT_PULLUP);
  pinMode(BTN_CANDIDATO_3, INPUT_PULLUP);
  pinMode(BTN_CANDIDATO_4, INPUT_PULLUP);

  // Configura buzzer
  pinMode(BUZZER, OUTPUT);

  // Inicialmente a urna permanece bloqueada
  bloquearUrna();

  Serial.println("================================");
  Serial.println("URNA ELETRONICA INICIADA");
  Serial.println("Comandos disponiveis:");
  Serial.println("Vote    -> Libera um voto");
  Serial.println("Display -> Mostra apuracao parcial");
  Serial.println("Fim     -> Encerra a votacao");
  Serial.println("================================");
}

// ======================================================
// LOOP PRINCIPAL
// ======================================================

void loop() {

  // Sempre verifica comandos recebidos pelo Serial
  verificarComandosSerial();

  // Apenas permite votar se a urna estiver liberada
  if (estadoAtual == LIBERADA) {
    lerVoto();
  }

}

// ======================================================
// VERIFICA COMANDOS RECEBIDOS PELO SERIAL
// ======================================================

void verificarComandosSerial() {

  if (Serial.available() > 0) {

    // Le comando ate encontrar quebra de linha
    String comando = Serial.readStringUntil('\n');

    // Remove espacos e quebras de linha
    comando.trim();

    // --------------------------------------------------
    // COMANDO VOTE
    // --------------------------------------------------

    if (comando == "Vote") {

      if (estadoAtual == ENCERRADA) {

        Serial.println("ERRO: A votacao ja foi encerrada.");

      }
      else if (estadoAtual == LIBERADA) {

        Serial.println("A urna ja esta liberada para um voto.");

      }
      else {

        liberarVoto();

      }

    }

    // --------------------------------------------------
    // COMANDO DISPLAY
    // --------------------------------------------------

    else if (comando == "Display") {

      mostrarApuracao();

    }

    // --------------------------------------------------
    // COMANDO FIM
    // --------------------------------------------------

    else if (comando == "Fim") {

      if (estadoAtual == ENCERRADA) {

        Serial.println("A votacao ja esta encerrada.");

      } else {

        encerrarVotacao();

      }

    }

    // --------------------------------------------------
    // COMANDO INVALIDO
    // --------------------------------------------------

    else {

      Serial.println("Comando invalido!");
      Serial.println("Use: Vote, Display ou Fim");

    }

  }

}

// ======================================================
// LIBERA EXATAMENTE UM VOTO
// ======================================================

void liberarVoto() {

  estadoAtual = LIBERADA;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Votacao");

  lcd.setCursor(0, 1);
  lcd.print("Liberada");

  Serial.println("Votacao liberada para UM eleitor.");

}

// ======================================================
// BLOQUEIA A URNA
// ======================================================

void bloquearUrna() {

  estadoAtual = BLOQUEADA;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Urna Bloqueada");

  lcd.setCursor(0, 1);
  lcd.print("Aguardando");

}

// ======================================================
// LEITURA DOS BOTOES
// ======================================================

void lerVoto() {

  // Candidato 1
  if (digitalRead(BTN_CANDIDATO_1) == LOW) {

    registrarVoto(0);
    aguardarSoltura();

  }

  // Candidato 2
  else if (digitalRead(BTN_CANDIDATO_2) == LOW) {

    registrarVoto(1);
    aguardarSoltura();

  }

  // Candidato 3
  else if (digitalRead(BTN_CANDIDATO_3) == LOW) {

    registrarVoto(2);
    aguardarSoltura();

  }

  // Candidato 4
  else if (digitalRead(BTN_CANDIDATO_4) == LOW) {

    registrarVoto(3);
    aguardarSoltura();

  }

}

// ======================================================
// REGISTRA O VOTO
// ======================================================

void registrarVoto(int candidato) {

  // Incrementa voto do candidato selecionado
  votos[candidato]++;

  // Mostra candidato escolhido
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Voto para:");

  lcd.setCursor(0, 1);
  lcd.print(nomes[candidato]);

  // Mensagem no Serial
  Serial.print("Voto computado para: ");
  Serial.println(nomes[candidato]);

  // Emite som de confirmacao
  tocarSomUrna();

  // Mantem mensagem por alguns segundos
  delay(2000);

  // Depois do voto, urna bloqueia novamente
  bloquearUrna();

}

// ======================================================
// AGUARDA TODOS OS BOTOES SEREM SOLTOS
// Evita que um pressionamento gere mais de um voto
// ======================================================

void aguardarSoltura() {

  delay(50);

  while (
    digitalRead(BTN_CANDIDATO_1) == LOW ||
    digitalRead(BTN_CANDIDATO_2) == LOW ||
    digitalRead(BTN_CANDIDATO_3) == LOW ||
    digitalRead(BTN_CANDIDATO_4) == LOW
  ) {
    // Aguarda botoes serem soltos
  }

  delay(50);

}

// ======================================================
// MOSTRA APURACAO PARCIAL
// Como existem 4 candidatos e LCD possui 2 linhas,
// os resultados sao exibidos em duas telas.
// ======================================================

void mostrarApuracao() {

  Serial.println("===== APURACAO PARCIAL =====");

  for (int i = 0; i < TOTAL_CANDIDATOS; i++) {

    Serial.print(nomes[i]);
    Serial.print(": ");
    Serial.print(votos[i]);
    Serial.println(" votos");

  }

  // Primeira tela
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Cand1: ");
  lcd.print(votos[0]);

  lcd.setCursor(0, 1);
  lcd.print("Cand2: ");
  lcd.print(votos[1]);

  delay(3000);

  // Segunda tela
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Cand3: ");
  lcd.print(votos[2]);

  lcd.setCursor(0, 1);
  lcd.print("Cand4: ");
  lcd.print(votos[3]);

  delay(3000);

  // Retorna ao estado correto
  if (estadoAtual == BLOQUEADA) {
    bloquearUrna();
  }
  else if (estadoAtual == LIBERADA) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Votacao");

    lcd.setCursor(0, 1);
    lcd.print("Liberada");

  }

}

// ======================================================
// ENCERRA A VOTACAO
// ======================================================

void encerrarVotacao() {

  estadoAtual = ENCERRADA;

  Serial.println("================================");
  Serial.println("VOTACAO ENCERRADA");
  Serial.println("================================");

  mostrarResultadoFinal();

}

// ======================================================
// CALCULA E MOSTRA RESULTADO FINAL
// ======================================================

void mostrarResultadoFinal() {

  int maiorNumeroVotos = votos[0];
  int vencedor = 0;

  bool empate = false;

  // Procura candidato com maior numero de votos
  for (int i = 1; i < TOTAL_CANDIDATOS; i++) {

    if (votos[i] > maiorNumeroVotos) {

      maiorNumeroVotos = votos[i];
      vencedor = i;

      empate = false;

    }
    else if (votos[i] == maiorNumeroVotos) {

      empate = true;

    }

  }

  // Exibe resultado no Serial
  Serial.println("RESULTADO FINAL:");

  for (int i = 0; i < TOTAL_CANDIDATOS; i++) {

    Serial.print(nomes[i]);
    Serial.print(": ");
    Serial.print(votos[i]);
    Serial.println(" votos");

  }

  // Nenhum voto
  if (maiorNumeroVotos == 0) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Fim da eleicao");

    lcd.setCursor(0, 1);
    lcd.print("Sem votos");

    Serial.println("Nenhum voto computado.");

  }

  // Empate
  else if (empate) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Fim da eleicao");

    lcd.setCursor(0, 1);
    lcd.print("EMPATE");

    Serial.println("Resultado: EMPATE");

  }

  // Vencedor
  else {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Vencedor:");

    lcd.setCursor(0, 1);
    lcd.print(nomes[vencedor]);

    Serial.print("VENCEDOR: ");
    Serial.println(nomes[vencedor]);

    Serial.print("TOTAL DE VOTOS: ");
    Serial.println(maiorNumeroVotos);

  }

}

// ======================================================
// SOM DE CONFIRMACAO DO VOTO
// ======================================================

void tocarSomUrna() {

  // Pequenos bipes iniciais
  for (int i = 0; i < 4; i++) {

    tone(BUZZER, 1337);
    delay(90);
    tone(BUZZER, 1437);
    delay(90);
  }

  // Som final mais longo
  tone(BUZZER, 1337, 120);
  delay(120);

}