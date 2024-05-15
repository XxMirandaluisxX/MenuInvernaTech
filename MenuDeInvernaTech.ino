#include "Wire.h"            // Incluye la librería Wire para la comunicación I2C
#include "I2C_LCD12864.h"    // Incluye la librería I2C_LCD12864 para controlar el display LCD
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "config.h"
#include <HX711_ADC.h>
#include <NewPing.h>
#include <Servo.h>
#include <stdbool.h>

#define TRIGGER_PIN  38  //Pin trigger del sensor ultrasonico de nivel
#define ECHO_PIN     37  //Pin echo del sensor ultrasonico de nivel
#define MAX_DISTANCE 200 //Distancia maxima

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //Parametros para el sensor de nivel

//Se establecen los servos que se van a utilizar
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;
Servo servo9;
Servo servo10;
Servo servo11;
Servo servo12;

//Declaración de las galgas extensiometricas que se van a utilizar
HX711_ADC Galga1(23, 22);
HX711_ADC Galga2(25, 24);
HX711_ADC Galga3(27, 26);
HX711_ADC Galga4(29, 28);
HX711_ADC Galga5(31, 30);
HX711_ADC Galga6(33, 32);

I2C_LCD12864 lcd(&Wire);     // Crea un objeto 'lcd' de tipo I2C_LCD12864 y lo inicializa con el objeto Wire para la comunicación I2C

//Variables maquina de estados
int n_state = 0;
int p_state = 0;
int input = 0;

//Variables de los botones
int botonArriba = 34;
int botonAbajo = 35;
int botonOk = 36;
int botonCancelar = 37;

//Variables del motor
int motor = 46;
bool EstadoMotor = false;

//Variables de selección
String Cultivo = "";
int LitrosMenu = 0;
int Etapa = 0;

//Variables Electrovalvulas
int ElecAcido = 41;
int ElecInTanque = 42;
int ElecOutTanque = 43;

//Variables para detectar litros (Sensor de nivel)
volatile float distancia;
float distancia2;
float distanciaConve = 0.00;
float x0 = 0.00;
float y0 = 0.00;
float x1 = 0.00;
float y1 = 0.00;
float x = 0.00;
float y = 0.00;
float LitrosTanque = 0.00;

//Variables gramajes
volatile float KNO3;
volatile float MgSO4;
volatile float CaNO32;
volatile float H3B03;
volatile float Multimicro;
volatile float NH4H;
volatile float H3PO4;

//Variables sensor de flujo
const int sensor = 2; // Pin digital para el sensor de flujo
volatile int pulsos = 0; // Variable que almacena el número de pulsosFunciones
unsigned long tiempoAnterior = 0; // Variable para calcular el tiempo transcurrido
unsigned long pulsos_Acumulados = 0; // Variable que almacena el número de pulsos acumulados
float litros; // // Variable que almacena el número de litros acumulados

//Funciones
int NextState(int p_state, int input);
void Output(int p_state);
void PantallaMenuPrincipal1();
void PantallaLimpieza1();
void PantallaLimpieza2();
void PantallaLimpieza3();
void PantallaMenuPrincipal2();
void PantallaCantidad25L();
void PantallaCantidad50L();
void PantallaCantidad100L();
void PantallaCultivoTomate();
void PantallaCultivoPepino();
void PantallaCultivoFresa();
void PantallaCultivoChile();
void PantallaEtapaCrecimiento1TomateChile();
void PantallaEtapaCrecimiento2TomateChile();
void PantallaEtapaCrecimiento3TomateChile();
void PantallaEtapaCrecimiento1PepinoFresa();
void PantallaEtapaCrecimiento2PepinoFresa();
void PantallaVerSeleccionTomateChile();
void PantallaVerSeleccionPepinoFresa();
void PantallaIniciandoProceso();
void PantallaFinalizandoProceso();
void PantallaMenuPrincipal3();
//void PantallaCapacidadTanque();
void PantallaMenuPrincipal4();
void PantallaVaciadoTanque();
void PantallaVaciadoTanque2();
void PantallaVaciadoTanque3();
void LitrosDelTanque();
void AccionLlenadoContenedor25lts();
void AccionLlenadoContenedor50lts();
void AccionLlenadoContenedor100lts();
void DispensadodeSustancias();
void DispensadoSustanciasTomate1();
void DispensadoSustanciasTomate2();
void DispensadoSustanciasTomate3();
void DispensadoSustanciasTomate4();
void DispensadoSustanciasTomate5();
void DispensadoSustanciasTomate6();
void DispensadoSustanciasTomate7();
void DispensadoSustanciasTomate8();
void DispensadoSustanciasTomate9();
void DispensadoSustanciasPepino1();
void DispensadoSustanciasPepino2();
void DispensadoSustanciasPepino3();
void DispensadoSustanciasPepino4();
void DispensadoSustanciasPepino5();
void DispensadoSustanciasPepino6();
void DispensadoSustanciasFresa1();
void DispensadoSustanciasFresa2();
void DispensadoSustanciasFresa3();
void DispensadoSustanciasFresa4();
void DispensadoSustanciasFresa5();
void DispensadoSustanciasFresa6();
void DispensadoSustanciasChile1();
void DispensadoSustanciasChile2();
void DispensadoSustanciasChile3();
void DispensadoSustanciasChile4();
void DispensadoSustanciasChile5();
void DispensadoSustanciasChile6();
void DispensadoSustanciasChile7();
void DispensadoSustanciasChile8();
void DispensadoSustanciasChile9();
void GramajeEtapa1Tomate25lts();
void GramajeEtapa2Tomate25lts();
void GramajeEtapa3Tomate25lts();
void GramajeEtapa1Tomate50lts();
void GramajeEtapa2Tomate50lts();
void GramajeEtapa3Tomate50lts();
void GramajeEtapa1Tomate100lts();
void GramajeEtapa2Tomate100lts();
void GramajeEtapa3Tomate100lts();
void GramajeEtapa1Pepino25lts();
void GramajeEtapa2Pepino25lts();
void GramajeEtapa1Pepino50lts();
void GramajeEtapa2Pepino50lts();
void GramajeEtapa1Pepino100lts();
void GramajeEtapa2Pepino100lts();
void GramajeEtapa1Fresa25lts();
void GramajeEtapa2Fresa25lts();
void GramajeEtapa1Fresa50lts();
void GramajeEtapa2Fresa50lts();
void GramajeEtapa1Fresa100lts();
void GramajeEtapa2Fresa100lts();
void GramajeEtapa1Chile25lts();
void GramajeEtapa2Chile25lts();
void GramajeEtapa3Chile25lts();
void GramajeEtapa1Chile50lts();
void GramajeEtapa2Chile50lts();
void GramajeEtapa3Chile50lts();
void GramajeEtapa1Chile100lts();
void GramajeEtapa2Chile100lts();
void GramajeEtapa3Chile100lts();
void AccionInicioMotorPrincipal();
void AccionFinMotorPrincipal();
void controlarMotor();
void controlsvmotor1();
void controlsvmotor2();
void controlsvmotor3();
void controlsvmotor4();
void controlsvmotor5();
void controlsvmotor6();
void controlsvmotor7();
void controlsvmotor8();
void controlsvmotor9();
void controlsvmotor10();
void controlsvmotor11();
void controlsvmotor12();
void ActivarElecAcido();
void DesacElecAcido();
void ActivarElecInTanque();
void DesacElecInTanque();
void ActivarElecOutTanque();
void DesacElecOutTanque();

