# Sistema de Monitoramento de Temperatura com Arduino e Alertas via Telegram

Projeto acadêmico desenvolvido para a disciplina de **Microprocessadores** – Estácio Via Corps.

O sistema realiza o monitoramento contínuo de temperatura e umidade, utilizando um **Arduino** como dispositivo de coleta e um **script em Python** como unidade de processamento, com envio de **alertas automáticos via Telegram** em situações críticas.

---

## Informações Acadêmicas

- Disciplina: Microprocessadores  
- Professor: Boanerges Almeida  
- Ano: 2025  
- Instituição: Estácio Via Corps  
- Local: Fortaleza – CE  

### Integrantes do Grupo
- Antônio Jorge – Montagem e documentação  
- Davi Peixoto – Líder, montagem e desenvolvimento do código  
- Felipe Holanda – Montagem e documentação  
- João Victor – Montagem e documentação  
- Roberta Lima – Documentação e banner  

---

## Objetivo do Projeto

Desenvolver um sistema de monitoramento térmico em tempo real capaz de:

- Medir temperatura e umidade do ambiente  
- Exibir informações em um display LCD  
- Permitir ajuste do setpoint via controle remoto infravermelho  
- Enviar alertas automáticos via Telegram quando a temperatura sair dos limites definidos  

O sistema é voltado para ambientes críticos, como câmaras frias, hospitais, UTIs, laboratórios e locais de armazenamento de alimentos e medicamentos.

---

## Conceitos Aplicados

- Microprocessadores e sistemas embarcados  
- Sensores de temperatura e umidade  
- Comunicação serial (UART)  
- Integração entre hardware e software  
- Automação e monitoramento ambiental  
- Integração com APIs (Telegram Bot API)  

---

## Tecnologias Utilizadas

### Hardware
- Arduino Uno (ATmega328P)  
- Sensor DHT11  
- Display LCD 16x2 com interface I²C  
- Receptor infravermelho (IR)  
- Controle remoto IR  
- LED, resistores, protoboard e jumpers  

### Software
- Linguagem C (Arduino IDE)  
- Python 3  
- Bibliotecas Python:
  - pyserial  
  - requests  
  - csv  

---

## Arquitetura do Sistema

Sensor DHT11  
↓  
Arduino Uno  
↓ (Serial USB)  
Python  
↓ (API)  
Telegram  

O Arduino realiza a leitura dos sensores, o Python processa os dados e toma decisões, e o Telegram recebe os alertas em tempo real.

---

## Funcionamento do Sistema

1. O Arduino lê temperatura e umidade do sensor DHT11  
2. O usuário ajusta o setpoint utilizando um controle IR  
3. Os dados são enviados pela porta serial  
4. O script em Python interpreta os valores recebidos  
5. Caso a temperatura ultrapasse os limites definidos:
   - Um alerta é enviado via Telegram  
   - O evento é registrado em um arquivo CSV  
6. O display LCD exibe as informações em tempo real  
7. Um LED pisca em código Morse (SOS) em situação crítica  

---

## Metodologia

O desenvolvimento do projeto foi dividido nas seguintes etapas:

1. Montagem do hardware  
2. Programação do Arduino  
3. Desenvolvimento do script em Python  
4. Integração com a API do Telegram  
5. Testes e validações em ambiente simulado  

---

## Resultados Obtidos

- Leitura contínua e estável da temperatura  
- Alertas enviados em frações de segundo  
- Comunicação serial sem perda de dados  
- Sistema funcional e adequado ao objetivo proposto  

---

## Estrutura do Repositório

monitoramento-temperatura-arduino/  
├── README.md  
├── docs/  
│   └── Microprocessadores__2_.docx  
├── arduino/  
│   └── monitoramento_temperatura.ino  
├── python/  
│   └── monitoramento_telegram.py  
└── imagens/  
    └── (fotos do protótipo e testes)  

---

## Conclusão

O projeto demonstrou, na prática, a integração entre microprocessadores, sensores e aplicações modernas baseadas em software.

Com baixo custo e tecnologias acessíveis, foi possível implementar um sistema eficiente de monitoramento térmico, com potencial de expansão futura, como a substituição do notebook por uma placa ESP32 com Wi-Fi, tornando o sistema totalmente autônomo.

---

## Observações Finais

Este repositório possui finalidade exclusivamente acadêmica e documenta todas as etapas do desenvolvimento do projeto.
