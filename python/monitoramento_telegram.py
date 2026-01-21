# -*- coding: utf-8 -*-
"""
Monitoramento de Temperatura com Arduino e Alertas via Telegram
Autor: Davi Peixoto
"""

import serial
from serial.tools import list_ports
import time
import requests
import csv
from datetime import datetime

TOKEN = "SEU_TOKEN_AQUI"
CHAT_ID = "SEU_CHAT_ID_AQUI"
BAUD_RATE = 9600
TEMPO_ALERTA = 15
CSV_FILE = "leituras_arduino.csv"

def detectar_arduino():
    for p in serial.tools.list_ports.comports():
        if "arduino" in p.description.lower() or "usb" in p.description.lower():
            return p.device
    return None

def enviar_telegram(msg):
    url = f"https://api.telegram.org/bot{TOKEN}/sendMessage"
    requests.post(url, data={"chat_id": CHAT_ID, "text": msg})

def salvar_csv(temp, umid, spmin, spmax, status):
    existe = False
    try:
        open(CSV_FILE, "r")
        existe = True
    except:
        pass

    with open(CSV_FILE, "a", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        if not existe:
            writer.writerow(["DataHora","Temperatura","Umidade","SPmin","SPmax","Status"])
        writer.writerow([
            datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            temp, umid, spmin, spmax, status
        ])

porta = detectar_arduino()
ser = serial.Serial(porta, BAUD_RATE, timeout=1)
time.sleep(2)

ultimo_alerta = 0

while True:
    linha = ser.readline().decode().strip()
    if "Temperatura:" in linha:
        partes = linha.split(";")
        temp = float(partes[0].split(":")[1])
        umid = float(partes[1].split(":")[1])
        spmin = float(partes[2].split(":")[1])
        spmax = float(partes[3].split(":")[1])

        status = "NORMAL"
        agora = time.time()

        if temp > spmax:
            status = "ALTA"
            if agora - ultimo_alerta > TEMPO_ALERTA:
                enviar_telegram(f"⚠️ ALERTA ALTA: {temp}°C")
                ultimo_alerta = agora

        elif temp < spmin:
            status = "BAIXA"
            if agora - ultimo_alerta > TEMPO_ALERTA:
                enviar_telegram(f"⚠️ ALERTA BAIXA: {temp}°C")
                ultimo_alerta = agora

        salvar_csv(temp, umid, spmin, spmax, status)
        print(temp, umid, status)