//Funcion principal del programa
void setup() 
{
  //Se establecen pines de los servomotores y se establece que todos estan parados
  servo1.attach(4);
  servo1.write(90);
  servo2.attach(5);
  servo2.write(90);
  servo3.attach(6);
  servo3.write(90);
  servo4.attach(7);
  servo4.write(90);
  servo5.attach(8);
  servo5.write(90);
  servo6.attach(9);
  servo6.write(90);
  servo7.attach(10);
  servo7.write(90);
  servo8.attach(11);
  servo8.write(90);
  servo9.attach(12);
  servo9.write(90);
  servo10.attach(13);
  servo10.write(90);
  servo11.attach(44);
  servo11.write(90);
  servo12.attach(45);
  servo12.write(90);

  //Galgas extensiometricas
  Galga1.begin(); //Se inicia conexión con el modulo HX711
  Galga1.start(2000); //Estabilización a 2000 ms
  Galga1.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)
  
  Galga2.begin(); //Se inicia conexión con el modulo HX711
  Galga2.start(2000); //Estabilización a 2000 ms
  Galga2.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)

  Galga3.begin(); //Se inicia conexión con el modulo HX711
  Galga3.start(2000); //Estabilización a 2000 ms
  Galga3.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)
  
  Galga4.begin(); //Se inicia conexión con el modulo HX711
  Galga4.start(2000); //Estabilización a 2000 ms
  Galga4.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)

  Galga5.begin(); //Se inicia conexión con el modulo HX711
  Galga5.start(2000); //Estabilización a 2000 ms
  Galga5.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)
  
  Galga6.begin(); //Se inicia conexión con el modulo HX711
  Galga6.start(2000); //Estabilización a 2000 ms
  Galga6.setCalFactor(1768.0); //Factor de calibración (depende de cada galga)

  //Se establecen los pines de los botones, activando la resistencia interna del arduino INPUT_PULLUP
  pinMode(botonArriba, INPUT_PULLUP);
  pinMode(botonAbajo, INPUT_PULLUP);
  pinMode(botonOk, INPUT_PULLUP);
  pinMode(botonCancelar, INPUT_PULLUP);

  //Se establece la salida del motor
  pinMode(motor, OUTPUT);

  //Se establece las salidas de las electrovalvulas
  pinMode(ElecAcido, OUTPUT);
  pinMode(ElecInTanque, OUTPUT);
  pinMode(ElecOutTanque, OUTPUT);
  
  //Se inicializa la comunicación Serial I2C
  Wire.begin();

  interrupts(); // Habilito las interrupciones
  // Interrupción INT0, llama a la ISR llamada "flujo" en cada flanco de subida en el pin digital 2
  attachInterrupt(digitalPinToInterrupt(sensor), flujo, RISING);  
  tiempoAnterior = millis(); // Guardo el tiempo que tarda el ejecutarse el setup

  //Display LCD de 128 x 64
  lcd.init(); //Se inicializa el display
  lcd.clear(0, 0); //Se limpia por completo la pantalla
  lcd.clearBuffer(); //Se limpia el buffer
  lcd.setFont(z_font_5x8); //Se establece la fuente
  lcd.setBL_on(); //Se prende el display
  lcd.drawText(0, 20, "Bienvenidos a InvernaTech"); //Se escribe el mensaje
  lcd.sendBuffer(); //Se manda el mensaje
  delay(2000); //Delay de 2000 ms
  lcd.clear(0, 0); //Se limpia por completo la pantalla
  PantallaMenuPrincipal1(); //Nos manda a PantallaMenuPrincipal1
}

void loop() 
{
  //La lectura de los botnes se igualan a esas variables
  int botonArribaState = digitalRead(botonArriba);
  int botonAbajoState = digitalRead(botonAbajo);
  int botonOkState = digitalRead(botonOk);
  int botonCancelarState = digitalRead(botonCancelar);

  // Verificar si al menos un botón está presionado
  if (botonArribaState == LOW || botonAbajoState == LOW || botonOkState == LOW || botonCancelarState == LOW)
  {
    // Determinar cuál botón está presionado
    if (botonArribaState == LOW)
    {
      input = 1; // Botón Arriba
    } 
    else if (botonAbajoState == LOW)
    {
      input = 2; // Botón Abajo
    } 
    else if (botonOkState == LOW)
    {
      input = 3; // Botón OK
    } 
    else if (botonCancelarState == LOW)
    {
      input = 4; // Botón Cancelar
    }
    
    n_state = NextState(p_state, input);
    p_state = n_state;
    Output(p_state);
  }

  distancia = sonar.ping_cm(); //La variable distancia se iguala a los datos que arroje el sensor de nivel

  //Se actualizan los datos de la galga (Se ponen en el loop para que se actualicen los datos cuando no hay gramajes)
  Galga1.update();
  Galga2.update();
  Galga3.update();
  Galga4.update();
  Galga5.update();
  Galga6.update();

  //Se hace el control del motor principal desde loop para poder activarlo cuando se desee
  //Esto se hace cuando se detecte que EstadoMotor == true
  if (EstadoMotor == true) 
  {
    controlarMotor();
  }
}

