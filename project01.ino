#include <SoftwareSerial.h>                               //To Creat a software Serial communication.
#include <LiquidCrystal.h>                                //to establish 16x2 lcd connection
#include <TinyGPS++.h>                                    //this is to encode GPS data
#define in    2                                           //defining the pins for the IR sensors
#define out   3
char   msg[32],str[100],phh[100];                         //creating charactor arrays 
int    i=0,x=0,k=0,temp=0,n=0,p=0,b=0;   //creating some integers
String stringOne,stringTwo,stringThree,stringZero;        //creating some strings
String psw,replay,sendd,stringFour,seatss="20";
 int count=0,seats=20;
SoftwareSerial gpsSerial(4, 5);                           //creating software serial on 4,5 pins on arduino Rx,Tx
TinyGPSPlus    gps;                                       //creating a intstance for the GPS
LiquidCrystal  lcd(9, 8, 13, 12, 11, 10);                 //lcd connected on these pins(creating an instance)

void setup()                                              //one time execution code will be placed here 
{ 
  lcd.begin(16,2);                                        //initializing the LCD with 16 coloums and 2 rows  
  lcd.clear(); 
  lcd.print(" BUS TRACKING &");                           //prints on the screen only once
  lcd.setCursor(0,1);
  lcd.print(" SEATS AVAILABLE");
  delay(2000);
  gpsSerial.begin(9600);                                  //initialising GPS serial communication on software serial pins with baudrate 9600
  Serial.begin(9600);                                     // Setting the baud rate of Serial Monitor (Arduino)
  lcd.clear();
  gsm_init();                                             //this initialises the GSM module 
  pinMode(in,  INPUT);                                    //Counter pin for incoming passengers setting as input
  pinMode(out, INPUT);  //Counter pin for outgoing passengers setting as input (sensors to be connected to these pins)
  lcd.clear();
  lcd.println("SEATS LEFT:");
  lcd.setCursor(11,0);                               // sets curser at 11th coloum and 0th row
  lcd.print(seats);
  lcd.print("      ");
}

void loop()                                               //which runs forever
{
  while(gpsSerial.available() > 0)                        //GPS data will be updated for every second
  gps.encode(gpsSerial.read());                           //encode gps data from NMEA format to readable format 

  if (gps.location.isUpdated())                           //whenever the location updated this executes
  {                                                       //the below link compatable with Google maps-obtained from Google developers
   stringZero  = String("https://www.google.com/maps/search/?api=1&query=");
   stringOne   = String(gps.location.lat(),8);            //stores latitude in stringOne
   stringTwo   = String(gps.location.lng(),8);            //stores longitude value in stringTwo
   stringFour  = stringZero + stringOne + ',' + stringTwo+ stringThree;
  }
  
  if(temp == 0)RecieveMessage();                          //If temp variable is 0 then it will be in receive message mode
  for(unsigned int t=0;t<6000;t++)                        //This is the loop for checking the message
   {
    serialEvent();                                        //this tells the proper message received or not
    if(temp==5)                                           //temp variable will be 5 whenever serial event conforms a proper message
     {
      x=0,k=0,temp=0;                                    
      while(x<i)
       { 
        while(str[x]=='#')                               //message string starts from # detection
         {
          x++;
          while(str[x]!='*')                             //message string ends with * detection
           {
            msg[k++]=str[x++];                           
           }
         }
          x++;                                          
        }
       msg[k]='\0';                                      //message character array ends with null character
       Serial.println(msg);                              //prints received message on serial monitor
       if(p>0)                                           //this loop gets the phone number
        {
         for(i=0;i<13;i++)                               //including +91 total 13 chareters
         {
          phh[b++]=str[p++];
         }
        }
        Serial.println(phh);                            //prints phh number on serial monitor
        psw= (String)msg;                               //stores message in psw string
        replay=(String)phh;                             //stores phone number in reply string
        delay(1000);                                    //
        if(psw == "getloc")SendMessage();               //if the message contains getdata string it replies back
        p=0,i=0,x=0,k=0,n=0,b=0;                        //making all variables 0
      }
    } 
     if(!digitalRead(in))                               //if there is any change at incoming sensor
     IN();                                              //IN function will be called
     if(!digitalRead(out))                              //if there is any change at outgoing sensor
     OUT();                                             //then OUT function will be called
     
     seatss=(String)seats;                              //converting integer to string
    
     stringThree = String(" \nSeats Available: "+seatss);//
}

void SendMessage()                                      //to send a message this function will be called
{
  Serial.println("AT+CMGF=1");                          //getting into text mode
  //sendd=String("AT+CMGS=\"")+replay+String("\"\r");     //assigning the required strings to send a message
  //Serial.println(sendd);                                //the above string will be printed
  Serial.println("AT+CMGS=\"+08106005725\"\r");
  delay(1000);  
  Serial.println(stringFour);                           // The SMS text you want to send 
  delay(100);
  Serial.println((char)26);                             // ASCII code of CTRL+Z to terminate the entering text mode
  delay(3000);
  temp=0;
  
}

void RecieveMessage()                                   //this function called for receiving message mode
{                                         
  Serial.println("AT+CNMI=2,2,0,0,0");                  // AT Command to recieve a live SMS
  delay(500);

  temp=2;
}
 
void serialEvent()                                      //this function is called to check the received message
{
 while(Serial.available())                              //
  {
   char ch=(char)Serial.read();                     
   str[i++]=ch;
   if(ch== '+')
    {
     if(i-n==7)
      {
        p=i-1,n=0;
      }  
      n=i;
    }
     if(ch == '*')
      {
       temp=5;
       Serial.println("Message Received");
      }
  }
}

void gsm_init()                                           //this function calls to initialise the GSM module
{
  lcd.println("Initializing GSM......");
 boolean at_flag=1;                                       //
 while(at_flag)
  {
   Serial.println("AT");
   while(Serial.available()>0)
    {
     if(Serial.find("OK"))
     at_flag=0;
    }
    delay(1000);
  }
  boolean echo_flag=1;
  while(echo_flag)
   {
    Serial.println("ATE0");                               //to check network signal
    while(Serial.available()>0) 
     {
      if(Serial.find("OK"))
      echo_flag=0;
     }
     delay(1000);
   }
   boolean net_flag=1;
   while(net_flag)
    {
     Serial.println("AT+CPIN?");
     while(Serial.available()>0)
     {
       if(Serial.find("+CPIN: READY"))net_flag=0;
      }
      delay(1000);
    }
}
void OUT()                                                  //this function is called whenever the outgoing sensor toggled
{
    count--;
    lcd.clear();
    lcd.println("SEATS LEFT:");
    if(count<0)count=0;
    seats =20-count;
    lcd.setCursor(11,0);
    lcd.print(seats);
    lcd.print("           ");
    delay(450);
}
void IN()                                                   //this function is called whenever the incoming sensor toggled
{
    count++;
    lcd.clear();
    lcd.print("SEATS LEFT:");
    if(count>20)count=20;
    seats =20-count;
    lcd.setCursor(11,0);
    lcd.print(seats);
    delay(450);
}
