<template>
    <div v-show="pageActive">
        <h1> Login </h1>
        <br>
        <br>
        <v-sheet max-width="300" class="mx-auto">
            <v-text-field v-model="ipFirst" label="First IP"></v-text-field>
        </v-sheet>
        <br>
        <v-sheet max-width="300" class="mx-auto">
            <v-text-field v-model="ipSecond" label="Second IP"></v-text-field>
        </v-sheet>
        <br>
        <v-sheet max-width="300" class="mx-auto">
            <v-text-field v-model="ipThird" label="Third IP"></v-text-field>
        </v-sheet>
        <br>
        <br>
        <v-btn color="success" @click="connect" >
            Submit
        </v-btn>
        <br>
    </div>
    <div v-show="pageActive2" class="grid-container">

        <v-card color=#fcf5ed>
            <v-chip class="ma-2" :color="stateColor[state['P1C1']]" size="x-large">
              {{ stateText[state['P1C1']] }}
            </v-chip>
            <v-card-text>
                PI : 1 <br>
                Cluster : 1
            </v-card-text>
            <v-card-actions>
                    <v-spacer></v-spacer>
                <v-switch v-model="toggle['P1C1']" hide-details :label="`Switch: ${toggle['P1C1']}`"
                    @change="toggleF('P1C1')"></v-switch>
            </v-card-actions>
        </v-card>

        <v-card color=#fcf5ed>
            <v-chip class="ma-2" :color="stateColor[state['P2C1']]" size="x-large">
              {{ stateText[state['P2C1']] }}
            </v-chip>
            <v-card-text >
                PI : 2 <br>
                Cluster : 1
            </v-card-text>
            <div class="justify-center">
            <v-card-actions>
                    <v-spacer></v-spacer>
                <v-switch v-model="toggle['P2C1']" hide-details :label="`Switch: ${toggle['P2C1']}`"
                    @change="toggleF('P2C1')"></v-switch>
            </v-card-actions>
            </div>
        </v-card>

        <v-card color=#fcf5ed>
            <v-chip class="ma-2" :color="stateColor[state['P2C2']]" size="x-large">
              {{ stateText[state['P2C2']] }}
            </v-chip>
            <v-card-text>
                PI : 2 <br>
                Cluster : 2
            </v-card-text>
            <v-card-actions> 
                <v-spacer></v-spacer>
                <v-switch v-model="toggle['P2C2']" hide-details :label="`Switch: ${toggle['P2C2']}`"
                    @change="toggleF('P2C2')"></v-switch>
            </v-card-actions>
        </v-card>

        <v-card color=#fcf5ed>
            <v-chip class="ma-2" :color="stateColor[state['P3C2']]" size="x-large">
              {{ stateText[state['P3C2']] }}
            </v-chip>
            <v-card-text>
                PI : 3 <br>
                Cluster : 2
            </v-card-text>
            <v-card-actions>
                    <v-spacer></v-spacer>
                <v-switch v-model="toggle['P3C2']" hide-details :label="`Switch: ${toggle['P3C2']}`"
                    @change="toggleF('P3C2')"></v-switch>
            </v-card-actions>
        </v-card>

    </div>
</template>


