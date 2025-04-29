# Info_Security_Labs_CS6264

This repository contains a comprehensive collection of hands-on labs and exercises developed for the CS6264 Information Security course at Georgia Tech. The labs provide practical experience in various domains of cybersecurity, including binary exploitation, malware analysis, network security, web security, and mobile device rooting.

---

## 📚 Table of Contents

- [Overview](#overview)
- [Lab Modules](#lab-modules)
- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)

---

## 🧭 Overview

The CS6264 Information Security Labs aim to bridge the gap between theoretical knowledge and practical application in cybersecurity. Each lab is designed around real-world scenarios to help students gain hands-on experience with offensive and defensive techniques.

---

## 🧪 Lab Modules

The repository is organized into subdirectories, each representing a distinct lab:

### 1. `Binary_Exploitation/`
- Topics: Buffer overflows, format string attacks, return-oriented programming (ROP)
- Skills: Analyzing compiled programs and crafting exploits

### 2. `Malware_Analysis/`
- Topics: Static and dynamic malware analysis
- Skills: Behavior analysis, propagation techniques, and impact assessment

### 3. `Modeling_&_Attacking_PE_models/`
- Topics: Structure and exploitation of Portable Executable (PE) files
- Skills: Dissecting PE formats and understanding attack vectors

### 4. `Network_based_IDS/`
- Topics: Building and using intrusion detection systems
- Skills: Traffic analysis and threat detection using network tools

### 5. `Rooting_an_Android_Device/`
- Topics: Android architecture, rooting, and mobile security
- Skills: Gaining root access and exploring Android vulnerabilities

### 6. `Web_Security/`
- Topics: SQLi, XSS, CSRF, stored vs reflected attacks
- Skills: Identifying and mitigating web application vulnerabilities

---

## ⚙️ Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/skishu0413/Info_Security_Labs_CS6264.git
   cd Info_Security_Labs_CS6264

2. **Naviagte to the desired lab Module**
   ```bash
   cd [Lad_module_Name]

---

## 🛠️ Prerequisites

Each lab may have its own setup requirements. However, the general recommended setup includes:

### 💻 System Requirements

**Operating System:**
- Linux (Ubuntu preferred)
- Windows 10 (for PE analysis labs)
- Android Emulator or physical device (for rooting lab)

**Virtualization Tools:**
- VirtualBox or VMware Workstation
- Vagrant (optional for automation)

### 🔧 Tools and Frameworks

- **Binary Analysis**: `gdb`, `pwntools`, `radare2`, `ropper`, `angr`
- **Static Analysis**: `Ghidra`, `strings`, `objdump`, `file`, `readelf`
- **Network Analysis**: `tcpdump`, `Wireshark`, `Snort`
- **Malware Sandboxing**: `Cuckoo Sandbox` (for isolated dynamic analysis)
- **Web Security**: `Burp Suite`, `Postman`, `Firefox` with security extensions
- **Mobile Security**: `Android SDK`, `adb`, custom ROMs (e.g., `LineageOS`)
- **Web Development**: `Node.js`, `Express`, `Flask`, `nginx`

### 🧪 Programming Languages

- Python 3.x (primary scripting language)
- Bash
- C (for exploit compilation)
- Java (for Android lab)


