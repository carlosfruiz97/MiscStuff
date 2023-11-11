
#ifndef __TYPES_H
#define __TYPES_H

/* -- Constants---------------------------------------------------------------*/
#define MS_PER_SECOND (1000)

#define GAME_TYPE_CONTINUO  1
#define GAME_TYPE_FIJO      2

#define BUTTON_INTERVAL     25

/* -- Game config ------------------------------------------------------------*/
#define PIN_OUTPUT      13 /*ej rele*/
#define ACTIVE_SOLVED   LOW    /*estado al resolver*/

#define NB_BUTTONS      3
#define BUTTON_PINS     {2, 3, 4}

#define SEC_LENGTH      3
#define SECUENCIA       {0, 1, 2}

#define GAME_TYPE       GAME_TYPE_CONTINUO




/**
 * @enum State
*/
typedef enum GameState_e 
{
  GAME_STATE_IDLE,
  GAME_STATE_PLAYING,
  GAME_STATE_SOLVED,
} GameState_t;


/* -- DEBUG ------------------------------------------------------------------*/
#define DEBUG   1

#if DEBUG == 1
#define LOG(x) Serial.print(x)
#define LOGX(x) Serial.print(x, HEX)
#define LOGS(s, x)      \
  {                     \
    Serial.print(F(s)); \
    Serial.print(" ");  \
    Serial.print(x);    \
  }
#define LOGSX(s, x)       \
  {                       \
    Serial.print(F(s));   \
    Serial.print(" ");    \
    Serial.print(x, HEX); \
  }
#define LOGSN(s, x) { LOGS(s,x); LOGF("\n"); } 
#define LOGF(s) Serial.print(F(s))
#else
#define LOG(x)
#define LOGX(x)
#define LOGS(s, x)
#define LOGSX(s, x)
#define LOGSN(s, x)  
#define LOGF(s)
#endif

#endif /* __TYPES_H */