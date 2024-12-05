const char *indexHtml = R"====(
<!DOCTYPE html>
<html lang="cs">
    <head>
        <meta charset="utf-8">
        <title>Device setup</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <style>
        body {
            font-family: helvetica, sans-serif;
            font-size: 1em;
            margin: 0;
            padding: 0;
        }
        fieldset {
            background: #eee;
            border: 0;
            border-radius: .25em;
            padding: .5em;
            margin: .5em;
        }
        .fl {
            display: block;
            padding: .1em;
        }
        label {
            width: 50%;
            display: inline-block;
        }
        legend {
            font-weight: bold;
            display: block;
            margin: 0;
            padding: 0;
        	float: left;
	        width: 100%;
            padding-bottom: .25em;
            margin-bottom: .25em;
	        border-bottom: 1px solid #ddd;
        }
    </style>
    <body>
        <form method="post" id="config">
            <fieldset>
                <legend>Stand Alone</legend>
                <div class="fl">
                    <label>Solid color</label>
                    <input type="color" name="E00" value="">
                </div>
            </fieldset>
            <fieldset>
                <legend>DMX</legend>
                <div class="fl">
                    <label>DMX Universe</label>
                    <input type="text" name="E01" value="">
                </div>
                <div class="fl">
                    <label>Start channel</label>
                    <input type="text" name="E02" value="">
                </div>
                <div class="fl">
                    <label>Cells</label>
                    <select name="E03" id="celset">
                        <option value="60">1 (3ch)</option>
                        <option value="30">2 (6ch)</option>
                        <option value="20">3 (9ch)</option>
                        <option value="15">4 (12ch)</option>
                        <option value="12">5 (15ch)</option>
                        <option value="10">6 (18ch)</option>
                        <option value="6">10 (30ch)</option>
                        <option value="5">12 (36ch)</option>
                        <option value="4">15 (45ch)</option>
                        <option value="3">20 (60ch)</option>
                        <option value="2">30 (90ch)</option>
                        <option value="1">60 (180ch)</option>
                    </select>
                </div>
            </fieldset>

            <fieldset>
                <legend>Local AP (setup only)</legend>
                <div class="fl">
                    <label>Local AP name</label>
                    <input type="text" name="E04" value="">
                </div>
                <div class="fl">
                    <label>Local AP password</label>
                    <input type="text" name="E05" value="">
                </div>
            </fieldset>
            <fieldset>
                <legend>Network</legend>
                <div class="fl">
                    <label>Network</label>
                    <select name="E06" id="NetList"></select>
                    <button type="button" id="scan">Scan</button>
                </div>
                <div class="fl">
                    <label>Network Password</label>
                    <input type="text" name="E07" value="">
                </div>
                <div class="fl">
                    <label>DHCP</label>
                    <input type="checkbox" name="E08" value="1">
                </div>
                <div class="fl">
                    <label>IP</label>
                    <input type="text" name="E09" value="">
                </div>
                <div class="fl">
                    <label>Mask</label>
                    <input type="text" name="E10" value="">
                </div>
                <div class="fl">
                    <label>Gateway</label>
                    <input type="text" name="E11" value="">
                </div>
            </fieldset>
            <fieldset>
                <button type="submit">Save config</button>
            </fieldset>
        </form>
        <script type="text/javascript">
            var xhr = new XMLHttpRequest();
            var donefunc = null;
            xhr.onreadystatechange = (e) => {
                if(e.target.readyState == 4) {
                    if(typeof(donefunc) == 'function') {
                        donefunc(e.target.responseText);
                    }
                }
            };

            var scanBtn = document.getElementById('scan');
            var netlist = document.getElementById('NetList');
            scanBtn.addEventListener('click', scanStart);

            loadConfig();

            document.getElementById('config').addEventListener('submit', (e) => {
                e.preventDefault();
                let el = e.target.elements;
                let output = [];
                for(let i of el) {
                    if(i.name != '') {
                        if(i.type == 'checkbox') {
                            output.push(i.name + ':' + (i.checked ? '1' : '0'));
                        } else {
                            output.push(i.name + ':' + i.value);
                        }
                    }
                }
                output.sort();
                donefunct = null;
                xhr.open('POST', '/save');
                xhr.send(output.join(';'));
            });

            function loadConfig() {
                donefunc = loadConfigDone;
                xhr.open('GET', 'config');
                xhr.send();
            }

            function loadConfigDone(d) {
                let config = d.split(';');
                let setup = {};
                for(let e of config) {
                    e = e.split(':');
                    setup[e[0]] = e[1];
                    if(e[0] != '') {
                        let el = document.querySelector('input[name=' + e[0] + ']');
                        if(el != null) {
                            if(el.type == 'checkbox') {
                                el.checked = (e[1] == '1');
                            } else {
                                el.value = e[1];
                            }
                        }
                    }
                }

                let option = document.createElement('option');
                option.text = setup.E06;
                netlist.add(option);
                document.getElementById('celset').value = setup.E03;

            }

            function scanStart() {
                scanBtn.disabled = true;
                netlist.disabled = true;
                donefunc = scanProgress;
                xhr.open('GET', '/scan');
                xhr.send();
            }

            function scanProgress(r) {
                if(r != '.') {
                    scanDone(r);
                } else {
                    setTimeout(scanStart, 1000);
                }
            }

            function scanDone(r) {
                while(netlist.length > 0) {
                        netlist.remove(netlist.length - 1);
                }
                for(let n of r.split('\n')) {
                    if(n != '') {
                        let option = document.createElement('option');
                        option.text = n.trim();
                        netlist.add(option);
                    }
                }
                scanBtn.disabled = false;
                netlist.disabled = false;
            }
        </script>
    </body>
</html>
)====";