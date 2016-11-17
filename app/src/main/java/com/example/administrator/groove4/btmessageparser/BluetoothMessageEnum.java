package com.example.administrator.groove4.btmessageparser;

/**
 * Created by Home on 2016-11-17.
 */
public enum BluetoothMessageEnum {
    GPS_STOP_MSG("GPS_STOP"),
    GPS_STOP_RESPONSE("GPS_STOPPED"),
    GPS_START_MSG("GPS_START"),
    GPS_START_RESPONSE("GPS_STARTED"),
    GPS_DISTANCE_MSG("GPS_DISTANCE"),
    GPS_SPEED_MSG("GPS_SPEED"),
    GPS_ALT_MSG("GPS_ALT"),
    GPS_HDG_MSG("GPS_HEADING"),
    GPS_WAITSIG_MSG("GPS_WAITSIG"),
    GPS_INIT_MSG("GPS_INIT"),
    GPS_USE_ALTITUDE("GPS_USE_ALT"),
    GPS_NOUSE_ALTITUDE("GPS_NOUSE_ALT"),
    GPS_USE_ALT_RESPONSE("GPS_START_USE_ALT"),
    GPS_NOUSE_ALT_RESPONSE("GPS_STOP_USE_ALT"),
    GPS_SAT_IN_USE_MSG("GPS_SAT_IN_USE"),
    HRM_MSG("HEART_RATE"),
    ERROR("ERROR")
    ;

    private final String _text;

    private BluetoothMessageEnum(final String txt) {
        this._text = txt;
    }

    @Override
    public String toString() {
        return _text;
    }
}