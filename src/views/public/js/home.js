(() => {
    function createModal(bText, fBtnText, onClose) {
        let wrp = document.getElementById("modal-wrapper");
        wrp.classList.remove("hidden");

        let btn = wrp.children[0].children[2].firstChild;

        btn.innerHTML = fBtnText;
        wrp.children[0].children[1].innerHTML = bText;

        btn.addEventListener("click", () => {
            wrp.classList.add("hidden");
            onClose();
        });
    }

    document.getElementById("connections").addEventListener("click", (e) => {
        //const conns = "%CONNECTIONS%";
        const conns = "first:00 03 04,second:00 01 04";
        let content = "";
        for (let conn of conns.split(",")) {
            if (conn.indexOf(":") < 0) {
                content += "<li>No connections</li>";
                break;
            }
            content += `<li>
                            <a href="/fixed/${conn.split(":")[1].split(" ").join("")}">
                                ${conn.split(":")[0]} (${conn.split(":")[1]})
                            </a>
                        </li>`;
        }
        createModal(`<b>Connections:</b><div class='conn-list'><ol>${content}</ol></div>`, "Select", () => {
            // do whatever
        });
    });
    document.querySelectorAll("#modal-wrapper .background, #modal-wrapper .btn-close").forEach((v) => {
        v.addEventListener("click", () => {
            document.getElementById("modal-wrapper").classList.toggle("hidden");
        });
    });
})();