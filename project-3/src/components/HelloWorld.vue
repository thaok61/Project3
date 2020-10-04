<template>
  <div>
  Nhiet do la:
    {{temp}}
    <br>
  Do am la: 
    {{humidity}}  
  </div>
</template>

<script>
  export default {
    name: 'HelloWorld',
    mounted () {
      this.$mqtt.subscribe("esp8266/+")
    },
    data () {
      return {
        temp:'',
        humidity: ''
      }
    },
    mqtt: {
      "esp8266/temperature" (data) {
        this.temp = data
      },
      "esp8266/humidity" (data) {
        this.humidity = data
      }
    },
    methods: {
      login() {
        if (this.userName == this.$store.state.userName
        && this.password == this.$store.state.password) {
          this.$store.state.was_login = true
        }
      }
    },
    computed: {

    }
  }
</script>
