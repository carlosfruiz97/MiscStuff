/**
 *
 */
/* -- Libraries --------------------------------------------------------------*/
#include <Bounce2.h>
#include "types.h"

/* -- Setup Buttons ----------------------------------------------------------*/
const uint8_t btn_pins[NB_BUTTONS] = BUTTON_PINS;
const uint8_t secuencia[SEC_LENGTH] = SECUENCIA;

Bounce *buttons = new Bounce[NB_BUTTONS];

GameState_t game_state = GAME_STATE_IDLE;
uint8_t sec_cnt = 0;

/* -- Setup ------------------------------------------------------------------*/
/**
 * @brief Setup Game
 */
void setup()
{
#if (defined DEBUG) && (DEBUG == 1)
    Serial.begin(115200);
    LOGF("\n\n-----------------------------\n"
         "Begin:\n" __FILE__
         "\n");
#endif

    /* Init Buttons */
    for (uint8_t i = 0; i < NB_BUTTONS; i++)
    {
        buttons[i].attach(btn_pins[i], INPUT_PULLUP);
        buttons[i].interval(BUTTON_INTERVAL);
    }

    pinMode(PIN_OUTPUT, OUTPUT);
    set_output(false);

    /* Start condition */
    game_to_start();
}

/**
 * @brief loop
 */
void loop()
{
    switch (game_state)
    {
    case GAME_STATE_IDLE:
        update_idle();
        break;
    case GAME_STATE_PLAYING:
        update_playing();
        break;
    case GAME_STATE_SOLVED:
        update_solved();
        break;
    }
    delay(10);
}

/* -- Game update ------------------------------------------------------------*/
/**
 * @brief update game on GAME_STATE_IDLE
 */
void update_idle(void)
{
}

/**
 * @brief update game on GAME_STATE_PLAYING
 */
void update_playing(void)
{
    uint8_t btn = update_buttons();
    if (btn == 0xFF){
        return;
    }

#if (GAME_TYPE == GAME_TYPE_CONTINUO)

    /* Boton correcto */
    if (btn == secuencia[sec_cnt])
    {
        update_correct(btn);
        if(sec_cnt >= SEC_LENGTH) 
        {
            game_to_solved();
        }
    }

    /* Boton incorrecto */
    else 
    {
        update_wrong(btn);
        if (btn == secuencia[0]){
            /* Se reinicio la secuencia */
            update_correct(btn);
        }
    }

#else /* GAME_TYPE == GAME_TYPE_FIJO*/

#endif /* GAME_TYPE */

}

/**
 * @brief Update when correct btn pressed of secuence
 * @param btn   Index of button pressed
*/
void update_correct(uint8_t btn)
{
    LOGSN("Correct Button", btn);
    sec_cnt++;
}

/**
 * @brief Update when player makes mistake in sequence
*/
void update_wrong(uint8_t btn)
{
    LOGSN("Wrong Button", btn);
    sec_cnt = 0;
}

/**
 * @brief update game on GAME_STATE_SOLVED
 */
void update_solved(void)
{
    delay(10*MS_PER_SECOND);
    game_to_start();
}

/* -- Game transiton ---------------------------------------------------------*/
/**
 * @brief got to GAME_STATE_IDLE
 */
void game_to_idle(void)
{
    sec_cnt = 0;
    game_state = GAME_STATE_IDLE;
    LOGF("Game to IDLE\n");
    set_output(false);
}

/**
 * @brief got to GAME_STATE_PLAYING
 */
void game_to_start(void)
{
    sec_cnt = 0;
    game_state = GAME_STATE_PLAYING;
    LOGF("Game to PLAYING\n");
    update_buttons(); // update once to reset state
    set_output(false);
}

/**
 * @brief got to GAME_STATE_SOLVED
 */
void game_to_solved(void)
{
    game_state = GAME_STATE_SOLVED;
    LOGF("Game to SOLVED\n");
    set_output(true);
}

/* -- Gpio update ------------------------------------------------------------*/
/**
 * @brief Updates buttons
 *        Returns button index of last pressed button
 *        Return 0xFF if no button pressed
 */
uint8_t update_buttons(void)
{
    uint8_t btn = 0xFF;
    /* Update */
    for (uint8_t i = 0; i < NB_BUTTONS; i++)
    {
        buttons[i].update();
    }

    /* Check for pressed*/
    for (uint8_t i = 0; i < NB_BUTTONS; i++)
    {
        if (buttons[i].fell())
        {
            btn = i;
        }
    }
    return btn;
}

/**
 * @brief Set ouput state (true/false)
 */
void set_output(bool state)
{
    digitalWrite(PIN_OUTPUT, state == ACTIVE_SOLVED);
}
