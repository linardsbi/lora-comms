(() => {
    const STARTTR = 1;
    const ENDTR = 4;
    const FIXEDTR = 17;
    const CLEARTR = 18;

    let debounce = false;
    let socket = new WebSocket(`ws://${window.location.host}/messages`);
    let addressee = document.getElementById("send").getAttribute("data-addressee").split(" ");
    let bytes = new Uint8Array(4);
    
    for (let i = 1; i < 4; i++) {
        bytes[i] = Number(addressee[i-1]);
    }

    addressee = addressee.join("");

    function scrollBottom() {
        const box = document.getElementById("chat-window").children[0];
        if (box.childElementCount - 2 == 0) return;
        box.scrollTop = (box.childElementCount - 2 + 12) * box.children[2].clientHeight;
    }

    function addMessageBox(text, me = true) {
        let el = (me) ? document.querySelector(".message-me").cloneNode(true) : document.querySelector(".message-other").cloneNode(true);
        el.children[0].innerHTML = text;
        el.children[1].innerHTML = (new Date()).toLocaleTimeString();
        document.getElementById("chat-window").children[0].appendChild(el);
        el.classList.remove("hidden");
        scrollBottom();
    }

    document.getElementById("send").addEventListener("click", (e) => {
        if (debounce) return;
        debounce = true;
        setTimeout(() => {
            debounce = false
        }, 1000);

        let val = document.getElementById("msg-input").value;
        if (!val) return;

        addMessageBox(val);
        bytes[0] = FIXEDTR;
        socket.send(`${STARTTR}${bytes[0]}${bytes[1]}${bytes[2]}${bytes[3]}${val}${ENDTR}`);

        document.getElementById("msg-input").value = "";
    });

    document.getElementById("msg-input").addEventListener("keydown", (e) => {
        if (e.keyCode == 13) {
            document.getElementById("send").click();
        }
    });

    socket.onmessage = function (event) {
        if (event.data.split(":")[0] == addressee || event.data.split(":")[0] == "x") { // "x" is debug override
            addMessageBox(event.data.split(":")[1], false);
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

    scrollBottom();
})();