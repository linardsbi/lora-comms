(() => {
    function startSocket() {
        let socket = new WebSocket(`ws://${window.location.host}/messages`);

        socket.onopen = function (e) {
            console.log("[open] Connection established");
        };

        socket.onmessage = function (event) {
            console.log(`[message] Data received from server: ${event.data}`);
            const msgBar = document.getElementById("msg-bar");
            if (msgBar) {
                msgBar.children[2].innerHTML = event.data;
            }
            
        };

        socket.onclose = function (event) {
            if (event.wasClean) {
                console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
            } else {
                // e.g. server process killed or network down
                // event.code is usually 1006 in this case
                console.log('[close] Connection died');
            }
        };

        socket.onerror = function (error) {
            console.log(`[error] ${error.message}`);
        };
    }

    startSocket();
})();