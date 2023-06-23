# ABP: Relógio Digital

O projeto ABP (Aprendizagem Baseada em Projetos) consiste em um projeto digital desenvolvido para a disciplina de Sistemas Embarcados. Ele tem como objetivo explorar os conceitos e práticas relacionados à programação de microcontroladores utilizando o ATmega328 e a plataforma Arduino.

Este é um programa de relógio desenvolvido para o Arduino. Ele permite exibir as horas e minutos em um display de LED de sete segmentos, definir um alarme para tocar a partir do buzzer e alterar o horario de forma manual mostrado no display. O programa é escrito em linguagem C++ e usa as bibliotecas padrão do Arduino.

## Componentes necessários
* 1 Arduino Uno
* 1 Placa de ensaio
* 1 Display de LED de sete segmentos com quatro dígitos
* 6 Resistores de 330 ohms 
* 4 Botões
* 1 Buzzer
* 1 Potenciômetro 
* 2 Leds
* Cabo USB
* Diversos jumpers para conexões

## Configuração do hardware
Conecte os pinos do display de sete segmentos aos pinos digitais do Arduino, conforme a tabela abaixo:

|Pino do display |	Pino do Arduino   |
|----------------|-------------------- |
|              A | A5   |
|              B | A4   |	
|              C | A3   |	
|              D | A2   |	
|              E | A1   |	
|              F | A0   |  	
|              G | 13   |
|             DP | 8	   |
|             D1 | 12   |
|             D2 | 11   |
|             D3 | 10   |
|             D4 | 9    |

|Componentes     |	Pino do Arduino   |
|----------------|-------------------- |
|      Led Alarme| 6                   |
|      Led Ajuste| 7	                  |
|   Potenciômetro| 5	                  |
|          Buzzer| 0	                  |	
|       Botão Inc| 2                   |
|       Botão Dec| 3                   |
|      Botão Stop| 1                   |
|      Botão Modo| 4                   |

![Esquemático do Relógio](/relogioesq.png)

## Configuração do software
1. [Instale a IDE do Arduino através do site oficial](https://www.arduino.cc/en/software) e abra o software em seu computador.
2. Crie um novo projeto e salve-o com o nome desejado.
3. Conecte a placa Arduino Uno ao computador por meio do cabo USB.
4. Faça upload do código através da aba "Arquivo".
5. Selecione a placa correta e a porta serial adequada nas configurações.
6. Verifique se não há erros no código clicando no botão "Verificar" (ícone de marca de seleção).
7. Faça o upload do programa para o Arduino clicando no botão "Enviar" (ícone de seta).

## Autores
- [Jefferson Barzan Alexandrino](https://github.com/JeffAlexandrino)
- [João Augusto da Silva Borges](https://github.com/autor2)
- [João Paulo Nunes Martins](https://github.com/joaopaulomts)
- [Vitor Koch Wessler](https://github.com/VitorKW)

