<template>
  <div v-show="pageActive">
    <h1> Login </h1>
    <br>
    <v-sheet
      max-width="300"
      class="mx-auto">
    <v-text-field
      v-model="ipFirst"
      label="First IP"
      ></v-text-field>
    </v-sheet>
    <br>
    <v-sheet
        max-width="300"
        class="mx-auto">
    <v-text-field
      v-model="ipSecond"
      label="Second IP"
      ></v-text-field>
    </v-sheet>
    <br>
    <v-sheet
        max-width="300"
        class="mx-auto">
    <v-text-field
      v-model="ipThird"
      label="Third IP"
      ></v-text-field>
      </v-sheet>
    <br>
    <br>
    <v-btn
            color="success"
            @click="connect"
          >
            Submit
          </v-btn>
    <br>
  </div>
  <div v-show="pageActive2" class="grid-container">

      <v-card color=#fcf5ed>
        <v-card-title>
          current state : {{ state["P1C1"] }}
        </v-card-title>
        <v-card-text>
          PI : 1  <br>
          Cluster : 1
        </v-card-text>
        <v-card-actions>
          <v-switch
            v-model="toggle['P1C1']"
            hide-details
            :label="`Switch: ${toggle['P1C1']}`"
            @change="toggleF('P1C1')"
          ></v-switch>
        </v-card-actions>
      </v-card>

      <v-card color=#fcf5ed>
        <v-card-title>
          current state : {{ state["P2C1"] }}
        </v-card-title>
        <v-card-text>
          PI : 2  <br>
          Cluster : 1
        </v-card-text>
        <v-card-actions>
          <v-switch
            v-model="toggle['P2C1']"
            hide-details
            :label="`Switch: ${toggle['P2C1']}`"
            @change="toggleF('P2C1')"
          ></v-switch>
        </v-card-actions>
      </v-card>

      <v-card color=#fcf5ed>
        <v-card-title>
          current state : {{ state["P2C2"] }}
        </v-card-title>
        <v-card-text>
          PI : 2  <br>
          Cluster : 2
        </v-card-text>
        <v-card-actions>
          <v-switch
            v-model="toggle['P2C2']"
            hide-details
            :label="`Switch: ${toggle['P2C2']}`"
            @change="toggleF('P2C2')"
          ></v-switch>
        </v-card-actions>
      </v-card>

      <v-card color=#fcf5ed>
        <v-card-title>
          current state : {{ state["P3C2"] }}
        </v-card-title>
        <v-card-text>
          PI : 3  <br>
          Cluster : 2
        </v-card-text>
        <v-card-actions>
          <v-switch
            v-model="toggle['P3C2']"
            hide-details
            :label="`Switch: ${toggle['P3C2']}`"
            @change="toggleF('P3C2')"
          ></v-switch>
        </v-card-actions>
      </v-card>
      
  </div>

</template>


<script>
    export default{
        name:'loginPage',
        components: {
        },
    data() {
        return{
            message : "temp",
            pageActive : true,
            pageActive2 : false,
            ipFirst : "",
            ipSecond : "",
            ipThird : "",
            show: "all",
            undoneTasks: 0,
            doneTasks: 0, 
            list: [],
            dict : {},
            state : {
              "P1C1":"P1C1",
              "P2C1":"P2C1",
              "P2C2":"P2C2",
              "P3C2":"P3C2"
            },
            connectSucceed : 0,
            toggle : {
              "P1C1": false,
              "P2C1": false,
              "P2C2": false,
              "P3C2": false
            },
            
            isActive : false
        }
    },
    methods: {
        toggleF(ID){
          console.log(`${ID} switched to ${this.toggle[ID]}`);
        },
        connect(){
          // 建立 WebSocket (本例 server 端於本地運行)
          this.isActive =  true
          let url = 'ws://' + this.ipFirst // ws://localhost:3000
          var wsf = new WebSocket(url)
          wsf.onopen = () => {
            console.log('open connection')
            this.this.dict["ipFirst"] = url
            this.this.dict["ipWsf"] = wsf
            this.connectSucceed += 1
          }
          wsf.onclose = () => {
            console.log('close connection');
          }
          let urls = 'ws://' + this.ipSecond // ws://localhost:3000
          var wss = new WebSocket(urls)
          wss.onopen = () => {
            console.log('open connection')
            this.this.dict["ipSecond"] = urls
            this.this.dict["ipWss"] = wss
            this.connectSucceed += 1
          }
          wss.onclose = () => {
            console.log('close connection');
          }
          let urlt = 'ws://' + this.pThird // ws://localhost:3000
          var wst = new WebSocket(urlt)
          wst.onopen = () => {
            console.log('open connection')
            this.this.dict["ipThird"] = urlt
            this.this.dict["ipWst"] = wst
            this.connectSucceed += 1
          }
          wst.onclose = () => {
            console.log('close connection');
          }
          if(this.connectSucceed == 3){
            console.log("succeed")
            this.pageActive = false
            this.pageActive2 = true
          }else{
            let num = 3 - this.connectSucceed
            alert('There are ' + num + ' ip errors')
          }
          this.pageActive = false
          this.pageActive2 = true
          this.isActive =  false
        }
    },
    computed: {
    }
        
}


</script>


<style>
.grid-container {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  grid-template-rows: repeat(2, 1fr);
  grid-gap: 10px;
  padding: 20px;
}

.item {
  background-color: #fcf5ed;
  padding: 20px;
}
</style>