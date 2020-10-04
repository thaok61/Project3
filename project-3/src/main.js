import Vue from 'vue'
import App from './App.vue'
import VueMqtt from 'vue-mqtt'
Vue.config.productionTip = false
Vue.use(VueMqtt, 'ws://192.168.43.179:9001', 
{
  clientId: 'WebClient-' + parseInt(Math.random() * 100000), 
  username: 'thao', 
  password: '1'
})
new Vue({
  render: h => h(App),
}).$mount('#app')
