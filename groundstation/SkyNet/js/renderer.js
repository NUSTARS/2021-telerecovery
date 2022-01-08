// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const serialport = require('serialport')
const tableify = require('tableify')

async function listSerialPorts() {
  await serialport.list().then((ports, err) => {
    if(err) {
      document.getElementById('error').textContent = err.message
      return
    } else {
      document.getElementById('error').textContent = ''
    }
    console.log('ports', ports);

    if (ports.length === 0) {
      document.getElementById('error').textContent = 'No Ports Discovered'
    }

    tableHTML = tableify(ports)
    document.getElementById('ports').innerHTML = tableHTML
  })
}

// Set a timeout that will check for new serialPorts every 2 seconds.
// This timeout reschedules itself.
setTimeout(function listPorts() {
  listSerialPorts();
  setTimeout(listPorts, 2000);
}, 2000);

// datatojson
const dataStringToJSON = (stringData) => {
  const stringToArray = stringData.replace(/(\r\n|\n|\r)/gm, "").split(",");
  return JSON.stringify ({
    "ToF": stringToArray[0],
    "Temp": stringToArray[1],
    "Pressure": stringToArray[2],
    "Altitude": stringToArray[3],
    "Acceleration": {
      "X": stringToArray[4],
      "Y": stringToArray[5],
      "Z": stringToArray[6]
    },
    "Gyro": {
      "X": stringToArray[7],
      "Y": stringToArray[8],
      "Z": stringToArray[9]
    },
    "Mag": {
      "X": stringToArray[10],
      "Y": stringToArray[11],
      "Z": stringToArray[12]
    },
    "RSSI": stringToArray[13]
  });
}


// Serial Window
window.$ = window.jQuery = require('./js/jquery.min.js');
let port = null;
serialport.list().then((ports,err) => {
    for (let item of ports) {
        $('.com').append(`<option>${item.path}</option>`)
    }
    console.log(ports);
});
$('.btn-submit').click((data) => {
    let COM = $('select option:selected').text();
    let BaudRate = $('#BaudRate').val();
    console.log(COM);
    console.log(BaudRate);
    port = new serialport(COM, {
        baudRate: parseInt(BaudRate)
    });
    $('.receive-windows').text(`Opening: ${COM}, Using Baud Rate: ${BaudRate}`);
    $('.receive-windows').append('<br/>=======================================<br/>');
    port.on('data', data => {
      const jsonData = JSON.parse(dataStringToJSON(data.toString()));
        console.log(`DATA: ${dataStringToJSON(data.toString())}`);
        $('.receive-windows').append(data.toString());
        updateAccelerationChart(jsonData.Acceleration);
        updateGyroChart(jsonData.Gyro);
        updateMagChart(jsonData.Mag);
        updateLabels();
        $('.altitude-display').text(`${parseFloat(jsonData.Altitude).toFixed(1)} M`);
        $('.temp-display').text(`${parseFloat(jsonData.Temp).toFixed(1)} C`);
        if (jsonData.ToF < 5) {
          $('.tof-display').text(`Deployed`);
          $(('.tof-card')).removeClass('bg-danger').addClass('bg-success');
        }
        $('.receiver-connected').css("display", "").text(`RSSI: ${jsonData.RSSI}`);
    });
});

// Click to send message
$('.btn-send').click(() => {
    var sendData = $('.input-send-data').val();
    if (port != {} && port != null) {
        console.log(`SendData: ${sendData}`);
        port.write(sendData);
    }
})
// Clear Message
$('.btn-reset').click(() => {
    $('.input-send-data').val('');
})

// Check for Updates Button
document.getElementById('checkupdate').addEventListener("click", displayVersion);
function displayVersion() {
  alert("You are running the latest version of SkyNet.");
}

// For todays date;
Date.prototype.today = function () { 
  return ((this.getDate() < 10)?"0":"") + this.getDate() +"/"+(((this.getMonth()+1) < 10)?"0":"") + (this.getMonth()+1) +"/"+ this.getFullYear();
}

// For the time now
Date.prototype.timeNow = function () {
   return ((this.getHours() < 10)?"0":"") + this.getHours() +":"+ ((this.getMinutes() < 10)?"0":"") + this.getMinutes() +":"+ ((this.getSeconds() < 10)?"0":"") + this.getSeconds();
}

// Set a timeout that will update clocks every second.
// This timeout reschedules itself.
setInterval(function updateClocks() {
  const now = new Date();
  $('.local-time-display').text(now.toLocaleString('en-US', { hour12: false }));
  $('.utc-time-display').text(`${now.getUTCMonth() + 1}/${now.getUTCDate()}/${now.getUTCFullYear()}, ${now.getUTCHours()}:${now.getUTCMinutes()}:${now.getUTCSeconds()}`);
}, 1000);