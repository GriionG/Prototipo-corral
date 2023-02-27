<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Examen parcial 2</title>
  <link rel="stylesheet" href="./bootstrap.min.css">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.3.0/css/all.min.css" integrity="sha512-SzlrxWUlpfuzQ+pcUCosxcglQRNAq/DZjVsC0lE40xsADsfeQoEypE+enwcOiGjk/bSuGGKHEyjSoQ1zVisanQ==" crossorigin="anonymous" referrerpolicy="no-referrer"/>
  <script src="./jquery.min.js"></script>
  <script type="text/javascript">
    function Iluminacion(v) {
      const Url = "http://192.168.43.250/puerta/" + v;
      $.ajax({
        url: Url,
        type: "GET",
        success: function(datos) {
          console.log(datos);
          // datos = JSON.parse(datos);
          document.getElementById('dist').innerHTML = datos.distancia;
          document.getElementById('cont').innerHTML = datos.contador;
          document.getElementById('puerta').innerHTML = datos.puerta;
          document.getElementById('temp').innerHTML = datos.temperatura;
        }
      });
    }

    function Cargar_Datos() {
      const Url = "http://192.168.43.250";
      $.ajax({
        url: Url,
        type: "GET",
        success: function(datos) {
          console.log(datos);
          document.getElementById('dist').innerHTML = datos.distancia;
          document.getElementById('cont').innerHTML = datos.contador;
          document.getElementById('puerta').innerHTML = datos.puerta;
          document.getElementById('temp').innerHTML = datos.temperatura;
        }
      });
    }

    function Traer_temp() {
      const Url = "http://192.168.43.250/luz/";
      $.ajax({
        url: Url,
        type: "GET",
        success: function(datos) {
          console.log(datos);
          document.getElementById('dist').innerHTML = datos.distancia;
          document.getElementById('cont').innerHTML = datos.contador;
          document.getElementById('puerta').innerHTML = datos.puerta;
          document.getElementById('temp').innerHTML = datos.temperatura;
        }
      });
    }

    function color(){
      let etiqueta = document.querySelector('#temp');
      if (etiqueta.innerHTML <= 25){
        etiqueta.style.color="red";
      } else if (etiqueta.innerHTML > 25){
        etiqueta.style.color="green";
      }
    }

    setInterval(() => {
      Cargar_Datos(""),color()
    }, 1000);
  </script>

</head>

<body>

  <div class="container-fluid bg-light">
    <div class="container">
      <nav class="navbar navbar-expand-lg bg-light">
        <div class="container-fluid">
          <h3>Examen Corral</h3>
        </div>
      </nav>
    </div>
  </div>

  <div class="w-50 p-3 container-xxl mt-4">
    <div class="mx-auto">
      <div class="card">
        <div class="card-body">
          <div>
            <div class="abs-center" >
              <h2 class="text-center">Informacion del corral</h2>
                <p class="fs-4 text-center" >Distancia: <span id="dist"></span></p>
                  <p class="fs-4 text-center" >Contador: <span id="cont"></span></p>
                  <p class="fs-4 text-center" >Temperatura: <span id="temp"></span></p>

            </div>
            <div class="text-center">
              <h2>Control de puerta</h2>
                <p class="fs-4 text-center" >Puerta: <span id="puerta"></span></p>
                <button type="button" class="btn btn-success" id="btnencender" name="btnencender" value="ON" onclick="javascript: Iluminacion('ABIERTO');"><i class="fa-solid fa-power-off"></i></button>
                <button type="button" class="btn btn-danger" id="btnapagar" name="btnapagar" value="OFF" onclick="javascript: Iluminacion('CERRADO');"><i class="fa-solid fa-power-off"></i></button>
                <i class="" id="puertaico"></i>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>

</body>

</html>