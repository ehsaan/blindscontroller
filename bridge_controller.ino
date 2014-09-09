/*
1. Set up pins for motor control
2. check an external file /mnt/sd/curtain.cmd
2.1 if 0, do nothing
2.2 if 1, turn FORWARD (open), clear the file to 9
2.2 if 2, turn BACKWARD (close), clear the file to 9
2.3 if 3, turn FORWARD (open) for two thirds, clear file to 9
2.4 if 4, turn BACKWARD (close) for two thirds, clear file to 9
*/
#include <FileIO.h>
#include <Process.h>
int DIR_A = 12;
int BRK_A = 9;
int PWM_A = 3;
int DIRECTION= HIGH;
char inChar=-1;     //direction
int rotLength=1000; //time delay length
char* cmdFile = "/mnt/sd/curtain/command.cmd";
int doTurn =0;     
Process date;                 // process used to get the date
void setup() {
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  //Setup Channel A
  pinMode(DIR_A, OUTPUT); //Initiates Motor Channel A pin
  pinMode(BRK_A, OUTPUT); //Initiates Brake Channel A pin
  pinMode(PWM_A, OUTPUT); // Initialize Speed for Channel A

  
  //while(!Serial);               // wait for Serial Monitor to open
  Serial.println("Setting up Blinds Controller 0.1");  // Title of sketch

}
  
void loop() {
  File dataFile = FileSystem.open(cmdFile, FILE_READ);
  {
    if (dataFile) 
    {
      inChar = dataFile.read();
      dataFile.close();
    }  
    else 
    {
      Serial.println("error opening datalog.txt");
    }    
    switch (inChar)
    {
      case '1':
        {
          DIRECTION = HIGH;
          rotLength=1000;
          doTurn=1;
        }
        break;
      case '2':
        {
          DIRECTION = LOW;
          rotLength=1000;
          doTurn=1;
        }  
        break;
     case '3':
        {
          DIRECTION = HIGH;
          rotLength=3000;
          doTurn=1;
        }
        break;
      case '4':
        {
          DIRECTION = LOW;
          rotLength=3000;
          doTurn=1;
        }  
        break;
      case '0':
      case '9':
      default:
      {
        dataFile.close();
        doTurn=0;
      }
        break; 
    }//switch     
     if (doTurn)
     {
        digitalWrite(DIR_A, DIRECTION); 
        digitalWrite(BRK_A, LOW);   //Disengage the Brake for Channel A
        analogWrite(PWM_A, 255);   //Spins the motor on Channel A at full speed
        delay(rotLength);
        Serial.print("Rotated:"); 
        Serial.print(DIRECTION);         
        digitalWrite(BRK_A, HIGH); //Engage the Brake for Channel A
        dataFile.close();
        File myOut = FileSystem.open(cmdFile, FILE_WRITE);
        myOut.println('9');
        myOut.close(); 
     }  
   }//while data available
   delay(5000);
}
