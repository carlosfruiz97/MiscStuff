/////// DEFINIMOS ESTRUCTURAS //////////////////////////////////////////////////
struct paso_t
{
  uint8_t  dest[NUM_SERVOS];
  uint16_t duracion;
};

#define SEC_MAX    30
struct secuencia_t
{
  uint8_t    sec_len;
  paso_t     pasosArry[SEC_MAX];
  const char *nombre;
};


#define DELTA_TIME    100U // ms

class SecuenciaManager
{
public:
  secuencia_t *activeSeq;
  bool        done;
  uint8_t     paso_cnt;
  paso_t      *pasoActual;
  uint32_t    t_inicio_paso;
  uint8_t     n_i;
  uint8_t     startPos[NUM_SERVOS];

  uint8_t N_Steps;
  float   deltaX[NUM_SERVOS];

  SecuenciaManager()
  {
  }


  void activar(secuencia_t& seqAddr)
  {
    done      = false;
    activeSeq = &seqAddr;
    paso_cnt  = 0;

    empezar_paso();
  }


  void empezar_paso()
  {
    /* Extraer paso actual */
    pasoActual = &activeSeq->pasosArry[paso_cnt];

    pasoActual->duracion = pasoActual->duracion < DELTA_TIME ?  DELTA_TIME : pasoActual->duracion;

    LOGSN("Duracion: ", pasoActual->duracion);

    /* Calcular cositas */
    N_Steps = pasoActual->duracion / DELTA_TIME;
    LOGSN("Nsteps: ", N_Steps);
    LOGF("DeltaX: [");
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      deltaX[i]   = (float)(pasoActual->dest[i] - estado.x[i]) / (float)N_Steps;
      startPos[i] = estado.x[i];
      LOGS(",", deltaX[i]);
    }
    n_i = 1;
    LOGN("]");

    mover_paso();
  }


  void mover_paso()
  {
    t_inicio_paso = millis();
    LOGF("Moviendo a : [");
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      estado.x[i] = startPos[i] + deltaX[i] * n_i;
      LOGS(",", estado.x[i]);
    }
    LOGN("]");
    writeServos();
  }


  /* Ejecutar paso */
  bool actualizar_paso()
  {
    if ((millis() - t_inicio_paso) >= DELTA_TIME)
    {
      // Sigueinte etapa
      n_i++;
      if (n_i <= N_Steps)
      {
        mover_paso();
      }
      else
      {
        return true;
        // SIGUIENTE PASO
      }
    }
    return false;
  }


  bool update()
  {
    if (done)
    {
      return true;
    }

    if (actualizar_paso())   // Paso done?
    {
      // Siguiente
      paso_cnt++;
      if (paso_cnt < activeSeq->sec_len)
      {
        empezar_paso();
      }
      else
      {
        done = true;
        return true;
      }
    }

    return false;
  }


  void displayInfo()
  {
    display.clearDisplay();//cls

    display.setCursor(0, 0);
    display.println(F("Modo: PLAY"));
    // display.print(F("Secuencia: "));
    // display.println(activeSeq->nombre);

    // display.print(F("Paso  : "));
    // display.print(paso_cnt);
    // display.print(F(" / "));
    // display.println(activeSeq->sec_len);
    // display.print(F("Done  : "));
    // display.println(done);
    //
    // display.print(F("{"));
    // for (uint8_t i = 0; i < NUM_SERVOS; i++)
    // {
    //   display.print(pasoActual->dest[i]);
    //   if (i != (NUM_SERVOS - 1))
    //   {
    //     display.print(F(", "));
    //   }
    // }
    // display.println(F("}"));
    display.display();
  }
};

SecuenciaManager SeqManag;

//////// DEFINIMOS FUNCIONES ///////////////////////////////////////////////

// void mover_a_paso(secuencia_t *pSec, uint8_t pasoCnt)
// {
//   for (uint8_t i = 0; i < NUM_SERVOS; i++)
//   {
//     estado.x[i] = pSec->pasosArry[pasoCnt].dest[i];
//   }
//   pSec->start_time = millis();
//
//   writeServos();
// }
//
//
// void start_secuencia(secuencia_t *pSec)
// {
//   pSec->activa     = true;
//   pSec->done       = false;
//   pSec->sec_cnt    = 0;
//   pSec->start_time = millis();
//
//
//   mover_a_paso(pSec, 0);
//
//   LOGFN("Iniciamos secuencia");
// }
//
//
// bool is_paso_done(secuencia_t *pSec)
// {
//   return millis() >= (pSec->start_time + pSec->pasosArry[pSec->sec_cnt].delta_ms);
// }
//
//
// // return si a finalizado!
// bool actualizar_secuencia(secuencia_t *pSec)
// {
//   // Guard Clause
//   if (pSec->done)
//   {
//     return true;
//   }
//
//   paso_t *pPasoActual = &pSec->pasosArry[pSec->sec_cnt];
//
//   if (is_paso_done(pSec))
//   {
//     pSec->sec_cnt++;
//     if (pSec->sec_cnt >= pSec->sec_len)
//     {
//       pSec->sec_cnt = pSec->sec_len - 1;
//       pSec->done    = true;
//       pSec->activa  = false;
//       LOGFN("Secuencia finalizada");
//       return true;
//     }
//
//     // Mover al siguiente paso
//     LOGSN("Siguiente Paso: ", pSec->sec_cnt);
//     mover_a_paso(pSec, pSec->sec_cnt);
//   }
//
//   return false;
// }


// void update_display(secuencia_t *pSec)
// {

// }
