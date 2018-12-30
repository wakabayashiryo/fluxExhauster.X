# flux Exhauster Repository

- ### pin definitation
	|GPIOx | contents|
	|:--:|:--|
	|GPIO0|	ADC input|
	|GPIO1|	Status LED|
	|GPIO2|	Tact Switch 1|
	|GPIO3|	Tact Switch 2|
	|GPIO4|	Fan control|
	|GPIO5|	LED control|

- ### battery monitor design
	V_max : The Voltage that battery is full charged
	V_end : The End voltage when battery is empty
	V_adc : The input voltage of ADC
	Value_adc : The value is V_adc was converted by ADC

- ### conditions
	- partial voltage 
		R1 = R2 = 5.6k
		Ration = 1/2 
	- The voltage when battery is full charged 
		V_max = 6.0V = 1.5V x 4
		V_ adc = 3.0V = V_max x Ration
	- The End voltage of battery
		V_end = 4.2V = 1.05V x 4
		V_adc = 2.1V = V_end x Ration
		
	- ADC specification
		- Reference voltage
			V_ref = V_dd = 3.3V
		- Resolution
			res = 10bits = 1024 steps
- ### Calculate ADC value when end voltage
	V_adc = V_end x 1/2 
	V_adc = (V_ref / res) x Value_adc
	
	- The final expression
		Value_adc = (res x V_end) / (2 x V_ref)
		(1024 x 4.2) / (2 x 3.3) = 651
	### **Notify When the ADC value is 651 or less**