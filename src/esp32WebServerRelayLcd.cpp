/*********
  Servidor Web con ESP32:
  Incluye un display lcd H44780 conectado via bus I2C.
  El display muestra la dirección IP obtenida via DHCP
  junto con elestado de los dos relés (ON-OFF)
  El estado de los relé puedenmodificarse desde la paǵina web embebida en el microcontrolador.
  Este sketch se basa en el ejemplo original Web Server de RUi Santos, fue
  modificado por mí en Ramos Mejía, ARGENTINA, en Agosto de 20202.
  - Osvaldo Cantone  correo@cantone.com.ar
  PINS:
  relés:
    Relé1 GPIO.15
    Relé2 GPIO.2
  display:
    SDA GPIO.21
    SCL GPIO.22
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Instanciamos un ojeto 'lcd' y configuramos el display
// en la dirección 0x27 del bus I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//Aquí declaramos dos variables que contienen el SSID de la WIFi a la que 
//se conectará el ESP32 y la password. Para este ejemplo usamos
//los valores "miwifi" como SSID y "123456" como password. 
//Reemplazar por los valores correspondientes.

const char* ssid = "La de abajo";
const char* password = "06012405ep";

// Configuramos el puerto TCP del web server en 80 (HTTP)
WiFiServer server(80);

// Contendrá la peticón HTTP
String header;

// Estado de las salidas (on - off)
String relay1State = "off";
String relay2State = "off";

//GPIO pins de los relay
const int Relay1 = 15;
const int Relay2 = 2;

//Tiepo actual
unsigned long currentTime = millis();
//Tiempo anterior
unsigned long previousTime = 0; 
//Define timeout en millisegundos.
const long timeoutTime = 2000;
int show = -1;

void setup() {

  int error;

  Serial.begin(9600);
  // Inicializa los GPIO como salidas y asigna el estado bajo.
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  
  //Config. inicial del display
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD encontrado.");
    show = 0;
//    lcd.begin(16, 2); // inicializa el display lcd 
 } else {
    Serial.println(": LCD no encontrado.");
  } // if
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
//  lcd.print("0123456789ABCDEF");
    lcd.print("Servidor Esp32  ");
    

  // Conecta a la Wi-Fi.
  Serial.print("conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Muestra por Serial la IP obtenida 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
delay(2000);
    lcd.setCursor(0, 0);
    lcd.print("ESP32 ");
    lcd.print(WiFi.localIP());
    lcd.setCursor(0, 1);
    lcd.print("R1: ON   R2: ON ");


  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Espera la petición de un cliente.

  if (client) {                             // Si se conecta un cliente...
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // Muestra un mensaje por el puerto Serial.
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              relay1State = "on";
              digitalWrite(Relay1, HIGH);
                lcd.setCursor(4, 1);
                lcd.print("OFF");

            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              relay1State = "off";
              digitalWrite(Relay1, LOW);
                lcd.setCursor(4, 1);
                lcd.print("ON ");

            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              relay2State = "on";
              digitalWrite(Relay2, HIGH);
                lcd.setCursor(13, 1);
                lcd.print("OFF");
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              relay2State = "off";
              digitalWrite(Relay2, LOW);
                lcd.setCursor(13, 1);
                lcd.print("ON ");
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            


            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>Adaptado por Osvaldo Cantone <a href=https://tecteach.net.ar/>tecteach.net.ar</a> </p>");
            client.println("<p>Descargar el código desde GitHub <a href=https://github.com/ocantone/en-ESP32-web-server-on-off-control-lcd-display-driving> here</a> </p>");
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Relay 1 (GPIO 15) - Estado: " + relay1State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (relay1State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
            
            
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>Relay 2 (GPIO 2) - Estado: " + relay2State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (relay2State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Limpia la variable 'header'.
    header = "";
    // Cierre de la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
