var labels = [0];
const accData = {
    labels: labels,
    datasets: [{
        label: 'X',
        backgroundColor: 'rgb(0, 50, 130)',
        borderColor: 'rgb(0, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Y',
        backgroundColor: 'rgb(150, 50, 130)',
        borderColor: 'rgb(150, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Z',
        backgroundColor: 'rgb(150, 250, 100)',
        borderColor: 'rgb(150, 250, 100)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    }]
};

const accConfig = {
    type: 'line',
    data: accData,
    options: {
        responsive: true,
        animation: {
            duration: 100 * 1.5,
            easing: 'linear'
        },

    }
};

const accelerationChart = new Chart(
    document.getElementById('accelerationChart'),
    accConfig
);

const gyroData = {
    labels: labels,
    datasets: [{
        label: 'X',
        backgroundColor: 'rgb(0, 50, 130)',
        borderColor: 'rgb(0, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Y',
        backgroundColor: 'rgb(150, 50, 130)',
        borderColor: 'rgb(150, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Z',
        backgroundColor: 'rgb(150, 250, 100)',
        borderColor: 'rgb(150, 250, 100)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    }]
};

const gyroConfig = {
    type: 'line',
    data: gyroData,
    options: {
        responsive: true,
        animation: {
            duration: 100 * 1.5,
            easing: 'linear'
        },

    }
};

const gyroChart = new Chart(
    document.getElementById('gyroChart'),
    gyroConfig
);

const magData = {
    labels: labels,
    datasets: [{
        label: 'X',
        backgroundColor: 'rgb(0, 50, 130)',
        borderColor: 'rgb(0, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Y',
        backgroundColor: 'rgb(150, 50, 130)',
        borderColor: 'rgb(150, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    },{
        label: 'Z',
        backgroundColor: 'rgb(150, 250, 100)',
        borderColor: 'rgb(150, 250, 100)',
        borderJoinStyle: 'miter',
        data: [0],
        lineTension: 0.1,
        spanGaps: true,
    }]
};

const magConfig = {
    type: 'line',
    data: magData,
    options: {
        responsive: true,
        animation: {
            duration: 100 * 1.5,
            easing: 'linear'
        },

    }
};

const magChart = new Chart(
    document.getElementById('magChart'),
    magConfig
);

const updateAccelerationChart = (accData) => {
    accelerationChart.data.datasets[0].data.push(accData.X);
    accelerationChart.data.datasets[1].data.push(accData.Y);
    accelerationChart.data.datasets[2].data.push(accData.Z);
    accelerationChart.update('easeInOutQuad');
};

const updateGyroChart = (gyroData) => {
    gyroChart.data.datasets[0].data.push(gyroData.X);
    gyroChart.data.datasets[1].data.push(gyroData.Y);
    gyroChart.data.datasets[2].data.push(gyroData.Z);
    gyroChart.update('easeInOutQuad');
};

const updateMagChart = (magData) => {
    magChart.data.datasets[0].data.push(magData.X);
    magChart.data.datasets[1].data.push(magData.Y);
    magChart.data.datasets[2].data.push(magData.Z);
    magChart.update('easeInOutQuad');
};