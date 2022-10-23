/*******************************************************************************
 *  Titulo: Probar IMU
 *
 * Placa: Nano
 * Descripcion:
 *
 * Subir Codigo:
 *     arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328 IMU_MPU.ino
 *     arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328 IMU_MPU.ino
 *
 *     arduino-cli monitor -p /dev/ttyUSB0
 *
 * TODO: Revisar si se cuelga el I2C y reiniciarlo!
 *
 ******************************************************************************/
#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)           Serial.println(x)
#define LOG(x)            Serial.print(x)
#define LOGX(x)           Serial.print(x, HEX);
#define LOGSN(s, x)       { Serial.print(F(s)); Serial.print(" "); Serial.println(x); }
#define LOGS(s, x)        { Serial.print(F(s)); Serial.print(" "); Serial.print(x); }
#define LOGSX(s, x)       { Serial.print(F(s)); Serial.print(" "); Serial.print(x, HEX); }
#define PRINTF(s, ...)    { Serial.printf(s, __VA_ARGS__); }
#define LOGF(s)           Serial.print(F(s))
#define LOGFN(s)          Serial.println(F(s))
#else
#define LOG(x)
#define LOGN(x)
#define LOGX(x)
#define LOGSN(s, x)
#define LOGS(s, x)
#define LOGSX(s, x)
#deinfe PRINTF(s, ...)
#define LOGF(s)
#define LOGFN(s)
#endif


/*
 * SCL - A5
 * SDA - A4
 */

/************************************************
 *  Librerias Externas
 ***********************************************/
#include <MPU6050_tockn.h>
#include <Wire.h>


/*******************************************************************************
*  ACELEROMETRO Y JUEGO INCLINACION
*******************************************************************************/
MPU6050 mpu6050(Wire);


enum cara_t
{
  xp, xn, yp, yn, zp, zn, nada
};

enum estado_juego_t
{
  STANDBY,
  EN_JUEGO,
  VICTORIA
}
estado_juego = STANDBY;

#define SEC_LEN    5
cara_t  sec_correcta[SEC_LEN] = { xp, zp, xp, yn, xp };
uint8_t sec_counter           = 0;

/***************** PARAMETROS ACELEROMETRO Y FILTRO **************************/
/*margen de error sobre aceleracion (0-1000) para detectar cara actal*/
const int16_t margen_error = 450;

/* % de cuttoff de valor filtrado para considerar Cara*/
const int8_t thFilt = 70;

/* alpha entre  0 y 1
 * alpha ~1 Peso a lo reciente
 * alpha ~0 Peso al historial
 */
float alpha = 0.1;

/******** VARIABLES USADAS EN EL FILTRO **************************************/
int16_t gx, gy, gz;           // Valores raw acelerometro
float   cara_arry_raw[6];     // Array valor cara raw
float   cara_arry_filt[6];    // Array valor cara filtrado

cara_t cara_actual   = nada,  // Valor de cara actual con cambio
       cara_anterior = nada;  // Valor de cara actual con cambio anterior (por si vibra sin rotar)

/************************** INICIAR FUNCIONES *********************************/
void ActualizarJuego();
void InicioJuego();
void Victoria();
void update_mpu();
bool calcular_nueva_cara();
void setRawArryToZero();
uint8_t calcular_cara_actual();



/*****************************************************************************
*  @brief Setup
*****************************************************************************/
void setup()
{
  ////////////////////////////
  // Serial Init
#if DEBUG == 1
  Serial.begin(9600);
  while (!Serial)
  {
  }
  delay(50);
  //  LOGN("\n\n---------------\n  EMPEZAMOS");
  //  LOGN("x\ty\tz\tmax\tmin\n");
  LOGN("x\ty\tz\n");
#endif

  Wire.begin();
  mpu6050.begin();

  InicioJuego();
  //  mpu6050.calcGyroOffsets(false);
}


/*****************************************************************************
*  @brief Loop
*****************************************************************************/
void loop()
{
  switch (estado_juego)
  {
  case EN_JUEGO:
    ActualizarJuego();
    break;

  case STANDBY:
    break;

  case VICTORIA:
    break;
  }
}


