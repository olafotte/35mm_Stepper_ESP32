#include <AccelStepper.h>
#include <BleKeyboard.h>

// --- CONFIGURAÇÃO BLUETOOTH ---
BleKeyboard bleKeyboard("Scanner Cinema", "Maker", 100);
int contadorFotos = 0;

// --- LADO A DO BARRAMENTO (Motor e Buzzer) ---
const int stepPin = 12;
const int dirPin = 14;
const int enablePin = 13;
const int buzzerPin = 27; 

// --- LADO B DO BARRAMENTO (Sensores Touch) ---
const int pinTouchPassos = 32;
const int pinTouchReverso = 33;
const int pinTouchLoop = 4;
const int pinTouchIncremento = 15;

// Configurações
const int threshold = 40; 
int passosBase = 400;
int direcao = 1; 
bool emLoop = false;
unsigned long delayCaptura = 500; // Aumentado levemente para dar tempo do foco da câmera

AccelStepper stepper(1, stepPin, dirPin);

// --- FUNÇÃO DISPARAR FOTO ---
void dispararFoto() {
  if (bleKeyboard.isConnected()) {
    contadorFotos++;
    Serial.printf(">> FOTO #%d DISPARADA <<\n", contadorFotos);
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);     
  } else {
    Serial.println("Erro: Bluetooth desconectado. Aguardando pareamento...");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("--- Sistema Iniciado ---");
  
  bleKeyboard.begin();
  Serial.println("Bluetooth 'Scanner Cinema' iniciado!");

  pinMode(buzzerPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); 

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
}

void loop() {
  verificarSerial();

  // Lógica do Buzzer: HIGH enquanto houver passos para percorrer
  if (stepper.distanceToGo() != 0) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // 1. Pino 15: Incrementa a quantidade de passos
  if (touchRead(pinTouchIncremento) < threshold) {
    passosBase += 200;
    Serial.print("Passos incrementados: "); Serial.println(passosBase);
    pararLoop(); 
    delay(300); 
  }

  // 2. Pino 33: Inverte a direção
  if (touchRead(pinTouchReverso) < threshold) {
    direcao *= -1;
    Serial.println(direcao == 1 ? "Direcao: Horario" : "Motor em reverso");
    pararLoop();
    delay(300);
  }

  // 3. Pino 32: Gira os passos definidos manualmente
  if (touchRead(pinTouchPassos) < threshold) {
    Serial.print("Movendo manual: "); Serial.println(passosBase);
    stepper.move(passosBase * direcao);
    pararLoop();
    delay(300);
  }

  // 4. Pino 4: Ativa o Loop de Captura
  if (touchRead(pinTouchLoop) < threshold) {
    if (!emLoop) {
      Serial.println("MODO CAPTURA ATIVADO");
      emLoop = true;
      delay(500); 
    }
  }

  if (emLoop) {
    executarCicloCaptura();
  }

  stepper.run();
}

void verificarSerial() {
  if (Serial.available() > 0) {
    int novoValor = Serial.parseInt();
    if (novoValor > 0) {
      passosBase = novoValor;
      Serial.print(">>> Novo passosBase definido: "); Serial.println(passosBase);
    }
  }
}

void executarCicloCaptura() {
  // Só age se o motor estiver parado
  if (stepper.distanceToGo() == 0) {
    static unsigned long tempoAnterior = 0;
    static bool esperando = false;

    if (!esperando) {
      dispararFoto(); // Chama a lógica Bluetooth
      esperando = true;
      tempoAnterior = millis();
    }

    // Espera o delayCaptura após a foto para mover o motor novamente
    if (millis() - tempoAnterior >= delayCaptura) {
      stepper.move(passosBase * direcao);
      esperando = false;
    }
  }
}

void pararLoop() {
  if (emLoop) {
    emLoop = false;
    Serial.println("Loop interrompido.");
    stepper.stop();
    contadorFotos = 0; // Reseta contador ao parar
  }
}