//Funciones
int NextState(int p_state, int input) 
{
  int n_state = p_state;

  switch (p_state) 
  {
    case 0: //PantallaMenuPrincipal1()
      if (input == 3) 
      {
        n_state = 1;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 4;
      }
      else if (input == 1)
      {
        n_state = 23;
      }
      else
      {
        n_state = 0;
      }
    break;

    case 1: //PantallaLimpieza1()
      if (input == 3) 
      {
        n_state = 2;
        lcd.clear(0, 0);
      }
      else if (input == 4) 
      {
        n_state = 0;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 1;
      }
    break;

    case 2: //PantallaLimpieza2()
      if (input == 3) 
      {
        n_state = 3;
        lcd.clear(0, 0);
      } 
      else
      {
        n_state = 2;
      }
    break;

    case 3: //PantallaLimpieza3()
      if (input == 3) 
      {
        n_state = 0;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 3;
      }
    break;

    case 4: //PantallaMenuPrincipal2()
      if (input == 3) 
      {
        n_state = 5;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 0;
      }
      else if (input == 2)
      {
        n_state = 21;
      }
      else
      {
        n_state = 4;
      }
    break;

    case 5: //PantallaCantidad25L()
      if (input == 3) 
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 6;
      }
      else if (input == 1)
      {
        n_state = 7;
      }
      else if (input == 4)
      {
        n_state = 4;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 5;
      }
    break;

    case 6: //PantallaCantidad50L()
      if (input == 3) 
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 7;
      }
      else if (input == 1)
      {
        n_state = 5;
      }
      else if (input == 4)
      {
        n_state = 4;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 6;
      }
    break;

    case 7: //PantallaCantidad100L()
      if (input == 3) 
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 6;
      }
      else if (input == 2)
      {
        n_state = 5;
      }
      else if (input == 4)
      {
        n_state = 4;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 7;
      }
    break;

    case 8: //PantallaCultivoTomate()
      if (input == 3) 
      {
        n_state = 12;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 9;
      }
      else if (input == 1)
      {
        n_state = 11;
      }
      else if (input == 4)
      {
        n_state = 5;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 8;
      }
    break;

    case 9: //PantallaCultivoPepino()
      if (input == 3) 
      {
        n_state = 15;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 10;
      }
      else if (input == 1)
      {
        n_state = 8;
      }
      else if (input == 4)
      {
        n_state = 5;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 9;
      }
    break;

    case 10: //PantallaCultivoFresa()
      if (input == 3) 
      {
        n_state = 15;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 11;
      }
      else if (input == 1)
      {
        n_state = 9;
      }
      else if (input == 4)
      {
        n_state = 5;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 10;
      }
    break;

    case 11: //PantallaCultivoChile()
      if (input == 3) 
      {
        n_state = 12;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 10;
      }
      else if (input == 2)
      {
        n_state = 8;
      }
      else if (input == 4)
      {
        n_state = 5;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 11;
      }
    break;

    case 12: //PantallaEtapaCrecimientoTomateChile1()
      if (input == 3) 
      {
        n_state = 17;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 13;
      }
      else if (input == 1)
      {
        n_state = 14;
      }
      else if (input == 4)
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 12;
      }
    break;

    case 13: //PantallaEtapaCrecimientoTomateChile2()
      if (input == 3) 
      {
        n_state = 17;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 14;
      }
      else if (input == 1)
      {
        n_state = 12;
      }
      else if (input == 4)
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 13;
      }
    break;

    case 14: //PantallaEtapaCrecimientoTomateChile3()
      if (input == 3) 
      {
        n_state = 17;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 13;
      }
      else if (input == 2)
      {
        n_state = 12;
      }
      else if (input == 4)
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 14;
      }
    break;

    case 15: //PantallaEtapaCrecimientoPepinoFresa1()
      if (input == 3)
      {
        n_state = 18;
        lcd.clear(0, 0);
      }
      else if (input == 2)
      {
        n_state = 16;
      }
      else if (input == 1)
      {
        n_state = 16;
      }
      else if(input == 4)
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 15;
      }
    break;

    case 16: //PantallaEtapaCrecimientoPepinoFresa2()
      if (input == 3)
      {
        n_state = 18;
        lcd.clear(0, 0);
      }
      else if(input == 2)
      {
        n_state = 15;
      }
      else if (input == 1)
      {
        n_state = 15;
      }
      else if (input == 4)
      {
        n_state = 8;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 16;
      }
    break;

    case 17: //PantallaVerSeleccionTomateChile()
      if (input == 3) 
      {
        n_state = 19;
        lcd.clear(0, 0);
      }
      else if (input == 4)
      {
        n_state = 12;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 17;
      }
    break;

    case 18: //PantallaVerSeleccionPepinoFresa()
      if (input == 3) 
      {
        n_state = 19;
        lcd.clear(0, 0);
      }
      else if (input == 4)
      {
        n_state = 15;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 18;
      }
    break;

    case 19: //PantallaIniciandoProceso
    if (input == 3)
    {
      n_state = 27;
      lcd.clear(0, 0);
    }
    else
    {
      n_state = 19;
    }
    break;

    case 20: //DispensadodeSustancias
    if (input == 3)
    {
      n_state = 0;
      lcd.clear(0, 0);
    }
    else
    {
      n_state = 20;
    }
    break;

    case 21: //PantallaMenuPrincipal3
      if (input == 3)
      {
        n_state = 22;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 4;
      }
      else if (input == 2)
      {
        n_state = 23;
      }
      else
      {
        n_state = 21;
      }
    break;

    case 22: //PantallaCapacidadTanque(distancia)
    if (input == 3)
    {
      n_state = 0;
      lcd.clear(0, 0);
    }
    else if (input == 4)
    {
      n_state = 21;
      lcd.clear(0, 0);
    }
    else
    {
      n_state = 22;
    }
    break;

    case 23: //PantallaMenuPrincipal4()
      if (input == 3)
      {
        n_state = 24;
        lcd.clear(0, 0);
      }
      else if (input == 1)
      {
        n_state = 21;
      }
      else if (input == 2)
      {
        n_state = 0;
        lcd.clear(0, 0);
        lcd.clearBuffer();
      }
      else 
      {
        n_state = 23;
      }
    break;

    case 24: //PantallaVaciadoTanque();
      if (input == 3)
      {
        n_state = 25;
        lcd.clear(0, 0);
      }
      else if (input == 4)
      {
        n_state = 23;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 24;
      }
    break;

    case 25: //PantallaVaciadoTanque2();
      if (input == 4)
      {
        n_state = 26;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 25;
      }
    break;

    case 26: //PantallaVaciadoTanque3()
      if (input == 3)
      {
        n_state = 0;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 26;
      }
    break;

    case 27: //LitrosTanque()
      if (input == 3)
      {
        n_state = 20;
        lcd.clear(0, 0);
      }
      else
      {
        n_state = 27;
      }

    default:
    break;
  }

  return n_state;
}

