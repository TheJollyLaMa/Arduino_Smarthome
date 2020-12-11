'use strict'

/*global angular*/
var app = angular.module('ChartTests', ['ngRoute']);
app.config(['$routeProvider', function ($routeProvider) {$routeProvider

  .when("/", {controller: "ChartTestsController", templateUrl: "views/home.html"})
  .when("/home", {controller: "ChartTestsController", templateUrl : "views/home.html"})
  .when("/charts", {controller: "ChartTestsController", templateUrl : "views/charts.html"})

  .otherwise({redirectTo: '/'});

}]);
