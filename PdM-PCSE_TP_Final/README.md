# TP Final PdM & PCSE
 
## Menu de selección con pulsador y LCD16x2
 
_Christian S. - Diciembre 2023_  

Placa utilizada: NUCLEO STM32-F103RB  

Modulos utilizados: I2C, GPIO, UART, TIMER  

### Descripción

**Navegación**
El sistema permite la navegación por un menu de opciones visualizado en un display LCD 16x2 (comunicación I2C), por medio de un único pulsador (B1 azul NUCLEO).  
La navegación y selección por el menu se basa en la Máquina de Estados Finitos (MEF).  

Control por el menu:  
- Avance: un pulso de B1 inferior a T0 (500 ms) indica el avance a la siguiente opción.  
- Selección/ejecución: un pulso de B1 superior a T1 (1000 ms) indica la selección/ejecución de la opcion actual.  
  
**Opciones disponibles**  

1-LED ON >> encendido del LD2 led de la Nucleo  
2-LED OFF >> apagado del LD2 led de la Nucleo  
3-BLINK LED >> parpadeo del led con un periodo de semiciclo definido un _periodo_ y cantidad de _repeticiones_ ingresadas en la configuracion inicial  
4-MOTOR CW+ >> giro del motor paso a paso en sentido horario con la _frecuencia_ y _duracion_ ingresadas en la configuracion inicial  
5-MOTOR CCW- >> giro del motor paso a paso en sentido anti-horario con la _frecuencia_ y _duracion_ ingresadas en la configuracion inicial  
6-FREE >> opcion libre. Genera una animacion en el LCD.

Las opciones (3) a (5) permiten la detencion al presionar el pulsador.

**Inicio y configuracion**  
Al iniciar el sistema, pueden ingresarse nuevos parámetros por medio de la terminal serie, a utilizar en las opciones (3) a (4), o presionar ENTER y aceptar los valores por defecto.

**Accionamiento del motor**  
El motor paso a paso de 12V 4W es controlado por medio del integrado DRV8825, el cual posee 3 entradas principales:  
- STEP: pulso de avance de cada paso. El control aqui se basa en una senal Output Compare que genera el Toggle del pin STEP con TIMER2.
- DIR: sentido de giro del motor. HIGH/LOW
- EN: habilitacion del controlador. LOW/habilitado

**DRV8825**  

![image](https://github.com/Christianjs89/tpGIT_cjs/assets/62712067/d95444c9-d635-4515-8b6c-3cc4cba85aac)


**Esquema de componentes**  

![Esquema_completo_NavegadorMenu](https://github.com/Christianjs89/tpGIT_cjs/assets/62712067/e2d22ba6-78cd-4a6f-bba4-70367ce09b2c)


**Maquina de Estados Finitos**  

<img width="960" alt="MEF" src="https://github.com/Christianjs89/tpGIT_cjs/assets/62712067/f6e6b3dd-71a5-4e95-a916-84c12de70752">
