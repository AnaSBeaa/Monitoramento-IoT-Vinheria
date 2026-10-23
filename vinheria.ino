
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>

// Sensores
#define DHT_PIN 15
#define DHT_TYPE DHT22
#define LDR_PIN 34

DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi do Wokwi
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// Servidor MQTT
const char* MQTT_SERVER = "3.135.234.101";
const int MQTT_PORT = 1883;

const char* MQTT_CLIENT_ID = "ESP32-device042";
const char* MQTT_TOPIC = "/TEF/device042/attrs";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void configurarHorario() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Sincronizando horario...");
}

bool conectarMQTT() {
  if (mqttClient.connected()) {
    return true;
  }

  Serial.print("Conectando ao MQTT...");

  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println(" conectado!");
    return true;
  }

  Serial.print(" falha. Codigo: ");
  Serial.println(mqttClient.state());

  return false;
}

String obterTimestamp() {
  struct tm horario;

  if (getLocalTime(&horario, 1000)) {
    char timestamp[25];

    strftime(
      timestamp,
      sizeof(timestamp),
      "%Y-%m-%dT%H:%M:%SZ",
      &horario
    );

    return String(timestamp);
  }

  return "";
}

void setup() {
  Serial.begin(115200);

  dht.begin();
  analogReadResolution(12);

  Serial.println();
  Serial.println("Vinheria Agnello - Monitoramento IoT");

  conectarWiFi();
  configurarHorario();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setBufferSize(512);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!mqttClient.connected()) {
    if (!conectarMQTT()) {
      delay(3000);
      return;
    }
  }

  mqttClient.loop();

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int luminosidade = analogRead(LDR_PIN);

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler o DHT22!");
    delay(2000);
    return;
  }

  String timestamp = obterTimestamp();

  if (timestamp.length() == 0) {
    Serial.println("Horario ainda nao sincronizado.");
    delay(2000);
    return;
  }

  int luminosidadePct = map(luminosidade, 0, 4095, 0, 100);

  // Mensagem MQTT no formato esperado pelo IoT Agent
  String mensagem = "t|" + String(temperatura, 1) +
                    "|h|" + String(umidade, 1) +
                    "|l|" + String(luminosidadePct) +
                    "|ts|" + timestamp;

  // Exibição organizada no monitor serial
  Serial.println("\nDados coletados:");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(umidade, 1);
  Serial.println(" %");

  Serial.print("Luminosidade: ");
  Serial.print(luminosidadePct);
  Serial.println(" %");

  Serial.print("Horario: ");
  Serial.println(timestamp);

  if (mqttClient.publish(MQTT_TOPIC, mensagem.c_str())) {
    Serial.println("Mensagem MQTT enviada!");
    Serial.print("Topico: ");
    Serial.println(MQTT_TOPIC);
  } else {
    Serial.println("Falha ao publicar a mensagem MQTT.");
  }

  Serial.println("----------------------------");

  delay(5000);
}
