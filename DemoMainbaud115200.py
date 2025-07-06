import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# Serial Port Configuration
SERIAL_PORT = 'COM5'  # Change this based on your system
BAUD_RATE = 115200  # Updated baud rate
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Data Storage
time_vals = []
temp_vals = []
motor_speed_vals = []
efficiency_vals = []

# Efficiency Calculation Function
def calculate_efficiency(motor_speed, temperature):
    if motor_speed == 0:
        return 0
    efficiency = (motor_speed / (temperature - 24))  # Example formula
    return max(0, min(efficiency, 100))  # Limit efficiency between 0-100%

# Figure and Subplots Setup
fig, axs = plt.subplots(3, 1, figsize=(8, 8))
axs[0].set_title("Motor Speed (RPM)")
axs[1].set_title("Temperature (°C)")
axs[2].set_title("Efficiency (%)")

# Update Function for Animation
def update_graph(frame):
    try:
        data = ser.readline().decode('utf-8').strip()
        temp, motor_speed = map(float, data.split(','))

        time_vals.append(len(time_vals))
        temp_vals.append(temp)
        motor_speed_vals.append(motor_speed)
        efficiency_vals.append(calculate_efficiency(motor_speed, temp))

        # Keep only last 100 values for better visualization
        time_vals_trimmed = time_vals[-100:]
        temp_vals_trimmed = temp_vals[-100:]
        motor_speed_vals_trimmed = motor_speed_vals[-100:]
        efficiency_vals_trimmed = efficiency_vals[-100:]

        # Clear and re-plot data
        axs[0].cla()
        axs[1].cla()
        axs[2].cla()

        axs[0].plot(time_vals_trimmed, motor_speed_vals_trimmed, 'r', label="Motor Speed")
        axs[1].plot(time_vals_trimmed, temp_vals_trimmed, 'b', label="Temperature")

        # Plot efficiency as a bar graph
        axs[2].bar(time_vals_trimmed, efficiency_vals_trimmed, color='g', label="Efficiency")

        axs[0].legend()
        axs[1].legend()
        axs[2].legend()
    except Exception as e:
        print(f"Error: {e}")

# Animation Function
ani = animation.FuncAnimation(fig, update_graph, interval=500)
plt.tight_layout()
plt.show()
