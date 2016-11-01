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
HardwareSerial &bt = Serial;
//블루투스 baud rate는 모듈에 맞춰서 변경가능
//GPS baud rate는 9600 고정
static const uint32_t BT_BAUD = 115200;
static const uint16_t GPS_BAUD = 9600;

//GPS 신호 읽을 시간, ms 단위
static const uint16_t DELAY_MILLISEC = 1000;
//GPS 초기화 사이클 회숫
static const uint8_t GPS_INIT_CYCLE = 5;
//GPS 오차 무시하기 위한 최저 속도
static const double GPS_IGNORE_SPEED = 1.5;

// GPS 데이터를를 DELAY_MILLISEC/1000초동안 입력받아 TinyGPSPlus 객체가 처리할 수 있게 함
void getGPSData()
{
  uint32_t start = millis();

  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < DELAY_MILLISEC);
}

//GPS 사용여부 불리언 변수
bool bUseGPS = true;
//이동거리 계산 위함(위도, 경도)
double lastLongitude, lastLatitude;

void setup() {
  bt.begin(BT_BAUD);
  ss.begin(GPS_BAUD);

  //주어진 사이클만큼 GPS 준비시킴
  bt << "Initializing GPS for " << GPS_INIT_CYCLE * DELAY_MILLISEC / 1000<< " seconds..." << endl;
  for (register uint8_t i = 0; i < GPS_INIT_CYCLE; ++i)
  {
    getGPSData();
    if(gps.location.isUpdated() && gps.location.isValid())
    {
      lastLatitude = gps.location.lat();
      lastLongitude = gps.location.lng();
    }
  }
}

void loop() {
  if (bUseGPS)
  {
    getGPSData();

    if (gps.location.isUpdated())
    {
      if (gps.location.isValid())
      {
        double lat = gps.location.lat();
        double lng = gps.location.lng();
        double currentSpd = gps.speed.kmph();

        if (currentSpd >= GPS_IGNORE_SPEED)
        {
          bt << "Moved distance : " << gps.distanceBetween(lastLatitude, lastLongitude, lat, lng) << "m" << endl;
          bt << "Current Speed : " << currentSpd << "km/h" << endl;
        }
        else
          bt << "Moved distance : 0m" << endl << "Current Speed : 0km/h" << endl;

        lastLatitude = lat;
        lastLongitude = lng;
      }
    }
    else bt << "Waiting for good GPS signal..." << endl;
  }
}
