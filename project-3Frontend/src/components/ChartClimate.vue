<template lang="html">

  <div>
      <apexchart width="500" type="line" :options="getOptions" :series="getSeries"></apexchart>
  </div>

</template>

<script lang="js">
  export default  {
    name: 'chart-climate',
    props: [],
    mounted () {
      
    },
    data () {
      return {
        options: {
          chart: {
            id: 'vuechart-example'
          },
          xaxis: {
            categories: []
          },
        },
        series: [
            {
              name: 'Humidity',
              data: []
            },
            {
              name: 'Temperature',
              data: []
            }
          ]
      }
    },
    methods: {
      
    },
    computed: {
      getOptions () {
        var options = {
        chart: {
            id: 'vuechart-example'
          },
          xaxis: {
            categories: this.$store.state.realTime
          },
        };
        return options;
      },
      getSeries () {
        var series = [
        {
              name: 'Humidity',
              data: this.$store.state.humidities
            },
            {
              name: 'Temperature',
              data: this.$store.state.temperatures
            }
        ]
        return series;
      }
    },
}


</script>

<style scoped lang="scss">
  .chart-climate {

  }
</style>
