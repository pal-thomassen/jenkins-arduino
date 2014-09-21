#!/usr/local/bin/python3

import time
import serial
from jenkinsapi.jenkins import Jenkins

def read_from_serial(ser):
  print("Reading...")
  try:
    line = ser.readline().decode(encoding="UTF-8")
    if "start_job" in line:
      print("Start job already!")
      start_job("Jenkins test job")
  except serial.serialutil.SerialException:
    print("Failing with serialport")


def get_server_instance():
  jenkins_url = 'http://localhost:8080/jenkins'
  server = Jenkins(jenkins_url)
  return server

def get_job_details(ser):
  server = get_server_instance()
  for job in server.get_jobs():
    job_instance = server.get_job(job[0])
    check_job_status(job_instance, ser)

def check_job_status(job_instance, ser):
  if (job_instance.is_running()):
    writeSerial(b"job_running\n", ser)
  elif not job_instance.get_last_build().is_good():
    writeSerial(b"job_fail\n", ser)
  else:
    writeSerial(b"job_good\n", ser)

def writeSerial(string_to_write, ser):
    ser.write(string_to_write)

def start_job(job_name):
  server = get_server_instance()
  job_instance = server.get_job(job_name)
  if not job_instance.is_queued():
    job_instance.invoke()


if __name__ == '__main__':
  ser = serial.Serial("/dev/cu.usbserial-A40168KZ", 9600, timeout=5)
  try:
    while True:
      read_from_serial(ser)
      get_job_details(ser)
      time.sleep(1)
  finally:
    ser.close()
