/*
 Copyright (c) 2011 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#include <serial.h>
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

/*
 * The serial connection.
 */
static HANDLE serial;

/*
 * The baud rate in bps.
 */
static int baudrate = 500000;

/*
 * Connect to a serial port.
 */
int serial_connect(char* portname)
{
  int ret = 0;
  DWORD accessdirection = GENERIC_READ | GENERIC_WRITE;
  char scom[16];
  snprintf(scom, sizeof(scom), "\\\\.\\%s", portname);
  serial = CreateFile(scom, accessdirection, 0, 0, OPEN_EXISTING, 0, 0);
  if (serial == INVALID_HANDLE_VALUE)
  {
    ret = -1;
  }
  DCB dcbSerialParams =
  { 0 };
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  if (!GetCommState(serial, &dcbSerialParams))
  {
    ret = -1;
  }
  dcbSerialParams.BaudRate = baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  if (!SetCommState(serial, &dcbSerialParams))
  {
    ret = -1;
  }
  COMMTIMEOUTS timeouts =
  { 0 };
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if (!SetCommTimeouts(serial, &timeouts))
  {
    ret = -1;
  }
  return ret;
}

/*
 * Send a usb report to the serial port.
 */
int serial_send(void* pdata, unsigned int size)
{
  DWORD dwBytesWrite = 0;

  WriteFile(serial, (uint8_t*)pdata, size, &dwBytesWrite, NULL);

  return dwBytesWrite;
}

int serial_read(void* pdata, unsigned int size)
{
  DWORD dwBytesWrite = 0;

  if(ReadFile(serial, (uint8_t*)pdata, size, &dwBytesWrite, NULL))
  {
    return dwBytesWrite;
  }
  return 0;
}

int serial_recv(void* pdata, unsigned int size)
{
	/*
	 * TODO: test this.
	 */
  return serial_read(pdata, size);
}

/*
 * Close the serial port.
 */
void serial_close()
{
  CloseHandle(serial);
}