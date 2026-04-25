# Scanner Cinema - Automação de Captura com ESP32

Este projeto utiliza um ESP32 para controlar um motor de passo e disparar fotos remotamente via Bluetooth (HID), simulando um teclado. É ideal para digitalização de filmes, stop motion ou scanners 3D.

## 🚀 Funcionalidades

* **Disparo Bluetooth:** Simula a tecla "Volume Up" para disparar a câmera de smartphones ou computadores.
* **Controle de Motor:** Movimentação precisa usando a biblioteca `AccelStepper`.
* **Sensores Touch:** Interface simplificada utilizando os pinos capacitivos do ESP32.
* **Modo Loop (Captura Automática):** Ciclo contínuo de movimento seguido de disparo de foto.
* **Feedback Sonoro:** Buzzer ativo durante o movimento do motor.
* **Ajuste via Serial:** Permite alterar a quantidade de passos enviando valores pelo monitor serial.

## 🛠️ Hardware Necessário

* ESP32 (DevKit V1 ou similar)
* Driver de Motor de Passo (A4988, DRV8825, etc.)
* Motor de Passo (NEMA 17, por exemplo)
* Buzzer Ativo/Passivo
* Fios/Jumpers para sensores touch

## 📌 Pinagem (GPIOs)

| Componente | Pino ESP32 | Função |
| :--- | :--- | :--- |
| **Motor STEP** | 12 | Pulso do motor |
| **Motor DIR** | 14 | Direção do motor |
| **Motor ENABLE** | 13 | Ativa/Desativa o driver |
| **Buzzer** | 27 | Feedback sonoro |
| **Touch Passo** | 32 | Movimento manual único |
| **Touch Reverso** | 33 | Inverte o sentido do giro |
| **Touch Loop** | 4 | Inicia o ciclo automático |
| **Touch Incr.** | 15 | Aumenta +200 passos na base |

## 💻 Requisitos de Software

As seguintes bibliotecas devem estar instaladas na sua Arduino IDE:
1.  [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/)
2.  [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)

## ⚙️ Como Usar

1.  **Pareamento:** Ligue o ESP32 e procure por um dispositivo Bluetooth chamado **"Scanner Cinema"**. Conecte-se a ele.
2.  **Configuração:** Abra o app de câmera no seu celular (o app deve suportar disparar fotos com o botão de volume).
3.  **Operação:**
    * Toque no pino **15** para ajustar a distância do salto.
    * Toque no pino **33** se precisar inverter a direção do filme/motor.
    * Toque no pino **32** para testar um avanço manual.
    * Toque no pino **4** para iniciar o modo de captura automática.
4.  **Interrupção:** Qualquer comando manual durante o Loop irá pará-lo por segurança.

## 📝 Notas de Versão
* **v1.0:** Lógica básica de movimento, controle touch e integração BLE Keyboard.

---
Desenvolvido por Maker.