<script>
export default {
    name: 'loginPage',
    components: {
    },
    data() {
        return {
            message: "temp",
            pageActive: true,
            pageActive2: false,
            ipFirst: "",
            ipSecond: "",
            ipThird: "",
            wsf: null,
            wss: null,
            wst: null,
            show: "all",
            undoneTasks: 0,
            doneTasks: 0,
            list: [],
            dict: {},
            state: {
                "P1C1": "NM_STATE_BUS_SLEEP",
                "P2C1": "NM_STATE_BUS_SLEEP",
                "P2C2": "NM_STATE_BUS_SLEEP",
                "P3C2": "NM_STATE_BUS_SLEEP"
            },
            connectSucceed: 0,
            nodeIDs: {
                "P1C1": 1,
                "P2C1": 0,
                "P2C2": 2,
                "P3C2": 3
            },
            toggle: {
                "P1C1": false,
                "P2C1": false,
                "P2C2": false,
                "P3C2": false
            },
            stateColor:{
                "NM_STATE_BUS_SLEEP": "shades",
                "NM_STATE_PREPARE_BUS_SLEEP": "grey",
                "NM_STATE_REPEAT_MESSAGE": "indigo",
                "NM_STATE_NORMAL_OPERATION": "green",
                "NM_STATE_READY_SLEEP": "orange"
            },
            stateText:{
                "NM_STATE_BUS_SLEEP": "Bus Sleep State",
                "NM_STATE_PREPARE_BUS_SLEEP": "Prepare Bus Sleep State",
                "NM_STATE_REPEAT_MESSAGE": "Repeat Message State",
                "NM_STATE_NORMAL_OPERATION": "Normal Operation State",
                "NM_STATE_READY_SLEEP": "Ready Sleep State"
            },
            testSwitch: false,
            isActive: false
        }
    },
    methods: {
        toggleF(ID) {
            console.log(`${ID} switched to ${this.toggle[ID]}`);
            let jsonString = JSON.stringify({
                "nodeID": this.nodeIDs[ID],
                "NetworkRequested": this.toggle[ID],
            })
            this.wsf.send(jsonString)
            this.wss.send(jsonString)
            this.wst.send(jsonString)
        },
        connect() {
            // 建立 WebSocket (本例 server 端於本地運行)
            this.isActive = true
            let url = 'ws://' + this.ipFirst // ws://localhost:3000
            this.wsf = new WebSocket(url)
            this.wsf.onopen = () => {
                console.log('open connection 1')
                this.this.dict["ipFirst"] = url
                this.this.dict["ipWsf"] = this.wsf
                this.connectSucceed += 1
            }
            this.wsf.onclose = () => {
                console.log('close connection');
            }
            this.wsf.onmessage = (event) => {
                console.log(event.data)
                this.handleJson(event.data)
            }
            let urls = 'ws://' + this.ipSecond // ws://localhost:3000
            this.wss = new WebSocket(urls)
            this.wss.onopen = () => {
                console.log('open connection 2')
                this.this.dict["ipSecond"] = urls
                this.this.dict["ipWss"] = this.wss
                this.connectSucceed += 1
            }
            this.wss.onclose = () => {
                console.log('close connection');
            }
            this.wss.onmessage = (event) => {
                console.log(event.data)
                this.handleJson(event.data)
            }
            let urlt = 'ws://' + this.ipThird // ws://localhost:3000
            this.wst = new WebSocket(urlt)
            this.wst.onopen = () => {
                console.log('open connection 3')
                this.this.dict["ipThird"] = urlt
                this.this.dict["ipWst"] = this.wst
                this.connectSucceed += 1
            }
            this.wst.onclose = () => {
                console.log('close connection');
            }
            this.wst.onmessage = (event) => {
                console.log(event.data)
                this.handleJson(event.data)
            }
            if (this.connectSucceed == 3) {
                console.log("succeed")
                this.pageActive = false
                this.pageActive2 = true
            } else {
                let num = 3 - this.connectSucceed
                alert('There are ' + num + ' ip errors')
            }
            this.pageActive = false
            this.pageActive2 = true
            this.isActive = false
        },
        handleJson(jsonString){
            let json = JSON.parse(jsonString)
            if(json["NodeID"] == 0){
                this.state["P2C1"] = json["CurrentState"]
                this.toggle["P2C1"] = json["NetworkRequested"]
            }else if (json["NodeID"] == 1){
                this.state["P1C1"] = json["CurrentState"]
                this.toggle["P1C1"] = json["NetworkRequested"]
            }else if (json["NodeID"] == 2){
                this.state["P2C2"] = json["CurrentState"]
                this.toggle["P2C2"] = json["NetworkRequested"]
            }else if (json["NodeID"] == 3){
                this.state["P3C2"] = json["CurrentState"]
                this.toggle["P3C2"] = json["NetworkRequested"]
            }
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