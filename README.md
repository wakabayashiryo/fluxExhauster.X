# flux Exhauster Repository

- ### pin definitation
	|GPIOx | contents|
	|:--:|:--|
	|GPIO0|	Detect voltage|
	|GPIO1|	Status LED|
	|GPIO2|	Tact Switch 1|
	|GPIO3|	Tact Switch 2|
	|GPIO4|	Fan output|
	|GPIO5|	BCM output|

- ### battery monitor design
	~~~math
	V_cc  : battery voltage
	V_adc : The input of ADC voltage
	Value_adc : The value is V_adc was converted
	V_max : The Voltage that battery is full charged
	V_low : The notify when Low voltage 
	
	V_adc =  \frac{Value_adc * V_ref}/{2_{10}}
	V_adc = V_cc * 	
	
	
	
	~~~
	
	
//Full charged 6.0V = 1.5V x 4
//Input voltage when full charaged: 3.0V
//R1 = 5.6k
//R2 = 5.6k
//End voltage:4.2V = 1.05V x 4
//Input voltage when end voltage:2.1V
//ADC value :651
