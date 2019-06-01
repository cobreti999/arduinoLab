#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN

#define pinMotor1PWM 11
#define pinMotor2PWM 3
#define pinMotor3PWM 5
#define pinMotor4PWM 6

#define qtdeCI   1

#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4
#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6

int j=0;

int buzzer = 13; //Pino do Buzzer
int BTState = 1; // Pino do Bluetooth
//int ledfrente = 2;
//int ledtras = 13;

char state;       //Letra recebida pelo Bluetooth 

void ci74HC595Write(byte pino, bool estado);

void setup() 
{
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinEnable, OUTPUT);
   pinMode(pinDS, OUTPUT);

   pinMode(pinMotor1PWM, OUTPUT);
   pinMode(pinMotor2PWM, OUTPUT);
   pinMode(pinMotor3PWM, OUTPUT);
   pinMode(pinMotor4PWM, OUTPUT);

   digitalWrite(pinEnable, LOW);

   pinMode(BTState,INPUT);

   Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {

    state = Serial.read();
    
    if(state == 'F')        //Frente(Sentido Horário)
    {
      ci74HC595Write(bitMotor1A, LOW);//LEDS TRAS
      ci74HC595Write(bitMotor1B, LOW);//LEDS TRAS
      ci74HC595Write(bitMotor2A, LOW);//LEDS FRENTE
      ci74HC595Write(bitMotor2B, HIGH);//LEDS FRENTE
      ci74HC595Write(bitMotor3A, LOW);
      ci74HC595Write(bitMotor3B, HIGH);
      ci74HC595Write(bitMotor4A, HIGH);
      ci74HC595Write(bitMotor4B, LOW);
      analogWrite(pinMotor1PWM, 60);
      analogWrite(pinMotor2PWM, 60);
      analogWrite(pinMotor3PWM, 255);
      analogWrite(pinMotor4PWM, 255);
      //digitalWrite(pinfrente, HIGH)

    }
    else if (state == 'S')     //Freiando
    {
      ci74HC595Write(bitMotor3A, LOW);
      ci74HC595Write(bitMotor3B, LOW);
      ci74HC595Write(bitMotor4A, LOW);
      ci74HC595Write(bitMotor4B, LOW);
      analogWrite(pinMotor3PWM, 255);
      analogWrite(pinMotor4PWM, 255);  

    }
    else if (state == 'B')    //Dando Ré(Sentido Anti-Horário)
    {
      ci74HC595Write(bitMotor1A, HIGH);//LEDS TRAS
      ci74HC595Write(bitMotor1B, LOW);//LEDS TRAS
      ci74HC595Write(bitMotor2A, LOW);//LEDS FRENTE
      ci74HC595Write(bitMotor2B, LOW);//LEDS FRENTE
      ci74HC595Write(bitMotor3A, HIGH);
      ci74HC595Write(bitMotor3B, LOW);
      ci74HC595Write(bitMotor4A, LOW);
      ci74HC595Write(bitMotor4B, HIGH);
      analogWrite(pinMotor1PWM, 60);
      analogWrite(pinMotor2PWM, 60);
      analogWrite(pinMotor3PWM, 255);
      analogWrite(pinMotor4PWM, 255);

    }
    else if (state == 'R')    //Virando para Direita
    {
      digitalWrite(pinMotor1PWM, HIGH);
      digitalWrite(pinMotor2PWM, HIGH);
      ci74HC595Write(bitMotor3A, LOW);
      ci74HC595Write(bitMotor3B, HIGH);
      ci74HC595Write(bitMotor4A, HIGH);
      ci74HC595Write(bitMotor4B, HIGH);
      analogWrite(pinMotor3PWM, 255);
      analogWrite(pinMotor4PWM, 255);
    }
    else if (state == 'L')    //Virando para Esquerda
    {
      digitalWrite(pinMotor1PWM, HIGH);
      digitalWrite(pinMotor2PWM, HIGH);
      ci74HC595Write(bitMotor3A, HIGH);
      ci74HC595Write(bitMotor3B, HIGH);
      ci74HC595Write(bitMotor4A, HIGH);
      ci74HC595Write(bitMotor4B, LOW);
      analogWrite(pinMotor3PWM, 255);
      analogWrite(pinMotor4PWM, 255);
    }

    else if (state == 'V') //Aciona o Toner uma vez
    { 
        if (j == 0) 
        {
            tone(buzzer, 1000);
            j = 1;
        }
        else if (j == 1) 
        {
            noTone(buzzer);
            j = 0;
        }
        state  = 'v';
    }
       
  }
  
}

void ci74HC595Write(byte pino, bool estado) 
{
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
  
          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
          
          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
          
          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}