void Output(int p_state) 
{
  switch (p_state)
  {
    case 0:
      PantallaMenuPrincipal1();
    break;

    case 1:
      PantallaLimpieza1();
    break;

    case 2:
      PantallaLimpieza2();
    break;

    case 3:
      PantallaLimpieza3();
    break;
    
    case 4:
      PantallaMenuPrincipal2();
    break;

    case 5:
      PantallaCantidad25L();
    break;

    case 6:
      PantallaCantidad50L();
    break;

    case 7:
      PantallaCantidad100L();
    break;

    case 8:
      PantallaCultivoTomate();
    break;

    case 9:
      PantallaCultivoPepino();
    break;

    case 10:
      PantallaCultivoFresa();
    break;

    case 11:
      PantallaCultivoChile();
    break;

    case 12:
      PantallaEtapaCrecimiento1TomateChile();
    break;

    case 13:
      PantallaEtapaCrecimiento2TomateChile();
    break;

    case 14:
      PantallaEtapaCrecimiento3TomateChile();
    break;

    case 15:
      PantallaEtapaCrecimiento1PepinoFresa();
    break;

    case 16:
      PantallaEtapaCrecimiento2PepinoFresa();
    break;

    case 17:
      PantallaVerSeleccionTomateChile();
    break;

    case 18:
      PantallaVerSeleccionPepinoFresa();
    break;

    case 19:
      PantallaIniciandoProceso();
    break;

    case 20:
      DispensadodeSustancias();
    break;

    case 21:
      PantallaMenuPrincipal3();
    break;

    case 22:
      PantallaCapacidadTanque(distancia);
    break;

    case 23:
      PantallaMenuPrincipal4();
    break;

    case 24:
      PantallaVaciadoTanque();
    break;

    case 25:
      PantallaVaciadoTanque2();
    break;

    case 26:
      PantallaVaciadoTanque3();
    break;

    case 27:
      LitrosDelTanque();
    break;

    default:
    break;
  }
}

void PantallaMenuPrincipal1()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Menu");
  lcd.drawText(2, 12, "1. Limpieza <");
  lcd.drawText(54, 12, "|");
  lcd.drawText(55, 12, "-");
  lcd.drawText(0, 22, "2. Solucion nutritiva");
  lcd.drawText(0, 32, "3. Capacidad del tanque");
  lcd.drawText(0, 42, "4. Vaciado del tanque");
  lcd.sendBuffer();
}

