'use strict'

/*global angular*/
var app = angular.module('SmartHome', ['ngRoute']);
app.config(['$routeProvider', function ($routeProvider) {$routeProvider

  .when("/", {controller: "SmartHomeController", templateUrl: "views/home.html"})
  .when("/about", {controller: "SmartHomeController", templateUrl : "views/about.html"})
  .when("/home", {controller: "SmartHomeController", templateUrl : "views/home.html"})
  .when("/power", {controller: "SmartHomeController", templateUrl : "views/power.html"})
  .when("/water", {controller: "SmartHomeController", templateUrl : "views/water.html"})
  .when("/dashboard", {controller: "SmartHomeController", templateUrl : "views/dashboard.html"})
  .when("/brewery", {controller: "SmartHomeController", templateUrl : "views/brewery.html"})
  .when("/shroomery", {controller: "SmartHomeController", templateUrl : "views/shroomery.html"})
  .when("/greenhouse", {controller: "SmartHomeController", templateUrl : "views/greenhouse.html"})
  .when("/coop", {controller: "SmartHomeController", templateUrl : "views/coop.html"})
  .when("/ledger", {controller: "SmartHomeController", templateUrl : "views/ledger.html"})
  .when("/video", {controller: "SmartHomeController", templateUrl : "views/video.html"})
  .when("/charts", {controller: "SmartHomeController", templateUrl : "views/charts.html"})
  .when("/weather", {controller: "SmartHomeController", templateUrl : "views/weather.html"})

  .otherwise({redirectTo: '/'});

}]);

app.directive('toggleButton', function() {
  return {
    require: 'ngModel',
    scope: {
      activeText: '@activeText',
      inactiveText: '@inactiveText',
      lightState: '=ngModel'
    },
    replace: true,
    transclude: true,
    template: '<div>' +
              '<span ng-transclude></span> ' +
              '<button class="btn" ng-class="{\'btn-primary\': state.value}" ng-click="state.toggle()">{{activeText}}</button>' +
              '<button class="btn" ng-class="{\'btn-primary\': !state.value}" ng-click="state.toggle()">{{inactiveText}}</button>' +
              '</div>',
    link: function postLink(scope) {
      scope.lightState = scope.inactiveText;

      scope.state = {
        value: false,
        toggle: function() {
          this.value = !this.value;
          scope.lightState = this.value ? scope.activeText : scope.inactiveText;
          if (scope.lightState = "Off") {
            $('.datadisplay').get('/data/put/Light/0');
            console.log(scope.lightState);
            $('.datadisplay').get('/data/put/Water/0');
          }else if (scope.lightState = "On") {
            $('.datadisplay').get('/data/put/Light/1');
            console.log(scope.lightState);
            $('.datadisplay').load('/data/put/Water/1');
          }
        }
      };
    }
  }
});
