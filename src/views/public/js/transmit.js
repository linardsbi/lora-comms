(() => {
    let debounce = false;

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

        let input = document.getElementById("msg-input");

        if (!input.value) return;
        addMessageBox(input.value);

        let xhttp = new XMLHttpRequest();
        // todo: sanitize input
        // xhttp.onreadystatechange = (this) => {
        //     if (this.readyState == 4 && this.status == 200) {
        //     }
        // };

        // xhttp.open("POST", `/fixed/${e.getAttribute("addressee").split(" ").join("")}`, true);
        // xhttp.send(`msg=${input.value}`);
        input.value = "";
    });

    scrollBottom();
})();