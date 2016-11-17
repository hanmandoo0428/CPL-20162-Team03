package com.example.administrator.groove4.btmessageparser;

import java.io.InputStream;

/**
 * Created by Home on 2016-11-17.
 */
public class BluetoothMessageParser {
    static public BluetoothMessageEnum getmessageType(String msgFront) {
        switch(msgFront) {
            case "GPS_STOPPED":
                return BluetoothMessageEnum.GPS_STOP_RESPONSE;
            case "GPS_START_RESPONSE":
                return BluetoothMessageEnum.GPS_START_RESPONSE;
            case "GPS_DISTANCE":
                return BluetoothMessageEnum.GPS_DISTANCE_MSG;
            case "GPS_SPEED":
                return BluetoothMessageEnum.GPS_SPEED_MSG;
            case "GPS_ALT":
                return BluetoothMessageEnum.GPS_ALT_MSG;
            case "GPS_HEADING":
                return BluetoothMessageEnum.GPS_HDG_MSG;
            case "GPS_WAITSIG":
                return BluetoothMessageEnum.GPS_WAITSIG_MSG;
            case "GPS_INIT":
                return BluetoothMessageEnum.GPS_INIT_MSG;
            case "GPS_START_USE_ALT":
                return BluetoothMessageEnum.GPS_USE_ALT_RESPONSE;
            case "GPS_STOP_USE_ALT":
                return BluetoothMessageEnum.GPS_NOUSE_ALT_RESPONSE;
            case "GPS_SAT_IN_USE":
                return BluetoothMessageEnum.GPS_SAT_IN_USE_MSG;
            case "HRM_MSG":
                return BluetoothMessageEnum.HRM_MSG;
            default:
                return BluetoothMessageEnum.ERROR;
        }
    }
}
