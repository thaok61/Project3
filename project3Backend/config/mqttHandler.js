const mqtt = require('mqtt');
const crypto = require('crypto-js');
const key = '2B7E151628AED2A6ABF7158809CF4F3C' // change to your key
const base64_iv = 'AAAAAAAAAAAAAAAAAAAAAA=='
const plain_iv = new Buffer(base64_iv, 'base64').toString('hex');

const Climate = require('../models/Climate')

// Decrypt a mqtt message
const aesDencrypt= (message) => {
  const cipher = crypto.AES.decrypt(message, crypto.enc.Hex.parse(key), {
    iv: crypto.enc.Hex.parse(plain_iv),
    mode: crypto.mode.CBC,
    padding: crypto.pad.ZeroPadding
  })
  var plaintext = cipher.toString(crypto.enc.Base64);
  var decoded_b64msg =  new Buffer(plaintext, 'base64').toString('ascii');
  var decoded_msg =     new Buffer( decoded_b64msg , 'base64').toString('ascii')
  return decoded_msg
}

const utf8ArrayToString = (aBytes) => {
  var sView = "";

  for (var nPart, nLen = aBytes.length, nIdx = 0; nIdx < nLen; nIdx++) {
    nPart = aBytes[nIdx];
  
    sView += String.fromCharCode(
      nPart > 251 && nPart < 254 && nIdx + 5 < nLen ? /* six bytes */
          /* (nPart - 252 << 30) may be not so safe in ECMAScript! So...: */
          (nPart - 252) * 1073741824 + (aBytes[++nIdx] - 128 << 24) + (aBytes[++nIdx] - 128 << 18) + (aBytes[++nIdx] - 128 << 12) + (aBytes[++nIdx] - 128 << 6) + aBytes[++nIdx] - 128
      : nPart > 247 && nPart < 252 && nIdx + 4 < nLen ? /* five bytes */
          (nPart - 248 << 24) + (aBytes[++nIdx] - 128 << 18) + (aBytes[++nIdx] - 128 << 12) + (aBytes[++nIdx] - 128 << 6) + aBytes[++nIdx] - 128
      : nPart > 239 && nPart < 248 && nIdx + 3 < nLen ? /* four bytes */
          (nPart - 240 << 18) + (aBytes[++nIdx] - 128 << 12) + (aBytes[++nIdx] - 128 << 6) + aBytes[++nIdx] - 128
      : nPart > 223 && nPart < 240 && nIdx + 2 < nLen ? /* three bytes */
          (nPart - 224 << 12) + (aBytes[++nIdx] - 128 << 6) + aBytes[++nIdx] - 128
      : nPart > 191 && nPart < 224 && nIdx + 1 < nLen ? /* two bytes */
          (nPart - 192 << 6) + aBytes[++nIdx] - 128
      : /* nPart < 127 ? */ /* one byte */
          nPart
    );
  }

  return sView;
}

class MqttHandler {
  constructor() {
    this.mqttClient = null;
    this.host = 'mqtt://localhost';
    this.username = 'thao'; // mqtt credentials if these are needed to connect
    this.password = '1';
  }
  
  connect() {
    // Connect mqtt with credentials (in case of needed, otherwise we can omit 2nd param)
    this.mqttClient = mqtt.connect(this.host, { username: this.username, password: this.password });

    // Mqtt error calback
    this.mqttClient.on('error', (err) => {
      console.log(err);
      this.mqttClient.end();
    });

    // Connection callback
    this.mqttClient.on('connect', () => {
      console.log(`mqtt client connected`);
    });

    // mqtt subscriptions
    this.mqttClient.subscribe('esp8266/+', {qos: 0});

    // When a message arrives, console.log it
    this.mqttClient.on('message', function (topic, message) {
      var climate = new Climate({
        site: "BK-HN",
        type: "",
        value: ""
      })
      if (topic.toString() == 'esp8266/temperature') {
        var stringData = utf8ArrayToString(message)
        var data = aesDencrypt(stringData)
        console.log("Data Encrypt: " + data);
        climate.type = "temperature";
        climate.value = data.toString();
        climate.save();
      }

      if (topic.toString() == 'esp8266/humidity') {
        var stringData = utf8ArrayToString(message)
        var data = aesDencrypt(stringData)
        console.log("Data Encrypt: " + data);
        climate.type = "humidity";
        climate.value = data.toString();
        climate.save();
      }

      console.log(topic.toString());
      console.log(message.toString());
    });

    this.mqttClient.on('close', () => {
      console.log(`mqtt client disconnected`);
    });
  }

  // Sends a mqtt message to topic: mytopic
  sendMessage(message) {
    this.mqttClient.publish('mytopic', message);
  }
  
}

module.exports = MqttHandler;