var admin = require("firebase-admin");
var serviceAccount = require('./service-account.json');
admin.initializeApp({
  credential:admin.credential.cert(serviceAccount),
  databaseURL:"https://iot-thcntt1.firebaseio.com"
}
);
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline');
const port1 = new SerialPort('/dev/cu.usbserial-14220', {
  baudRate: 19200
})
var counter =0;
const parser = port1.pipe(new Readline({ delimiter: '\r\n' }));



//db
var db = admin.database();


var logNode= db.ref('/log')
//on added
logNode.child('test').on('child_added',(childSnapshot,prevChildKey)=>{
  
  console.log("----");
  console.log(counter++)
  console.log(childSnapshot.val());
  console.log("----");
})


// logNode.child('timestamp').once('value').then((dataSnapshot)=>{
//   console.log("Read : ")
//   console.log(dataSnapshot.val())
// })



function autoPush(data){
    let timeset = new Date
    logNode.child('test').child(timeset.toString()).set(
      data
    )
}

// db.child('timestamp').on('child_changed',(childSnapshot,prevChildKey)=>{
//   childSnapshot.key
//   childSnapshot.val()
//   console.log(childSnapshot.val());
// })

function handleWatering(Humninity){
  console.log(typeof(Humninity))
  var Pump = false
  if (Humninity>29) {
    //donothing
    Pump=true
    port1.write("pump")
  }
  else{
    console.log('notrunning')
    port1.write("0")
     // autoPush(HumTemp)
    //trigger watering

  }
}



var HumTemp
//serial
port1.on("open", () => {
    console.log('serial port open');
  });
  parser.on('data', data =>{
    // console.log('got word from arduino:', data);
    // console.log(JSON.parse(data));
    try {
      HumTemp = JSON.parse(data);
      console.log(HumTemp)
    } catch (error) {
      console.log(error)
    }
    // finally{
    //   HumTemp=data
    // }
    handleWatering(HumTemp["temp"])
    
  });

port1.write("0")

