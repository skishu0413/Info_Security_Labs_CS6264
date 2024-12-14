sandy1.py
import os
import time
import subprocess
from pyshark import FileCapture
import hashlib

LOG_DIR = "/home/lab3/received_logs"  
PROCESSED_LOGS = set()

def process_log(log_path):
    if extract_binary(log_path):
        print(f"Finished processing snort log: {log_path}")

def extract_binary(log_path):
    try:
        cap = FileCapture(log_path)
        extracted_binaries = {}

        for packet in cap:
            if hasattr(packet, 'tcp') and hasattr(packet.tcp, 'payload'):
                payload = packet.tcp.payload.binary_value

                elf_index = payload.find(b'\x7fELF')
                if elf_index != -1:
                    elf_payload = payload[elf_index:]
                    payload_hash = hashlib.md5(elf_payload).hexdigest()

                    if payload_hash not in extracted_binaries:
                        exec_path = os.path.join(LOG_DIR, f"exec_binary_{payload_hash}")
                        with open(exec_path, "wb") as f:
                            f.write(elf_payload)
                        extracted_binaries[payload_hash] = exec_path
                        run_binary(exec_path)  
                    else:
                        print("Duplicate paylaod detected.")
                else:
                    print("binary did not found in this payload")
        return True if extracted_binaries else None
    except Exception as e:
        print(f"Failed to process snort log {log_path}: {e}")
        return None

def run_binary(exec_path):
    os.chmod(exec_path, 0o755)  
    subprocess.Popen([exec_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def monitor_logs():
    while True:
        snort_logs = [f for f in os.listdir(LOG_DIR) if f.startswith('snort.log.')]
        for filename in snort_logs:
            file_path = os.path.join(LOG_DIR, filename)
            if filename not in PROCESSED_LOGS:
                process_log(file_path)
                PROCESSED_LOGS.add(filename)
        time.sleep(10)

if __name__ == "__main__":
    monitor_logs()