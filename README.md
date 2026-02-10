# 📦 Automated Storage and Retrieval Inventory Management System

An **ARM-based automated inventory management system** using a **robotic arm**, **QR code identification**, and a **Linux GUI application** running on a **Raspberry Pi**.  
This project automates **storage, retrieval, and real-time inventory tracking** of products.

---

## 🔍 Project Overview

Manual inventory management is slow, error-prone, and requires continuous human intervention.  
This project demonstrates how **embedded Linux**, **inter-process communication (IPC)**, and **robotic automation** can be integrated to build a **smart inventory management system**.

The system:
- Identifies products using **QR codes**
- Stores and retrieves items using a **robotic arm**
- Automatically updates inventory count
- Provides a **GUI-based interface** for user interaction

---

## 🧠 System Architecture

### Hardware Components
- Raspberry Pi (ARM Processor)
- Servo-based Robotic Arm (Multi-DOF)
- Relay Module
- 5V SMPS Power Supply (HLK series)
- QR Code Labels
- Custom Control and Driver Board

### Software Components
- Raspberry Pi OS (Linux)
- C Programming
- GTK (GUI)
- POSIX Threads (`pthread`)
- IPC using FIFO (Named Pipes)
- CSV files for inventory storage

---

## 🖼️ Hardware Setup

### Robotic Arm and Storage Area
![Robotic Arm Setup](images/arm1.jpg)

![Robotic Arm Side View](images/arm2.jpg)

![Robotic Arm Close-Up](images/arm3.jpg)

---

### Raspberry Pi Control Board
![Raspberry Pi Control Board](images/pi.jpg)

---

## 🖥️ GUI Interface

### Inventory Management GUI
![GUI Screenshot](images/gui.jpg)

---

## 🔄 Working Flow

### 1️⃣ System Startup
- Raspberry Pi boots into Linux
- Main program initializes:
  - GPIO pins
  - FIFO (Named Pipe)
  - GUI components
  - Threads for motor control and file handling

---

### 2️⃣ Input Mode (Storage Operation)
1. User enters the **Product Name** through the GUI
2. QR code corresponding to the product is detected
3. Robotic arm picks the product
4. Product is placed in the assigned storage slot
5. Inventory count is incremented and saved in a CSV file

---

### 3️⃣ Output Mode (Retrieval Operation)
1. User selects **Output Mode**
2. Requested product is identified
3. Robotic arm retrieves the product
4. Inventory count is decremented
5. GUI updates inventory status in real time

---

### 4️⃣ Inter-Process Communication
- GUI and control logic run as **separate processes**
- Communication is handled using **FIFO (`mkfifo`)**
- Ensures safe and synchronized data exchange

---

## 🧵 Software Design

### Multi-Process Architecture
- `fork()` is used to separate GUI and control logic
- Improves system reliability and modularity

### Multi-Threading
Threads are used to handle:
- Servo motor movement
- Inventory file read/write operations
- GUI responsiveness

### IPC Mechanism
- FIFO (Named Pipe)
- Simple, reliable, and easy to debug
- Suitable for related processes

---

## 🖥️ GUI Features

- Input Mode / Output Mode selection
- Product name entry
- STOP and Idle control
- Live inventory table
- User-friendly GTK-based interface

---

## ✨ Key Features

- Automated storage and retrieval
- Real-time inventory tracking
- ARM-based embedded Linux system
- GUI-controlled operation
- FIFO-based inter-process communication
- Multi-threaded robotic control
- Scalable and modular design

---
