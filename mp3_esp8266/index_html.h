const char html_string[] PROGMEM = R"=====(
<html>

<head>
  <style>
    .button {
      border: 2px solid black;
      color: black;
      background-color: white;

      padding: 15px 32px;
      font-size: 24px;
      margin: 4px 2px;

      cursor: pointer;
      transition-duration: 0.4s;
      width: 90%
    }

    .button:hover {
      background-color: #BBBBBB;
    }

    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }

    .buttonStop {
      background-color: #fe9898;
      color: black;
    }

    .buttonStop:hover {
      background-color: #ff4949;
      color: white;
    }

    .center {
      text-align: center;
    }
  </style>
</head>

<title>MP3 WiFi</title>
<script>
  // const vol = 25;

  function play(pista) {
    // document.getElementById("demo").innerHTML = "Pista " + pista;

    var xhr = new XMLHttpRequest();
    var url = "play?pista=" + pista;
    xhr.open('POST', url, true);
    xhr.send();
  }

  function stop() {
    var xhr = new XMLHttpRequest();
    var url = "stop";
    xhr.open('POST', url, true);
    xhr.send();
  }
</script>

<body>
  <div class="center">
    <h2>Control MP3</h2>
    <button class="button buttonStop" onclick="stop()">Stop</button>

    <!-- <br> -->
    <h3>Seleccionar Pista:</h3>

    <button class="button" onclick="play(1)">Pista 1</button><br>
    <button class="button" onclick="play(2)">Pista 2</button><br>
    <button class="button" onclick="play(3)">Pista 3</button><br>
    <button class="button" onclick="play(4)">Pista 4</button><br>
    <button class="button" onclick="play(5)">Pista 5</button><br>
    <button class="button" onclick="play(6)">Pista 6</button><br>
    <button class="button" onclick="play(7)">Pista 7</button><br>
    <button class="button" onclick="play(8)">Pista 8</button><br>

  </div>
</body>

</html>
)=====";
