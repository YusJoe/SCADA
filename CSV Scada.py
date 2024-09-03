import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import csv

SERIAL_PORT = 'COM13'
BAUD_RATE = 9600

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

x_vals = []
sensorPin1_data = []
sensorPin2_data = []

def read_and_process_data():
    line = ser.readline().decode('utf-8').strip()
    sensorValues = line.split(', ')

    x_vals.append(float(sensorValues[0]))
    sensorPin1_data.append(int(sensorValues[1]))
    sensorPin2_data.append(int(sensorValues[2]))

    print(f'Time: {sensorValues[0]}, Sensor 1: {sensorValues[1]}, Sensor 2: {sensorValues[2]}')

def update_plot(frame):
    read_and_process_data()
    plt.cla()
    plt.plot(x_vals, sensorPin1_data, label='Sensor 1')
    plt.plot(x_vals, sensorPin2_data, label='Sensor 2')
    plt.xlabel('Time')
    plt.ylabel('Sensor Values')
    plt.legend()

def on_close(event):
    with open('arduino_data.csv', 'w', newline='') as csvfile:
        writer = csv.wwriter(csvfile)
        writer.writerow(['Time', 'Sensor 1', 'Sensor 2'])
        for x, s1, s2 in zip(x_vals, sensorPin1_data, sensorPin2_data):
            writer.writerow([x, s1, s2])

fig, ax = plt.subplots()
fig.canvas.mpl_connect('close_event', on_close)

ani = FuncAnimation(fig, update_plot, interval=10)
plt.show()
