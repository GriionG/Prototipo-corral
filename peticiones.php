<?php

require 'bd/conexion_bd.php';

$obj = new BD_PDO();

// se verifica que se hayan enviado los parametros 
@$disp = $_POST['disp'];
@$temp = $_POST['temp'];
@$hum = $_POST['hum'];


if(isset($_POST['disp']))
{
	// Esta sección sería si se desea enviar los datos por el método POST
	$obj->Ejecutar_Instruccion("Insert into sentemp(fecha,hora,temperatura,humedad,id_asignacion) values(Now(),DATE_FORMAT(NOW( ), '%H:%i:%S'),$temp,$hum,$disp)");
}
elseif (isset($_GET['disp'])) 
{
	// Esta sección sería si se desea enviar los datos por la URL, utilizando el método GET
	$disp = $_GET['disp'];
	$temp = $_GET['temp'];
	$hum = $_GET['hum'];

	$obj->Ejecutar_Instruccion("Insert into sentemp(fecha,hora,temperatura,humedad,id_asignacion) values(Now(),DATE_FORMAT(NOW( ), '%H:%i:%S'),$temp,$hum,$disp)");
}
?>