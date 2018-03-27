# busTrackingAndSeatAvailability
FOLLOW THESE STEPS TO UNDERSTAND THE PROJECT
Step00:
Install the Arduino software provided (software folder) or latest, then install the libraries
in the Libraries folder(goto Sketch>include library>include Zip library>brouse to zip file
provided)
Step01:
Have a look on hardware, open the block diagram and circuit diagram. Compare the
hardware connections to the connections in the cktDiagram(blockDiagram folder).
Step02:
Open the code in the firmware folder(project01.ino).each line of the code well
commented to understand. check I/O pins declared in the code mapped with physical
connections.
Step03:
Do any modifications u want like changing delay for IR sensors for the count. Connect
the arduino board to computer then select the port and board (goto tools>port>
,,tools>boards>arduinoUNO) then upload the code by clicking on the right arrow symbol.
Step04:
Warning: while uploading the code the two switches provided on the board must be
turned off because the same Rx,Tx lines used for gsm communication and arduino flash.
Step05:
Functionality: first checks for GSM connection and Network then prints the available
seats. Two IR sensors provided one for incoming count another for outgoing. If anyone enters
into the bus the number of seats will be decreased. If we send a message to the module with
proper password then the device replies with location link and available seats.
Step06:
Errors: at the time of reply operation the IR sensors will not work for 1sec because controller
cannot do two simultaneous operations at a time.
