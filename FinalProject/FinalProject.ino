#include <ESP32_MailClient.h>


#include <LiquidCrystal_I2C.h>
#include "ESP32_MailClient.h"

const char* ssid = "2WIRE832";
const char* password = "0359992962";

#define emailSenderAccount    "esp32testemail@gmail.com"    
#define emailSenderPassword   "password12$"
#define emailRecipient        "dhymes16@apu.edu"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test Subject"


SMTPData smtpData;



const int wPin = 12;
int wPinVal = 0;


// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
void sendCallback(SendStatus info);

void setup(){
  Serial.begin(115200);
  // initialize LCD
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
  Serial.println("Preparing to send email");
  Serial.println();
  
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Checking Water Level</h1><p>- Sent from ESP32 board</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();

  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

}
 
void loop(){
  // set cursor to first column, first row
  wPinVal = analogRead(wPin);
  lcd.setCursor(0, 0);
  // print message
  if(wPinVal < 450){
    lcd.println("DRY              ");
  }
  else if(wPinVal >= 450 && wPinVal < 600){
    lcd.println("WATER           "); 
  }
  else if(wPinVal >= 600){
    lcd.println("ALOT            ");
  }
  delay(1000);
  // clears the display to print new message
//  lcd.clear();
//  // set cursor to first column, second row
//  lcd.setCursor(0,1);
//  lcd.print("Hello, World!");
//  delay(1000);
//  lcd.clear(); 
}

void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
