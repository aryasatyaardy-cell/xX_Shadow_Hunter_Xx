#include "Web.h"

void InitateWifi() {

  const char *wifiName = "Futuristik";
  const char *password = "H4f1dzGen";

  WiFi.begin(wifiName, password);

  led.setBrightness(20);
  led.setPixelColor(0, led.Color(0, 255, 255));
  led.show();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  led.clear();
  led.show();
}

void Web() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    
    <title>ESP32 Graph</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <meta charset="UTF-8">
    <style>
        .SoundGraph {
            width: 500px;
            height: 400px;
            margin: 0 auto;
            background-color: #111111;
        }

        .SoundGraph h1 {
            text-align: center;
        }

        .SoundGraph canvas {
            text-align: center;
            padding-bottom: 100px;
            width: 1000px;
        }

        .INMP_Button {
            text-align: center;
            margin-top: auto;
            background-color: #ff9d9d;
        }

        .btn {
            padding: 12px 24px;
            font-size: 16px;
            margin: 5px;
            cursor: pointer;
            border: none;
            border-radius: 5px;
            color: white;
        }

        .btn.off {
            background-color: #e74c3c;
        }

        .btn.on {
            background-color: #2ecc71;
        }

        .log {
            display: flex;
            flex-direction: column;
            justify-content: center;
            background-color: #f0f0f0;
            border: 1px solid #ccc;
            border-radius: 5px;
            width: 420px;
            padding-left: 10px;
            padding-right: 10px;
            margin: 20px auto;
        }

        #log {
            width: 400px;
            height: 260px;
            overflow-y: auto;
            overflow-x: auto;
            margin: 0 auto;
            padding: 10px;
            background: #111;
            color: #0f0;
            font-family: monospace;
            font-size: 13px;
            text-align: left;
        }

        #log h1 {
            font-size: 15px;
            text-align: center;
        }

        #log div {
            margin-bottom: 8px;
        }
    </style>
</head>

<body>
    <div class="SoundGraph">
        <h1>Sound Detector</h1>
        <canvas id="I2S_Chart" width="500" height="400"></canvas>
    </div>

    <div class="INMP_Button">
        <h1>INMP441 Microphone</h1>
        <button id="micBtn" class="btn off" onclick="ToggleMic()">OFF</button>
    </div>

    <div class="log">
        <h1>Spider's Log History</h1>
        <div id="log"></div>
    </div>


    <script>

        let micState = false;

        async function ToggleMic() {
            micState = !micState;

            const res = await fetch(micState ? `/StateOn` : `/StateOff`);
            const text = await res.text();
            logMessage(text);

            updateButton(micState);
        }

        async function LoudNoise() {

            const res = await fetch(/LoudNoise);
            const text = await res.text();
            logMessage(text);

        }

        function updateButton(isOn) {
            const btn = document.getElementById("micBtn");
            btn.textContent = isOn ? "ON" : "OFF";
            btn.className = "btn " + (isOn ? "on" : "off");
        }

        const I2S_Graph = document.getElementById('I2S_Chart').getContext('2d');

        const chart = new Chart(I2S_Graph, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Sound',
                    data: [],
                    borderWidth: 2,
                    borderColor: 'white',
                    fill: false,
                    pointRadius: 0
                }]
            },
            options: {
                animation: false,
                scales: {
                    y: {
                        min: -200,
                        max: 200,
                        grid: {
                            color: 'rgba(255, 255, 255, 0.1)'  // faint grid lines
                        }
                    },
                    x: {
                        grid: {
                            color: 'rgba(255, 255, 255, 0.1)'
                        }
                    }
                }
            }
        });

        let I2S_Counter = 0;
        let zeroCount = 0;

        async function fetchData() {
            if (!micState) {
                if (zeroCount < 50) {
                    const value = 0;
                    chart.data.labels.push(I2S_Counter++);
                    chart.data.datasets[0].data.push(parseFloat(value));
                    if (chart.data.labels.length > 50) {
                        chart.data.labels.shift();
                        chart.data.datasets[0].data.shift();
                    }
                    chart.update();
                    zeroCount++;
                }
                return;
            }

            zeroCount = 0;  // reset so next time it's turned off, it fills again

            const response = await fetch('/I2S_Data');
            const value = await response.text();

            chart.data.labels.push(I2S_Counter++);
            chart.data.datasets[0].data.push(parseFloat(value));
            if (chart.data.labels.length > 50) {
                chart.data.labels.shift();
                chart.data.datasets[0].data.shift();
            }
            chart.update();
        }

        function logMessage(message) {
            const logDiv = document.getElementById("log");
            const now = new Date();
            const date = now.toLocaleDateString('en-GB').split('/').join('-');
            const time = now.toLocaleTimeString('en-GB', { hour: '2-digit', minute: '2-digit' });
            const entry = document.createElement("div");
            entry.textContent = `[${date}] (${time}) ESP32: ${message}`;
            logDiv.appendChild(entry);
            logDiv.scrollTop = logDiv.scrollHeight;
        }

        setInterval(fetchData, 50);
    </script>
</body>

</html>
)rawliteral";

    request->send(200, "text/html", html);
  });
}

void INMP441_Data() {
  server.on("/I2S_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    int value = Mic_Data();
    request->send(200, "text/plain", String(value));
  });
}