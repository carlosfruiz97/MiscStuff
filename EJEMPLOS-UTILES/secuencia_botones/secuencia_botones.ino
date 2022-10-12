
/************************************************
    Ejemlpo Secuenica de botones
 ***********************************************/

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)        Serial.println(x)
#define LOG(x)         Serial.print(x)
#define LOGX(x)        Serial.print(x, HEX);
#define LOGSN(s, x)    { Serial.print(F(s)); Serial.print(" "); Serial.println(x); }
#define LOGS(s, x)     { Serial.print(F(s)); Serial.print(" "); Serial.print(x); }
#define LOGSX(s, x)    { Serial.print(F(s)); Serial.print(" "); Serial.print(x, HEX); }
#else
#define LOG(x)
#define LOGN(x)
#define LOGX(x)
#define LOGSN(s, x)
#define LOGS(s, x)
#define LOGSX(s, x)
#endif

/************************************************
    Librerias Externas
 ***********************************************/
#include <Bounce2.h>

/************************************************
    Botones
 ***********************************************/
/*
  Ejemplo: sec_correcta = {b0,b1,b2,b0}
  -true:  b0, b0, b0, b1, b2 ,b0 CORRECTO!
  -false: b0, b0, b1, b2 FALSO!, b0, b1, b2, b0 CORRECTO!
*/
const bool intento_continuo = true;

// GPIO
#define N_BOTONES  4
const uint8_t BUTTON_PINS[N_BOTONES] = {D3, D4, D5, D6};

enum boton_t {b0, b1, b2, b3, bNone};
Bounce * buttons = new Bounce[N_BOTONES];

// Secuencia correcta, con indice
#define SEC_LEN  5
const boton_t sec_correcta[SEC_LEN] = {b0, b1, b2, b3, b0};

uint8_t sec_cnt = 0;
bool correcto = true;
enum estado_juego_bot_t {
  STANDBY,
  VICTORIA,
  DERROTA
}
estado_juego_bot = STANDBY;

/************************************************
   @brief Setup
 ***********************************************/
void setup()
{
  ////////////////
  // Serial Init
#if DEBUG == 1
  Serial.begin(9600);
  while (!Serial)
  {  }
  delay(50);
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

  /////////////////
  // Setup Botones
  for (int i = 0; i < N_BOTONES; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(25);              // interval in ms
  }
}

/************************************************
   @brief Loop
 ***********************************************/
void loop()
{
  //  actualizar_juego_botones_continuo();
  actualizar_juego_botones_fijo();
}



/************************************************
   @brief Actualiza maquina estado botones

   Modo solucion continua:
   Sec: b1,b2,b3,b4
   b0,b0,b0,b1,b2,b3,b4 CORRECTO!
 ***********************************************/
// sec_cnt, estado_juego_bot
void actualizar_juego_botones_continuo()
{ //  boton_t bPressed = update_buttons();
  switch (estado_juego_bot)
  {
    case (STANDBY):
      // Get pressed boton
      boton_t bPressed = update_buttons();
      if (bPressed == bNone) {
        return;
      }

      boton_t bCorrecto =  sec_correcta[sec_cnt++];

      LOGS("bPressed: ", bPressed);
      LOGS("\tbCorrecto: ", bCorrecto);
      LOGS("\tsec_cnt: ", sec_cnt);
      LOG("\n");

      if (bPressed != bCorrecto)
      {
        sec_cnt = 0;
        LOGN("INCORRECTO");
        return;
      }


      if (sec_cnt >= SEC_LEN)
      {
        estado_juego_bot = VICTORIA;
        Victoria();
      }

      break;
  }
}

/************************************************
   @brief Actualiza maquina estado botones

   Modo solucion fija:
   Sec: b1,b2,b3
   b0,b1,b2,INCORRECTO, b1,b2,b3 CORRECTO!
 ***********************************************/
// sec_cnt, estado_juego_bot
void actualizar_juego_botones_fijo()
{
  switch (estado_juego_bot)
  {
    case (STANDBY):
      // Get pressed boton
      boton_t bPressed = update_buttons();
      if (bPressed == bNone) {
        return;
      }

      boton_t bCorrecto =  sec_correcta[sec_cnt++];
      correcto &= bPressed == bCorrecto;

      LOGS("bPressed: ", bPressed);
      LOGS("\tbCorrecto: ", bCorrecto);
      LOGS("\tsec_cnt: ", sec_cnt);
      LOGS("\tcorrecto: ", correcto);
      LOG("\n");

      if (sec_cnt < SEC_LEN)
      {
        return;
      }

      if (correcto)
      {
        estado_juego_bot = VICTORIA;
        Victoria();
      }
      else {
        estado_juego_bot = DERROTA;
        Derrota();
      }
      reset_juego();
  }
}

/************************************************
  @brief Reset del juego
 ***********************************************/
void reset_juego()
{
  sec_cnt = 0;
  correcto = true;
  estado_juego_bot = STANDBY;
}

/************************************************
  @brief Funcion al solucionar el juego
 ***********************************************/
void Victoria()
{
  LOGN("VICTORIA!");
}

/************************************************
  @brief Funcion al perder el juego
 ***********************************************/
void Derrota()
{
  LOGN("INCORRECTO!");
}
/************************************************
   @brief Actualizar botones y obtener si pulsado
 ***********************************************/
boton_t update_buttons()
{
  boton_t bPressed = bNone;
  for (int i = 0; i < N_BOTONES; i++) {
    buttons[i].update();

    if (buttons[i].fell())
    {
      bPressed = (boton_t) i;
    }
  }
  return bPressed;
}
