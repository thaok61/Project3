import Vue from 'vue'
import App from './App.vue'
import VueMqtt from 'vue-mqtt'
import vuetify from './plugins/vuetify';
import store from './store'
import 'bootstrap/dist/css/bootstrap.min.css'
import 'jquery/src/jquery.js'
import 'bootstrap/dist/js/bootstrap.min.js'
import VueApexCharts from 'vue-apexcharts'

Vue.use(VueApexCharts)
Vue.component('apexchart', VueApexCharts)

Vue.config.productionTip = false
Vue.use(VueMqtt, 'ws://192.168.43.179:9001', 
{
  clientId: 'WebClient-' + parseInt(Math.random() * 100000), 
  username: 'thao', 
  password: '1'
})
new Vue({
  vuetify,
  store,
  render: h => h(App)
}).$mount('#app')
