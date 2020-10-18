import Vue from "vue";
import Vuex from "vuex";
 
Vue.use(Vuex);
export default new Vuex.Store({
    state: {
        userName: "admin",
        password: "admin",
        was_login: false,
    },
    getters: {},
    mutations: {},
    actions: {}
});