/*****************************************************************************
*  @brief Actualizamos Estado Juego
*****************************************************************************/
void ActualizarJuego()
{
  // Actualizar valores accelerometro
  update_mpu();

  // Ver si hay nueva cara -> cara_actual
  if (!calcular_nueva_cara())
  {
    return;
  }

  LOGSN("\nSec Counter:     ", sec_counter);
  LOGSN("Colocamos a Cara:", cara_actual);
  LOGSN("Cara Correcta:   ", sec_correcta[sec_counter]);

  // Si la Cara actual no es la que toca -> MAL
  if (cara_actual != sec_correcta[sec_counter])
  {
    sec_counter = 0;
    LOGFN(" > INCORRECTO");
    return;
  }

  LOGFN(" > CORRECTO");
  sec_counter++;

  if (sec_counter >= SEC_LEN)
  {
    Victoria();
  }
}


/*******************************************
*  @brief Iniciar Juego
*******************************************/
void InicioJuego()
{
  sec_counter  = 0;
  estado_juego = EN_JUEGO;
}


/*******************************************
*  @brief Victoria
*******************************************/
void Victoria()
{
  LOGFN("\n ------- VICTORIA -------\n");
  estado_juego = VICTORIA;
}


/*******************************************
*  @brief Actualizar accelerometro
*******************************************/
void update_mpu()
{
  mpu6050.update();

  gx = mpu6050.getAccX() * 1000;
  gy = mpu6050.getAccY() * 1000;
  gz = mpu6050.getAccZ() * 1000;
}


/*******************************************
*  @brief Calcular si nueva cara
*******************************************/
bool calcular_nueva_cara()
{
  /* Ver cual es la cara actual */
  cara_t cara_raw = calcular_cara_actual();

  /* Aplicar Filtro */
  setRawArryToZero();
  if (cara_raw != nada)
  {
    cara_arry_raw[cara_raw] = 100;
  }

  //  LOGF("Vfilt: ");
  bool es_cara_anterior, es_cara_actual;
  for (uint8_t i = 0; i < 6; i++)
  {
    // Calcular si estaba ya en la cara actual
    es_cara_anterior = cara_arry_filt[i] > thFilt ? true : false;

    // Calcular Valor filtrado de cada cara
    cara_arry_filt[i] = alpha * cara_arry_raw[i] + (1 - alpha) * cara_arry_filt[i];

    // Calcula si esta en cara actual
    es_cara_actual = cara_arry_filt[i] > thFilt ? true : false;

    //    LOGS("|", cara_arry_filt[i]);
    //    LOGS("", es_cara_arry[i]);
    //    LOGF("|\t");

    // Detectar si pico
    if (es_cara_actual && !es_cara_anterior)
    {
      cara_actual = i;

      if (cara_actual == cara_anterior)
      {
        return false;
      }

      cara_anterior = cara_actual;
      return true;
    }
  }
  //  LOGN("");

  return false;
}


/*******************************************
*  @brief Pone array a zero
*******************************************/
void setRawArryToZero()
{
  for (uint8_t i = 0; i < 6; i++)
  {
    cara_arry_raw[i] = 0;
  }
}


/*******************************************
*  @brief Calcula la cara actual
*******************************************/
uint8_t calcular_cara_actual()
{
  bool inX, inY, inZ;

  inX = abs(gx) > (1000 - margen_error);
  inY = abs(gy) < margen_error;
  inZ = abs(gz) < margen_error;
  if (inX && inY && inZ)
  {
    return gx > 0 ? xp : xn;
  }

  inX = abs(gx) < margen_error;
  inY = abs(gy) > (1000 - margen_error);
  inZ = abs(gz) < margen_error;
  if (inX && inY && inZ)
  {
    return gy > 0 ? yp : yn;
  }

  inX = abs(gx) < margen_error;
  inY = abs(gy) < margen_error;
  inZ = abs(gz) > (1000 - margen_error);
  if (inX && inY && inZ)
  {
    return gz > 0 ? zp : zn;
  }

  return nada;
}
