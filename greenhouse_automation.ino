//Programa: Greenhouse Automation
//Autor: Daniel Ortega
//Plataforma: Arduino Mega 2560
//Versão: 0.1.2
//Data: 03/02/2017

#include <Ultrasonic.h>
#include "DHT.h"
#include <LiquidCrystal.h>
 
//Configuração sensor ultrassonico
//Define os pinos do Arduino ligados ao Trigger e Echo
#define PINO_TRG  2
#define PINO_ECHO 3

//Configuração sensores temperatura e umidade
//Define o modelo do sensor DHT e o pino
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define em centimetros o nivel minimo e máximo da caixa de agua
float lvl_max = 20;
float lvl_min = 40;

//Define nome e pino dos reles
int relay_1 = 5;
int relay_2 = 4;
int relay_3 = 11;
int relay_4 = 10;
int relay_5 = 9;
int relay_6 = 8;
int relay_7 = 7;
int relay_8 = 6;

//Inicializa o sensor ultrasonico nos pinos especificados acima
Ultrasonic ultrasonic(PINO_TRG, PINO_ECHO);

//Configura os pinos do Arduino para se comunicar com o LCD
//Sintaxe para usar o display no modo 4 bits  
//LiquidCrystal lcd(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 34, 28, 26, 24, 22); 
 

void setup()
{
  //Inicializa a serial
  Serial.begin(9600);
  
  //Inicializa o DHT
  dht.begin();

  //Define os pinos dos reles como saida
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(relay_5, OUTPUT);
  pinMode(relay_6, OUTPUT);
  pinMode(relay_7, OUTPUT);
  pinMode(relay_8, OUTPUT);

  lcd.begin(16, 2); //Inicia o LCD com dimensões 16x2(Colunas x Linhas)
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna(0) e na primeira linha(0) do LCD
  lcd.print("Olivopampa");
  lcd.setCursor(0, 1); //Posiciona o cursor na primeira coluna(0) e na segunda linha(1) do LCD
  lcd.print("Gestão de Viveiro");
}

void loop()
{
  //Variaveis para guardar os valores em cm (cmSec)
  float cmMsec;
  
  //Le os valores do sensor ultrasonico
  long microsec = ultrasonic.timing();
  
  //Atribui os valores em cm ou polegadas as variaveis
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  if (cmMsec < lvl_max)
  {
    digitalWrite(relay_1, HIGH);
  }
 
  if (cmMsec > lvl_min)
  {
    digitalWrite(relay_1, LOW);
  }
  
   //Le temperatura e umidade
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //Ativa ventiladores se temperatura maior que 30 graus
  if (t >= 30)
  {
    digitalWrite(relay_2, LOW);  
  }

  else 
    digitalWrite(relay_2, HIGH);

  //Ativa aquecedores se temperatura menor que 20 graus
   if (t <= 20)
   {
    digitalWrite(relay_3, LOW); 
   }

   else
    digitalWrite(relay_3, HIGH);

  //Mantem os outros reles desligados
  digitalWrite(relay_3, HIGH);
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_5, HIGH);
  digitalWrite(relay_6, HIGH);
  digitalWrite(relay_7, HIGH);
  digitalWrite(relay_8, HIGH);

  //Mostra na serial os dados lidos pelos sensores
  Serial.print("Distancia: ");
  Serial.print(cmMsec);
  Serial.print(" cm ");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");
  
  delay(5000);
}
