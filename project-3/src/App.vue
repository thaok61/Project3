<template>
  <v-app>
    <v-main>
      
      <div v-if="this.$store.state.was_login">
        <div style="position:absolute; right:10px; top:10px; z-index: 1;">
            <button class="btn btn-outline-success my-2 my-sm-0" @click="logout">Log Out</button>
        </div>
        <Home/>
      </div>
      <div v-else>
        <Login/>
      </div>
    </v-main>
    <!-- <Login/> -->
  </v-app>
</template>

<script>
import Login from './components/Login';
import Home from './components/Home'
export default {
  name: 'App',

  components: {
    Login,
    Home,
  },

  data: () => ({
    //
  }),
  methods: {
    logout () {
      this.$store.state.was_login = false
      }
  },
};
</script>
