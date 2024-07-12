uint8_t pin_ldr1 = 36;
uint8_t pin_ldr2 = 39;
uint8_t pin_gate1 = 23;
uint8_t pin_gate2 = 22;
const int pot1Pin = 34;
const int pot2Pin = 35;
int ldr1_value;
int ldr2_value;
int maxADCValue = 4096;  // Máximo valor para un ADC de 12 bits

unsigned long previousMillis = 0;  // Almacena el último tiempo registrado
int interval1 = 100;               // Intervalo de tiempo en milisegundos (1000 ms = 1 segundo)
unsigned long previousMillis2 = 0;
int interval2 = 50;
const int offset = 1000;  // Offset a eliminar de las lecturas

void debugging() {

  Serial.print("LDR1:");
  Serial.print(ldr1_value);

  Serial.print(", LDR2:");
  Serial.println(ldr2_value);

  Serial.print(", interval:");
  Serial.println(interval1);
}

void setup() {
  Serial.begin(9600);
  pinMode(pin_gate1, OUTPUT);
  pinMode(pin_gate2, OUTPUT);

}

void loop() {
  interval1 = analogRead(pot1Pin) + 1;
  interval1 = map(interval1, 0, 4095, 10, 1000);
  interval2 = analogRead(pot2Pin) + 1;
  interval2 = map(interval2, 0, 4095, 10, 1000);

  ldr1_value = maxADCValue - analogRead(pin_ldr1);
  ldr2_value = maxADCValue - analogRead(pin_ldr2);
  // Escalar el valor para DAC utilizando map (asumiendo una resolución de 8 bits para el DAC)
  ldr1_value = map(ldr1_value, 1500, 3500, 0, 255);
  ldr2_value = map(ldr2_value, offset, 3000, 0, 255);

  // Asegurarse de que los valores no sean negativos
  if (ldr1_value < 0) {
    ldr1_value = 0;
  }
  if (ldr2_value < 0) {
    ldr2_value = 0;
  }

  unsigned long currentMillis = millis();

  // Comprobar si ha pasado el intervalo de tiempo
  if (currentMillis - previousMillis >= interval1) {
    // Guardar el último tiempo
    previousMillis = currentMillis;

    dacWrite(DAC1, ldr1_value);

    if (ldr1_value > 150) {

      digitalWrite(pin_gate1, HIGH);
    } else {
      digitalWrite(pin_gate1, LOW);
    }
    //debugging();
  }

  unsigned long currentMillis2 = millis();

  // Comprobar si ha pasado el intervalo de tiempo
  if (currentMillis2 - previousMillis2 >= interval2) {
    // Guardar el último tiempo
    previousMillis2 = currentMillis2;
    dacWrite(DAC2, ldr2_value);

    if (ldr2_value > 150) {

      digitalWrite(pin_gate2, HIGH);
    } else {
      digitalWrite(pin_gate2, LOW);
    }
  }
}
