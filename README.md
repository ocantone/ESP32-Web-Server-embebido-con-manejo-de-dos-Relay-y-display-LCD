# ESP32 Web Server embebido con manejo de dos Relay y display LCD via bus I<sup>2</sup>C
<p>
  Servidor Web con ESP32:<br/>
  Se trata de un microcontrolador ESP32 que act&uacute;a como servidor HTTP (puerto 80).<br/>
  Incluye manejo de un display LCD 16x2 (H44780) conectado v&iacute;a bus I<sup>2</sup>C y dos rel&eacute; para control de dispositivos el&eacute;ctricos.<br/>
  El micro ESP32 se conecta por WiFi, mostrando la dirección IP obtenida v&iacute;a DHCP en el display.<br/>
  Tambi&eacute;n muestra en el display el elestado de los dos rel&eacute;s (ON-OFF)<br/>
  El estado de los rel&eacute; puede modificarse desde la p&aacute;gina web embebida en el microcontrolador.
  <p>
  Este sketch se basa en el ejemplo original Web Server de Rui Santos. Modificado Osvaldo Cantone, Ramos Mej&iacute;a, ARGENTINA, Agosto de 2022.
  <p>
  - Osvaldo Cantone  ocantone@gmail.com
    <p alingn=center>
    <img src=images/ESP32WebServer.png width="80%">
