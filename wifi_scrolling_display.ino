#include <ESP8266WiFi.h>  // including the required libraries and header files!
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Your SSID";  // Wifi Credentials
const char* pass = "Password";

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Connect I2C Display SCL & SDA with D1 and D2

ESP8266WebServer server(80);  // assigning the port for the server (port 80 for http)

String message;

const int LAN_status_LED = LED_BUILTIN;
void show_Data();
void scroll_Text();
void setup() {
  WiFi.begin(ssid, pass);
  WiFi.hostname("WiFi Scrolling Display");

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting......");
  pinMode(LAN_status_LED, OUTPUT);
  digitalWrite(LAN_status_LED, 1);

  //Checking for WiFi connection at boot!
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LAN_status_LED, 0);
    delay(500);
    digitalWrite(LAN_status_LED, 1);
    delay(500);
    delay(2000);
  }
  server.begin();
  lcd.clear();
  lcd.print("Connected, IP:-");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());  // view IP in I2C Display
  delay(5000);
  lcd.clear();
  server.on("/", HTTP_GET, get_data);
  server.on("/ReadData", HTTP_POST, show_Data);
}

void get_data() {
  const char* WebPage = R"(
    <!DOCTYPE html>
    <html lang="en">

    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>WiFi Scrolling Display By @ElectroVilla</title>
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet"
            integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
        <style>
            body {
                background-color: lightgray;
            }

            header {
                background-color: #6f42c1;
                color: #fff;
            }

            footer {
                background-color: #6f42c1;
                color: #fff;
                text-align: center;
                height: 10vh;
                margin-top: 100vh;
            }

            .main {
                margin: 100px 5vw;
                text-align: center;
            }

            input[type=text] {
                padding: 0.5rem;
                border: 1px solid blueviolet;
                border-radius: 10px;
                margin-right: 1rem;
                width: 50vw;
            }

            input[type="button"] {
                background-color: #6f42c1;
                color: #fff;
                border-radius: 10px;
                padding: 0.5rem 1rem;
                border-radius: 5px;
                cursor: pointer;
            }

            footer>a {
                text-decoration: none;
                color: #87cefa;
            }

            span {
                display: flex;
            }
        </style>
    </head>

    <body>
        <header class="navbar navbar-expand-lg bd-navbar sticky-top">
            <nav class=" container-xxl bd-gutter flex-wrap flex-lg-nowrap" aria-label="Main navigation">
                <div class="container">
                    <h1 class="h1 text-center">
                        WiFi Scrolling Display
                    </h1>
                </div>
            </nav>
        </header>
        <main>
            <div class="main">
                <input type="text" id="msg_box" placeholder="Enter Your Message..." required>

                <input type="button" id="send_btn" value="Send">

            </div>
        </main>
        <footer>
            &copy; Made by ElectroVilla
            <a href="https://www.youtube.com/@ElectroVilla" target="_blank">Visit YouTube Channel</a>
        </footer>
        <script>
            send_btn.addEventListener("click", () => {
                let msg = document.getElementById("msg_box");
                let data = new XMLHttpRequest;
                data.open('POST', '/ReadData', true);
                data.send(msg.value);
            })
        </script>
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"
            integrity="sha384-C6RzsynM9kWDrMNeT87bh95OGNyZPhcTNXj1NW7RuBCsyN/o0jlpcV8Qyq46cDfL"
            crossorigin="anonymous"></script>
    </body>

    </html>
  )";

  server.send(200, "text/html", WebPage);
}

void show_Data() {
  message = server.arg("plain");
  lcd.clear();
  for (int i = 0; i <= 16 + message.length(); i++) {
    lcd.setCursor(0, 0);

    String displayMessage = message.substring(i, i + 16);
    lcd.print(displayMessage);
    delay(350);
    lcd.clear();
  }
}

void loop() {
  server.handleClient();
  show_Data();
}
