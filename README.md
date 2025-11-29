# IOT-Based-Smart-Parking-System
The proposed Smart Parking System integrates a network of sensors, micro-controllers, cloud storage, and user-facing applications to provide a real-time, automated parking management solution. It is modular, scalable, and designed to be energyefficient and user-friendly

# The smart parking system consists of

· Sensing Layer:
Ultrasonic or IR sensors installed in each parking slot detect the presence of a vehicle. These sensors generate signals indicating whether a slot is occupied or vacant.
·Control Layer:
A micro controller unit, such as the Node MCU ESP8266, gathers data from sensors and processes it. It sends real-time status updates over Wi-Fi using MQTT or HTTP protocols.

·Cloud Layer:
Slot occupancy data is continuously synchronised to cloud services like Firebase or Thing Speak, which act as central hubs for storing and updating real-time information.. It serves as the back end for real-time
synchronisation between hardware and user interfaces.

·User Interface Layer:
A mobile app  and a web application (built with HTML, CSS, JavaScript) allow users to:
View real-time slot availability
Reserve a parking space
Navigate to the location using GPS

