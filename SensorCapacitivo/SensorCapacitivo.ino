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
//#define PRINTF(s, ...)    { Serial.printf(s, __VA_ARGS__); }
#else
#define LOG(x)
#define LOGN(x)
#define LOGX(x)
#define LOGSN(s, x)
#define LOGS(s, x)
#define LOGSX(s, x)
#define PRINTF(s, ...)
#define LOGF(s)
#define LOGFN(s)
#endif

const uint8_t controlPin[4] = {5, 4, 3, 2};

/*****************************************************************************/
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
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

const uint16_t thresh = 1000;

class Mano {
  private:
    bool timerStarted;
    uint16_t tlow;
    bool brose, bfell;
    bool rawPuls, pMano, pManoAnt;
    uint16_t low_interval;

  public:
    Mano()
    {
      this->low_interval = 120;
      pManoAnt = false;
      pMano = false;
    }

    void update()
    {
      rawPuls = analogRead(A0) < thresh;

      if (rawPuls)
      {
        pMano = true;
        timerStarted = false;
      }
      else
      {
        /* timer started */
        if (!timerStarted)
        {
          timerStarted = true;
          tlow = millis();
        }
        else {
          if ((millis() - tlow) > low_interval)
          {
            pMano = false;
          }
        }
      }

      brose = false;
      bfell = false;

      if (pManoAnt != pMano)
      {
        if (pMano) brose = true;
        else  bfell = true;
      }

      pManoAnt = pMano;
    }

    bool read()
    {
      return pMano;
    }

    bool rose()
    {
      return brose;
    }
    bool fell()
    {
      return bfell;
    }


};

Mano manosArry[2] ;

void loop() {

  //  setChannel(2);

  for (uint8_t i = 0; i < 2; i++)
  {
    // Select Channel
    SelectChannel(i);

    // Update!
    manosArry[i].update();

    uint16_t val = analogRead(A0);
    //    LOG(val); LOG("\t");
    LOG(((uint8_t)manosArry[i].read()) * 100 * (i + 1)); LOG("\t");

  }
  LOG(1025); LOG("\t");

  LOG("\n");

  delay(10);
}
