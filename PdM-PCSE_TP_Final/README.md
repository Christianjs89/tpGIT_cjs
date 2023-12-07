# TP Final PdM & PCSE
 
## Menu de selección con pulsador y LCD16x2
 
*Christian S. - Diciembre 2023 *  

Placa utilizada: NUCLEO STM32-F103RB  

Modulos utilizados: I2C, GPIO, UART, TIMER  

** Descripcion **
* Navegación *
El sistema permite la navegación por un menu de opciones visualizado en un display LCD 16x2 (comunicación I2C), por medio de un único pulsador (B1 azul NUCLEO).  
Control por el menu:  
- Avance: un pulso de B1 inferior a T0 (500 ms) indica el avance a la siguiente opción.  
- Selección/ejecución: un pulso de B1 superior a T1 (1000 ms) indica la selección/ejecución de la opcion actual.  
  
* Opciones disponibles *


* Inicio y configuracion *