void PantallaLimpieza1()
{
  lcd.clearBuffer();
  lcd.drawText(47, 0, "Limpieza");
  lcd.drawText(0, 12, "Paso 1. Para comenzar, vaya");
  lcd.drawText(0, 22, "a la seccion '4' del menu prin-");
  lcd.drawText(0, 32, "cipal para vaciar el tanque.");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaLimpieza2()
{
  lcd.clearBuffer();
  lcd.drawText(47, 0, "Limpieza");
  lcd.drawText(0, 12, "Paso 2. Proceso de llenado");
  lcd.drawText(0, 22, "iniciado. Abra la llave de");
  lcd.drawText(0, 32, "paso para llenar el tanque");
  lcd.drawText(0, 42, "a 25 litros.");

  AccionLlenadoContenedorLimpieza25lts();
  
  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(47, 0, "Limpieza");
  lcd.drawText(0, 12, "Proceso de llenado del");
  lcd.drawText(0, 22, "tanque finalizado. Pulse");
  lcd.drawText(0, 32, "boton Ok para continuar con");
  lcd.drawText(0, 42, "la limpieza.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void PantallaLimpieza3()
{
  AccionInicioMotorPrincipal();
  lcd.clearBuffer();
  lcd.drawText(47, 0, "Limpieza");
  lcd.drawText(0, 12, "Comenzando ciclo de limpieza.");
  lcd.drawText(0, 22, "Espere a");
  lcd.drawText(0, 32, "que termine el proceso. Favor");
  lcd.drawText(0, 42, "de no pulsar ningun boton.");
  lcd.sendBuffer();
  delay(10000);
  AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(47, 0, "Limpieza");
  lcd.drawText(0, 12, "Ciclo de limpieza finalizado.");
  lcd.drawText(0, 22, "Pulse Ok para regresar al");
  lcd.drawText(0, 32, "Menu Principal.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void PantallaMenuPrincipal2()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Menu");
  lcd.drawText(2, 12, "1. Limpieza");
  lcd.drawText(0, 22, "2. Solucion nutritiva <");
  lcd.drawText(96, 22, "|");
  lcd.drawText(97, 22, "-");
  lcd.drawText(0, 32, "3. Capacidad del tanque");
  lcd.drawText(0, 42, "4. Vaciado del tanque");
  lcd.sendBuffer();
}

void PantallaCantidad25L()
{
  lcd.clearBuffer();
  LitrosMenu = 25;
  lcd.drawText(0, 0, "Eilja la cantidad de llenado");
  lcd.drawText(2, 12, "1. 25 litros <");
  lcd.drawText(58, 12, "|");
  lcd.drawText(59, 12, "-");
  lcd.drawText(0, 22, "2. 50 litros");
  lcd.drawText(0, 32, "3. 100 litros");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaCantidad50L()
{
  lcd.clearBuffer();
  LitrosMenu = 50;
  lcd.drawText(0, 0, "Eilja la cantidad de llenado");
  lcd.drawText(2, 12, "1. 25 litros");
  lcd.drawText(0, 22, "2. 50 litros <");
  lcd.drawText(58, 22, "|");
  lcd.drawText(59, 22, "-");
  lcd.drawText(0, 32, "3. 100 litros");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}
void PantallaCantidad100L()
{
  lcd.clearBuffer();
  LitrosMenu = 100;
  lcd.drawText(0, 0, "Eilja la cantidad de llenado");
  lcd.drawText(2, 12, "1. 25 litros");
  lcd.drawText(0, 22, "2. 50 litros");
  lcd.drawText(0, 32, "3. 100 litros <");
  lcd.drawText(62, 32, "|");
  lcd.drawText(63, 32, "-");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaCultivoTomate()
{
  lcd.clearBuffer();
  Cultivo = "Tomate";
  lcd.drawText(0, 0, "Elija el cultivo");
  lcd.drawText(2, 12, "1. Tomate <");
  lcd.drawText(51, 12, "|");
  lcd.drawText(52, 12, "-");
  lcd.drawText(0, 22, "2. Pepino");
  lcd.drawText(0, 32, "3. Fresa");
  lcd.drawText(75, 12, "4. Chile");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaCultivoPepino()
{
  lcd.clearBuffer();
  Cultivo = "Pepino";
  lcd.drawText(0, 0, "Elija el cultivo");
  lcd.drawText(2, 12, "1. Tomate");
  lcd.drawText(0, 22, "2. Pepino <");
  lcd.drawText(47, 22, "|");
  lcd.drawText(48, 22, "-");
  lcd.drawText(0, 32, "3. Fresa");
  lcd.drawText(75, 12, "4. Chile");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaCultivoFresa()
{
  lcd.clearBuffer();
  Cultivo = "Fresa";
  lcd.drawText(0, 0, "Elija el cultivo");
  lcd.drawText(2, 12, "1. Tomate");
  lcd.drawText(0, 22, "2. Pepino");
  lcd.drawText(0, 32, "3. Fresa <");
  lcd.drawText(45, 32, "|");
  lcd.drawText(46, 32, "-");
  lcd.drawText(75, 12, "4. Chile");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaCultivoChile()
{
  lcd.clearBuffer();
  Cultivo = "Chile";
  lcd.drawText(0, 0, "Elija el cultivo");
  lcd.drawText(2, 12, "1. Tomate");
  lcd.drawText(0, 22, "2. Pepino");
  lcd.drawText(0, 32, "3. Fresa");
  lcd.drawText(75, 12, "4. Chile <");
  lcd.drawText(113, 12, "|");
  lcd.drawText(114, 12, "-");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer(); 
}

void PantallaEtapaCrecimiento1TomateChile()
{
  lcd.clearBuffer();
  Etapa = 1;
  lcd.drawText(0, 0, "Elija la etapa de crecimiento");
  lcd.drawText(2, 12, "1. Etapa 1 <");
  lcd.drawText(52, 12, "|");
  lcd.drawText(53, 12, "-");
  lcd.drawText(0, 22, "2. Etapa 2");
  lcd.drawText(0, 32, "3. Etapa 3");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaEtapaCrecimiento2TomateChile()
{
  lcd.clearBuffer();
  Etapa = 2;
  lcd.drawText(0, 0, "Elija la etapa de crecimiento");
  lcd.drawText(2, 12, "1. Etapa 1");
  lcd.drawText(0, 22, "2. Etapa 2 <");
  lcd.drawText(54, 22, "|");
  lcd.drawText(55, 22, "-");
  lcd.drawText(0, 32, "3. Etapa 3");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaEtapaCrecimiento3TomateChile()
{
  lcd.clearBuffer();
  Etapa = 3;
  lcd.drawText(0, 0, "Elija la etapa de crecimiento");
  lcd.drawText(2, 12, "1. Etapa 1");
  lcd.drawText(0, 22, "2. Etapa 2");
  lcd.drawText(0, 32, "3. Etapa 3 <");
  lcd.drawText(54, 32, "|");
  lcd.drawText(55, 32, "-");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaEtapaCrecimiento1PepinoFresa()
{
  lcd.clearBuffer();
  Etapa = 1;
  lcd.drawText(0, 0, "Elija la etapa de crecimiento");
  lcd.drawText(2, 12, "1. Etapa 1 <");
  lcd.drawText(52, 12, "|");
  lcd.drawText(53, 12, "-");
  lcd.drawText(0, 22, "2. Etapa 2");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaEtapaCrecimiento2PepinoFresa()
{
  lcd.clearBuffer();
  Etapa = 2;
  lcd.drawText(0, 0, "Elija la etapa de crecimiento");
  lcd.drawText(2, 12, "1. Etapa 1");
  lcd.drawText(0, 22, "2. Etapa 2 <");
  lcd.drawText(54, 22, "|");
  lcd.drawText(55, 22, "-");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaVerSeleccionTomateChile()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Haz seleccionado");
  lcd.drawText(0, 12, "1. Capacidad: " + String(LitrosMenu) + " litros.");
  lcd.drawText(0, 22, "2. Cultivo: " + Cultivo + ".");
  lcd.drawText(0, 32, "3. Etapa de crecimiento: " + String(Etapa) + ".");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaVerSeleccionPepinoFresa()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Haz seleccionado");
  lcd.drawText(0, 12, "1. Capacidad: " + String(LitrosMenu) + " litros.");
  lcd.drawText(0, 22, "2. Cultivo: " + Cultivo + ".");
  lcd.drawText(0, 32, "3. Etapa de crecimiento: " + String(Etapa) + ".");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para continuar");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();
}

void PantallaIniciandoProceso()
{
  lcd.clearBuffer();
  lcd.drawText(50, 0, "Llenado del tanque");
  lcd.drawText(0, 12, "Pulse el boton de Ok para");
  lcd.drawText(0, 22, "llenar el tanque a la can-");
  lcd.drawText(0, 32, "tidad de litros selec-");
  lcd.drawText(0, 42, "cionado.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void PantallaMenuPrincipal3()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Menu");
  lcd.drawText(2, 12, "1. Limpieza");
  lcd.drawText(0, 22, "2. Solucion nutritiva");
  lcd.drawText(0, 32, "3. Capacidad del tanque <");
  lcd.drawText(110, 32, "|");
  lcd.drawText(111, 32, "-");
  lcd.drawText(0, 42, "4. Vaciado del tanque");
  lcd.sendBuffer();
}

void PantallaCapacidadTanque(float distancia)
{
  lcd.clearBuffer();
  delay(50); // Esperar 500ms entre pings (aproximadamente 20 pings/segundo). 29ms debería ser el tiempo de espera más corto entre pings.

  /*Programación para establecer los litros
        x                      y
    distancia (cm)          litros
        0                       0
      17.5                    25
      30                      50
      54                     100

      */
  distanciaConve = /*distanciamax -*/ distancia;

  if (distanciaConve <= 17.5)
  {
    x0 = 0;
    y0 = 0;
    x1 = 17.5;
    y1 = 25;
    x = distanciaConve;

    y = y0 + (y1 - y0/x1 - x0)*(x - x0);
  }
  else if (17.5 <= distanciaConve && distanciaConve <= 30)
  {
    x0 = 17.5;
    y0 = 25;
    x1 = 30;
    y1 = 50;
    x = distanciaConve;

    y = y0 + (y1 - y0/x1 - x0)*(x - x0);
  }
  else if (30 <= distanciaConve && distanciaConve <= 54)
  {
    x0 = 30;
    y0 = 50;
    x1 = 54;
    y1 = 100;
    x = distanciaConve;

    y = y0 + (y1 - y0/x1 - x0)*(x - x0);
  }

  lcd.drawText(0, 0, "El nivel del tanque es:");
  lcd.drawText(0, 10, String(y) + " litros.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void PantallaMenuPrincipal4()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Menu"); 
  lcd.drawText(2, 12, "1. Limpieza");
  lcd.drawText(0, 22, "2. Solucion nutritiva");
  lcd.drawText(0, 32, "3. Capacidad del tanque");
  lcd.drawText(0, 42, "4. Vaciado del tanque <");
  lcd.drawText(99, 42, "|");
  lcd.drawText(100, 42, "-");
  lcd.sendBuffer(); 
}

void PantallaVaciadoTanque()
{
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Vaciado del tanque");
  lcd.drawText(0, 12, "Paso 1. Pulse ok para abrir el");
  lcd.drawText(0, 22, "paso de la solucion nutritiva.");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse ok para abrir paso");
  lcd.drawText(0, 56, "- Pulse cancelar para volver");
  lcd.sendBuffer();   
}

void PantallaVaciadoTanque2()
{
  ActivarElecOutTanque();
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Vaciado del tanque");
  lcd.drawText(0, 12, "Paso 2. Pulse cancelar para");
  lcd.drawText(0, 22, "cerrar el paso de la solucion");
  lcd.drawText(0, 32, "nutritva.");
  lcd.drawHLine(0, 40, 130, 1, 1);
  lcd.drawText(0, 46, "- Pulse cancelar para cerrar");
  lcd.drawText(0, 56, "el paso");
  lcd.sendBuffer();   
}

void PantallaVaciadoTanque3()
{
  DesacElecOutTanque();
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Vaciado del tanque");
  lcd.drawText(0, 12, "Paso 3. Proceso finalizado.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void flujo()
{
  pulsos++; // Incrementa en una unidad el número de pulsos
}
  
void LitrosDelTanque()
{
	if (LitrosMenu == 25)
  {
    AccionLlenadoContenedor25lts();
  }
  else if (LitrosMenu == 50)
  {
    AccionLlenadoContenedor50lts();
  }
  else if (LitrosMenu == 100)
  {
    AccionLlenadoContenedor100lts();
  }
}

void AccionLlenadoContenedorLimpieza25lts()
{
  ActivarElecInTanque();
  while (distancia > distancia2) //(Esta la tengo que agregar segun la distancia de llenado para 25 lts)
  {
    distancia = sonar.ping_cm(); //La variable distancia se iguala a los datos que arroje el sensor de nivel
    delay(50);
  }
  DesacElecInTanque();
}

void AccionLlenadoContenedor25lts()
{
  lcd.clearBuffer();
  ActivarElecInTanque();
  while (distancia > distancia2) //(Esta la tengo que agregar segun la distancia de llenado para 25 lts)
  {
    distancia = sonar.ping_cm(); //La variable distancia se iguala a los datos que arroje el sensor de nivel
    delay(50);
    lcd.drawText(50, 0, "Llenado del Tanque");
    lcd.drawText(0, 12, "Abra la llave de paso");
    lcd.drawText(0, 22, "hasta que se llene el");
    lcd.drawText(0, 12, "tanque a la canti-");
    lcd.drawText(0, 32, "dad deseada.");
    lcd.sendBuffer();
  }
  DesacElecInTanque();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(50, 0, "Llenado del Tanque");
  lcd.drawText(0, 12, "Proceso terminado. Cierre la");
  lcd.drawText(0, 22, "llave de paso. Pulse Ok para con-");
  lcd.drawText(0, 32, "tinuar con el dispensado de los");
  lcd.drawText(0, 42, "los fertilizantes.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void AccionLlenadoContenedor50lts()
{
  lcd.clearBuffer();
  ActivarElecInTanque();
  while (distancia > distancia2) //(Esta la tengo que agregar segun la distancia de llenado para 50 lts)
  {
    distancia = sonar.ping_cm(); //La variable distancia se iguala a los datos que arroje el sensor de nivel
    delay(50);
    lcd.drawText(50, 0, "Llenado del Tanque");
    lcd.drawText(0, 12, "Abra la llave de paso");
    lcd.drawText(0, 22, "hasta que se llene el");
    lcd.drawText(0, 12, "tanque a la canti-");
    lcd.drawText(0, 32, "dad deseada.");
    lcd.sendBuffer();
  }
  DesacElecInTanque();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(50, 0, "Llenado del Tanque");
  lcd.drawText(0, 12, "Proceso terminado. Cierre la");
  lcd.drawText(0, 22, "llave de paso. Pulse Ok para con-");
  lcd.drawText(0, 32, "tinuar con el dispensado de los");
  lcd.drawText(0, 42, "los fertilizantes.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void AccionLlenadoContenedor100lts()
{
  lcd.clearBuffer();
  ActivarElecInTanque();
  while (distancia > distancia2) //(Esta la tengo que agregar segun la distancia de llenado para 100 lts)
  {
    distancia = sonar.ping_cm(); //La variable distancia se iguala a los datos que arroje el sensor de nivel
    delay(50);
    lcd.drawText(50, 0, "Llenado del Tanque");
    lcd.drawText(0, 12, "Abra la llave de paso");
    lcd.drawText(0, 22, "hasta que se llene el");
    lcd.drawText(0, 12, "tanque a la canti-");
    lcd.drawText(0, 32, "dad deseada.");
    lcd.sendBuffer();
  }
  DesacElecInTanque();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(50, 0, "Llenado del Tanque");
  lcd.drawText(0, 12, "Proceso terminado. Cierre la");
  lcd.drawText(0, 22, "llave de paso. Pulse Ok para con-");
  lcd.drawText(0, 32, "tinuar con el dispensado de los");
  lcd.drawText(0, 42, "los fertilizantes.");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
  lcd.sendBuffer();
}

void DispensadodeSustancias()
{
	if (Etapa == 1 && LitrosMenu == 25 && Cultivo == "Tomate")
	{ 	
		DispensadoSustanciasTomate1();
	}
	else if (Etapa == 1 && LitrosMenu == 50 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate2();
	}
	else if (Etapa == 1 && LitrosMenu == 100 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate3();
	}
	else if (Etapa == 2 && LitrosMenu == 25 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate4();
	}
	else if (Etapa == 2 && LitrosMenu == 50 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate5();
	}
	else if (Etapa == 2 && LitrosMenu == 100 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate6();
	}
	else if (Etapa == 3 && LitrosMenu == 25 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate7();
	}
	else if (Etapa == 3 && LitrosMenu == 50 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate8();
	}
	else if (Etapa == 3 && LitrosMenu == 100 && Cultivo == "Tomate")
	{
		DispensadoSustanciasTomate9();
	}
	else if (Etapa == 1 && LitrosMenu == 25 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino1();
	}
	else if (Etapa == 1 && LitrosMenu == 50 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino2();
	}
	else if (Etapa == 1 && LitrosMenu == 100 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino3();
	}
	else if (Etapa == 2 && LitrosMenu == 25 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino4();
	}
	else if (Etapa == 2 && LitrosMenu == 50 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino5();
	}
	else if (Etapa == 3 && LitrosMenu == 100 && Cultivo == "Pepino")
	{
		DispensadoSustanciasPepino6();
	}
	else if (Etapa == 1 && LitrosMenu == 25 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa1();
	}
	else if (Etapa == 1 && LitrosMenu == 50 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa2();
	}
	else if (Etapa == 1 && LitrosMenu == 100 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa3();
	}
	else if (Etapa == 2 && LitrosMenu == 25 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa4();
	}
	else if (Etapa == 2 && LitrosMenu == 50 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa5();
	}
	else if (Etapa == 2 && LitrosMenu == 100 && Cultivo == "Fresa")
	{
		DispensadoSustanciasFresa6();
	}
	else if (Etapa == 1 && LitrosMenu == 25 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile1();
	}
	else if (Etapa == 1 && LitrosMenu == 50 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile2();
	}
	else if (Etapa == 1 && LitrosMenu == 100 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile3();
	}
	else if (Etapa == 2 && LitrosMenu == 25 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile4();
	}
	else if (Etapa == 2 && LitrosMenu == 50 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile5();
	}
	else if (Etapa == 2 && LitrosMenu == 100 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile6();
	}
	else if (Etapa == 3 && LitrosMenu == 25 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile7();
	}
	else if (Etapa == 3 && LitrosMenu == 50 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile8();
	}
	else if (Etapa == 3 && LitrosMenu == 100 && Cultivo == "Chile")
	{
		DispensadoSustanciasChile9();
	}
}

void DispensadoSustanciasTomate1()
{
	AccionInicioMotorPrincipal();
  
  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Tomate25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");

}

void DispensadoSustanciasTomate2()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Tomate25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate3()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Tomate25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate4()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Tomate50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate5()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Tomate50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate6()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Tomate50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate7()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Tomate100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate8()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Tomate100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasTomate9()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Tomate100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino1()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Pepino25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino2()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Pepino25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino3()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Pepino50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino4()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Pepino50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino5()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Pepino100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasPepino6()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Pepino100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa1()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Fresa25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa2()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Fresa25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa3()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Fresa50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa4()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Fresa50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa5()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Fresa100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasFresa6()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Fresa100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile1()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Chile25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile2()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Chile25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile3()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Chile25lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile4()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Chile50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile5()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Chile50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile6()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Chile50lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile7()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa1Chile100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile8()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa2Chile100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void DispensadoSustanciasChile9()
{
	AccionInicioMotorPrincipal();

  lcd.drawText(0, 0, "Agitador accionado. Comen-");
  lcd.drawText(0, 12, "zando el dispensado de sustancias");
  delay(3000);
  lcd.clear(0, 0);
  lcd.clearBuffer();

	GramajeEtapa3Chile100lts();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Terminando el proceso de");
  lcd.drawText(0, 12, "mezclado. Espere unos mo-");
  lcd.drawText(0, 22, "mentos");
  delay(10000);

	AccionFinMotorPrincipal();

  lcd.clear(0, 0);
  lcd.clearBuffer();
  lcd.drawText(0, 0, "Proceso de mezclado fi-");
  lcd.drawText(0, 12, "nalizado");
  lcd.drawHLine(0, 50, 130, 1, 1);
  lcd.drawText(0, 56, "- Pulse ok para continuar");
}

void GramajeEtapa1Tomate25lts()
{
  while (KNO3 < 5)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 5)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 5)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Tomate25lts()
{
  while (KNO3 < 5)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 10)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 15)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Tomate25lts()
{
  while (KNO3 < 15)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 10)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 15)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Tomate50lts()
{
  while (KNO3 < 10)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 10)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 15)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Tomate50lts()
{
  while (KNO3 < 15)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 15)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 25)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Tomate50lts()
{
  while (KNO3 < 25)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 20)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 15)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Tomate100lts()
{
  while (KNO3 < 15)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 15)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 25)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Tomate100lts()
{
  while (KNO3 < 35)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 35)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Tomate100lts()
{
  while (KNO3 < 45)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 40)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 60)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Pepino25lts()
{
  while (KNO3 < 10)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 15)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 5)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Pepino25lts()
{
  while (KNO3 < 20)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 15)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 5)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Pepino50lts()
{
  while (KNO3 < 30)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 20)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 25)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 10)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Pepino50lts()
{
  while (KNO3 < 35)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 40)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 10)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Pepino100lts()
{
  while (KNO3 < 35)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 45)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 20)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Pepino100lts()
{
  while (KNO3 < 70)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 60)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 75)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 15)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Fresa25lts()
{
  while (KNO3 < 15)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 10)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}


void GramajeEtapa2Fresa25lts()
{
  while (KNO3 < 15)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 10)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Fresa50lts()
{
  while (KNO3 < 25)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Fresa50lts()
{
  while (KNO3 < 25)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 20)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Fresa100lts()
{
  while (KNO3 < 50)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 60)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Fresa100lts()
{
  while (KNO3 < 50)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 30)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 60)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 1)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Chile25lts()
{
  while (KNO3 < 12)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 14)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 14)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Chile25lts()
{
  while (KNO3 < 24)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 28)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 28)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Chile25lts()
{
  while (KNO3 < 24)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 28)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 28)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 0.5)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Chile50lts()
{
  while (KNO3 < 25)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 28)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 28)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Chile50lts()
{
  while (KNO3 < 48)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 56)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 56)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Chile50lts()
{
  while (KNO3 < 48)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 56)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 56)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa1Chile100lts()
{
  while (KNO3 < 50)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 55)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 57)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa2Chile100lts()
{
  while (KNO3 < 96)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 112)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 112)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void GramajeEtapa3Chile100lts()
{
  while (KNO3 < 96)
  {
    controlsvmotor1();
    Galga1.update();
    KNO3 = Galga1.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: ");
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor2();

  while (MgSO4 < 112)
  {
    controlsvmotor3();
    Galga2.update();
    MgSO4 = Galga2.getData();    
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: ");
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor4();

  while (CaNO32 < 112)
  {
    controlsvmotor5();
    Galga3.update();
    CaNO32 = Galga3.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: ");
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor6();

  while (H3B03 < 1)
  {
    controlsvmotor7();
    Galga4.update();
    H3B03 = Galga4.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3: " + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: ");
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor8();

  while (Multimicro < 4)
  {
    controlsvmotor9();
    Galga5.update();
    Multimicro = Galga5.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: ");
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor10();

  while (NH4H < 0)
  {
    controlsvmotor11();
    Galga6.update();
    NH4H = Galga6.getData();
    lcd.clearBuffer();
    lcd.drawText(15, 0, "Dispensado de fertilizantes");
    lcd.drawText(0, 12, "KNO3:" + String(KNO3));
    lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
    lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
    lcd.drawText(0, 42, "H3B03: " + String(H3B03));
    lcd.drawText(50, 12, "Multi: " + String(Multimicro));
    lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
    lcd.drawText(50, 32, "H3PO43: ");
    lcd.sendBuffer();
  }

  controlsvmotor12();

  ActivarElecAcido();
  while (H3PO4 < 0.01)
  {
    if(millis() - tiempoAnterior > 1000)
    {
      // Realizo los cálculos
      tiempoAnterior = millis(); // Actualizo el nuevo tiempo
      pulsos_Acumulados += pulsos; // Número de pulsos acumulados
      litros = pulsos_Acumulados*1.0/370; // Cada 450 pulsos son un litro
      pulsos = 0; // Pongo nuevamente el número de pulsos a cero

      lcd.clearBuffer();
      lcd.drawText(15, 0, "Dispensado de fertilizantes");
      lcd.drawText(0, 12, "KNO3:" + String(KNO3));
      lcd.drawText(0, 22, "MgSO4: " + String(MgSO4));
      lcd.drawText(0, 32, "CaNO32: " + String(CaNO32));
      lcd.drawText(0, 42, "H3B03: " + String(H3B03));
      lcd.drawText(50, 12, "Multi: " + String(Multimicro));
      lcd.drawText(50, 22, "NH4H2PO4: " + String(NH4H));
      lcd.drawText(50, 32, "H3PO43: " + String(H3PO4));
      lcd.sendBuffer();
    }
  }
  DesacElecAcido();

  pulsos_Acumulados = 0;
}

void AccionInicioMotorPrincipal()
{
  EstadoMotor = true;
}

void AccionFinMotorPrincipal()
{
  EstadoMotor = false;
}

void controlarMotor() 
{
  analogWrite(motor, 255);
  delay(100); // Ajusta según sea necesario para la velocidad del motor
}

void controlsvmotor1()
{
  servo1.write(0);
  delay(863);
  servo1.write(90);
  delay(1000);
  servo1.write(180);
  delay(863);
  servo1.write(90);
}

void controlsvmotor2()
{
  servo2.write(0);
  delay(863);
  servo2.write(90);
  delay(50);
  servo2.write(180);
  delay(863);
  servo2.write(90);
}

void controlsvmotor3()
{
  servo3.write(0);
  delay(863);
  servo3.write(90);
  delay(1000);
  servo3.write(180);
  delay(863);
  servo3.write(90);
}

void controlsvmotor4()
{
  servo4.write(0);
  delay(863);
  servo4.write(90);
  delay(50);
  servo4.write(180);
  delay(863);
  servo4.write(90);
}

void controlsvmotor5() 
{
  servo5.write(0);
  delay(863);
  servo5.write(90);
  delay(1000);
  servo5.write(180);
  delay(863);
  servo5.write(90);
}

void controlsvmotor6()
{
  servo6.write(0);
  delay(863);
  servo6.write(90);
  delay(50);
  servo6.write(180);
  delay(863);
  servo6.write(90);
}

void controlsvmotor7()
{
  servo7.write(0);
  delay(863);
  servo7.write(90);
  delay(1000);
  servo7.write(180);
  delay(863);
  servo7.write(90);
}

void controlsvmotor8()
{
  servo8.write(0);
  delay(863);
  servo8.write(90);
  delay(50);
  servo8.write(180);
  delay(863);
  servo8.write(90);
}

void controlsvmotor9()
{
  servo9.write(0);
  delay(863);
  servo9.write(90);
  delay(1000);
  servo9.write(180);
  delay(863);
  servo9.write(90);
}

void controlsvmotor10()
{
  servo10.write(0);
  delay(863);
  servo10.write(90);
  delay(50);
  servo10.write(180);
  delay(863);
  servo10.write(90);
}

void controlsvmotor11()
{
  servo11.write(0);
  delay(863);
  servo11.write(90);
  delay(1000);
  servo11.write(180);
  delay(863);
  servo11.write(90);
}

void controlsvmotor12()
{
  servo12.write(0);
  delay(863);
  servo12.write(90);
  delay(50);
  servo12.write(180);
  delay(863);
  servo12.write(90);
}

void ActivarElecAcido()
{
  digitalWrite(ElecAcido, HIGH);
}

void DesacElecAcido()
{
  digitalWrite(ElecAcido, LOW);
}

void ActivarElecInTanque()
{
  digitalWrite(ElecInTanque, HIGH);
}

void DesacElecInTanque()
{
  digitalWrite(ElecInTanque, LOW);
}

void ActivarElecOutTanque()
{
  digitalWrite(ElecOutTanque, HIGH);
}

void DesacElecOutTanque()
{
  digitalWrite(ElecOutTanque, LOW);
}