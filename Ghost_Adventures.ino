#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
                           // as described in the footer comment below
                           
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

                           

// the Account SID from your Twilio account
const String TWILIO_ACCOUNT_SID = "sid";

// the Auth Token from your Twilio account
const String TWILIO_AUTH_TOKEN = "token";

// your Twilio phone number, e.g., "+1 555-222-1212"
const String TWILIO_NUMBER = "+your twilio number";

// the number to which the SMS should be sent, e.g., "+1 555-222-1212"
const String RECIPIENT_NUMBER = "+your phone number";

// a flag to indicate whether we've attempted to send the SMS yet or not
boolean attempted = false; 

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  // for debugging, wait until a serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    delay(150);
    

 if (pirState == LOW) {
      // we have just turned on
      Serial.println("Ghost detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
    
    
            Serial.println("Running SendAnSMS...");
    
    // we need a Process object to send a Choreo request to Temboo
    TembooChoreo SendSMSChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked and repopulated with
    // appropriate arguments each time its run() method is called.
    SendSMSChoreo.begin();
    
    // set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Twilio > SMSMessages > SendSMS)
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Twilio/SMSMessages/SendSMS/ 
    // for complete details about the inputs for this Choreo

    // the first input is a your AccountSID
    SendSMSChoreo.addInput("AccountSID", TWILIO_ACCOUNT_SID);
    
    // next is your Auth Token
    SendSMSChoreo.addInput("AuthToken", TWILIO_AUTH_TOKEN);
 
    // next is your Twilio phone number
    SendSMSChoreo.addInput("From", TWILIO_NUMBER);
    
    // next, what number to send the SMS to
    SendSMSChoreo.addInput("To", RECIPIENT_NUMBER);

    // finally, the text of the message to send
    SendSMSChoreo.addInput("Body", "Ghost Adventures Ghost Alert!!");
    
    // tell the Process to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = SendSMSChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Serial.println("SMS sent");
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (SendSMSChoreo.available()) {
        char c = SendSMSChoreo.read();
        Serial.print(c);
            }
    } 
    SendSMSChoreo.close();
    
    // set the flag indicatine we've tried once.
    attempted=true;
    
    
    
    
  } else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      delay(300);    
      if (pirState == HIGH){
      // we have just turned of
      Serial.println("Coast is clear!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}











