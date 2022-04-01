euclidean-lights
==================
12 channel light installation by Ólafur Elíasson
------------------


### Hardware

The installation utilizes two redundant Arduino Uno microcontrollers, each with a [DMX shield](./documentation/DMX%20Shield%20User%20manual.pdf).  
The Arduinos are powered by a 5V Meanwell PSU. 5V VIN can be toggled with a flip switch on the front of the panel.

For saving the current state of the installation to the internal EEPROM on power loss, a simple supercapacitor circuit is wired to the power circuit, which powers the microcontroller after power loss for the amount of time needed to save the state to EEPROM. The saving function is called via PIN 13 (Which is active low, and directly connected to the 5V VIN).

### Software

Program is written in the [Platform.io](https://platformio.org/) environment, which allows C++ usage and better code/library management for *arduinoesque* microcontrollers.  

To upload code to the microcontroller, put jumper EN to <span style="text-decoration:overline">EN</span> on DMX shield. This disables usage of the serial port by the dmx shield, thus preventing USB serial connection. After successfull upload revert the position of the jumper back to EN.  

<p>&nbsp;</p>
DMX signals are ouptut on channel 1-12. Receiving end is a 12 channel DMX dimmer.  

Because of the usage of the serial port by the [*Conceptinetics*](https://github.com/alfo/arduino-libraries/tree/master/Conceptinetics) library, any usage of the serial port (Such as debugging via ``Serial.println()`` is disabled when the library is integrated.) For successfull build including the *Conceptinetics* library, delete or comment all code including ``Serial``.


<p>&nbsp;</p>

Timings for divisions and fadetimes are changed in the int[] array in [./src/variables.cpp](./src/variables.cpp) (in seconds).  

### *default timings*

<table>
  <tr>
    <th>Light #</th>
    <th>1</th>
    <th>2</th>
    <th>3</th>
    <th>4</th>
    <th>5</th>
    <th>6</th>
    <th>7</th>
    <th>8</th>
    <th>9</th>
    <th>10</th>
    <th>11</th>
    <th>12</th>
  </tr>
  <tr>
    <th>Division (in sec)</th>
    <td>34</td>
    <td>27</td>
    <td>44</td>
    <td>38</td>
    <td>23</td>
    <td>28</td>
    <td>30</td>
    <td>22</td>
    <td>29</td>
    <td>42</td>
    <td>43</td>
    <td>36</td>
  </tr>
  <tr>
    <th>Fadetime (in sec)</th>
    <td>22</td>
    <td>21</td>
    <td>27</td>
    <td>24</td>
    <td>19</td>
    <td>27</td>
    <td>29</td>
    <td>29</td>
    <td>32</td>
    <td>25</td>
    <td>26</td>
    <td>22</td>
  </tr>
</table>
