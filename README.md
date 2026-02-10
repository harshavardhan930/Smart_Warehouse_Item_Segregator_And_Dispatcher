# 📦 Smart Warehouse Item Segregator and Dispatcher

An **ARM-based automated inventory management system** using a **robotic arm**, **QR code identification**, and a **Linux GUI application** running on a **Raspberry Pi**.  
This project automates **storage, retrieval, segregation, dispatching, and real-time inventory tracking** of products.

---

## 🔍 Project Overview

Manual inventory management is slow, error-prone, and requires continuous human intervention.  
This project demonstrates how **embedded Linux**, **inter-process communication (IPC)**, and **robotic automation** can be integrated to build a **smart warehouse system**.

The system:
- Identifies products using **QR codes**
- Segregates and dispatches items automatically
- Stores and retrieves items using a **robotic arm**
- Updates inventory count in real time
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

## 🖼️ Hardware Setup & Visualization

### 🔧 Robotic Arm Assembly and Operation

| Front View | Side View |
|-----------|----------|
| ![Robotic Arm Front](images/arm1.jpeeg) | ![Robotic Arm Side](images/arm2.jpeg) |

| Close-Up View |
|--------------|
| ![Robotic Arm Close-Up](images/arm3.jpeg) |

> The robotic arm performs pick-and-place operations for storage and dispatch based on QR code identification.

---

### ⚙️ Raspberry Pi Control Board

| Raspberry Pi & Power Module |
|-----------------------------|
| ![Raspberry Pi Control Board](images/pi.jpeg) |

> Raspberry Pi acts as the central controller, handling GUI, IPC, file management, and robotic control.

---

## 🖥️ GUI Interface Visualization

### 📊 Inventory Management GUI

| GUI Dashboard |
|---------------|
| ![GUI Screenshot](images/gui.jpeg) |

> The GUI allows the user to switch between input/output modes, monitor inventory status, and control operations.

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

### 2️⃣ Input Mode (Storage & Segregation)
1. User enters the **Product Name** via the GUI
2. QR code corresponding to the product is detected
3. Product is identified and classified
4. Robotic arm picks the product
5. Product is placed in the assigned storage location
6. Inventory count is updated and saved in a CSV file

---

### 3️⃣ Output Mode (Dispatch Operation)
1. User selects **Output Mode**
2. Requested product is identified
3. Robotic arm retrieves the product
4. Product is dispatched
5. Inventory count is decremented
6. GUI updates inventory status in real time

---

### 4️⃣ Inter-Process Communication
- GUI and control logic run as **separate processes**
- Communication is handled using **FIFO (`mkfifo`)**
- Ensures safe and synchronized data exchange

---

## 🧵 Software Design

### Multi-Process Architecture
- `fork()` is used to separate GUI and control logic
- Improves system reliability and fault isolation

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
- GTK-based user-friendly interface

---

## ✨ Key Features

- Automated item segregation and dispatch
- Real-time inventory tracking
- ARM-based embedded Linux system
- GUI-controlled operation
- FIFO-based inter-process communication
- Multi-threaded robotic control
- Scalable and modular design

---

