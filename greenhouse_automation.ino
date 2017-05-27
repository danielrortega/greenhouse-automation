//Programa: Greenhouse Automation
//Autor: Daniel Ortega
//Plataforma: Arduino Mega 2560
//Versão: 0.1.7
//Data: 27/05/2017

//#include <Ultrasonic.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

//Configuração sensor ultrassonico-------------------------------------------
//Define os pinos do Arduino ligados ao Trigger e Echo
#define PINO_TRG  14
#define PINO_ECHO 15

//Define em centimetros o nivel minimo e máximo da caixa de agua
float lvl_max = 20;
float lvl_min = 40;

//Inicializa o sensor ultrasonico nos pinos especificados acima
Ultrasonic ultrasonic(PINO_TRG, PINO_ECHO);

//Configuração sensores temperatura e umidade--------------------------------

//Define o modelo do sensor DHT e o pino
#define DHTPIN 8
#define DHTPIN1 9

#define DHTTYPE DHT22
#define DHTTYPE1 DHT22

DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);


//Define nome e pino dos reles
int relay_1 = 5;
int relay_2 = 6;
int relay_3 = 7;
int relay_4 = 52;
int relay_5 = 9;
int relay_6 = 10;
int relay_7 = 11;
int relay_8 = 12;

//Configuração LCD-----------------------------------------------------------

//Configura os pinos do Arduino para se comunicar com o LCD
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

// Array simbolo grau
byte grau[8] ={ B00001100, 
                B00010010, 
                B00010010, 
                B00001100, 
                B00000000, 
                B00000000, 
                B00000000, 
                B00000000,}; 

//Configuração ethernet-------------------------------------------------------

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xCD, 0xB8 };
EthernetClient client;


//Cria a variável data para envio ao DB---------------------------------------

String data;
  

void setup()
{
  
  //Inicializa a serial
  Serial.begin(9600);

  Serial.println("Porta Serial OK");
  Serial.println("Aguardando dados...");
  
  // Inicializa o display
  lcd.begin(16, 2);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Conectando...");
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("A configuracao da rede falhou. Reinicie o sistema");
  };

  data = "";
    
  //Inicializa o DHT
  dht.begin();
  dht1.begin();
  
  //Define os pinos dos reles como saida
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(relay_5, OUTPUT);
  pinMode(relay_6, OUTPUT);
  pinMode(relay_7, OUTPUT);
  pinMode(relay_8, OUTPUT);

  //Mensagem boas vindas
  lcd.clear(); 
  lcd.createChar(0, grau); // Cria o caracter customizado com o simbolo do grau
  lcd.setCursor(1,0); // Informacoes iniciais no display
  lcd.print("Greenhouse ATM");
  lcd.setCursor(4,1);
  lcd.print("Welcome");

}

