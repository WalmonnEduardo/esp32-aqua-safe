/***************************************************
* Techeonics
* Turbidity module mjkdz
* @author - Gaurav Kumar (Techeonics)
<thetecheonics@gmail.com>
Youtube- https://www.youtube.com/c/THETECHEONICS
See <http://www.techeonics.com> for details.
****************************************************/
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

const char* ssid = "Gabriel Galaxy A54";
const char* password = "Fwwc8888";
const int id = 123456;
int temp;
const String serverUrl = "http://192.168.157.55/aqua-safe/logica.php";

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ajuste as dimensões do LCD aqui

int sensorPin = A0; // A0 FOR ARDUINO/ 36 FOR ESP

void setup() { 
  Serial.begin(115200);
  sensors.begin();

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();

  // Conexão Wi-Fi
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi");
}

void loop() {
  int turbidity = analogRead(sensorPin); // Leitura do sensor
  delay(100);

  sensors.requestTemperatures(); 
  Serial.println("Temp: ");
  lcd.setCursor(9, 0);
  lcd.print("Temp:");

  // Atualiza o LCD
  lcd.setCursor(0, 0);
  lcd.print("Tbz:");
  lcd.print("   ");
  lcd.setCursor(4, 0);
  lcd.print(turbidity);
  
  if (turbidity > 3999) {
    lcd.setCursor(0, 1);
    lcd.print(" Agua Limpa ");
    Serial.println("Agua Limpa");
  } else {
    lcd.setCursor(0, 1);
    lcd.print(" Agua Impura! ");
    Serial.println("Agua Impura!");
  }

   // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  int temp = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(temp != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(temp);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

  lcd.setCursor(14, 0);
  lcd.print(temp);
  
  // Envio HTTP
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; // Declaração do objeto HTTP
    String url = serverUrl + "?id=" + id + "&turb=" + turbidity + "&temp=" + temp;

    Serial.println("URL: " + url);
    http.begin(url); // Inicia a conexão
    int httpResponseCode = http.GET(); // Faz a solicitação GET

    // Verifica a resposta
    if (httpResponseCode > 0) {
      Serial.println("Resposta do servidor: " + http.getString());
    } else {
      Serial.println("Erro na solicitação HTTP");
    }

    http.end(); // Finaliza a conexão
  }

  delay(2000); // Aguarde um pouco antes do próximo ciclo
}
