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

// Serial Window
window.$ = window.jQuery = require('./js/jquery.min.js');
let port = null;
serialport.list().then((ports,err) => {
    for (let item of ports) {
        $('.com').append(`<option>${item.comName}</option>`)
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
        console.log(`DATA: ${data}`);
        $('.receive-windows').append(data.toString());
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
