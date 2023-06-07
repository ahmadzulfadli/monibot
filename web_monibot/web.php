  <html>

  <head>
      <title>MONIBOT</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
      body {
          background-color: whitesmoke;
          font-family: Arial;
          text-align: center;
          margin: 0px auto;
          padding-top: 30px;
      }

      table {
          margin-left: auto;
          margin-right: auto;
      }

      td {
          padding: 8 px;
      }

      .button {
          width: 70px;
          height: 40px;
          background-color: #079bfd;
          border: none;
          border-radius: 4px;
          color: #F4EEE0;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 13px;
          margin: 1px 1px;
          cursor: pointer;
          -webkit-touch-callout: none;
          -webkit-user-select: none;
          -khtml-user-select: none;
          -moz-user-select: none;
          -ms-user-select: none;
          user-select: none;
          -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
      }

      .servo {
          width: 60px;
          height: 30px;
          background-color: #393646;
          font-size: 11px;
          margin: 1px 1px;
      }

      .stop {
          background-color: #393646;
          width: 114px;
      }

      .on {
          background-color: green;
      }

      .off {
          background-color: red;
      }

      table {
          margin: 20px auto;
          border-collapse: collapse;
      }

      td {
          padding: 5px;
          text-align: center;
          border: none;
      }

      img {
          width: auto;
          max-width: 100%;
          height: 400px;
          transform: rotate(180deg);
      }

      @keyframes buttonAnimation {
          0% {
              transform: scale(1);
          }

          50% {
              transform: scale(1.1);
          }

          100% {
              transform: scale(1);
          }
      }
      </style>
  </head>

  <body>
      <h1>MONIBOT</h1>
      <img src="" id="photo">
      <table>
          <tr>
              <td colspan="3" align="center">
                  <button class="button" onmousedown="toggleCheckbox('forward'); animateButton(this);"
                      ontouchstart="toggleCheckbox('forward'); animateButton(this);" onmouseup="toggleCheckbox('stop');"
                      ontouchend="toggleCheckbox('stop');">Maju</button>
              </td>
          </tr>
          <tr>
              <td align="center">
                  <button class="button" onmousedown="toggleCheckbox('left'); animateButton(this);"
                      ontouchstart="toggleCheckbox('left'); animateButton(this);" onmouseup="toggleCheckbox('stop');"
                      ontouchend="toggleCheckbox('stop');">Kiri</button>
              </td>
              <td align="center">
                  <button class="button stop" onmousedown="toggleCheckbox('stop'); animateButton(this);"
                      ontouchstart="toggleCheckbox('stop');">MONIBOT</button>
              </td>
              <td align="center">
                  <button class="button" onmousedown="toggleCheckbox('right'); animateButton(this);"
                      ontouchstart="toggleCheckbox('right'); animateButton(this);" onmouseup="toggleCheckbox('stop');"
                      ontouchend="toggleCheckbox('stop');">Kanan</button>
          </tr>
          <td colspan="3" align="center">
              <button class="button" onmousedown="toggleCheckbox('backward'); animateButton(this);"
                  ontouchstart="toggleCheckbox('backward'); animateButton(this);" onmouseup="toggleCheckbox('stop');"
                  ontouchend="toggleCheckbox('stop');">Mundur</button>
          </td>
          </tr>

          <tr>
              <td colspan="4" align="center">
                  <button class="button servo" onmousedown="toggleCheckbox('servo_up'); animateButton(this);"
                      ontouchstart="toggleCheckbox('servo_up'); animateButton(this);">Atas</button>

                  <button class="button servo" onmousedown="toggleCheckbox('servo_left'); animateButton(this);"
                      ontouchstart="toggleCheckbox('servo_left'); animateButton(this);">Kiri</button>

                  <button class="button servo" onmousedown="toggleCheckbox('servo_right'); animateButton(this);"
                      ontouchstart="toggleCheckbox('servo_right'); animateButton(this);">Kanan</button>

                  <button class="button servo" onmousedown="toggleCheckbox('servo_backward'); animateButton(this);"
                      ontouchstart="toggleCheckbox('servo_backward'); animateButton(this);">Bawah</button>
              </td>
          </tr>

          <tr>
              <td colspan="3" align="center">
                  <button class="button on" onmousedown="toggleCheckbox('lampu_hidup'); animateButton(this);"
                      ontouchstart="toggleCheckbox('lampu_hidup'); animateButton(this);">LED<br>Hidup</button>
                  <button class="button off" onmousedown="toggleCheckbox('lampu_mati'); animateButton(this);"
                      ontouchstart="toggleCheckbox('lampu_mati'); animateButton(this);">LED<br>Mati</button>
              </td>
          </tr>


      </table>
      <script>
      function toggleCheckbox(x) {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/action?go=" + x, true);
          xhr.send();
      }

      function animateButton(button) {
          button.style.animation = 'buttonAnimation 0.3s';
          setTimeout(function() {
              button.style.animation = '';
          }, 300);
      }

      window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
      </script>
  </body>

  </html>