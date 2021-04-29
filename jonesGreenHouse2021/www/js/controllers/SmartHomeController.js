app.controller('SmartHomeController', ['$scope', '$route', '$http', '$interval', '$filter', 'BridgeFactory', function($scope, $route, $http, $interval, $filter, BridgeFactory) {
    $scope.title = "Smart Home Dashboard";
    $scope.aboutmsg = "this is a testview for the new Smart Home Dashboard";
    $scope.timeWindowList = ['doy','woy','moy'];
    $scope.showPinState = function() {$('.datadisplay').load('/data/get');};
    $scope.digitaltest = function () {$scope.digitaltestdata = $('.datadisplay').load('/arduino/digital_test');};
    $scope.greenhouse_variables = [];$scope.dailyGenData = {};$scope.powerGenData = [];$scope.power = [];$scope.datetime = [];$scope.line1 = [];$scope.line2 = [];$scope.line3 = [];$scope.line4 = [];
  //$scope.showTristar = function() {$('.tristar_display').load('http://192.168.0.244');};
    $scope.target_voltage_arrData = new Array;
    $scope.arrLabels = new Array;
    $scope.array_voltage_arrData = new Array;
    $scope.battery_temp_F_arrData = new Array;
    $scope.battery_current_arrData = new Array;
    $scope.battery_sense_voltage_arrData = new Array;
    $scope.parseData = function() {
        $http.get('/data/get').then(function(data){
            var ard_var = data.data.value;
            angular.forEach(ard_var, function(value, key){
                $scope.greenhouse_variables[key] = value;
            });//console.log($scope.greenhouse_variables.total_days);
            $scope.fetchPowerGenerationData();
        });
    };

    $scope.fetchPowerGenerationData = function (){
          var url = '/sd/jonesGreenHouse/data/';//console.log(url);
              url += $scope.greenhouse_variables.total_days;//console.log(url);
              url += '/powerGenerationData.json';//console.log(url);

          $http.get(url).then(function(data){

                  //console.log(data);
                  Object.keys(data.data).sort().forEach(key => {
                    $scope.dailyGenData[key] = data.data[key];
                  });
                  angular.forEach($scope.dailyGenData, function (entry, index) {
                    $scope.arrLabels.push(index);
                    $scope.target_voltage_arrData.push(entry.target_voltage);
                    $scope.battery_temp_F_arrData.push(entry.battery_temp_F);
                    $scope.battery_current_arrData.push(entry.battery_current);
                    $scope.array_voltage_arrData.push(entry.array_voltage);
                    $scope.battery_sense_voltage_arrData.push(entry.battery_sense_voltage);
                  });

                  $scope.series = ['array_voltage', 'battery_sense_voltage'];
                  $scope.onClick = function (points, evt) {
                    console.log(points, evt);
                  };
                  $scope.datasetOverride = [{ yAxisID: 'y-axis-1' },{ yAxisID: 'y-axis-2' }];
                  $scope.data = {
                      datasets: [{
                                  data: $scope.array_voltage_arrData,
                                  label: "Solar Array Voltage",
                                  borderColor: "#3e95cd",
                                  fill: false
                                 },
                                 {
                                  data:$scope.battery_sense_voltage_arrData,
                                  label: "Battery Sensor Voltage",
                                  borderColor: "#3e95cd",
                                  fill: false
                                 }
                                ]};
                  $scope.options = {
                    title: { display: true, text: 'Power Data Charts'},
                    legend: {display: true},
                    scales: {
                      yAxes: [
                        {
                          id: 'y-axis-1',
                          type: 'linear',
                          display: true,
                          position: 'left',
                          scaleLabel: { display: true, labelString: 'Solar Array Voltage Max Power Point' }
                        },
                        {
                          id: 'y-axis-2',
                          type: 'linear',
                          display: true,
                          position: 'right',
                          gridLines: {drawOnChartArea: false},
                          scaleLabel: { display: true, labelString: 'Battery Voltage Sensor' }
                        }
                      ],
                      xAxes: [
                        {
                          scaleLabel: {
                            display: true,
                            labelString: 'Time of Data Entry'
                          }
                        }
                      ]
                    }
                  };
          });
    };

    $scope.fetchPowerConsumptionData = function (){
          doy = $scope.doy;
          y = $scope.y;
          url = '/sd/jonesgarden/data/'+doy+'_'+y+'/powerData.txt';
          $http.get(url)
            .then(function(data){
                  $scope.powerConsData = data;
                  $scope.datetime = data["datetime"];
                  $scope.line1 = data["line1"];
                  $scope.line2 = data["line2"];
                  $scope.line3 = data["line3"];
                  $scope.line4 = data["line4"];
                  console.log(data);
                }
        )
    };
    $scope.sun = function (){
        $scope.parseInstantData();
        console.log($scope.doy);
        url = '/sd/jonesgarden/data/'+doy+'_'+y+'/sunData.txt';
        $interval(
          $http.get(url)
              .then(function(data){
                $scope.vegSunT5State = data["vegSunT5State"];
                $scope.vegSunBlueState = data["vegSunBlueState"];
                $scope.vegSunRed1State = data["vegSunRed1State"];
                $scope.vegSunRed2State = data["vegSunRed2State"];
                $scope.bloomSunHPSState = data["bloomSunHPSState"];
                $scope.bloomSunMHState = data["bloomSunMHState"];
                if ($scope.vegSunT5State == "1") {
                    $("#sun").removeClass("sunOff");
                    $("#sun").addClass("sunOn");
                }else {
                    $("#sun").removeClass("sunOn");
                    $("#sun").addClass("sunOff");
                }
              }
          ),5000)
    };

    $scope.rain = function (){
        doy = $scope.doy;
        y = $scope.y;
        url = '/sd/jonesgarden/data/'+doy+'_'+y+'/rainData.txt';
        $http.get('/sd/jonesgarden/data/rainData.txt')
              .then(function(data){
                $scope.pump1State = data["pump1State"];
                $scope.pump2State = data["pump2State"];
                if ($scope.pump1State == "1" || $scope.pump2State == "1") {
                    $("#rain").removeClass("rainOff");
                    $("#rain").addClass("rainOn");
                }else {
                    $("#rain").removeClass("rainOn");
                    $("#rain").addClass("rainOff");
                }
              });
    };

    $scope.fetchClimateData = function (){
          doy = $scope.doy;
          y = $scope.y;
          url = '/sd/jonesgarden/data/'+doy+'_'+y+'/climateData.txt';
          $http.get(url)
            .then(function(data){
                  $scope.data = data;
                  console.log(data);
                  $scope.temperature = data["temperature"];
                  $scope.humidity = data["humidity"];
                });
    };
    $scope.wind = function (){
        doy = $scope.doy;
        y = $scope.y;
        url = '/sd/jonesgarden/data/'+doy+'_'+y+'/windData.txt';
        $http.get(url)
              .then(function(data){
                $scope.fanState = data["fanState"];
                $scope.humidity = data["dehumidifierState"];
              });
    };
    $scope.plants = function () {
      //$scope.plantdata = $('.datadisplay').load('/arduino/digital_test');
    };
    $scope.ledger = function () {
      //$scope.ledgerdata = $('.datadisplay').load('/arduino/digital_test');
    };


}]);
