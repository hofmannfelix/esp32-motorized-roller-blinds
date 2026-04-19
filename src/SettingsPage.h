#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

const char SETTINGS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Blind Settings</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 500px; margin: 0 auto; padding: 20px; background: #1a1a2e; color: #e0e0e0; }
        h1 { color: #4cc9f0; text-align: center; }
        h2 { color: #7b2ff7; border-bottom: 1px solid #333; padding-bottom: 8px; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; font-weight: bold; color: #b0b0b0; }
        input[type="text"], input[type="number"], input[type="password"] {
            width: 100%; padding: 10px; border: 1px solid #444; border-radius: 4px;
            background: #16213e; color: #e0e0e0; box-sizing: border-box;
        }
        input:focus { border-color: #4cc9f0; outline: none; }
        button {
            width: 100%; padding: 12px; background: #7b2ff7; color: white;
            border: none; border-radius: 4px; font-size: 16px; cursor: pointer; margin-top: 10px;
        }
        button:hover { background: #9d4edd; }
        .nav { text-align: center; margin-bottom: 20px; }
        .nav a { color: #4cc9f0; text-decoration: none; margin: 0 10px; }
        .nav a:hover { text-decoration: underline; }
        .status { text-align: center; padding: 10px; border-radius: 4px; margin-top: 15px; display: none; }
        .success { background: #2d6a4f; color: #b7e4c7; }
        .error { background: #6a2d2d; color: #e4b7b7; }
        .separator { border: none; border-top: 1px solid #333; margin: 20px 0; }
    </style>
</head>
<body>
    <div class="nav">
        <a href="/">Home</a>
        <a href="/settings">Settings</a>
        <a href="/about">About</a>
    </div>

    <h1>⚙️ Settings</h1>

    <form id="settingsForm">
        <h2>MQTT Configuration</h2>
        <div class="form-group">
            <label for="mqttServer">MQTT Server IP</label>
            <input type="text" id="mqttServer" name="mqttServer" placeholder="e.g. 192.168.1.100">
        </div>
        <div class="form-group">
            <label for="mqttPort">MQTT Port</label>
            <input type="number" id="mqttPort" name="mqttPort" value="1883" placeholder="1883">
        </div>
        <div class="form-group">
            <label for="mqttUser">MQTT Username (optional)</label>
            <input type="text" id="mqttUser" name="mqttUser" placeholder="Leave empty for anonymous">
        </div>
        <div class="form-group">
            <label for="mqttPwd">MQTT Password (optional)</label>
            <input type="password" id="mqttPwd" name="mqttPwd" placeholder="Leave empty for anonymous">
        </div>

        <hr class="separator">

        <h2>Device Settings</h2>
        <div class="form-group">
            <label for="deviceName">Device Name (hostname)</label>
            <input type="text" id="deviceName" name="deviceName" placeholder="e.g. blind-east">
        </div>
        <div class="form-group">
            <label for="steppersRpm">Stepper Speed (RPM)</label>
            <input type="number" id="steppersRpm" name="steppersRpm" value="30" min="1" max="100">
        </div>

        <button type="submit">💾 Save &amp; Restart</button>
    </form>

    <div id="status" class="status"></div>

    <script>
        // Load current settings
        fetch('/settings/json')
            .then(r => r.json())
            .then(data => {
                document.getElementById('mqttServer').value = data.mqttServer || '';
                document.getElementById('mqttPort').value = data.mqttPort || 1883;
                document.getElementById('mqttUser').value = data.mqttUser || '';
                document.getElementById('mqttPwd').value = data.mqttPwd || '';
                document.getElementById('deviceName').value = data.deviceName || '';
                document.getElementById('steppersRpm').value = data.steppersRpm || 30;
            })
            .catch(err => console.log('Could not load settings:', err));

        document.getElementById('settingsForm').addEventListener('submit', function(e) {
            e.preventDefault();
            var statusEl = document.getElementById('status');
            statusEl.style.display = 'block';
            statusEl.className = 'status';
            statusEl.textContent = 'Saving...';

            var formData = new FormData(this);
            var params = new URLSearchParams();
            for (var pair of formData.entries()) {
                params.append(pair[0], pair[1]);
            }

            fetch('/settings/save', {
                method: 'POST',
                body: params.toString(),
                headers: { 'Content-Type': 'application/x-www-form-urlencoded' }
            })
            .then(r => r.json())
            .then(data => {
                if (data.success) {
                    statusEl.classList.add('success');
                    statusEl.textContent = '✅ Settings saved! Device will restart in 3 seconds...';
                } else {
                    statusEl.classList.add('error');
                    statusEl.textContent = '❌ Error: ' + (data.error || 'Unknown error');
                }
            })
            .catch(err => {
                statusEl.classList.add('error');
                statusEl.textContent = '❌ Connection lost - device is likely restarting';
            });
        });
    </script>
</body>
</html>
)rawliteral";

#endif