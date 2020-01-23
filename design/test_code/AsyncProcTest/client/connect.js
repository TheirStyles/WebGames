
var socket = null;

function Send() {

    socket = new WebSocket("ws://localhost:31400");

    socket.onopen = function(e){
        alert(e)
        socket.send("Hello World");
    }

    socket.onmessage = function(e){
        alert(e.data);
        socket.close();
    } ;   
};
