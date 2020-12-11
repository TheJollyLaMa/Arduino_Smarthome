const http = require('http');

const hostname = '192.168.0.6';
const port = 3333;

var server = http.createServer(function(req, res){
  res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
      res.end('Node JS Test Server');
      });
      
      server.listen(port, hostname, function(){
        console.log('Server running at http://${hostname}:${port}/');
        });
