var labels = [
    1,
    2,
    3,
    4,
    5,
    6,
];
const data = {
    labels: labels,
    datasets: [{
        label: 'My First dataset',
        backgroundColor: 'rgb(0, 50, 130)',
        borderColor: 'rgb(0, 50, 130)',
        borderJoinStyle: 'miter',
        data: [0, 10, 5, 2, 20, 30, 45],
        lineTension: 0.1,
        spanGaps: true,
    }]
};
const config = {
    type: 'line',
    data: data,
    options: {
        responsive: true,
        animation: {
            duration: 100 * 1.5,
            easing: 'linear'
        },

    }
};
const myChart = new Chart(
    document.getElementById('myChart'),
    config
);

function updateChart() {
    myChart.data.datasets[0].data.push(myChart.data.datasets[0].data.at(-1) + 10);
    labels.push(labels.at(-1) + 1);
    myChart.update('easeInOutQuad');
};