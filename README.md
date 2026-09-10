# Urna Eletrônica com Arduino

**Universidade do Oeste de Santa Catarina (UNOESC)**  
**Disciplina:** Introdução ao Hardware  
**Integrantes:** 
- Eduardo Jean Padilha de Andrade
- Gabriel Lunelli
- Vandoir Gerhard
- Vinicius Gabriel Flores da Cunha

---

Este projeto implementa a simulação de uma urna eletrônica utilizando a plataforma Arduino. O sistema conta com um display LCD para interface visual, botões para a seleção de candidatos, um buzzer para emissão sonora da confirmação do voto e controle de fluxo da eleição através da comunicação serial.

## 🛠️ Componentes e Pinagem

O circuito requer a montagem dos seguintes componentes conectados aos pinos digitais do Arduino:

| Componente | Pinos no Arduino | Observações |
| :--- | :--- | :--- |
| **Display LCD 16x2** | 12, 11, 7, 8, 9, 10 | Conexões padrão RS, E, D4, D5, D6, D7. |
| **Botão Candidato 1** | 5 | Configurado com resistor interno (`INPUT_PULLUP`). |
| **Botão Candidato 2** | 4 | Configurado com resistor interno (`INPUT_PULLUP`). |
| **Botão Candidato 3** | 3 | Configurado com resistor interno (`INPUT_PULLUP`). |
| **Botão Candidato 4** | 2 | Configurado com resistor interno (`INPUT_PULLUP`). |
| **Buzzer** | 6 | Responsável pelo som de confirmação. |

## 💻 Instruções de Uso e Comandos

O controle da votação (função do mesário) é feito inteiramente pelo **Monitor Serial** da IDE do Arduino. 

**Passos Iniciais:**
1. Realize a montagem física do hardware.
2. Faça o upload do código para o Arduino.
3. Abra o Monitor Serial e certifique-se de configurá-lo para **9600 baud**.

**Comandos Suportados no Monitor Serial:**

*   **`Vote`**: Libera a urna para receber **exatamente um voto**. O sistema permanecerá bloqueado até que este comando seja enviado novamente.
*   **`Display`**: Mostra a apuração parcial dos votos em andamento, exibindo os dados tanto no Monitor Serial quanto intercalando os valores no display LCD.
*   **`Fim`**: Encerra permanentemente o processo de votação, calcula os totais e exibe o vencedor (ou empate) na tela.

## ⚙️ Funcionamento e Estados da Urna

O sistema opera utilizando uma máquina de estados simples, garantindo a integridade dos votos:

*   **BLOQUEADA**: Estado padrão. Os botões não registram nenhuma ação.
*   **LIBERADA**: Ativada via comando `Vote`. Aguarda o pressionamento de um único botão. Após o registro e a emissão do som, retorna automaticamente ao estado BLOQUEADA.
*   **ENCERRADA**: Ativada via comando `Fim`. A urna é travada de forma definitiva até o sistema ser reiniciado.
