let oxygenArr = [],
   heatRateArr = [],
   sittingTimeArr = [],
   upArr = [];
let myChart = Highcharts.chart("container1", {
   title: {
      text: "Smart Sit Tracker Dashboard",
   },

   subtitle: {
      text: "Monitor Your Health and Sitting Habits in Real-Time",
   },

   yAxis: {
      title: {
         text: "Value",
      },
   },

   xAxis: {
      categories: upArr,
   },

   legend: {
      layout: "vertical",
      align: "right",
      verticalAlign: "middle",
   },

   plotOptions: {
      series: {
         label: {
            connectorAllowed: false,
         },
      },
   },
   series: [
      {
         name: "oxygen",
         data: [],
      },
      {
         name: "heartRate",
         data: [],
      },
      {
         name: "sittingTime",
         data: [],
      },
   ],

   responsive: {
      rules: [
         {
            condition: {
               maxWidth: 500,
            },
            chartOptions: {
               legend: {
                  layout: "horizontal",
                  align: "center",
                  verticalAlign: "bottom",
               },
            },
         },
      ],
   },
});

let getUserData = function () {
   $.ajax({
      type: "GET",
      url: "https://smartsit-bucket.s3.us-west-1.amazonaws.com/sensor-info",
      dataType: "json",
      async: false,
      success: function (data) {
         console.log("data", data);
         drawChart(data);
      },
      error: function (xhr, status, error) {
         console.error("JSON error: " + status);
      },
   });
};

let drawChart = function (data) {
   let { oxygen, heartRate, sittingTime, timestamps } = data;

   oxygenArr.push(Number(oxygen));
   heatRateArr.push(Number(heartRate));
   sittingTimeArr.push(Number(sittingTime));
   upArr.push(Number(timestamps));

   myChart.series[0].setData(oxygenArr, true);
   myChart.series[1].setData(heatRateArr, true);
   myChart.series[2].setData(sittingTimeArr, true);
};

let intervalTime = 3 * 1000;
setInterval(() => {
   getUserData();
}, intervalTime);
