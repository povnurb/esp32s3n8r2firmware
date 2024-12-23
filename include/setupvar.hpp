void setupVar()
{
    deviceSerial = DeviceID();
    deviceManofactura = String(device_manufacture);
    hwVersion = String(device_hw_version);
    sdkVersion = String(ESP.getSdkVersion());
}