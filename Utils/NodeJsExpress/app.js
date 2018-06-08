var express = require("express");
 
var app = express();
 
app.get("/", function(request, response){
     
    response.send("Hello from Express");
});

app.get("/protocolslave", function(request, response){
     
    response.send("Some data for ProtocolSlave");
});


app.listen(3000);
