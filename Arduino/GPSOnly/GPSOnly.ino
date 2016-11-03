#include <TinyGPS++.h>
#include <AltSoftSerial.h>
#include <Streaming.h>
#include <math.h>
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

//GPS 정지 메세지(임의로 설정해 놓았기 때문에 변경가능)
static const char* GPS_STOP_MSG = "GPS_STOP";
static const char* GPS_STOP_RESPONSE = "GPS_STOPPED";
//GPS 시작 메세지(임의로 설정해 놓았기 때문에 변경가능)
static const char* GPS_START_MSG = "GPS_START";
static const char* GPS_START_RESPONSE = "GPS_STARTED";
//GPS 속도, 거리, 고도 메세지, 여기에서 값 붙여서 보냄(임의로 설정해 놓았기 때문에 변경가능)
static const char* GPS_DISTANCE_MSG = "GPS_DISTANCE ";
static const char* GPS_SPEED_MSG = "GPS_SPEED ";
static const char* GPS_ALT_MSG = "GPS_ALT ";
//GPS 신호대기 메세지(임의로 설정해 놓았기 때문에 변경가능)
static const char* GPS_WAITSIG_MSG = "GPS_WAITSIG";
//GPS 최초 시작 메세지(임의로 설정해 놓았기 때문에 변경가능)
static const char* GPS_INIT_MSG = "GPS_INIT ";
//GPS 거리 계산에 고도 사용여부 메세지
static const char* GPS_USE_ALTITUDE = "GPS_USE_ALT";
static const char* GPS_NOUSE_ALTITUDE = "GPS_NOUSE_ALT";
static const char* GPS_USE_ALT_RESPONSE = "GPS_START_USE_ALT";
static const char* GPS_NOUSE_ALT_RESPONSE = "GPS_STOP_USE_ALT";

//GPS 사용여부 불리언 변수
bool bUseGPS = true;
//이동거리 계산에 고도 계산여부 불리언 변수
bool bUseGPSAlt = false;
//이동거리 계산 위함(위도, 경도, 고도)
double lastLongitude, lastLatitude, lastAltitude;

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

inline bool operator==(const char* & lhs,  const String &rhs) { return rhs.equals(lhs); }
void setup() {
  bt.begin(BT_BAUD);
  ss.begin(GPS_BAUD);

  //주어진 사이클만큼 GPS 준비시킴
  bt << GPS_INIT_MSG << GPS_INIT_CYCLE * DELAY_MILLISEC / 1000 << endl;
  for (register uint8_t i = 0; i < GPS_INIT_CYCLE; ++i)
  {
    getGPSData();
    if(gps.location.isUpdated() && gps.location.isValid())
    {
      lastLatitude = gps.location.lat();
      lastLongitude = gps.location.lng();
      lastAltitude = gps.altitude.meters();
    }
  }
}

void loop() {
  if (true == bUseGPS)
  {
    getGPSData();

    if (gps.location.isUpdated())
    {
      if (gps.location.isValid())
      {
        double currentLat = gps.location.lat();
        double currentLng = gps.location.lng();
        double currentSpd = gps.speed.kmph();
        double currentAlt = gps.altitude.meters();
        double movedDistance = gps.distanceBetween(lastLatitude, lastLongitude, currentLat, currentLng);

        if (true == bUseGPSAlt)
          movedDistance = sqrt(movedDistance * movedDistance + pow(abs(lastAltitude - currentAlt), 2));

        //거리와 고도는 m(미터)단위, 속도는 km/h 단위
        if (currentSpd >= GPS_IGNORE_SPEED)
        {
          bt << GPS_DISTANCE_MSG << movedDistance << endl;
          bt << GPS_SPEED_MSG << currentSpd << endl;
          bt << GPS_ALT_MSG << currentAlt << endl;
        }
        else
          bt << GPS_DISTANCE_MSG << '0' << endl << GPS_SPEED_MSG << '0' << endl << GPS_ALT_MSG << lastAltitude << endl;

        lastLatitude = currentLat;
        lastLongitude = currentLng;
        lastAltitude = currentAlt;
      }
    }
    else bt << GPS_WAITSIG_MSG << endl;
  }

  //메세지 처리 -> 명령 받으면 실행 후 답장 보냄
  if (bt.available())
  {
    String msg = bt.readString();
    if (true == bUseGPS && GPS_STOP_MSG == msg)
    {
      bUseGPS = false;
      bt.println(GPS_STOP_RESPONSE);
    }
    else if (false == bUseGPS && GPS_START_MSG == msg)
    {
      bUseGPS = true;
      bt.println(GPS_START_RESPONSE);
    }
    else if (false == bUseGPSAlt && GPS_USE_ALTITUDE == msg)
    {
      bUseGPSAlt = true;
      bt.println(GPS_USE_ALT_RESPONSE);
    }
    else if (true == bUseGPSAlt && GPS_NOUSE_ALTITUDE == msg)
    {
      bUseGPSAlt = false;
      bt.println(GPS_NOUSE_ALT_RESPONSE);
    }
  }
}
