import Vue from "vue";
import Vuex from "vuex";
import axios from "axios"

const baseURL = "http://localhost:5001"
Vue.use(Vuex);

export default new Vuex.Store({
    state: {
        was_login: false,
        humidities: [],
        temperatures: [],
        realTime: [],
        token: '',
    },
    getters: {},
    mutations: {
        SET_HUMIDITIES(state, humidities) {
            for (let i = 0; i < humidities.length; i++) {
                var date = new Date(humidities[i].date)
                state.humidities.push(humidities[i].value);
                state.realTime.push(date.toLocaleString("en-GB"));
            }          
        },
        SET_TEMPERATURES(state, temperatures) {
            for (let i = 0; i < temperatures.length; i++) {
                state.temperatures.push(temperatures[i].value);
            }      
        },
        SET_TOKEN(state, token) {
            state.token = token;
            state.was_login = true;
        }
    },
    actions: {
        getHumidities({commit}) {
            axios.get(baseURL + '/api/data/humidity', {
                headers: {
                    'Content-Type': 'application/json',
                    'x-auth-token': this.state.token,
                }
            })
            .then(response => {
                console.log(response);
                commit('SET_HUMIDITIES', response.data);
            })
            .catch(error => {
                console.log(error.response.data)
            })
        },
        getTemperatures({commit}) {
            axios.get(baseURL + '/api/data/temperature', {
                headers: {
                    'Content-Type': 'application/json',
                    'x-auth-token': this.state.token,
                }
            })
            .then(response => {
                console.log(response);
                commit('SET_TEMPERATURES', response.data);
            })
            .catch(error => {
                console.log(error.response.data)
            })
        },
        login({commit},{email, password}) {
            axios.post(baseURL + '/api/auth', {
                email: email,
                password: password
            }, {
                headers: {
                    'Content-Type': 'application/json',
                }
            })
            .then(response => {
                console.log(response.data.token);
                commit('SET_TOKEN', response.data.token);
            })
            .catch(error => {
                console.log(error.response.data);
            })
        },
        refreshData() {
            this.state.was_login = false;
            this.state.humidities= [];
            this.state.temperatures= [];
            this.state.realTime= [];
            this.state.token= '';
        }
    }
});