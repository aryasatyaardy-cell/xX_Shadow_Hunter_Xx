# xX_Shadow_Hunter_Xx (Work in Progress)
A quadrepic data collecting robot designed with adaptive power management as an apporach to natural disaster (mainly earthquake) mitigation trough robotics

## Overview
Shadow Hunter is a four-legged data collecting robot made for an IoT competition built around arduino ecosystem, controlled remotely trough a dedicated website

This robot is aimed to aid rescue worker passively by providing infield-real-time data in the background, its purpose is not to be seen, but rather to venture to areas that are hard/dangerous to approach

## Feautres
### Quadruped Locomotion

Provided with 4 legs, Shadow Hunter is designed to be able to relocate across rough terrains after an earthquake struck, the quadruped approach is to give Asta (the maker of this project) a room to expand this feature further in the future by giving it a self auto levelling or implementing manuver system in rougher terrains such us climbing slopes

### Modes of Operations

There are 3 modes implemented in Shadow Hunter's system

**Hibernating** is the deffault mode upon activating, it does not turn on any system but the wireless connection to operator

**Walking** is the manuvaring mode, it reposition the legs to be extended to a walking position and are ready to re-position whenever the operator press ```W/A/S/D/Q/E``` keys on their keyboard

**Standby** is the deffault data collecting mode where it reposition its leg to be half folded, using its wider foot surface to be harder to knocked over and is planned to be self-levelling above sloped ground

### Sensors
- Temperature
- Humidity
- Vibration
- Air quality
_ Sound

## Implemented Concept
- **IoT** or best known as Internet of Things is where hardwares can communicate with each others wirelessly without the help of human configuration during practice
- **Real-time monitoring** is implemented to fulfill Shadow Hunter's purpose in gathering onfield datas and sharing it to the operator in an instant
- **Adaptive power management** which is one of the core concept in my initial idea, as post-disastourus area commonly have limited power supply, Shadow Hunter are designed to effectively use the available power by activating functions which are needed
- **Modular architecture** started taken into account as I'm a forgetful person, and by having the system separated per-function, I can remember and debug it easier in the future
- **Quadruped movement** are made as Shadow Hunter's main mode of locomotion because post-earthquake terrains are known to get rough

## Repository Structure
### Website Control Panel

```Web.html``` is the main website structure

```Web.css``` is the ```Web.html``` styling

```Web.js``` to give the website some intellect (such as letting it communicate with Shadow Hunter)

**Website try-out**
https://shadowhunterdataviewer.netlify.app/

N.B. upon pressing buttons, error message is expected to be appeared in the log window as this website is designed to be connected to xX_Shadow_Hunter_Xx

### xX_Shadow_Hunter_Xx

```xX_Shadow_Hunter_Xx.ino``` is the main arduino sketch

N.B. this part (the arduinos) aren't finish so I won't be wasting my time in writing the readmes

## Hardwares
N.B. all of Shadow Hunter's body part is free to print and assess, I have include its stl .zip file in this repo, just use the latest one if you want to print your very own personal Shadow Hunter

### Brain
Shadow Hunter main controller is the **ESP32 S3 DevKitC-1 N8R2 N16R8** microcontroller
### Legs
Each legs are driven with 3 lots of **sg90 all metal 180° micro servo** and connected to **pca9685** servo driver which then connected to esp32
### Sensors
- Temperature and humidity sensor is DHT22
- Vibration sensor is GY-BMI160 IMU 6DOF Gyroscope Accelerometer
- Air quality sensor is MQ-135
- Sound sensor (is actually a speaker but I've got not much time to configure it that way so I just use it to gather the presence of loud noise) is INMP441 omnidirectional microphone

xX_Shadow_Hunter_Xx™ © 2026 Aryasatya Mahdiya Ardy — Open-source and free to use, modify, and redistribute with attribution required; commercial use is prohibited without prior written permission.