void loop()
{
  
  //Automacao Reservatorio Agua
  
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
  
   
   //Le temperatura e umidade----------------------------------------------------
   
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  
  // Verifica se o sensor esta respondendo
  if (isnan(h) || isnan(t))
  {
    Serial.println("Falha ao ler dados do sensor externo");
    return;
  }
   if (isnan(h1) || isnan(t1))
  {
    Serial.println("Falha ao ler dados do sensor interno");
    return;
  }

  // Computa sensacao termica
  float hic = dht.computeHeatIndex(t, h, false);
  float hic1 = dht.computeHeatIndex(t1, h1, false);

  //Computa o ponto de orvalho
  double gamma = log(h / 100) + ((17.62 * t) / (243.5 + t));
  double dp = 243.5 * gamma / (17.62 - gamma);
  
  //Automacao Climatizadores----------------------------------------------------
  
 
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
  

  //Dados dos sensores na serial-------------------------------------------------------------
  
  Serial.print("Distancia: ");
  Serial.print(cmMsec);
  Serial.println(" cm ");
  Serial.print("Temperatura Ext: ");
  Serial.print(t,1);
  Serial.print(" *C ");
  Serial.print("Umidade Ext: ");
  Serial.print(h,1);
  Serial.print(" % ");
  Serial.print("Sensacao Termica Ext: ");
  Serial.print(hic,1);
  Serial.print(" *C ");
  Serial.print("Ponto de Orvalho: ");
  Serial.print(dp,1);
  Serial.println(" *C");
  Serial.print("Temperatura Int: ");
  Serial.print(t1,1);
  Serial.print(" *C ");
  Serial.print("Umidade Int: ");
  Serial.print(h1,1);
  Serial.print(" % ");
  Serial.print("Sensacao Termica Int: ");
  Serial.print(hic1,1);
  Serial.println(" *C");
  
  //HTTP Post-------------------------------------------------------------------
  
  String temp = String(t,1);
  
  String umid = String (h,1);

  String temp1 = String(t1,1);
  
  String umid1 = String (h1,1);

  String sens = String(hic,1);
  
  String sens1 = String (hic1,1);

  String dewp = String (dp,1);
  
  data = "greenhouse_atm,SOURCE=sws_1 Temperatura=" + temp + ",Umidade=" + umid + ",Temperatura_Int=" + temp1 + ",Umidade_int=" + umid1 + ",Sens_Ext=" + sens + ",Sens_Int=" + sens1 + ",DewPoint=" + dewp;

  if (client.connect("192.168.1.7",8086)) {
      client.println("POST /write?db=olivopampa_atm HTTP/1.1");
      client.println("Host: 192.168.1.7");
      client.println("User-Agent: Arduino/1.0");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(data.length()); 
      client.println(); 
      client.print(data);
      Serial.println(data);
      
      delay(50); //aguardo o servidor processar os dados

  } 

  if (client.connected()) { 
    client.stop();  // Desconecta do servidor
    }

  //LCD Display-------------------------------------------------------------
  
  lcd.clear();
  
  // Mostra a temperatura do DHT22 no display
  lcd.setCursor(0,0);
  lcd.print("Temp Ext: ");
  lcd.setCursor(10,0);
  lcd.print(t,1);
  lcd.write(byte(0)); // Mostra o simbolo do grau
  lcd.print("C");

  
  // Mostra a umidade do DHT22 no display
  lcd.setCursor(0,1);
  lcd.print("Umid Ext: ");
  lcd.setCursor(10,1);
  lcd.print(h,1);
  lcd.setCursor(15,1);
  lcd.print("%");

  delay(3000);
  
  lcd.clear();
  
  // Mostra a temperatura do DHT interno no display
  lcd.setCursor(0,0);
  lcd.print("Temp Int: ");
  lcd.setCursor(10,0);
  lcd.print(t1,1);
  lcd.write(byte(0));
  lcd.print("C");

  
  // Mostra a umidade do DHT interno no display
  lcd.setCursor(0,1);
  lcd.print("Umid Int: ");
  lcd.setCursor(10,1);
  lcd.print(h1,1);
  lcd.setCursor(15,1);
  lcd.print("%"); 

  delay(3000);
  
  lcd.clear();
  
  // Mostra a sensacao termica externa no display
  lcd.setCursor(0,0);
  lcd.print("Sens Ext: ");
  lcd.setCursor(10,0);
  lcd.print(hic,1);
  lcd.write(byte(0));
  lcd.print("C");

  
  // Mostra a sensacao termica interna no display
  lcd.setCursor(0,1);
  lcd.print("Sens Int: ");
  lcd.setCursor(10,1);
  lcd.print(hic1,1);
  lcd.write(byte(0));
  lcd.print("C");

  delay(3000);

  lcd.clear();
  
  // Mostra o ponto de orvalho no display
  lcd.setCursor(0,0);
  lcd.print("Ponto de Orvalho");
  lcd.setCursor(5,1);
  lcd.print(dp,1);
  lcd.write(byte(0));
  lcd.print("C");

  delay(3000);

  //------------------------------------------------------------------------
                              
}
