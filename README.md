# TrafficManagementUSART
## Microcontroller Lab Assignment 02: Road Traffic Management: Traffic Signal Customization


The objectives of the lab assignment are to understand and have hands-on training in micro-
controller (STM32F446RE) UART (Universal Asynchronous Receiver Transmitter) and TIMER
configuration and the operation of UART pins and ports as buffered input and output (using
interrupt subroutine).

### Assessment Policy

The student must complete and demonstrate the assignment by the due date. After completing
the assignment student must submit a report and upload the code to the microcontroller lab
website or google classroom. The students are obligated to complete the given problem; however,
we encourage further assignment extension to replicate a realistic application environment. The
student may get bonus marks for demonstrating an innovative extension of the lab assignment.

### Alert

It would be best if you take care of the microcontroller STM32F446RE; the input power to any
GPIO pin must not exceed 3.6V. Do not damage or deform any GPIO pin, and do not use a
conducting surface as the operating podium; otherwise, it may damage the microcontroller. Always
connect the microcontroller to the computer USB type-A to type-B to power on and program.

### What to do?

The assinment has two parts: (Later see the detail description)

- Keep your previous lab (assignment 01) configuration and operation of the adaptive traffic
    management system (optional).
- Design and develop a control system for central traffic monitoring and new configuration.

### Assignment Problem Description

To understand the operation and procedure for communication with the traffic control room
through USARTx ports, you must develop and solve the following lab assignment.

### 4.1 Traffic System Monitoring and Customization

In our country, traffic police usually control road traffic; however, the government has policies
to alleviate our country into a digital and gradually smart Bangladesh. Thus it (police hand
controlled) is against the policy towards smart Bangladesh. Nowadays, the police traffic control
center (left of Figure 1) has a facility to monitor the traffic of a metropolitan city. The control
center operator can monitor the traffic (right of Figure 1) situation through surveillance cameras
(Figure 2)and guide the field officer to increase the duration of the green signal to pass the very
important personnel (VIP) quickly. It burdened the working traffic force with managing the traffic
and diverse traffic in various directions.

Moreover, this routing policy builds a chaotic condition and adds serious traffic hazards in
the city that cause suffering for citizens. In affinity with the previous lab assignment, you plan
to develop an automated system that periodically reports the traffic status with a configurable
interval. Such as, the control center can define or change the interval to five seconds; let us assume
it was three seconds previously. To do so you must use a general purpose timer to keep the time,
create delay and feed traffic to the monitoring center according to the recent configuration.

Let the control center connects the traffic light and control system (developed in the previous
lab) via wireless media (WIFI or GSM). The following figure 3 demonstrates the connections
between the monitoring and control center and field traffic control and management system. The
microcontroller connects the traffic control system through a wireless (WIFI or GSM) chip/modem.
On the other hand traffic monitoring center’s microcontroller connects the TrafficNet using another
wireless modem. For simplicity, we ignore the intermediate TrafficNet and wireless modem; instead,
we directly connect (T xa→Rxb,T xb→Rxa) two UART ports of the microcontroller.

In addition, you must use the current UART2 to display traffic information, including traffic
signal light status, on the PC display (traffic monitoring system). You must use the other two
UARTa and UARTb, to transmit and receive traffic data or configure the traffic-signal interval
and transmission to the mechanism. Your developed system must transfer the configuration to
UARTb through UARTa, and receive monitor data on UARTa from UARTb. The UARTa receives
configuration commands from the microcontroller delivered through UART2 (currently available).
Furthermore, UARTa generated a monitoring report based on data from UARTb and sent them
to the PC display system through UART2.

### Technical Specification

You must select and configure a timer to implement a delay and calculate intervals. Consult the
reference manual to configure the required timer registers. You may need a timer interrupt to
keep the time count. For the UARTa and UARTb configurations, select the appropriate UARTs
excluding USART2. We use USART2 to communicate with PC. To configure a USART (in UART
mode), you may need the following:

- Enable clocks for the selected UART
- Determine the Tx, and Rx pins for the UART and configure them using an alternate function
- Configure UART interrupt for transmission and reception. You must enable transmission
    interrupt when you want to transfer character(s), and at the end of transmission, disable
    transmission-interrupt. You can keep receiving interrupts enabled all the time. The interrupt
    service routine (ISR) will handle the action transmission and reception between UARTa and
    UARTb. Set appropriate priority. You can implement different transmission and reception
    buffers for each of the UARTx.
- Configure UART using registers related to them.
- Develop send and receive function for receiving or transmitting characters
- Display report on the PC screen in text format.

### 4.2 Control commands

The system must be capable to configure traffic light interval with the following configuration
command

config trafic light x G Y R w y z u


Where ‘x’ is the traffic light number. ‘w,’ ‘y’, and ’z’ are the values (duration of ON) in seconds
for each traffic light. Whereas value ‘u’ is the number of seconds used, increasing the duration for
light traffic on the other road of the crossroad. The above command changes the configuration of
the traffic light interval. ‘G,’ ‘Y,’ and ‘R’ are the traffic-light colors – Green, Yellow, and Red.

config traffic monitor X

Where ‘X’ is the value in seconds to configure the interval to receive traffic status for monitoring
purposes.
Next, the ‘read’ command displays the current configuration of the whole system. In the case of
‘read’ without any argument displays the configurations of traffic lights and then the configuration
of the monitoring system, such as:

traffic light 1 G Y R 10 2 10 5
traffic light 2 G Y R 10 2 10 3
traffic monitor 30

However, ‘read’ with argument must display the appropriate values. Such as ‘read traffic light
1’ displays

```
traffic light 1 G Y R 10 2 10 5
```
whereas, ‘read traffic monitor’ displays:

traffic monitor 30

Now, if the duration for traffic monitoring is set to 20 seconds, then the system displays the
last twelve traffic data (three time stamps) with an interval of 20 seconds as

```
100 traffic light 1 ON ON OFF
100 traffic light 2 OFF OFF ON
100 road north south heavy traffic
100 road east west light traffic
120 traffic light 1 OFF OFF ON
120 traffic light 2 ON OFF OFF
120 road north south heavy traffic
120 road east west heavy traffic
.............................
............................
```
100 and 120 are the time stamps.
Note: for simplicity and to minimize the number of port usage, you are permitted not to use
the previous lab setup.

### Preliminaries

#### Data Structure associate to the GPIOx port

Use the same data structure for the GPIOx port developed in the previous lab. However, you must
configure the UARTx and TIMx required for the lab. You need to consult with the Datasheet and
reference manual of stm32F446re.


### Debugging

You can use Putty, Hercules, or Teraterm for debugging and output to the screen. Use the KEIL
debugger for your convenience. It is excellent and helps you to see the inside. KEIL using GDB.


