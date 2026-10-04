#ifndef WEB_JS_H
#define WEB_JS_H

const char WEB_JS[] PROGMEM = R"rawliteral(
async function LoudNoise() {
  const res = await fetch('/LoudNoise');
  const text = await res.text();
  logMessage(text);
}

let TemperatureState = false;
let VibrationState = false;
let AirState = false;
let SoundState = false;

async function ToggleTemperature() {
  TemperatureState = !TemperatureState;

  const res = await fetch(TemperatureState ? `/TemperatureStateOn` : `/TemperatureStateOff`);
  const text = await res.text();

  logMessage(text);

  UpdateTemperature(TemperatureState);
}

function UpdateTemperature(isOn) {
  const btn = document.getElementById("TemperatureButton");
  btn.textContent = isOn ? "ON" : "OFF";
  btn.className = isOn ? "TemperatureButtonOn" : "TemperatureButtonOff";
}

async function ToggleVibration() {
  VibrationState = !VibrationState;

  const res = await fetch(VibrationState ? `/VibrationStateOn` : `/VibrationStateOff`);
  const text = await res.text();

  logMessage(text);

  UpdateVibration(VibrationState);
}

function UpdateVibration(isOn) {
  const btn = document.getElementById("VibrationButton");
  btn.textContent = isOn ? "ON" : "OFF";
  btn.className = isOn ? "VibrationButtonOn" : "VibrationButtonOff";
}

async function ToggleAir() {
  AirState = !AirState;

  const res = await fetch(AirState ? `/AirStateOn` : `/AirStateOff`);
  const text = await res.text();

  logMessage(text);

  UpdateAir(AirState);
}

function UpdateAir(isOn) {
  const btn = document.getElementById("AirButton");
  btn.textContent = isOn ? "ON" : "OFF";
  btn.className = isOn ? "AirButtonOn" : "AirButtonOff";
}

async function ToggleSound() {
  SoundState = !SoundState;

  const res = await fetch(SoundState ? `/SoundStateOn` : `/SoundStateOff`);
  const text = await res.text();

  logMessage(text);

  UpdateSound(SoundState);
}

function UpdateSound(isOn) {
  const btn = document.getElementById("SoundButton");
  btn.textContent = isOn ? "ON" : "OFF";
  btn.className = isOn ? "SoundButtonOn" : "SoundButtonOff";
}

function logMessage(message) {
  const logDiv = document.getElementById("log");
  const now = new Date();
  const date = now.toLocaleDateString('en-GB').split('/').join('-');
  const time = now.toLocaleTimeString('en-GB', { hour: '2-digit', minute: '2-digit' });
  const entry = document.createElement("div");
  entry.textContent = `[${date}] (${time}) ESP32: ${message}`;
  entry.className = "LogLine";
  logDiv.appendChild(entry);
  logDiv.scrollTop = logDiv.scrollHeight;
}

$(document).ready(function () {
  const Temperature_Chart_Ctx = document.getElementById('Temperature_Chart').getContext('2d');

  function createChart(ctx, label) {
    return new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: label,
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
            min: -20,
            max: 50,
            ticks: {
    stepSize: 0.5
  },
            grid: { color: 'rgba(255, 255, 255, 0.1)' }
          },
          x: {
            min: 0,
            max: 50,
grid : {
color:
  'rgba(255, 255, 255, 0.1)'
}
          }
        }
      }
    });
  }

  const TemperatureChart = createChart(Temperature_Chart_Ctx, 'Temperature');
  let TemperatureCounter = 0;

    async function FetchTemperature() {
    if (!TemperatureState) return;
    try {
      const response = await fetch('/Temperature_Data');
      const value = await response.text();

      TemperatureChart.data.labels.push(TemperatureCounter);
      TemperatureChart.data.datasets[0].data.push(parseFloat(value));
      TemperatureCounter++;

      if (TemperatureCounter > 50) {
        TemperatureChart.options.scales.x.min = TemperatureCounter - 50;
        TemperatureChart.options.scales.x.max = TemperatureCounter;
      }

      TemperatureChart.update();
    } catch (err) {

    }
  }

  setInterval(FetchTemperature, 200);

});

$(document).ready(function () {

  const Vibration_Chart_Ctx = document.getElementById('Vibration_Chart').getContext('2d');
  const Air_Chart_Ctx = document.getElementById('Air_Chart').getContext('2d');
  const Sound_Chart_Ctx = document.getElementById('Sound_Chart').getContext('2d');

  function createChart(ctx, label) {
    return new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: label,
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
            grid: { color: 'rgba(255, 255, 255, 0.1)' }
          },
          x: {
            min: 0,
            max: 50,
            grid: { color: 'rgba(255, 255, 255, 0.1)' }
          }
        }
      }
    });
  }


  const VibrationChart = createChart(Vibration_Chart_Ctx, 'Vibration');
  const AirChart = createChart(Air_Chart_Ctx, 'Air');
  const SoundChart = createChart(Sound_Chart_Ctx, 'Sound');

  let VibrationCounter = 0;
  let AirCounter = 0;
  let SoundCounter = 0;

  async function FetchVibration() {
    if (!VibrationState) return;
    try {
      const response = await fetch('/Vibration_Data');
      const value = await response.text();

      VibrationChart.data.labels.push(VibrationCounter);
      VibrationChart.data.datasets[0].data.push(parseFloat(value));
      VibrationCounter++;

      if (VibrationCounter > 50) {
        VibrationChart.options.scales.x.min = VibrationCounter - 50;
        VibrationChart.options.scales.x.max = VibrationCounter;
      }

      VibrationChart.update();
    } catch (err) {

    }
  }

  async function FetchAir() {
    if (!AirState) return;
    try {
      const response = await fetch('/Air_Data');
      const value = await response.text();

      AirChart.data.labels.push(AirCounter);
      AirChart.data.datasets[0].data.push(parseFloat(value));
      AirCounter++;

      if (AirCounter > 50) {
        AirChart.options.scales.x.min = AirCounter - 50;
        AirChart.options.scales.x.max = AirCounter;
      }

      AirChart.update();
    } catch (err) {

    }
  }

  async function FetchSound() {
    if (!SoundState) return;
    try {
      const response = await fetch('/Sound_Data');
      const value = await response.text();

      SoundChart.data.labels.push(SoundCounter);
      SoundChart.data.datasets[0].data.push(parseFloat(value));
      SoundCounter++;

      if (SoundCounter > 50) {
        SoundChart.options.scales.x.min = SoundCounter - 50;
        SoundChart.options.scales.x.max = SoundCounter;
      }

      SoundChart.update();
    } catch (err) {

    }
  }

  setInterval(FetchVibration, 200);
  setInterval(FetchAir, 200);
  setInterval(FetchSound, 20);
});

function DeleteLogLine() {
  $('.LogLine').remove();
}

function DownloadLogLine() {
  var a = document.body.appendChild(
    document.createElement("a")
  );
  a.download = "LogLine.html";
  a.href = "data:text/html," + encodeURIComponent(document.getElementById("log").innerHTML);
  a.click();
}
)rawliteral";

#endif
