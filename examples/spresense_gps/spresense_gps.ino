#include <sunset.h>
#include <GNSS.h>

/* e.g. JST (Japan Standard Time) timezone is 9 */
#define TZ_OFFSET 9

SunSet sun;
SpGnss gps;
SpNavData nav;

void setup()
{
  Serial.begin(115200);

  printf("A sunrise/sunset example by using Spresense GPS\n");

  /* GNSS initialize and start */
  gps.begin();
  gps.select(GPS);
  gps.select(GLONASS);
  gps.select(QZ_L1CA);
  gps.start(COLD_START);
  /* Period to acquire GNSS data: 5sec */
  gps.setInterval(5);
}

void loop()
{
  int sunrise;
  int sunset;

  /* GNSS loop until getting time and position */

  do {
    gps.waitUpdate(); /* wait until 5sec timeout or data gotten */
    gps.getNavData(&nav);
  } while ((nav.posFixMode == FixInvalid) || !nav.posDataExist);

  printf("%04d/%02d/%02d lat=%lf lon=%lf\n",
         nav.time.year, nav.time.month, nav.time.day,
         nav.latitude, nav.longitude);

  /* Calcurate sunrise and sunset */

  sun.setPosition(nav.latitude, nav.longitude, TZ_OFFSET);
  sun.setCurrentDate(nav.time.year, nav.time.month, nav.time.day);
  sunrise = static_cast<int>(sun.calcSunrise());
  sunset = static_cast<int>(sun.calcSunset());

  printf("Sunrise at %02d:%02d, Sunset at %02d:%02d\n",
         (sunrise / 60), (sunrise % 60), (sunset / 60), (sunset % 60));
}
