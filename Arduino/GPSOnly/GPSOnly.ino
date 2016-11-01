#include <TinyGPS++.h>
#include <AltSoftSerial.h>
#include <Streaming.h>
//AltSoftSerial 라이브러리는 스케치-라이브러리 포함하기-라이브러리 관리 에서 다운로드 가능
//AltSoftSerial 은 핀 정해져 있음(Tx D9 Rx D8)
//TinyGPSPlus 라이브러리 출처 : http://arduiniana.org/libraries/tinygpsplus/, 설치하려면 ZIP 파일 다운로드 받아서 ZIP 라이브러리 추가 메뉴 사용
//Streaming 라이브러리 출처 : http://arduiniana.org/libraries/streaming/
//블루투스는 Tx를 D0, R1를 D0에 꽂으면 SoftwareSerial 사용 안해도 됨
//하지만 업로드 할 때는 블루투스 모듈을 빼놓아야 함

TinyGPSPlus gps;
AltSoftSerial ss;
static const uint16_t GPS_BAUD = 9600;
HardwareSerial &bt = Serial;
static const uint32_t BT_BAUD = 115200;

//GPS 사용여부 불리언 변수
bool bUseGPS = true;
//이동거리 계산 위함(위도, 경도)
double lastLongitude, lastLatitude;
//GPS를 처음으로 받으면 위도 경도 초기화 후 bFirstGPS를 false로 바꿔 거리 계산을 시작함
bool bFirstGPS = true;

void setup() {
  bt.begin(BT_BAUD);
  ss.begin(GPS_BAUD);
}

// GPS 데이터를를 1초동안 입력받아 TinyGPSPlus 객체가 처리할 수 있게 함
void getGPSData()
{
  static const uint16_t DELAY_MILLISEC = 1000;
  uint32_t start = millis();

  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < DELAY_MILLISEC);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bUseGPS)
  {
    getGPSData();

    if (gps.location.isUpdated())
    {
      if (gps.location.isValid())
      {
        float lat = gps.location.lat();
        float lng = gps.location.lng();

        if (bFirstGPS) bFirstGPS = false;
        else if (gps.speed.kmph() >= 1)
        {
          bt << "Moved distance : " << gps.distanceBetween(lastLatitude, lastLongitude, lat, lng) << "m" << endl;
          bt << "Current Speed : " << gps.speed.kmph() << "km/h" << endl;
        }
        else
          bt << "Moved distance : 0m" << endl << "Current Speed : 0km/h" << endl;

        lastLatitude = lat;
        lastLongitude = lng;
      }
    }
  }
}
