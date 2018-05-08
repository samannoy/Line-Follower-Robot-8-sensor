////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Line Follower Robot with 8 sensor array////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////Project Contributors: Samannoy Ghosh, Ritam Ghosh///////////////////////////////////////////////////////////////
////////Special Thanks to Sagnik Roy////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////Details: Line Follower Robot for a fast race track of white or black color//////////////////////////////////////


String readString;    //string to read data from android app using bluetooth
int s[8],fspeed=190;
int kp=18,kd=65,temp,sum,error=0,preverr=0,rate=0,corr,poscor,negcor,flag=0,d=0,j, flag2=0;
float ki=0,i=0;

void setup() 
{  
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(12,OUTPUT); /* 4 and 7 control the direction of right wheel. 3 controls the speed of right wheel. 8 and 12 control the direction of  wheel. 9 controls the speed of left wheel. */
  pinMode(13,OUTPUT); /* LED Pin, useful as indicator for checkpoints */
  pinMode(A0,INPUT);      //
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);      //8 sensor input pins
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);       //
}


//Custom functions are defined for use of use

void forward(float lspeed, float rspeed)
{
  digitalWrite(4,HIGH);
  digitalWrite(7,LOW);
  analogWrite(3,rspeed);
  digitalWrite(8,HIGH);
  digitalWrite(12,LOW);
  analogWrite(9,lspeed);
}

void backward(float lspeed, float rspeed)
{
  digitalWrite(4,LOW);
  digitalWrite(7,HIGH);
  analogWrite(3,rspeed);
  digitalWrite(8,LOW);
  digitalWrite(12,HIGH);
  analogWrite(9,lspeed);
}

void left(float lspeed, float rspeed)
{
  digitalWrite(4,HIGH);
  digitalWrite(7,LOW);
  analogWrite(3,rspeed);
  digitalWrite(8,LOW);
  digitalWrite(12,HIGH);
  analogWrite(9,lspeed);
}

void right(float lspeed, float rspeed)
{
  digitalWrite(4,LOW);
  digitalWrite(7,HIGH);
  analogWrite(3,rspeed);
  digitalWrite(8,HIGH);
  digitalWrite(12,LOW);
  analogWrite(9,lspeed);
}

void sensors()
{
  //storing sensor input data on an array
  
  s[0]=digitalRead(A0);
  s[1]=digitalRead(A1);
  s[2]=digitalRead(A2);
  s[3]=digitalRead(A3);
  s[4]=digitalRead(A4);
  s[5]=digitalRead(A5);
  s[6]=digitalRead(5);
  s[7]=digitalRead(6);

  if(s[0]==1 && s[7]==1) //black on white track
  flag2=1;

  if(s[0]==0 && s[7]==0) //white on black track
  flag2=0;

  if(flag2==1)
  {
    for(j=0;j<8;j++)
    {
    s[j]=1-s[j];
    }
  }

  sum=s[0]+s[1]+s[2]+s[3]+s[4]+s[5]+s[6]+s[7];        //s[0] is the left most sensor and s[7] is the right most sensor
}

void loop()
{


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//The following code is used to get the values of the pid tuning parameters
//from android phone. Instead of hardcoding the pid constant values and checking
//each time and correcting them after every run, the values are being sent
//through a bluetooth module HC-05 from an android app to the bot. The below
//code fetches the data received from the app and correspondingly saves the pid
//constant value during the run. In this way the bot can be tuned without stopping
//and uploading code after each run. 
// ***however the values will be deleted when powered down.If you want to store
//the values permanently, use EEPROM read and write commands Carefully!!


 while ( Serial.available() )
   {    // While there is data in the buffer
     delay( 3 ); 
     char c = Serial.read();
     readString += c;         
   }
 if ( readString.length() >0 )
 {
    Serial.println( readString );
         if ( readString[0] == 'p' ) //to check whether it is P or I or D
         {
          readString.remove(0,2); //remove p and =
          kp=readString.toInt();  //convert string to int and store in kp
         }
    
         if ( readString[0] == 'i' ) //to check whether it is P or I or D
         {
          readString.remove(0,2); //remove i and =
          int temp=readString.toInt(); //convert string to int and store in temp
          ki=(temp/1000000);  // convert int to float of the order of 0.000001
         }
         if ( readString[0] == 'd' ) //to check whether it is P or I or D
         {
          readString.remove(0,2); //remove d and =
          kd=readString.toInt();  //convert string to int and store in kd
         }
    
         if ( readString[0] == 's' ) //to check whether it is P or I or D or fspeed
         {
          readString.remove(0,2); //remove s and =
          fspeed=readString.toInt();  //convert string to int and store in fspeed
         }
    readString=""; 
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

  sensors();      //initialising the sensors to start reading data
  
  if(s[0]==1 && s[7]==0)      //sharp left or a left 90 degree turn
       {
         if (d==0)
         {flag=1;
          d=500;}
          
       }
  
  if(s[0]==0 && s[7]==1)      //sharp right or a right 90 degree turn
       {
         if (d==0)
         {flag=2;
         d=500;}
       }

 if (sum==0)                  //no line detected by the sensors
  {
    
    if (flag==1)              //remembers the last flag set previously and acts corrspondingly
     left(140,140);
    else if (flag==2)
     right(140,140);
     }

  else 
  
  {
    if ((sum==1 && (s[3]==1 || s[4]==1)) || (s[3]==1 && s[4]==1))     //4 point plus like crossing or a straight line
   error=0;
 
   else 
    error=((-3.5*s[0])+(-2.5*s[1])+(-1.5*s[2])+(-0.5*s[3])+(0.5*s[4])+(1.5*s[5])+(2.5*s[6])+(3.5*s[7]))*2/sum;
 
  
  rate=error-preverr;
  i=i+error;
  corr=kp*error+kd*rate+ki*i;
  
  poscor=fspeed+corr;
  negcor=fspeed-corr;
      
      if(poscor>255)
        poscor=255;   //set max pwm as 255
      if (poscor<-255)
        poscor=-255;
      if(negcor>255)
        negcor=255;
      if(negcor<-255)
        negcor=-255;
      
      if(poscor<0)
        left(-poscor,negcor);
      else if(negcor<0)
        right(poscor,-negcor);
      else 
        forward(poscor,negcor);
  }
 
  
      preverr=error;    //storing the error value in preverr
      if (d>0)
       d--;      
  
}


