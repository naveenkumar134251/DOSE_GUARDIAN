# 💊 DoseGuardian – Intelligent Pill-Taking Assistant

DoseGuardian is an embedded systems project designed to help users take their medicines on time.  
It uses a real-time clock (RTC), keypad-based configuration, and alert mechanisms to ensure medication adherence, especially for elderly patients and long-term treatments.

---

## 🎯 Aim

To design and implement a microcontroller-based intelligent medicine reminder system that alerts users at the correct time and tracks whether the medicine was taken or missed.

---

## 📌 Objectives

- Display real-time clock (date, time, day) on a 16×2 LCD  
- Allow users to edit RTC settings using a 4×4 keypad  
- Enable medicine time scheduling  
- Trigger alerts when medicine time matches current RTC time  
- Confirm medicine intake using a switch  
- Indicate missed doses using LED and buzzer alerts  

---

## 🧠 Working Principle

1. **Medicine Schedule Setup**
   - User presses a switch to enter menu mode
   - Medicine time is configured using a keypad
   - Settings are stored in the microcontroller

2. **Real-Time Monitoring**
   - RTC continuously updates current time
   - System compares RTC time with scheduled medicine time

3. **Alert Mechanism**
   - LCD displays “TIME TO TAKE MEDICINE”
   - Buzzer activates at scheduled time

4. **User Acknowledgment**
   - User confirms intake using a switch
   - If not confirmed within a fixed duration, the system marks the dose as missed

---

## 🧩 Hardware Requirements

- LPC2148 (ARM7 Microcontroller)
- 16×2 LCD Display
- 4×4 Matrix Keypad
- RTC Module
- Buzzer
- Push Buttons (Switch1, Switch2)
- LED
- USB-UART Converter / DB-9 Cable

---

## 💻 Software Requirements

- Embedded C
- Keil µVision IDE
- Flash Magic
- ARM7 LPC2148 Toolchain

---

## 🔄 Software Flow

1. Initialize RTC, LCD, Keypad, Buzzer, and GPIO
2. Display current date and time on LCD
3. Enter menu when switch is pressed
4. Edit RTC or Medicine Time as selected
5. Continuously compare RTC time with medicine time
6. Trigger alert when time matches
7. Wait for user acknowledgment
8. Indicate missed dose if no acknowledgment is received

---

## 📟 Menu Structure
### Display  
<img width="1284" height="880" alt="image" src="https://github.com/user-attachments/assets/e6bc9148-026b-4d88-b575-18ebeab90338" />  

-  Shows current time in HH:MM:SS format
-  Displays Date (DD/MM/YYYY) and Day
-  RTC runs continuously ⏳

### Main Menu  
<img width="1339" height="886" alt="image" src="https://github.com/user-attachments/assets/e06ce9e3-3bb8-437d-a855-7d11db1c0ebb" />

- MAIN MENU when the SW1 is pressed

