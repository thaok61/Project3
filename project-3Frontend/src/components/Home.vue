<template>
   <v-app id="inspire">
    <v-card
      class="mx-auto"
      max-width="900"
    >
      <v-list-item two-line>
        <v-list-item-content>
          <v-list-item-title class="headline">
            Viet Nam
          </v-list-item-title>
        </v-list-item-content>
      </v-list-item>
  
      <v-card-text>
        <v-row align="center">
          <v-col
            class="display-3"
            cols="8"
          >
            Nhiet Do: {{temp}}&deg;C
          </v-col>
          <v-col
            class="display-3"
            cols="8"
          >
            Do Am: {{humidity}}%
          </v-col>
        </v-row>
      </v-card-text>
  
      <v-divider></v-divider>
    </v-card>
    <ChartClimate></ChartClimate>
  </v-app>
</template>

<script>
  import ChartClimate from "./ChartClimate"
  import CryptoJS from 'crypto-js'
  const key = '2B7E151628AED2A6ABF7158809CF4F3C' // change to your key
  const base64_iv = 'AAAAAAAAAAAAAAAAAAAAAA=='
  const plain_iv = new Buffer(base64_iv, 'base64').toString('hex');
  export default {
    name: 'Home',

    components: {
      ChartClimate
    },

    mounted () {
      this.$mqtt.subscribe("esp8266/+");
      this.$store.dispatch('getHumidities')
      this.$store.dispatch('getTemperatures')
    },
    data () {
      return {
        temp: this.aesDencrypt('KsqzbRL7DAaGuhH+GhJaNQ=='),
        humidity: 0,
        
        }
    },
    mqtt: {
      "esp8266/temperature" (data) {
        
        // var stringData = new TextDecoder("utf-8").decode(data);
        // var stringData = Buffer.from(data).toString('utf-8')
        var stringData = this.utf8ArrayToString(data)
        console.log(stringData);
        this.temp = this.aesDencrypt(stringData)
      },
      "esp8266/humidity" (data) {
        // var stringData = new TextDecoder("utf-8").decode(data);
        var stringData = this.utf8ArrayToString(data)
        this.humidity = this.aesDencrypt(stringData)
      }
    },
    methods: {
      aesEncrypt(txt) {
        const cipher = CryptoJS.AES.encrypt(txt, CryptoJS.enc.Hex.parse(key), {
          iv: CryptoJS.enc.Hex.parse(plain_iv),
          mode: CryptoJS.mode.CBC,
          padding: CryptoJS.pad.ZeroPadding
        })
        return cipher
      },
      aesDencrypt(txt) {
        const cipher = CryptoJS.AES.decrypt(txt, CryptoJS.enc.Hex.parse(key), {
          iv: CryptoJS.enc.Hex.parse(plain_iv),
          mode: CryptoJS.mode.CBC,
          padding: CryptoJS.pad.ZeroPadding
        })
        var plaintext = cipher.toString(CryptoJS.enc.Base64);
        var decoded_b64msg =  new Buffer(plaintext, 'base64').toString('ascii');
        var decoded_msg =     new Buffer( decoded_b64msg , 'base64').toString('ascii')
        return decoded_msg
        // return cipher
      },
      utf8ArrayToString(aBytes) {
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
    },
    computed: {
    }
  }
</script>