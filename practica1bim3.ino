#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Constantes del sensor ultrasónico
const int trigPin = 4;
const int echoPin = 5;

// Constantes de los pulsadores
const int pausaPin = 2;
const int vaciarPin = 3;

// Constante del pin del relé
const int relePin = 6;
const int buzzer = 7;

// Constantes de límites del contenedor
const int limiteSuperior = 20; // En centímetros
const int limiteInferior = 5; // En centímetros

// Variables globales
volatile bool pausado = false;
int distanciaAgua = 0;


// Melodía personalizada para el buzzer
const int melodia[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int duracionNota = 200;

// Objeto LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C y dimensiones del LCD

void setup() {
  // Configuración de los pines
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pausaPin, INPUT_PULLUP);
  pinMode(vaciarPin, INPUT_PULLUP);
  pinMode(relePin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Configuración del LCD
  lcd.begin(16, 2);
  lcd.print("Cantidad de Agua:");

  // Configuración de la interrupción para el pulsador de pausa
  attachInterrupt(digitalPinToInterrupt(pausaPin), pausarPrograma, FALLING);
}

void loop() {
  if (!pausado) {
    // Medir la distancia con el sensor ultrasónico
    distanciaAgua = medirDistancia();

    // Mostrar la cantidad de agua en el LCD
    mostrarCantidadAgua(distanciaAgua);

    // Comprobar si se alcanzó el límite del contenedor
    if (distanciaAgua <= limiteInferior) {
      // Activar el relé
      encenderRele();
    } else {
      // Apagar el relé
      apagarRele();
    }

    // Comprobar el pulsador de vaciado del contenedor
    if (digitalRead(vaciarPin) == LOW) {
      vaciarContenedor();
    }
  }
}

int medirDistancia() {
  // Enviar el pulso ultrasónico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calcular la duración del eco
  unsigned long duracion = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  int distancia = duracion * 0.034 / 2;

  return distancia;
}

void mostrarCantidadAgua(int distancia) {
  lcd.setCursor(0, 1);
  lcd.print("                "); // Borrar el contenido anterior
  lcd.setCursor(0, 1);
  lcd.print(distancia);
  lcd.print(" cm");
}

void encenderRele() {
  digitalWrite(relePin, HIGH);
}

void apagarRele() {
  digitalWrite(relePin, LOW);
}

void vaciarContenedor() {
  (buzzer, LOW);
}

void pausarPrograma() {
  pausado = true;

  // Activar el relé durante 10 segundos
  encenderRele();
  delay(10000);
  apagarRele();

  // Pausar el programa durante 10 segundos
  delay(10000);

  // Reanudar el programa
  pausado = false;
}
