#include <ButtonEvent.h>

short Button1 = 230;
short Button2 = 365;
short Button3 = 460;
short Button4 = 510;
short Button5 = 600;

void setup() {
  //initial buffer for 5 buttons
  ButtonEvent.initialCapacity = sizeof(ButtonInformation)*5;

  ButtonEvent.addButton(0,        //analog button pin
                        Button1,  //analog value
                        40,       //deviation
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //onDouble event function
                        200);     //double time interval

  ButtonEvent.addButton(0,        //analog button pin
                        Button2,  //analog value
                        40,       //deviation
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //onDouble event function
                        200);     //double time interval

  ButtonEvent.addButton(0,        //analog button pin
                        Button3,  //analog value
                        40,       //deviation
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //onDouble event function
                        200);     //double time interval
                        
  ButtonEvent.addButton(0,        //analog button pin
                        Button4,  //analog value
                        40,       //deviation
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //onDouble event function
                        200);     //double time interval
                        
  ButtonEvent.addButton(0,        //analog button pin
                        Button5,  //analog value
                        40,       //deviation
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //onDouble event function
                        200);     //double time interval
						

  Serial.begin(9600);
}


void loop() {
  ButtonEvent.loop();
}

void onDown(ButtonInformation* Sender) {
  Serial.print("Button ");

  if (Sender->analogValue == Button1)
    Serial.print(1);
  else if (Sender->analogValue == Button2)
    Serial.print(2);
  else if (Sender->analogValue == Button3)
    Serial.print(3);
  else if (Sender->analogValue == Button4)      
    Serial.print(4);  
  else if (Sender->analogValue == Button5)      
    Serial.print(5);  

	
  Serial.println(" down!");
}

void onUp(ButtonInformation* Sender) {
  Serial.print("Button ");

  if (Sender->analogValue == Button1)
    Serial.print(1);
  else if (Sender->analogValue == Button2)
    Serial.print(2);
  else if (Sender->analogValue == Button3)
      Serial.print(3);
  else if (Sender->analogValue == Button4)      
    Serial.print(4);  
  else if (Sender->analogValue == Button5)      
    Serial.print(5);  
      

  Serial.println(" up!");
}

void onHold(ButtonInformation* Sender) {
  Serial.print("Button ");

  if (Sender->analogValue == Button1)
    Serial.print(1);
  else if (Sender->analogValue == Button2)
    Serial.print(2);
  else if (Sender->analogValue == Button3)
    Serial.print(3);
  else if (Sender->analogValue == Button4)      
    Serial.print(4);     
  else if (Sender->analogValue == Button5)      
    Serial.print(5);  

  Serial.print(" hold for ");
  Serial.print(Sender->holdMillis);
  Serial.println("ms!");
}

void onDouble(ButtonInformation* Sender) {
  Serial.print("Button ");

  if (Sender->analogValue == Button1)
    Serial.print(1);
  else if (Sender->analogValue == Button2)
    Serial.print(2);
  else if (Sender->analogValue == Button3)
    Serial.print(3);
  else if (Sender->analogValue == Button4)      
    Serial.print(4);  
  else if (Sender->analogValue == Button5)      
    Serial.print(5);  

  Serial.print(" double click in ");
  Serial.print(Sender->doubleMillis);
  Serial.println("ms!");
}