app.factory('BridgeFactory', ['$http', function($http) {
    return {
      fetchSmartHomeData: fetchSmartHomeData,
      power: power,
      digitaltest: digitaltest,
      breathe: breathe,
      temperature: temperature
    }
    function fetchSmartHomeData () {
      var url = '/data/get';
      console.log(url);
      $http.get(url).then(function(res) {
          console.log(res);
      });
    }
    function power () {
      var req = {method: "GET", url: 'http://smarthome.local/data/get', headers: {'Content-Type': 'application/x-www-form-urlencoded'}};
      return $http(req)
              .then(function(res) {
                return res;});
    }
    function digitaltest () {
      var req = {method: "GET", url: 'http://smarthome.local/arduino/digital_test', headers: {'Content-Type': 'application/x-www-form-urlencoded'}};
      return $http(req)
              .then(function(res) {
                return res;});
    }
    function breathe () {
      var req = {method: "GET", url: 'http://smarthome.local/arduino/breathe', headers: {'Content-Type': 'application/x-www-form-urlencoded'}};
      return $http(req)
              .then(function(res) {
                return res;});
    }
    function temperature () {
      var req = {method: "GET", url: 'http://smarthome.local/arduino/temperature', headers: {'Content-Type': 'application/x-www-form-urlencoded'}};
      return $http(req)
              .then(function(res) {
                return res;});
    }
}]);
