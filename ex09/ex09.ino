#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Very";
const char* password = "Mmsx627426946";

WebServer server(80);

#define TOUCH_PIN 4
#define LED_PIN 2

int threshold = 20;
int touchValue;

String getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>触摸传感器</title>
<style>
body { font-family: Arial; text-align: center; margin-top: 80px; background: #1a1a2e; color: white; }
#num { font-size: 120px; font-weight: bold; margin: 40px 0; color: #00d4ff; }
#st { font-size: 20px; color: #888; }
</style>
</head>
<body>
<h1>ESP32 触摸传感器</h1>
<div id="num">--</div>
<div id="st">等待数据...</div>
<script>
function getVal() {
  var x = new XMLHttpRequest();
  x.open("GET","/touch",true);
  x.onload=function(){
    if(x.status==200){
      var d=JSON.parse(x.responseText);
      document.getElementById("num").textContent=d.value;
      document.getElementById("num").style.color=d.touched?"#ff6b35":"#00d4ff";
      document.getElementById("st").textContent=d.touched?"触摸中":"未触摸";
    }
  };
  x.send();
}
setInterval(getVal,100);
</script>
</body>
</html>
)rawliteral";
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", getHTML());
}

void handleTouch() {
  String json = "{\"value\":" + String(touchValue) + ",\"touched\":" + (touchValue<threshold?"true":"false") + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/touch", handleTouch);
  server.begin();
}

void loop() {
  touchValue = touchRead(TOUCH_PIN);
  if (touchValue < threshold) { digitalWrite(LED_PIN, HIGH); }
  else { digitalWrite(LED_PIN, LOW); }
  server.handleClient();
  delay(100);
}
