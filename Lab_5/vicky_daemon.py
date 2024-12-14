import os
import time
from ftplib import FTP

SANDY_SERVER = '192.168.50.4'
SANDY_USER = 'lab3'
SANDY_PASSWORD = 'captainhook'
LOG_DIRECTORY = '/var/log/snort'
SENT_LOGS = set()

def send_file(filename):
    try:
        ftp = FTP(SANDY_SERVER)
        ftp.login(SANDY_USER, SANDY_PASSWORD)
        ftp.cwd('/home/lab3/received_logs')
        with open(os.path.join(LOG_DIRECTORY, filename), 'rb') as f:
            ftp.storbinary(f'STOR {filename}', f)
        ftp.quit()
        print(f"Sent {filename} to sandy successfully")
    except Exception as e:
        print(f"Failed to send {filename} to sandy: {e}")

def is_file_complete(filepath):
    prev_size = -1
    for _ in range(3):
        curr_size = os.path.getsize(filepath)
        if curr_size == prev_size:
            return True
        prev_size = curr_size
        time.sleep(10)

def monitor_logs():
    while True:
        for filename in os.listdir(LOG_DIRECTORY):
            filepath = os.path.join(LOG_DIRECTORY, filename)
            if filename not in SENT_LOGS and is_file_complete(filepath):
                send_file(filename)
                SENT_LOGS.add(filename)
        time.sleep(30)

if __name__ == "__main__":
    monitor_logs()